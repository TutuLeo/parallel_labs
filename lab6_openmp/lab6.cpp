#include <omp.h>
#include <stdio.h>
#include <iostream>
#include <string>
int main(int argc, char** argv)
{
  const std::string SECRET_WORD = "SecretWord";
  std::string words[] = {
    "1","2","3","4","5",
    "6","7","8","9","10",
    "11","12","13","14","15",
    "16","17","18","19","20",
    "21","22","23","24","SecretWord"
  };

  int i;
  for(i = 0; i<25;i+=5){
    if(words[i] == SECRET_WORD || 
      words[i+1] == SECRET_WORD ||
      words[i+2] == SECRET_WORD || 
      words[i+3] == SECRET_WORD || 
      words[i+4] == SECRET_WORD)
    {
      printf("Secret word from %d to %d\n", i+1,i+5);
    } 
    else 
    {
      printf("No Secret word from %d to %d\n", i+1,i+5);
    }
  }
  std::cout << std::endl;

  if(omp_get_max_threads() > 4){
    omp_set_num_threads(4);
  }

  #pragma omp parallel for
    for(i = 0; i<25;i+=5){
      if(words[i] == SECRET_WORD || 
        words[i+1] == SECRET_WORD ||
        words[i+2] == SECRET_WORD || 
        words[i+3] == SECRET_WORD || 
        words[i+4] == SECRET_WORD)
      {
        #pragma omp critical
        {
          printf("Secret word from %d to %d\n", i+1,i+5);
        }
      } 
      else 
      {
        #pragma omp critical
        {
          printf("No Secret word from %d to %d\n", i+1,i+5);
        }
      }
    }
 return 0;
} 