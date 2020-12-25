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

int getMultResult(int* numbers, int length){
  int result = 1;
  for(int i = 0; i < length; i++){
    result *= numbers[i];
  }
  return result;
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

  double tStart, tEnd;

  const int numbersCount = getNumberCount();

  int length = numbersCount / size;

  int* numbers; 
  if(rank == 0){
    numbers = readNumbers(0,numbersCount);
    tStart = MPI_Wtime();
    for(int i = 0; i < size; i++){
      MPI_Send(&numbers[i*length], length, MPI_INT, i, 0, MPI_COMM_WORLD);
    }
  }

  int* workNumbers = new int[length];

  MPI_Recv(workNumbers, length, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
  int result = getMultResult(workNumbers,length);

  MPI_Send(&result, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);

  if(rank == 0){
    int* masterResults = new int[size];
    for(int i = 0; i < size; i++){
      MPI_Recv(&masterResults[i], 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
    }
    int finalResult = getMultResult(masterResults,size);
    delete[] masterResults;

    for (int i = length*size; i < numbersCount; i++)
    {
      finalResult *= numbers[i];
    }
    
    delete[] numbers;
    tEnd = MPI_Wtime();
    std::cout << finalResult << " time: "<< tEnd - tStart << "\n";
  }

  delete[] workNumbers;
  MPI_Finalize();

  return 0;
}
