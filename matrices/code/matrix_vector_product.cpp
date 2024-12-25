
#include <iostream>

#include <blaze/Math.h>

void matrix_vector_product_blaze()
{
    // Setup of the 2x3 row-major dense matrix
    //
    //    ( 1  2  3 )
    //    ( 4  5  6 )
    //
    blaze::DynamicMatrix<int, blaze::rowMajor> dynA{
        {1, 2, 3},
        {4, 5, 6},
    };
    // Setup of the 3x2 column-major dense matrix
    //
    //    ( 1  4 )
    //    ( 2  5 )
    //    ( 3  6 )
    //
    blaze::DynamicMatrix<int, blaze::columnMajor> dynB{{1, 4}, {2, 5}, {3, 6}};

    // Instantiation of a 6x20 row-major static matrix
    blaze::StaticMatrix<double, 6UL, 20UL> A;
    // Instantiation of a row-major compressed matrix
    blaze::CompressedMatrix<double, blaze::rowMajor> B;
    // Instantiation of a column-major dynamic matrix
    blaze::DynamicMatrix<double, blaze::columnMajor> C;

    // ... Resizing and initialization

    C = A * B;

    auto m1 = blaze::rand<blaze::DynamicMatrix<int>>(5, 5, 1, 10);
    auto v1 =
        blaze::rand<blaze::DynamicVector<int, blaze::columnVector>>(5, 1, 10);

    // Matrix/column vector multiplication
    blaze::DynamicVector<int, blaze::columnVector> v2 = m1 * v1;
    std::cout << "matrix-vector-multiplication (blaze): " << v2 << "\n";

    // Row vector/matrix multiplication
    blaze::DynamicVector<int, blaze::rowVector> v3 = trans(v1) * m1;
    std::cout << "vector-matrix-multiplication (blaze): " << v3 << "\n";
}

int main()
{
    matrix_vector_product_blaze();
}
