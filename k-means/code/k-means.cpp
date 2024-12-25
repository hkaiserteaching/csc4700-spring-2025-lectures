
#include <algorithm>
#include <array>
#include <cstddef>
#include <limits>
#include <random>
#include <vector>

// def __init__(self, n_clusters, max_iter=100, random_state=123):
//     self.n_clusters = n_clusters
//     self.max_iter = max_iter
//     self.random_state = random_state
//
// def initialize_centroids(self, X):
//     np.random.RandomState(self.random_state)
//     random_idx = np.random.permutation(X.shape[0])
//     centroids = X[random_idx[:self.n_clusters]]
//     return centroids

// Initialize random number generator
std::random_device rd;
thread_local std::mt19937 gen(rd());

double get_random_number(size_t num_points)
{
    std::uniform_int_distribution<size_t> dis(0, num_points - 1);
    return dis(gen);
}

struct point
{
    std::array<double, 2> pt;
};

using points = std::vector<point>;
using centroids = std::vector<point>;
using labels = std::vector<size_t>;

centroids initialize_centroids(points const& pts, size_t num_centroids)
{
    centroids initial_centroids;
    initial_centroids.reserve(num_centroids);
    for (size_t i = 0; i != num_centroids; ++i)
        initial_centroids.push_back(pts[get_random_number(pts.size())]);
    return initial_centroids;
}

// def compute_centroids(self, X, labels):
//     centroids = np.zeros((self.n_clusters, X.shape[1]))
//     for k in range(self.n_clusters):
//         centroids[k, :] = np.mean(X[labels == k, :], axis=0)
//     return centroids

point sum(point lhs, point rhs)
{
    return {lhs.pt[0] + rhs.pt[0], lhs.pt[1] + rhs.pt[1]};
}

point mean(point cent, size_t num)
{
    return {cent.pt[0] / num, cent.pt[1] / num};
}

centroids compute_centroids(
    points const& pts, labels const& lab, int num_centroids)
{
    centroids new_centroids(num_centroids);

    std::vector<size_t> pt_count(num_centroids);

    for (size_t i = 0; i != pts.size(); ++i)
    {
        sum(new_centroids[lab[i]], pts[i]);
        ++pt_count[lab[i]];
    }

    for (size_t i = 0; i != num_centroids; ++i)
    {
        mean(new_centroids[i], pt_count[i]);
    }

    return new_centroids;
}

// def compute_distance(self, X, centroids):
//     distance = np.zeros((X.shape[0], self.n_clusters))
//     for k in range(self.n_clusters):
//         row_norm = norm(X - centroids[k, :], axis=1)
//         distance[:, k] = np.square(row_norm)
//     return distance

using distances = std::vector<double>;

double sqr(double val)
{
    return val * val;
}

double distance(point lhs, point rhs)
{
    return sqr(lhs.pt[0] - rhs.pt[0]) + sqr(lhs.pt[1] - rhs.pt[1]);
}

std::pair<distances, bool> compute_distances(
    points const& pts, centroids const& cents, labels& labs)
{
    bool changed = false;
    distances cent_distances(cents.size());

    // for each point find closest centroid
    for (size_t pt = 0; pt != pts.size(); ++pt)
    {
        double min_distance = std::numeric_limits<double>::max();
        size_t centroid = 0;

        for (size_t i = 0; i != cents.size(); ++i)
        {
            auto d = distance(pts[pt], cents[i]);
            if (d < min_distance)
            {
                min_distance = d;
                centroid = i;
            }
        }

        // store closest centroid as the current point's label
        if (labs[pt] != centroid)
        {
            changed = true;
            labs[pt] = centroid;
        }

        // accumulate sum of square distances for centroid
        cent_distances[centroid] += min_distance;
    }

    return {std::move(cent_distances), changed};
}

// def find_closest_cluster(self, distance):
//     return np.argmin(distance, axis=1)
//
// def compute_sse(self, X, labels, centroids):
//     distance = np.zeros(X.shape[0])
//     for k in range(self.n_clusters):
//         distance[labels == k] = norm(X[labels == k] - centroids[k], axis=1)
//     return np.sum(np.square(distance))
//
// def fit(self, X):
//     self.centroids = self.initialize_centroids(X)
//     for i in range(self.max_iter):
//         old_centroids = self.centroids
//         distance = self.compute_distance(X, old_centroids)
//         self.labels = self.find_closest_cluster(distance)
//         self.centroids = self.compute_centroids(X, self.labels)
//         if np.all(old_centroids == self.centroids):
//             break
//     self.error = self.compute_sse(X, self.labels, self.centroids)

labels k_means(points const& pts, size_t num_centroids, size_t max_iterations)
{
    labels labs(pts.size(), std::numeric_limits<size_t>::max());

    auto centroids = initialize_centroids(pts, num_centroids);
    for (size_t iter = 0; iter != max_iterations; ++iter)
    {
        auto [distances, changed] = compute_distances(pts, centroids, labs);
        if (!changed)
            break;
        centroids = compute_centroids(pts, labs, num_centroids);
    }

    return labs;
}

// def predict(self, X):
//     distance = self.compute_distance(X, self.centroids)
//     return self.find_closest_cluster(distance)

int main()
{
    return 0;
}