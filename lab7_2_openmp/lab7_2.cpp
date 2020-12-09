#include <omp.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <cmath>

int toInt(char c)
{
  if(int(c) != 0){
    return int(c) - 48;
  } else {
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
      subSum += toInt(A[i]) * pow(10,j);
      i--;
    }
    if(isPositive){
      totalSum += subSum;
    } else {
      totalSum -= subSum;
    }
    isPositive = !isPositive;
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

  std::cout << check2(A, length) << std::endl;
  std::cout << check3(A, length) << std::endl;
  std::cout << check4(A, length) << std::endl;
  std::cout << check5(A, length) << std::endl;
  std::cout << check6(A, length) << std::endl;
  std::cout << check7(A, length) << std::endl;
  std::cout << check8(A, length) << std::endl;
  std::cout << check9(A, length) << std::endl;
  std::cout << check10(A, length) << std::endl;

  std::ofstream resultFile;
  resultFile.open("result");
  for (size_t i = 0; i < length; i++)
  {
    resultFile << A[i];
  }
  resultFile.close();

  return 0;
}