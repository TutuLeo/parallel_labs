#include <mpi.h>
#include <iostream>
#include <fstream>

int getNumberCount(){
  std::ifstream inputStream("input", std::ifstream::binary);

  int numberCount = 1;
  if (inputStream)
  {
    char c;
    
    while(inputStream.get(c)){
      if(c == ' '){
        numberCount++;
      }
    }
    inputStream.close();
    return numberCount;
  }
  return -1;
}

int* readNumbers(int start, int end){
  int* numbers = new int[end-start];

  std::ifstream inputStream("input", std::ifstream::binary);
  if (inputStream)
  {
    int num;
    int i = 0;
    while(!inputStream.eof()) {
        inputStream >> num;
        if(i >= start && i < end)
        numbers[i-start] = num;

        i++;
    }
    inputStream.close();
  }

  return numbers;
}

void getMultResults(int* numbers, int length, int* results,int startResultIdx, int savedVal = 1){
  results[startResultIdx] = numbers[0]*savedVal;
  for(int i = 1; i < length; i++){
    results[i + startResultIdx] = results[i + startResultIdx -1]* numbers[i];
  }
  return;
}

void saveToFile(int* results, int length){
  std::ofstream resultFile;
  resultFile.open("result");
  for(int i = 0; i < length; i++){
    if(i != length - 1){
      resultFile << results[i] << " ";
    } else {
      resultFile << results[i];
    }
  }
  resultFile.close();
}

int main(int argc, char **argv)
{
  int rank, size;
  MPI_Status status;
  int errCode = MPI_Init(&argc, &argv);
  if (errCode != 0) {
      printf("Error init %d", errCode);
      return -1;
  }
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  const int numbersCount = getNumberCount();

  int start = rank * (numbersCount / size);
  int end = start + (numbersCount / size);
  if(rank == size-1){
    end += numbersCount % size;
  }

  int length = end - start;

  int* accomulateResults = new int[(numbersCount/size)*rank + length];

  int* numbers = readNumbers(start,end);

  if(rank != 0){
    MPI_Recv(accomulateResults, (numbersCount/size)*rank, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, &status);
    getMultResults(numbers,length, accomulateResults, (numbersCount/size)*rank, accomulateResults[start-1]);
  } else {
    getMultResults(numbers,length, accomulateResults, (numbersCount/size)*rank);
  }

  if(rank != size - 1){
    MPI_Send(accomulateResults, (numbersCount/size)*rank + length, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
  }

  std::cout << length << "\n";
  if(rank == size - 1){
    saveToFile(accomulateResults,(numbersCount/size)*rank + length);
  }

  delete[] numbers;
  delete[] accomulateResults;
  MPI_Finalize();
  return 0;
}
