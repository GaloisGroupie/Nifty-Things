def powerSet(originalSet = list()):
  powerSetSize = 2**len(originalSet)
  powerSet = list()

  for num in range(0, powerSetSize):
    aSet = list()
    indexSpot = 0
    for x in reversed(list(bin(num)[2:])):
      if x == '1':
        aSet.append(originalSet[indexSpot])
      indexSpot += 1

    
    powerSet.append(aSet)

  return powerSet
