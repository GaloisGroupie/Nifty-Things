def f(num):
  a = 0
  b = 1

  for x in range(0, num):
    b = a + b
    a = b - a

  return a
