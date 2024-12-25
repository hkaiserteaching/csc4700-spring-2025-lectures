#include <iostream>
#include <span>
#include <vector>

///////////////////////////////////////////////////////////////////////////////
void from_vector()
{
    std::vector<int> vec = {10, 20, 30, 40, 50};

    // Create a span over the vector
    std::span<int> span_from_vector(vec);

    // Modify elements via span
    span_from_vector[0] = 100;

    // Display modified vector
    for (int element : vec)
        std::cout << element << " ";
}

///////////////////////////////////////////////////////////////////////////////
void from_array()
{
    int data[] = {1, 2, 3, 4, 5};

    // Create a span over the entire array
    std::span<int> my_span(data);

    // Access elements
    for (int element : my_span)
        std::cout << element << " ";
}

///////////////////////////////////////////////////////////////////////////////
void print_values(std::span<int const> values)
{
    for (int value : values)
        std::cout << value << " ";
    std::cout << "\n";
}

void pass_to_function()
{
    int arr[] = {1, 2, 3, 4, 5};
    print_values(arr);    // Passes an array as a span

    std::vector<int> vec = {6, 7, 8, 9, 10};
    print_values(vec);    // Passes a vector as a span
}

///////////////////////////////////////////////////////////////////////////////
void process_partial_data(std::span<int const> partial_data)
{
    for (int value : partial_data)
        std::cout << value << " ";
    std::cout << "\n";
}

void partial_view()
{
    int data[] = {1, 2, 3, 4, 5, 6, 7, 8};
    std::span<int> full_span(data);    // Span over the full array

    // Creating subviews with only part of the data
    process_partial_data(full_span.subspan(2, 4));    // View of {3, 4, 5, 6}
}

int main()
{
    from_array();
    from_vector();
    pass_to_function();
    partial_view();

    return 0;
}
