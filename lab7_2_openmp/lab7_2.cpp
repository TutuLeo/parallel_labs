#include <omp.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <cmath>

int toInt(char c)
{
  if (int(c) != 0)
  {
    return int(c) - 48;
  }
  else
  {
    return 0;
  }
}

bool check2(char *A, int length)
{
  return !(toInt(A[length - 1]) % 2);
}

bool check3(char *A, int length)
{
  int sum = 0;
  for (size_t i = 0; i < length; i++)
  {
    sum += toInt(A[i]);
  }

  return !(sum % 3);
}

bool check4(char *A, int length)
{
  return !((toInt(A[length - 2]) * 10 + toInt(A[length - 1])) % 4);
}

bool check5(char *A, int length)
{
  return !(toInt(A[length - 1]) % 5);
}

bool check6(char *A, int length)
{
  return check2(A, length) && check3(A, length);
}

bool check7(char *A, int length)
{
  int totalSum = 0;
  bool isPositive = true;
  for (int i = length - 1; i > -1;)
  {
    int subSum = 0;
    for (size_t j = 0; j < 3; j++)
    {
      subSum += toInt(A[i]) * pow(10, j);
      i--;
    }
    if (isPositive)
    {
      totalSum += subSum;
    }
    else
    {
      totalSum -= subSum;
    }
    isPositive = !isPositive;
  }
  if (totalSum < 7)
  {
    return false;
  }
  return !(totalSum % 7);
}

bool check7parallel(char *A, int length)
{
  int totalSum = 0;

  int subSumArrSize = length / 3;
  if (length % 3)
  {
    subSumArrSize++;
  }
  int *subSumArr = new int[subSumArrSize];

  if (omp_get_max_threads() < subSumArrSize)
  {
    omp_set_num_threads(omp_get_max_threads());
  }
  else
  {
    omp_set_num_threads(subSumArrSize);
  }

  // #pragma omp parallel for
  for (int i = 0; i < subSumArrSize; i++)
  {
    subSumArr[i] = 0;
    for (int j = 0; j < 3; j++)
    {
      if ((length - 1 - i * 3 - j) >= 0)
      {
        subSumArr[i] += toInt(A[length - 1 - i * 3 - j]) * pow(10, j);
      }
    }
  }

  bool isPositive = true;
  for (int i = 0; i < subSumArrSize; i++)
  {
    if (isPositive)
    {
      totalSum += subSumArr[i];
    }
    else
    {
      totalSum -= subSumArr[i];
    }
    isPositive = !isPositive;
  }
  if (totalSum < 7)
  {
    return false;
  }

  return !(totalSum % 7);
}

bool check8(char *A, int length)
{
  return !((toInt(A[length - 3]) * 100 + toInt(A[length - 2]) * 10 + toInt(A[length - 1])) % 8);
}

bool check9(char *A, int length)
{
  int sum = 0;
  for (size_t i = 0; i < length; i++)
  {
    sum += toInt(A[i]);
  }
  return !(sum % 9);
}

bool check10(char *A, int length)
{
  return !(toInt(A[length - 1]) % 10);
}

int main(int argc, char **argv)
{
  std::ifstream inputStream("A", std::ifstream::binary);

  char *A;
  int length;
  if (inputStream)
  {
    inputStream.seekg(0, inputStream.end);
    length = inputStream.tellg();
    inputStream.seekg(0, inputStream.beg);

    A = new char[length];

    std::cout << "Reading " << length << " characters... " << std::endl;
    // read data as a block:
    inputStream.read(A, length);
  }
  std::ofstream resultFile;
  resultFile.open("result");

  resultFile << "% 2:          " << check2(A, length) << std::endl;
  resultFile << "% 3:          " << check3(A, length) << std::endl;
  resultFile << "% 4:          " << check4(A, length) << std::endl;
  resultFile << "% 5:          " << check5(A, length) << std::endl;
  resultFile << "% 6:          " << check6(A, length) << std::endl;
  resultFile << "% 7:          " << check7(A, length) << std::endl;
  resultFile << "% 7 parallel: " << check7parallel(A, length) << std::endl;
  resultFile << "% 8:          " << check8(A, length) << std::endl;
  resultFile << "% 9:          " << check9(A, length) << std::endl;
  resultFile << "% 10:         " << check10(A, length) << std::endl;

  resultFile.close();

  return 0;
}