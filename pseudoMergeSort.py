from collections import deque
def pseudoMergeSort(sortedArrays = list()):

  arrayQueue = deque(sortedArrays)
  while len(arrayQueue) > 1:
    firstArray = arrayQueue.popleft()
    secondArray = arrayQueue.popleft()
    mergedArray = pseudoMergeSortHelper(firstArray, secondArray)
    arrayQueue.append(mergedArray)

  return list(arrayQueue)

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
