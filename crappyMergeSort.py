"""Function adapted from another function used for something I can't
   quite remember. Just thought it was neat that although the original
   intent of the function was not for sorting, minimal modification
   gave a sorting algorithm"""

from collections import deque
def crappyMergeSort(arrayOfStuff = list()):

  bundledArray = [arrayOfStuff[x:x+1] for x in range(0, len(arrayOfStuff))]

  arrayQueue = deque(bundledArray)
  while len(arrayQueue) > 1:
    firstArray = arrayQueue.popleft()
    secondArray = arrayQueue.popleft()
    mergedArray = pseudoMergeSortHelper(firstArray, secondArray)
    arrayQueue.append(mergedArray)

  return list(arrayQueue)[0]

def pseudoMergeSortHelper(a = list(), b = list()):
  
  mergedArray = list()
  mergedSize = len(a) + len(b)
  aIndex = 0
  bIndex = 0
  mergedIndex = 0
  while mergedIndex < mergedSize:
    if aIndex == len(a):
      while bIndex != len(b):
        mergedArray.append(b[bIndex])
        mergedIndex += 1
        bIndex += 1

    elif bIndex == len(b):
      while aIndex != len(a):
        mergedArray.append(a[aIndex])
        mergedIndex += 1
        aIndex += 1

    else:
      if a[aIndex] < b[bIndex]:
        mergedArray.append(a[aIndex])
        mergedIndex += 1
        aIndex += 1
      else:
        mergedArray.append(b[bIndex])
        mergedIndex += 1
        bIndex += 1

  return mergedArray
