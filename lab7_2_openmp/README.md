# Lab7_2. OpenMP
## Задание (вариант 12):
Определить, делится ли целое число А, содержащее от 1 до 1000
значащих цифр, на 2, 3, 4, 5, 6, 7, 8, 9, 10. Входные данные: целое положительное число А, записанное в файле. Количество потоков зависит числа А, и
не является параметром задачи.

Для распараллеливания взята функция проверки кратности числа на 7, где идет вычисление подсумм числа А в разных потоках
```
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
  } else {
    omp_set_num_threads(subSumArrSize);
  }

#pragma omp parallel for
  for (int i = 0; i < subSumArrSize; i++)
  {
    subSumArr[i] = 0;
    for (int j = 0; j < 3; j++)
    {
      if ((i * 3 - j) >= 0)
      {
        subSumArr[i] += toInt(A[i * 3 - j]) * pow(10, j);
      }
    }
  }

  bool isPositive = true;
  for (int i = subSumArrSize - 1; i > -1; i--)
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
 ``` 
Результат для числа 4378326326735165:
```
% 2:          0
% 3:          0
% 4:          0
% 5:          1
% 6:          0
% 7:          0
% 7 parallel: 0
% 8:          0
% 9:          0
% 10:         0
``` 
Результат для числа 73156608:
```
% 2:          1
% 3:          1
% 4:          1
% 5:          0
% 6:          1
% 7:          1
% 7 parallel: 1
% 8:          1
% 9:          1
% 10:         0
``` 
