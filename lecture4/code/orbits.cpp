
#include <algorithm>
#include <concepts>
#include <cstdarg>
#include <iterator>
#include <type_traits>
#include <utility>
#include <tuple>

// Domain(F)
template <typename F>
struct domain;

template <typename F, typename T, typename... Ts>
    requires(std::regular_invocable<F, T, Ts...>)
struct domain<F(T, Ts...)>
{
    using type = T; // extract type of first argument
};

template <typename F>
using domain_t = typename domain<F>::type;

// DistanceType(F)
template <typename F>
struct distance_type
{
    using type = std::size_t; // convention!
};

template <typename F>
using distance_t = typename distance_type<F>::type;

//
template <typename F, typename T>
concept UnaryFunction = std::regular_invocable<F, T>;

template <typename F, typename T>
concept Operation = std::same_as<std::invoke_result_t<F, T>, T>;

template <typename F>
concept Transformation = UnaryFunction<F, domain_t<F>> && Operation<F, domain_t<F>>;

template <typename A>
concept Action = UnaryFunction<A, domain_t<A> &>;

template <typename F>
concept UnaryPredicate = UnaryFunction<F, domain_t<F>> &&
                         std::same_as<std::invoke_result_t<F, domain_t<F>>, bool>;

template <typename N>
concept Integer = std::is_integral_v<N>;

// Raise transformation F to its power N
template <Transformation F, Integer N>
domain_t<F> power_unary(domain_t<F> x, N n, F f)
{
    // Precondition: n >= 0 && (forall i in N) 0 < i <= n --> f^i(x) is defined
    while (n != N(0))
    {
        n = n - N(1);
        x = f(x);
    }
    return x;
}

template <Transformation F>
distance_t<F> distance(domain_t<F> x, domain_t<F> y, F f)
{
    // Precondition: y is reachable from x under f
    using N = distance_t<F>;
    N n(0);
    while (x != y)
    {
        x = f(x);
        n = n + N(1);
    }
    return n;
}

// Find collision point
template <Transformation F, UnaryPredicate P>
    requires(std::same_as<domain_t<F>, domain_t<P>>)
domain_t<F> collision_point(domain_t<F> const &x, F f, P p)
{
    // Precondition: p(x) <--> f(x) is defined
    if (!p(x))
        return x;
    domain_t<F> slow = x;    // slow = f^0(x)
    domain_t<F> fast = f(x); // fast = f^1(x)
                             // n = 0 (completed iterations)
    while (fast != slow)     // slow = f^n(x) and fast = f^2n+1(x)
    {
        slow = f(slow); // slow = f^n+1(x) fast = f^2n+1(x)
        if (!p(fast))
            return fast;
        fast = f(fast); // slow = f^n+1(x) fast = f^2n+2(x)
        if (!p(fast))
            return fast;
        fast = f(fast); // slow = f^n+1(x) fast = f^2n+3(x)
                        // n = n+1
    }
    return fast; // slow = f^n(x) fast = f^2n+1(x)
    // Postcondition: return value is terminal point or collision point
}

template <Transformation F, UnaryPredicate P>
    requires(std::same_as<domain_t<F>, domain_t<P>>)
bool terminating(domain_t<F> const &x, F f, P p)
{
    // Precondition: p(x) <--> f(x) is defined
    return !p(collision_point(x, f, p));
}

template <Transformation F, UnaryPredicate P>
    requires(std::same_as<domain_t<F>, domain_t<P>>)
bool circular(domain_t<F> const &x, F f, P p)
{
    // Precondition: p(x) <--> f(x) is defined
    domain_t<F> y = collision_point(x, f, p);
    return p(y) && x == f(y);
}

template <Transformation F>
distance_t<F> cycle_length(domain_t<F> const &x, F f)
{
    // precondition: x is part of a cycle
    return distance(f(x), x, f) + distance_t<F>(1);
}

template <Transformation F>
domain_t<F> convergent_point(domain_t<F> x0, domain_t<F> x1, F f)
{
    // Precondition: (exists n in DistanceType(F)) n >= 0 and f^n(x0) = f^n(x1)
    while (x0 != x1)
    {
        x0 = f(x0);
        x1 = f(x1);
    }
    return x0;
}

template <Transformation F, UnaryPredicate P>
    requires(std::same_as<domain_t<F>, domain_t<P>>)
domain_t<F> connection_point(domain_t<F> const &x, F f, P p)
{
    // Precondition: p(x) <--> f(x) is defined
    domain_t<F> y = collision_point(x, f, p);
    if (!p(y))
        return y;
    return convergent_point(x, f(y), f);
}

template <Transformation F, UnaryPredicate P>
    requires(std::same_as<domain_t<F>, domain_t<P>>)
std::tuple<distance_t<F>, distance_t<F>, domain_t<F>>
orbit_structure(domain_t<F> const &x, F f, P p)
{
    // Precondition: p(x) <--> f(x) is defined
    using N = distance_t<F>;
    domain_t<F> y = connection_point(x, f, p);
    N m = distance(x, y, f);
    N n(0);
    if (p(y))
        n = distance(f(y), y, f);
    // Terminating: m = h - 1 and n = 0
    // Otherwise: m = h and n = c - 1
    return {m, n, y};
}

// Duality between transformations and actions

// action from transformation
template <typename T>
void a(T &x) { x = f(x); }

// transformation from action
template <typename T>
T f(T x)
{
    a(x);
    return x;
}

template <Action A, Integer N>
void advance(domain_t<A> &x, N n, A a)
{
    while (n != N(0))
    {
        assert(is_defined(a, x));
        n = n - N(1);
        a(x);
    }
}

template <typename T>
concept Incrementable = requires(T t) { ++t; t++; };

template <Incrementable T>
struct increment
{
    void operator()(T &x) { ++x; }
};

template <typename T>
concept Decrementable = requires(T t) { --t; t--; };

template <Decrementable T>
struct decrement
{
    void operator()(T &x) { --x; }
};

template <typename T>
decrement<T> inverse(increment<T> const &)
{
    return decrement<T>();
}

template <typename T>
increment<T> inverse(decrement<T> const &)
{
    return increment<T>();
}

template <typename A>
concept InvertibleAction = Action<A> && requires(A a) { inverse(a); };

template <typename F, typename T1, typename T2>
concept BinaryFunction = std::invocable<F, T1, T2>;

template <InvertibleAction A, BinaryFunction<domain_t<A> &, domain_t<A> &> U,
          UnaryFunction<domain_t<A> &> V>
std::tuple<U, V, bool>
bidirectional_traversal(domain_t<A> &first, domain_t<A> &last, A a, U u, V v)
{
    auto b(inverse(a));
    while (first != last)
    {
        b(last);
        if (first == last)
        {
            v(first);
            return std::make_tuple(u, v, true);
        }
        u(first, last);
        a(first);
    }
    return std::make_tuple(u, v, false);
}

template <std::bidirectional_iterator I>
void reverse(I first, I last)
{
    bidirectional_traversal(first, last, increment<I>(), [](I i1, I i2)
                            { std::iter_swap(i1, i2); }, [](I i) {});
}

template <typename T, Integer N>
void advance(T &x, N n)
{
    advance(x, n, increment<T>());
}

template <Action A>
void advance_while_defined(domain_t<A> &x, A a)
{
    while (is_defined(a, x))
        a(x);
}

template <Action A>
void advance_until_last(domain_t<A> &first, domain_t<A> const &last, A a)
{
    while (first != last)
    {
        assert(is_defined(a, first));
        a(first);
    }
}

template <Action A, UnaryPredicate P>
    requires(std::same_as<domain_t<A>, domain_t<P>>)
void advance_till_predicate(domain_t<A> &x, P p, A a)
{
    while (!p(x))
    {
        assert(is_defined(a, x));
        a(x);
    }
}

int main() {}