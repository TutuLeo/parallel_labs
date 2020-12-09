#include <omp.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <math.h>

const int N = 10;
double a[N];
double b[N];

void sequentialFor()
{
  a[0] = 1;
  double w;

  int i;
  for (i = 1; i < N; i++)
  {
    w = sin(double(i) / N * 3.14);
    a[i] = w * a[i - 1];
  }

  std::cout << "Sequential Loop:" << std::endl;
  for (int j = 0; j < N; j++)
  {
    std::cout << a[j] << std::endl;
  }
}

void parrallelFor()
{
  double b[N];
  b[0] = 1;
  double w;

  int i;

#pragma omp parallel for
  for (i = 1; i < N; i++)
  {
    w = sin(double(i) / N * 3.14);
    b[i] = w * b[i - 1];
  }

  std::cout << "Parallel Loop:" << std::endl;
  for (int j = 0; j < N; j++)
  {
    std::cout << b[j] << std::endl;
  }
}

void parrallelForCorrect()
{
  double c[N];
  c[0] = 1;
#pragma omp parallel for  
  for (int j = 1; j < N; j++)
  {
    c[j] = 0.0000;
  }

  double w;

  int i;
#pragma omp parallel for
  for (i = 1; i < N; i++)
  {
    w = sin(double(i) / N * 3.14);
    while (c[i - 1] < 0.000000000000000000000001){}
    c[i] = w * c[i - 1];
  }

  std::cout << "Correct Parallel Loop:" << std::endl;
  for (int j = 0; j < N; j++)
  {
    std::cout << c[j] << std::endl;
  }
}

int main(int argc, char **argv)
{
  std::cout << "N = " << N << std::endl;
  sequentialFor();
  std::cout << std::endl;
  parrallelFor();
  std::cout << std::endl;
  parrallelForCorrect();
  return 0;
}
