def merge_sort(inputList):

  """If the input list is length 1 or 0, just return
   the list because it is already sorted"""
  inputListLength = len(inputList)
  if inputListLength == 0 or inputListLength == 1:
    return inputList

  """If the list size is greater than 1, recursively break
     it into 2 pieces and we will hit the base case of 1.
     We then start merging upwards"""
  return merge(merge_sort(inputList[:inputListLength//2]),
               merge_sort(inputList[inputListLength//2:]))

"""Takes 2 sorted lists and combines them together to 
   return 1 sorted list"""
def merge(l1,l2):
  l1Index = 0
  l2Index = 0

  #If either list is empty return the other 
  if len(l1) == 0:
    return l2
  if len(l2) == 0:
    return l1

  mergedSortedList = []
  while True:
    if l1[l1Index] <= l2[l2Index]:
      mergedSortedList.append(l1[l1Index])
      l1Index += 1
    elif l2[l2Index] < l1[l1Index]:
      mergedSortedList.append(l2[l2Index])
      l2Index += 1

    if not (l1Index < len(l1)):
      mergedSortedList.extend(l2[l2Index:])
      return mergedSortedList
    elif not (l2Index < len(l2)):
      mergedSortedList.extend(l1[l1Index:])
      return mergedSortedList

    print len(l1), l1Index, len(l2), l2Index
    
