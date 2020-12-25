import ctypes
from matplotlib import pyplot as plt

# Загрузка библиотеки
lib = ctypes.CDLL('./clib.so')


def funcConfig(func, arraySize):
    func.restype = ctypes.c_longlong
    func.argtypes = [ctypes.POINTER(ctypes.c_float * arraySize), ctypes.POINTER(ctypes.c_float * arraySize),
                     ctypes.POINTER(ctypes.c_float * arraySize), ctypes.POINTER(ctypes.c_float * arraySize),
                     ctypes.c_int]
    return func


def getAvg(arr):
    sum = 0
    for i in range(0,len(arr)):
        sum += arr[i]
    return sum / len(arr)


print('Введите максимальный размер массива')
N = int(input())
print()

print('Введите количество циклов валидации')
ValidationCount = int(input())
print()

result = {
    "No optimised": [],
    "Vect": [],
    "Parallel": [],
    "Vect + Parallel": [],
}

resultRelative = {
    "No optimised": [],
    "Vect": [],
    "Parallel": [],
    "Vect + Parallel": [],
}

X = range(N // 100, N, N // 10)

for i in X:
    calcSimple = funcConfig(lib.CalcSimple, i)
    calcVect = funcConfig(lib.CalcVect, i)
    calcParallel = funcConfig(lib.CalcParallel, i)
    calcVectParallel = funcConfig(lib.CalcVectParallel, i)

    noOptimisedSum = 0
    vectSum = 0
    parallelSum = 0
    vectParallelSum = 0

    for cicle in range(ValidationCount):
        testArr = (ctypes.c_float * i)()
        for j in range(0, i):
            testArr[j] = j * 1.5
        noOptimisedSum += calcSimple(testArr, testArr, testArr, testArr, i)
        for j in range(0, i):
            testArr[j] = j * 1.5
        vectSum += calcVect(testArr, testArr, testArr, testArr, i)
        for j in range(0, i):
            testArr[j] = j * 1.5
        parallelSum += calcParallel(testArr, testArr, testArr, testArr, i)
        for j in range(0, i):
            testArr[j] = j * 1.5
        vectParallelSum += calcVectParallel(testArr, testArr, testArr, testArr, i)

    result['No optimised'].append(noOptimisedSum / ValidationCount)
    result['Vect'].append(vectSum / ValidationCount)
    result['Parallel'].append(parallelSum / ValidationCount)
    result['Vect + Parallel'].append(vectParallelSum / ValidationCount)

    resultRelative['No optimised'].append(noOptimisedSum / ValidationCount / i)
    resultRelative['Vect'].append(vectSum / ValidationCount / i)
    resultRelative['Parallel'].append(parallelSum / ValidationCount / i)
    resultRelative['Vect + Parallel'].append(vectParallelSum / ValidationCount / i)

print('No optimised ', result['No optimised'][len(result['No optimised']) - 1])
print('Vectorized: ', result['Vect'][len(result['Vect']) - 1])
print('Parallel: ', result['Parallel'][len(result['Parallel']) - 1])
print('Vect + Parallel: ', result['Vect + Parallel'][len(result['Vect + Parallel']) - 1])

plt.figure()

plt.title('Эффективность методов оптимизации')

plt.xlabel('Размер массивов')
plt.ylabel('Время, нс')

plt.grid()

plt.plot(X, result['No optimised'],linestyle = '-',
        linewidth = 1, label='No optimised')
plt.plot(X, result['Vect'],linestyle = '--',
        linewidth = 1, label='Vectorized')
plt.plot(X, result['Parallel'],linestyle = '-.',
        linewidth = 1, label='Parallel')
plt.plot(X, result['Vect + Parallel'],linestyle = ':',
        linewidth = 1, label='Vectorized + Parallel')

plt.legend()

plt.show()
plt.savefig('result.png')

plt.figure()

plt.title('Эффективность методов оптимизации')

plt.xlabel('Размер массивов')
plt.ylabel('Время на один элемент массива, нс')

plt.grid()

plt.plot(X, resultRelative['No optimised'], label='No optimised')
plt.plot(X, resultRelative['Vect'], label='Vectorized')
plt.plot(X, resultRelative['Parallel'], label='Parallel')
plt.plot(X, resultRelative['Vect + Parallel'], label='Vectorized + Parallel')

plt.legend()

plt.show()
plt.savefig('resultRelative.png')
