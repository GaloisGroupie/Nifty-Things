def getAllStringPermutations(originalString):
  stringList = list(originalString)
  stringPermutationSet = ['']
  
  for letter in stringList:
    tempSet = list()

    while len(stringPermutationSet) != 0:
      stringToBuild = stringPermutationSet.pop()
      
      if len(stringToBuild) == 0:
        tempSet.append(letter)
      else:
        for x in range(0, len(stringToBuild)):
          builtString = stringToBuild[:x] + letter + stringToBuild[x:]
          tempSet.append(builtString)

    stringPermutationSet = tempSet

  return stringPermutationSet
