# Lab6. OpenMP
## Задание (вариант 12):
Охранное агентство разработало новую систему управления электронными замками. Для открытия двери клиент обязан произнести произвольную фразу из 25 слов. В этой фразе должно встречаться заранее оговоренное слово, причем только один раз. Требуется создать OpenMPприложение, управляющее замком. Потоки должны осуществлять сравнение
параллельно по 5 слов. 
## Результат:
### Без многопоточности:
 ```
 No Secret word from 1 to 5
 No Secret word from 6 to 10
 No Secret word from 11 to 15
 No Secret word from 16 to 20
 Secret word from 21 to 25
```
### С многопоточностью:
 ```
 No Secret word from 16 to 20
 No Secret word from 11 to 15
 Secret word from 21 to 25
 No Secret word from 1 to 5
 No Secret word from 6 to 10
 ```
