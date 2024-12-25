
#include <cmath>
#include <format>
#include <print>
#include <string>
#include <vector>

#include <blaze/Math.h>

#include "EasyBMP/EasyBMP.h"

// Various constants
double const PI = 2.0 * std::acos(0);
double const G = 6.67408e-11;            // gravitational constant
double const SYSTEM_SIZE = 3.5;          // Farthest particles in AU
double const SYSTEM_THICKNESS = 0.08;    // Thickness in AU
double const INNER_BOUND = 0.3;          // Closest particles to center in AU

double const SOLAR_MASS = 2.0e30;    // in kg
//  Disk mask as a portion of center star mass
double const EXTRA_MASS = 1.5 * SOLAR_MASS;

// Simulated time between integration steps, in seconds
double const TIME_STEP = 100000;
size_t const TIME_STEPS = 100;     // number of time steps evaluated
size_t const NUM_BODIES = 1000;    // number of particles

// Softens particles interactions at close distances
double const SOFTENING = 0.015;

// Size of rendered frame
size_t const WIDTH = 1000;
size_t const HEIGHT = 1000;
double const RENDER_SCALE = 1.0;    // "Zoom" of images produced

//
double to_meters(double au)
{
    return au * 1.496e11;
}

double extra_mass(double radius)
{
    return ((radius - INNER_BOUND) / SYSTEM_SIZE) * EXTRA_MASS;
}

// Initialize random number generator
std::random_device rd;
thread_local std::mt19937 gen(rd());

// generate random initial positions for bodies
double get_random_angle()
{
    std::uniform_real_distribution<double> dis(0, 2.0 * PI);
    return dis(gen);
}

double get_random_radius()
{
    std::uniform_real_distribution<double> dis(INNER_BOUND, SYSTEM_SIZE);
    return dis(gen);
}

double get_random_height()
{
    std::uniform_real_distribution<double> dis(
        -SYSTEM_THICKNESS / 2, SYSTEM_THICKNESS / 2);
    return dis(gen);
}

struct point
{
    point(double x = 0.0, double y = 0.0, double z = 0.0)
      : coords{x, y, z}
    {
    }
    point(blaze::StaticVector<double, 3> const& rhs)
      : coords{rhs}
    {
    }

    friend point operator-(point const& lhs, point const& rhs)
    {
        blaze::StaticVector<double, 3> result{lhs.coords - rhs.coords};
        return {result};
    }

    double norm() const
    {
        return blaze::norm(coords);
    }

    blaze::StaticVector<double, 3> coords;
};

struct body
{
    void update_velocity(double force, point const& pos_diff)
    {
        velocity_diff.coords += TIME_STEP * force * pos_diff.coords / mass;
    }

    void update_position()
    {
        velocity.coords += velocity_diff.coords;
        position.coords += TIME_STEP * velocity.coords;
        velocity_diff = {};
    }

    double mass = 0.0;
    point position{};
    point velocity{};
    point velocity_diff{};
};

using bodies = std::vector<body>;

bodies initial_conditions(size_t num_bodies)
{
    bodies pts;
    pts.reserve(num_bodies);

    // add sun, assume center of simulation
    pts.push_back(body{SOLAR_MASS});

    double const body_mass = EXTRA_MASS / num_bodies;

    for (size_t i = 1; i != num_bodies; ++i)
    {
        double angle = get_random_angle();
        double radius = get_random_radius();
        double angular_velocity = std::sqrt(
            (G * (SOLAR_MASS + extra_mass(radius))) / to_meters(radius));

        point position = {to_meters(radius * std::cos(angle)),
            to_meters(radius * std::sin(angle)),
            to_meters(get_random_height())};
        point velocity = {angular_velocity * std::sin(angle),
            -angular_velocity * std::cos(angle), 0.0};

        pts.push_back(body{body_mass, position, velocity});
    }

    return pts;
}

double sqr(double val)
{
    return val * val;
}

void calculate_forces(body& one, body& two)
{
    point pos_diff = one.position - two.position;

    double dist = pos_diff.norm();
    double force = (G * one.mass * two.mass) /
        (sqr(dist) + sqr(to_meters(SOFTENING)) * dist);

    one.update_velocity(-force, pos_diff);
    two.update_velocity(force, pos_diff);
}

void do_work(size_t time_step, bodies& all_bodies)
{
    std::println("time step: {}", time_step);
    for (size_t i = 0; i != all_bodies.size(); ++i)
    {
        for (size_t j = i + 1; j != all_bodies.size(); ++j)
        {
            calculate_forces(all_bodies[i], all_bodies[j]);
        }
    }
    for (size_t i = 0; i != all_bodies.size(); ++i)
    {
        all_bodies[i].update_position();
    }
}

double to_pixels(double p, size_t size)
{
    return (size / 2.0) * (1.0 + p / (to_meters(SYSTEM_SIZE) * RENDER_SCALE));
}

static RGBApixel const colors[] = {
    RGBApixel{66, 30, 15},
    RGBApixel{25, 7, 26},
    RGBApixel{9, 1, 47},
    RGBApixel{4, 4, 73},
    RGBApixel{0, 7, 100},
    RGBApixel{12, 44, 138},
    RGBApixel{24, 82, 177},
    RGBApixel{57, 125, 209},
    RGBApixel{134, 181, 229},
    RGBApixel{211, 236, 248},
    RGBApixel{241, 233, 191},
    RGBApixel{248, 201, 95},
    RGBApixel{255, 170, 0},
    RGBApixel{204, 128, 0},
    RGBApixel{153, 87, 0},
    RGBApixel{106, 52, 3},
};

RGBApixel get_rgb(int value)
{
    return colors[value % 16];
}

void render_frame(size_t time_step, bodies const& all_bodies)
{
    BMP n_body_frame;
    n_body_frame.SetBitDepth(24);
    n_body_frame.SetSize(WIDTH, HEIGHT);

    for (size_t i = 0; i != all_bodies.size(); ++i)
    {
        size_t pixel_x = to_pixels(all_bodies[i].position.coords[0], WIDTH);
        size_t pixel_y = to_pixels(all_bodies[i].position.coords[1], HEIGHT);

        if (pixel_x < WIDTH && pixel_y < HEIGHT)
        {
            double velocity = all_bodies[i].velocity.norm();
            n_body_frame.SetPixel(pixel_x, pixel_y, get_rgb(velocity));
        }
    }

    // Save the image
    std::string file_name = std::format("render/frame_{}.bmp", time_step);
    n_body_frame.WriteToFile(file_name.c_str());
}

int main()
{
    bodies all_bodies = initial_conditions(NUM_BODIES);

    for (size_t time_step = 0; time_step != TIME_STEPS; ++time_step)
    {
        do_work(time_step, all_bodies);
        render_frame(time_step, all_bodies);
    }

    return 0;
}