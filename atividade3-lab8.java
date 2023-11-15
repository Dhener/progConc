/* Disciplina: Programacao Concorrente */
/* Aluno: Dhener Rosemiro Silva */
/* Laboratório: 8 */
/* Codigo: Criando um pool de threads em Java */

import java.util.LinkedList;

//-------------------------------------------------------------------------------
//Classe que define um pool de threads 
//!!! Documentar essa classe !!!
class FilaTarefas {
    private final int nThreads;  //--Atributo que indica o número de threads no pool
    private final MyPoolThreads[] threads; //--Atributo que indica um array de de objetos MyPoolThreads que representam as threads do pool
    private final LinkedList<Runnable> queue;  //--Atributo que indica uma fila (LinkedList) que armazena as tarefas a serem executadas
    private boolean shutdown;  //--Atributo que indica uma flag indicando se o pool deve ser encerrado

    public FilaTarefas(int nThreads) {
        //--Inicializa os atributos e cria as threads 
        this.shutdown = false;
        this.nThreads = nThreads;
        queue = new LinkedList<Runnable>();
        threads = new MyPoolThreads[nThreads];
        for (int i=0; i<nThreads; i++) {
            //--Criando as Threads
            threads[i] = new MyPoolThreads();
            threads[i].start();
        } 
    }

    public void execute(Runnable r) {
        synchronized(queue) {
            if (this.shutdown) return;
            queue.addLast(r);  //--Adiciona uma tarefa à fila de execução
            queue.notify();   //--Notifica uma thread em espera (se houver) que há uma nova tarefa. 
        }
    }
    
    public int shutdown() {
        int total = 0; //--Variável que contabiliza os resultados encontrados de cada thread
        synchronized(queue) {
            this.shutdown=true;  //--Sinaliza que o pool deve ser encerrado.
            queue.notifyAll();  //--Notifica todas as threads (caso estejam em espera) que o pool está encerrando.
        }
        for (int i=0; i<nThreads; i++) {
          //--Aguarda a conclusão de todas as threads.
          try { 
            threads[i].join(); 
            total = total + threads[i].total; //--A cada vez que uma thread finaliza, o valor é contabilizado da quantidade de números primos encontrados por ela
          } catch (InterruptedException e) { return 0; }
        }
        return total;
    }

    private class MyPoolThreads extends Thread {
       //--Implementa a lógica de execução de cada thread no pool.
       int total; //--Variável particular de cada thread contabilizando a quantidade de números primos encontrados
       public MyPoolThreads(){
        total = 0; 
       }
       public void run() {
         Runnable r;
         //--Fica em um loop infinito, aguardando tarefas para executar.
         while (true) {
           synchronized(queue) {
             while (queue.isEmpty() && (!shutdown)) {
               try { queue.wait(); }
               catch (InterruptedException ignored){}
             }
             if (queue.isEmpty() && shutdown) return;  
             r = (Runnable) queue.removeFirst(); //--Quando há uma tarefa na fila, a retira.
           }
           try { 
            r.run(); //--Executa a tarefa.
            total = total + ((CountImpar) r).count; //--Após a execução contabiliza com 1 ou 0 unidade se a thread encontrou um primo ou não, respectivamente
           }
           catch (RuntimeException e) {}
         } 
       } 
    } 
}
//-------------------------------------------------------------------------------

//--PASSO 1: cria uma classe que implementa a interface Runnable 
class CountImpar implements Runnable {
   int number; //--Variável que armazenará o número a ser verificado
   int count; //--Variável que armazenará o valor do resultado do método ehPrimo()
   public CountImpar(int n) { number = n; count = 0; }

   //--Método que verificará se o número é ou não primo. Retornando 1 se for ou 0 se não for
   public int ehPrimo(int n){
    int i;
    if (n<=1) return 0;
    if (n==2) return 1;
    if (n%2==0) return 0;
    for (i=3; i<Math.sqrt(n)+1; i+=2)
        if(n%i==0) return 0;
    return 1;
   }

   //--metodo executado pela thread
   public void run() {
    count = ehPrimo(number);
   }  
}

//Classe da aplicação (método main)
class MyPool {
    private static final int NTHREADS = 10;
    private static int result_teste = 0;

    //--Método que verificará a corretude do problema
    public static void countNumberOfPrimes(Runnable r, int n){
      result_teste = result_teste + ((CountImpar) r).ehPrimo(n);
    } 

    public static void main (String[] args) {
      int q = 0;
      if(args.length == 1){
        try{
         q =  Integer.parseInt(args[0]);
        } catch(ArrayIndexOutOfBoundsException e){
          System.out.println("Use java Teste <quantidade de números>");
        }
      }

      //--PASSO 2: cria o pool de threads
      FilaTarefas pool = new FilaTarefas(NTHREADS); 
      int result;
      
      //--PASSO 3: dispara a execução dos objetos runnable usando o pool de threads
      for (int i = 0; i <= q; i++) {
        Runnable r = new CountImpar(i);
        pool.execute(r);
        countNumberOfPrimes(r, i);
      }

      //--PASSO 4: esperar pelo termino das threads
      result = pool.shutdown();
      System.out.println("A quantidade de numeros primos de 1 ate " + q + ": " + result);
      System.out.println(result == result_teste); //--Retorna true se a corretude funcionar
      System.out.println("Terminou");
   }
}
