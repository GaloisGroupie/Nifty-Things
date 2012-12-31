makeChange(amount, denominations = []):
    ways = list();
    
    #Set number of ways to make change for 0 be 1 way
    for x in range(0, amount+1):
        ways.append(0)        
    ways[0] = 1
    
    for i in range(0, len(denominations)):
        j = denominations[i]
        while j <= amount:
            ways[j] += ways[j - denominations[i]]
            j = j + 1
    
    return ways[amount]
