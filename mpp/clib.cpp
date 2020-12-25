#include <omp.h>
#include <chrono>
#include <iostream>

extern "C"{

#pragma GCC optimize ("-O0")
long long CalcSimple(float * a,float * b, float * c, float* resArr , int n) {
  auto start = std::chrono::high_resolution_clock::now();
  for(int i=0;i<n;i++) {
      resArr[i] = a[i]+b[i]+c[i];
  }
  auto finish = std::chrono::high_resolution_clock::now();
  return std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count();
}

#pragma GCC optimize ("-O0")
long long CalcVect(float * a,float * b, float * c, float* resArr , int n) {
  auto start = std::chrono::high_resolution_clock::now();
  #pragma omp simd
  for(int i=0;i<n;i++) {
      resArr[i] = a[i]+b[i]+c[i];
  }
  auto finish = std::chrono::high_resolution_clock::now();
  return std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count();
}

#pragma GCC optimize ("-O0")
long long CalcParallel(float * a,float * b, float * c, float* resArr , int n) {
  auto start = std::chrono::high_resolution_clock::now();
  #pragma omp parallel for
  for(int i=0;i<n;i++) {
      resArr[i] = a[i]+b[i]+c[i];
  }
  auto finish = std::chrono::high_resolution_clock::now();
  return std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count();
}

#pragma GCC optimize ("-O0")
long long CalcVectParallel(float * a,float * b, float * c, float* resArr , int n) {
  auto start = std::chrono::high_resolution_clock::now();
  #pragma omp parallel for simd
  for(int i=0;i<n;i++) {
      resArr[i] = a[i]+b[i]+c[i];
  }
  auto finish = std::chrono::high_resolution_clock::now();
  return std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count();
}

// int main(int argc, char **argv){
//   int N = 100000000;

//   float* A = new float[N];
//   float* B = new float[N];
//   float* C = new float[N];

//   for (int i = 0; i < N; i++)
//   {
//     A[i] = i * 1.5f;
//     B[i] = i * 1.5f;
//     C[i] = i * 1.5f;
//   }

//   float* resultArr = new float[N];

//   std::cout << CalcSimple(A,B,C,resultArr,N) << "\n";
//   std::cout << CalcVect(A,B,C,resultArr,N) << "\n";
//   std::cout << CalcParallel(A,B,C,resultArr,N) << "\n";
//   std::cout << CalcVectParallel(A,B,C,resultArr,N) << "\n";

//   delete[] resultArr;
//   return 0;
// }
}