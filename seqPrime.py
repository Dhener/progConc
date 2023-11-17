import time
import math
import sys

def isPrime(n):
    if n <= 1:
        return 0
    if n == 2:
        return 1
    if n%2 == 0:
        return 0
    for i in range(3,int(math.sqrt(n)+1),2):
        if n%i == 0:
            return 0
    return 1

def quantityOfPrimes(n):
    total = 0
    for i in range(1,n):
        total += isPrime(i)
    return total

if __name__ == '__main__':
    if len(sys.argv) != 2:
        #<quantidade de números> é a quantidade de números primos que deseja verificar do 0 até <quantidade de números>
        print(f"Usa: python {sys.argv[0]} <quantidade de números>")
        exit()

    #Inicia a contabilização do tempo
    start = time.time()
    number = int(sys.argv[1]) + 1
    print(quantityOfPrimes(number))
    end = time.time()
    #Finaliza a contabilização do tempo

    print('work took {} seconds'.format(end - start))
