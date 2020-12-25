#include <mpi.h>
#include <iostream>
#include <fstream>
#include <bits/stdc++.h>


#define m(x,y) matrix[x * size + y]

int rank, procesCount;

int* readMatrix(int & size) {
  size = 1;

  std::ifstream file;
  file.open("input");
  if (file.is_open()) {
      char c = 0;
      
      while(file.get(c) && c != '\n'){
        if(c == ' '){
          size++;
        }
      }
      file.close();
  } else {
    return nullptr;
  }


  int* matrix = new int[size * size];

  file.open("input");
  if (file.is_open()) {
    int i = 0;
    double a;
    while(!file.eof()) {
        file >> a;
        matrix[i] = a;
        i++;
    }
    file.close();
  }
  return matrix;
}

int getLength(int * matrix, int * isWasArrTemp, int size, int cityIdx, int currentCity = 1){

  int minLength = INT_MAX;

  int* isWasArr = new int[size];

  int* isWasArrClone = new int[size];
  for (int i = 0; i < size; i++)
  {
    isWasArrClone[i] = isWasArrTemp[i];
  }
  

  for (int i = 0; i < size; i++)
  {
    if(currentCity == 1){
      int rangeSize = (size - 1)/procesCount;
      int leftRange = rangeSize * rank + 1;
      int rightRange = rangeSize * rank + rangeSize + 1;

      if(rank == procesCount - 1){
        rightRange += (size - 1) % procesCount;
      }

      if( !(i >= leftRange && i < rightRange) ){
        continue;
      }
    }

    for (int j = 0; j < size; j++)
    {
      isWasArr[j] = isWasArrClone[j];
    }

    if(i != cityIdx){

      bool isLastCity = true;
      for (int j = 1; j < size; j++)
      {
        if(isWasArr[j] == 0){
          isLastCity = false;
          break;
        }
      }
      
      if(isLastCity){
        isWasArrTemp[0] = currentCity;

        delete[] isWasArrClone;
        return m(cityIdx,0);
      } else {
        if(isWasArr[i] != 0 || i == 0) continue;

        isWasArr[i] = currentCity;
        
        int lengthFromIdxToFinal = getLength(matrix,isWasArr,size,i,currentCity+1) + m(cityIdx,i);

        if(lengthFromIdxToFinal < minLength){
          minLength = lengthFromIdxToFinal;
          for (int j = 0; j < size; j++)
          {
            isWasArrTemp[j] = isWasArr[j];
          }
        }
      }
    }
  }

  delete[] isWasArrClone;
  return minLength;
}

int getMinTrace(int* trace,int length){
  int resultIdx = -1;

  for (int i = 0; i < procesCount; i++)
  {
    if(resultIdx == -1 || trace[(length / procesCount) * (i+1) - 1] < trace[resultIdx]){
      resultIdx = (length / procesCount) * (i+1) - 1;
    }
  }

  return resultIdx - length/procesCount + 1;
  
}

int main(int argc, char **argv)
{
  double tStart, tEnd;

  MPI_Status status;
  int errCode = MPI_Init(&argc, &argv);
  if (errCode != 0) {
      printf("Error init %d", errCode);
      return -1;
  }
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &procesCount);

  int size;
  int* matrix = readMatrix(size);

  int* isWasArr = new int[size];
  for (int i = 0; i < size; i++)
  {
    isWasArr[i] = 0;
  }
  
  if(rank == 0){
    tStart = MPI_Wtime();
  }

  int minLength = getLength(matrix,isWasArr,size,0);
  
  int* resultArr = new int[size+1];

  for (int i = 0; i < size; i++)
  {
    for (int j = 0; j < size; j++)
    {
      if(isWasArr[j] == i+1){
        resultArr[i] = j;
      }
    }
  }
  resultArr[size] = minLength;

  int* allResults;
  if(rank == 0){
    allResults = new int[procesCount*(size+1)];
  }

  MPI_Gather(resultArr, size+1, MPI_INT, allResults, size+1, MPI_INT, 0, MPI_COMM_WORLD);

  if(rank == 0){
    int resultIdx = getMinTrace(allResults,procesCount*(size+1));
    tEnd = MPI_Wtime();

    std::cout << "time: " << tEnd - tStart << "\n";
    std::cout << "trace:" << "\n";
    for (int i = resultIdx; i < resultIdx + size + 1; i++)
    {
      if(i == resultIdx + size){
        std::cout << "min length:" << "\n";
      }
      std::cout << allResults[i] << "\n";
    }
    delete[] allResults;
  }

  
  delete[] isWasArr;
  delete[] resultArr;
  MPI_Finalize();
  return 0;
}
