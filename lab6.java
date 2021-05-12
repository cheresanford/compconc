class Misteriosa extends Thread{
    private int id, nthreads;
    private int[] vetor;
    
    //construtor da classe	
    Misteriosa(int nthreads, int id, int[] vetor)
    {
        this.id = id;
        this.nthreads = nthreads;
        this.vetor = vetor;
    }
    //tarefa
    public void run()
    {
        //laco para incrementar o vetor
        for(int i = id; i < this.vetor.length; i += this.nthreads) {
            this.vetor[i]++;
        }
    }


}

class Main {
    public static void main(String[] args){
	
	//recebendo os parametros da linha de comando
        int n = Integer.parseInt(args[0]);
        int nthreads = Integer.parseInt(args[1]);
        
        Thread threads[] = new Thread[nthreads];
        //garantindo que nthreads eh sempre menor ou igual ao numero de elementos do vetor
        if(nthreads > n)
        {
            nthreads = n;
        }
        int[] vetor = new int[n];
	
	//inicializacao do vetor
	System.out.println("Vetor inicial:");
        for(int i = 0; i < vetor.length; i++)
        {
            vetor[i] = i + 3;
            System.out.print("[" + vetor[i] + "]" + " ");
        }
        System.out.print("\n\n");

	//criando as threads
        for(int i = 0; i < nthreads; i++){
            threads[i] = new Misteriosa(nthreads, i , vetor);
            threads[i].start();
        }

        //esperando o termino das threads
        for(int i = 0; i < threads.length; i++) 
        {
            try { 
                threads[i].join(); 
            } catch (InterruptedException e)
            { 
                return; 
            }
        }
        
	System.out.println("Vetor final [incrementado]:");
        for(int i = 0; i < vetor.length; i++)
        {
            System.out.print("[" + vetor[i] + "]" + " ");
        }
        System.out.print("\nFim do programa.\n");

    }
}
