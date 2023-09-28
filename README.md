# Programação Concorrente

## Atividade 3 do Laboratório 4
### Comentário sobre os resultados

Com teste da main_list_rwlock, main_list e atividade3-lab4 para 1 thread foi respectivamente de 2.14 s, 2.08 s e 2.45 s em média.
Com teste da main_list_rwlock, main_list e atividade3-lab4 para 2 thread foi respectivamente de 12.38 s, 10.93 s e 13.62 s em média.
Com teste da main_list_rwlock, main_list e atividade3-lab4 para 4 thread foi respectivamente de 20.46 s, 19.84 s e 29.61 s em média.

Com a implementação da pthread_rwlock_wrlock na main_list_rwlock o tempo foi parecido com a execução atividade3-lab4 para 1 e 2 threads, o que era o esperado pois a ideia de implementação é praticamente a mesma, porém com 4 threads o resultado não deu como o esperado. Outro resultado que não foi como esperado foi na execução da main_list que sua execução seria praticamente sequencial e os tempos foram menores. 
