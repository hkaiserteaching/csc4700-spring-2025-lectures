
#include <print>
#include <vector>

#include <blaze/Math.h>

void dot_product_vectors()
{
    std::vector<double> a = {1.0, 2.0, 3.0, 4.0, 5.0};
    std::vector<double> b = {5.0, 4.0, 3.0, 2.0, 1.0};

    double dot_result = 0.0;
    for (size_t i = 0; i != a.size(); ++i)
        dot_result += a[i] * b[i];

    std::println("dot-product: {}", dot_result);
}

void dot_product_blaze()
{
    blaze::DynamicVector<double> a = {1.0, 2.0, 3.0, 4.0, 5.0};
    blaze::DynamicVector<double> b = {5.0, 4.0, 3.0, 2.0, 1.0};
  
    std::println("dot-product (blaze): {}", (a, b));
}

int main()
{
    dot_product_vectors();
    dot_product_blaze();
}
