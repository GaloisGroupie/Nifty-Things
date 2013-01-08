def f(num):
  if num == 0:
    return 0
  elif num == 1:
    return 1
  elif num > 1:
    return f(num-1) + f(num-2)
  else:
    return -1
