#exemplo de uso de pool de processos em Python

from multiprocessing.pool import Pool
import time
import math
import seqPrime
import sys

#funcao que sera executada de forma concorrente
def task(n):
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

if __name__ == '__main__':
    if len(sys.argv) != 2:
        #<quantidade de números> é a quantidade de números primos que deseja verificar do 1 até <quantidade de números>
        print(f"Usa: python {sys.argv[0]} <quantidade de números>")
        exit()
    
    #Inicia a contabilização do tempo
    start = time.time()

    pool = Pool() #por default, cria um processo distinto para cada processador da maquina
    number = int(sys.argv[1]) + 1
    numbers = list(range(1,number))
    results = pool.map(task, numbers) #map aceita uma funcao e um objeto iteravel, o pool pega cada valor do objeto iteravel e passa para um processo disponivel no poll que aplica a funcao sobre esse valor
    results = sum(results) #realiza a soma dos elementos em results
    print(results)

    end = time.time()
    #Finaliza a contabilização do tempo

    #função que executa sequencialmente para verificar a corretude do problema
    r = seqPrime.quantityOfPrimes(number)
    print(results == r)

    print('work took {} seconds'.format(end - start))
