#include <blaze/Math.h>

int main()
{
   const size_t N(100);
   const size_t iterations(10);

   const size_t NN(N * N);

   blaze::CompressedMatrix<double, blaze::rowMajor> A(NN, NN);
   blaze::DynamicVector<double, blaze::columnVector> x(NN, 1.0), b(NN, 0.0), r(NN), p(NN), Ap(NN);
   double alpha, beta, delta;

   // ... Initializing the sparse matrix A

   // Performing the CG algorithm
   r = b - A * x;
   p = r;
   delta = (r, r);   // dot product

   for (size_t iteration = 0; iteration < iterations; ++iteration)
   {
      Ap = A * p;
      alpha = delta / (p, Ap);
      x += alpha * p;
      r -= alpha * Ap;
      beta = (r, r);
      if (std::sqrt(beta) < 1E-8)
         break;
      p = r + (beta / delta) * p;
      delta = beta;
   }

   return 0;
}
