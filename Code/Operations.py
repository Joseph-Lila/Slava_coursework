def fact(n):
    if n < 0:
        return 0
    if n == 0:
        return 1
    return n * fact(n - 1)

def sum(a: int , b: int) -> int:
    return a + b