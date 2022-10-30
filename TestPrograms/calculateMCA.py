from random import *
import math

# Highest common factor
def hcf(x, y):
    h = 0
    # choose the smaller number
    if x > y:
        smaller = y
    else:
        smaller = x
    for i in range(1, smaller+1):
        if((x % i == 0) and (y % i == 0)):
            h = i 
    return h

def calculate_prime_factors(N):
    prime_factors = set()
    if N % 2 == 0:
        prime_factors.add(2)
    while N % 2 == 0:
        N = N // 2
        if N == 1:
            return prime_factors
    for factor in range(3, N + 1, 2):
        if N % factor == 0:
            prime_factors.add(factor)
            while N % factor == 0:
                N = N // factor
                if N == 1:
                    return prime_factors


m = randint(1, 4294967295)

options_c = []
options_a = []
for i in range (1, 10000):
    if (hcf(i, m) == 1):
        options_c.append(i);
    
    else:
        prime_factors = calculate_prime_factors(i)
        b = True
        for n in prime_factors:
            if (i % n != 0):
                b = False
                break
        if (b):
            options_a.append(i)
            
    p_ = round((i/10000)*100)
    print(f"\r{p_}% complete...", end="\r")

print()
print()

t1 = randint(1, len(options_a))
r_a = options_a[t1]
t2 = randint(1, len(options_c))
r_c = options_a[t2]
print(f"m = {m}\nc = {r_c}\na = {r_a}")


            
            
    
    