import java.lang.reflect.Array;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Iterator;


public class Gerente {
    final public int tamanhoMemoria = 60;
    private final LeituraIndividual[] leituras = new LeituraIndividual[tamanhoMemoria];
    public int pos = -1;

    private int escrita = 0;
    private int querEscrita = 1;
    private int qtdAtuadores = 0;


    //A parte a seguir implementa o padrão leitor-escritor no sistema.

    /**
     * Gerencia a entrada dos sensores no padrão leitor-escritor
     * @param leitura uma leitura individual do sensor
     */
    public synchronized void entraSensor(LeituraIndividual leitura){

        System.out.printf("Sensor #%d - Quer gravar a temperatura de %d°C.\n", leitura.getIdSensor(), leitura.getTemperatura());

        while(this.escrita == 1 || this.qtdAtuadores > 0) {
            try {
                System.out.printf("Sensor #%d - Entrou em estado de espera.\n", leitura.getIdSensor());
                this.querEscrita = 1;
                wait();
            }
            catch(InterruptedException e){
                System.out.println("Erro na escrita!");
            }
        }

        this.escrita = 1;
    }

    /**
     * Gerencia a saída dos sensores do padrão leitor-escritor
     * @param leitura uma leitura individual do sensor
     */
    public synchronized void saiSensor(LeituraIndividual leitura){
        this.querEscrita = 0;
        this.escrita = 0;

        System.out.printf("Sensor #%d - Terminou de gravar a temperatura %d°C.\n",
                leitura.getIdSensor(),
                leitura.getTemperatura());

        notifyAll();
    }

    /**
     * Gerencia a entrada dos atuadores do padrão leitor-escritor
     * @param sensor sensor
     */
    public synchronized void entraAtuador(Sensor sensor){
        System.out.printf("Atuador #%d - Requisita entrada.\n", sensor.getIdentificador());

        while(this.querEscrita == 1) {
            try {
                System.out.printf("Atuador #%d - Entrou em estado de espera.\n", sensor.getIdentificador());
                wait();
            }
            catch(InterruptedException e) {
                System.out.println("Erro na escrita!");
            }
        }
        this.qtdAtuadores++;
    }

    /**
     * Gerencia a saída dos atuadores do padrão leitor-escritor
     * @param sensor sensor
     */
    public synchronized void saiAtuador(Sensor sensor){
        System.out.printf("Atuador #%d - Tarefa concluida.\n", sensor.getIdentificador());
        this.qtdAtuadores--;
        if(this.qtdAtuadores == 0){
            notify();
        }
    }

    /**
     * Verifica se uma dada LeituraIndividual não é nula
     * @param leitura LeituraIndividual
     * @return Retorna true caso a LeituraIndividual não seja nula. Falso caso contrário
     */
    boolean leituraIsNotNull(LeituraIndividual leitura) {
        return leitura != null;
    }

    /**
     * Funcao para conferir se o ID do sensor passado como parametro confere com o id armazenado em leituras[].
     * @param sensor sensor para ser comparado com os armazenados em leituras[]
     * @param index indice para acesso do array
     * @return Retorna a Leitura Individual do sensor de um dado ID, caso os IDs confiram. Retorna null caso
     */
    public LeituraIndividual confereLeitura(Sensor sensor, int index) {
        if(this.leituras[index].getIdSensor() == sensor.getIdentificador()) {
            LeituraIndividual leitura = new LeituraIndividual(leituras[index].getTemperatura(), leituras[index].getIdSensor(), leituras[index].getIdLeitura());
            return leitura;
        }
        else return null;
    }

    /**
     * Método que retorna as últimas 15 leituras do sensor
     * @param sensor sensor que se deseja obter as informações
     * @return uma ArrayList com as 15 leituras individuais
     */
    public ArrayList<LeituraIndividual> getLeiturasIndividuais(Sensor sensor) {
        this.entraAtuador(sensor);
        ArrayList<LeituraIndividual> lista = new ArrayList<>();

        int i = tamanhoMemoria - 1;
        //laco para percorrer as ultimas 15 leituras se existirem.
        while(lista.size() < 15) {
            if(leituraIsNotNull(this.leituras[i])) {
                LeituraIndividual leit = confereLeitura(sensor, i);
                if (leituraIsNotNull(leit)) { lista.add(leit); }


                //Lendo do mais velho para o mais novo . ><'
                if(i == 0) {
                    i = this.tamanhoMemoria;
                }
                i--;
            }
            else {
                break;
            }
        }
        this.saiAtuador(sensor);
        return lista;
    }


    /**
     * Funcao que retorna a temperatura media do sensor
     * @param sensor sensor em que sera avaliada a temperatura media .
     * @return retorna a temperatura media
     */
    public synchronized double getTemperaturaMedia(Sensor sensor){
        //Iterable<LeituraIndividual> iteravel = Arrays.asList(leituras);
        Iterator<LeituraIndividual> it = Arrays.asList(this.leituras).listIterator();
        int denominador = 0;
        double somaTemperatura = 0d;
        int qtdLeituras = leituras.length;

        for(int cont  = 0 ; cont < qtdLeituras ; cont++) {
            if(leituraIsNotNull(this.leituras[cont])) {
                if(leituras[cont].getIdSensor() == sensor.getIdentificador()) {
                    denominador++;
                    somaTemperatura += leituras[cont].getTemperatura();
                }
            }
            else {
                break;
            }
        }
        return((denominador == 0d) ?  0d : somaTemperatura / denominador);
    }

    /**
     * Insere uma LeituraIndividual na array de leituras individuais, atributo de Gerente.
     * @param leitura uma LeituraIndividual que deseja ser inserida
     */
    public void insereLeitura(LeituraIndividual leitura){

        this.entraSensor(leitura);

        this.pos = (this.pos + 1) % 60;
        this.leituras[this.pos] = leitura;

        this.saiSensor(leitura);

    }

}
