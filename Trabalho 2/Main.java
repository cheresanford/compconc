/**
 * Classe principal para inicializacao basica de algumas estruturas de dados e tarefas iniciais.
 */
import java.util.Iterator;
import java.util.ArrayList;
import java.util.concurrent.ThreadLocalRandom;

/**
 * Classe record para armazenar cada leitura individual de um sensor.
 * Recebe a temperatura lida, o identificador do sensor que a leu, e armazena o ID da leitura.
 */
record LeituraIndividual(int temperatura, int idSensor, int idLeitura) {

    //Getters e setters da classe

    public int getIdSensor() {
        return this.idSensor;
    }

    public int getIdLeitura() {
        return this.idLeitura;
    }

    public int getTemperatura() {
        return this.temperatura;
    }


}

/**
 * Classe que define os sensores
 */

class Sensor extends Thread{
    private int Identificador;
    private int idLeitura = 0;
    private Gerente gerente;


    /**
     * Construtor da classe
     * @param Identificador inteiro que sera utilizado como identificador do sensor
     * @param gerente classe que ira gerenciar o padrao leitores/escritores
     */
    Sensor(int Identificador, Gerente gerente){
        this.gerente = gerente;
        this.Identificador = Identificador;
    }


    public void run(){
        int temperatura;

        while(true){
            try {
                sleep(1000);
            }
            catch (InterruptedException e) {
                System.out.println(e.getMessage());
            }
            temperatura = ThreadLocalRandom.current().nextInt(25,40); //gera um numero aleatorio de 25 a 40.
            checaTemperatura(temperatura);
            this.idLeitura++;

        }
    }

    /**
     *  Funcao que simula a checagem de temperatura pelo sensor, escrevendo-a no espaco de memoria compartilhado entre os outros sensores e atuadores, obedecendo a logica
     *  das especificacoes do trabalho.
     * @param temperatura temperatura gerada em ThreadLocalRandom.current().nextint();
     */
    private void checaTemperatura(int temperatura) {
        //se a temperatura for maior que 30, procede para o armazenamento.
        if(temperatura > 30){
            LeituraIndividual leitura = new LeituraIndividual(temperatura, this.Identificador, this.idLeitura);
            gerente.insereLeitura(leitura);
        }
        else {
            //valores menores ou iguais a 30 podem ser ignorados.
            System.out.printf("Sensor #%d - %d°C lido - Menor ou igual a 30! Ignorando a escrita.\n", this.Identificador, temperatura);
        }
    }

    /**
     * Getter da classe
     * @return Identificador do sensor
     */
    public int getIdentificador(){
        return this.Identificador;
    }

}

/**
 * Classe que define os atuadores do sistema.
 */
class Atuador extends Thread{
    private Sensor sensor;
    private Gerente gerente;

    /**
     * Construtor da classe
     * @param sensor variavel para o sensor atrelado ao atuador
     * @param gerente classe que ira gerenciar o padrao leitores/escritores
     */
    Atuador(Sensor sensor, Gerente gerente){
        this.gerente = gerente;
        this.sensor = sensor;
    }

    /**
     * Funcao que imprime a temperatura media
     * @param id identificador do sensor
     * @param temp temperatura media
     */
    private void printaTemperaturaMedia(int id, double temp) {
        if(temp != 0d) System.out.printf("Atuador #%d: Temperatura media do sensor: %.4f°C\n", id, temp);
    }
    private void verificaCondicaoNormal(boolean teveAlerta, ArrayList<LeituraIndividual> lista) {
        if (!teveAlerta && lista.size() == 15) {
            System.out.printf("Atuador #%d: Condicoes normais de temperatura.\n", sensor.getIdentificador());
        }
    }

    public void run(){


        while(true){
            try{
                sleep(2000);
            } catch (InterruptedException e) {
                System.out.println("Erro na funcao sleep");
            }

            ArrayList<LeituraIndividual> leiturasIndividuais = this.gerente.getLeiturasIndividuais(this.sensor);
            Iterator<LeituraIndividual> it = leiturasIndividuais.iterator();

            int contadorTemperaturaAlertaAmarelo = 0;

            //temperatura do alerta vermelho > 35

            int contadorTemperaturaAlertaVermelho = 0;
            boolean teveAlerta = false;

            //avaliando as ultimas 15 leituras
            for( LeituraIndividual leitura : leiturasIndividuais) {
                if(leitura.getTemperatura() > 35) {
                    contadorTemperaturaAlertaAmarelo++;
                    contadorTemperaturaAlertaVermelho++;
                    if (contadorTemperaturaAlertaVermelho == 5) {
                        System.out.printf("Atuador #%d: Alerta vermelho! Weee wooo weee wooo!\n", sensor.getIdentificador());
                        contadorTemperaturaAlertaVermelho = 0;
                        contadorTemperaturaAlertaAmarelo = 0;
                        teveAlerta = true;
                    }
                    if(contadorTemperaturaAlertaAmarelo == 5) {
                        System.out.printf("Atuador #%d: Alerta amarelo!\n", sensor.getIdentificador());
                        contadorTemperaturaAlertaAmarelo = 0;
                        teveAlerta = true;
                    }
                }
                else {
                    contadorTemperaturaAlertaVermelho = 0;
                }
            }
            verificaCondicaoNormal(teveAlerta, leiturasIndividuais);
            printaTemperaturaMedia(sensor.getIdentificador(), gerente.getTemperaturaMedia(sensor));
        }
    }
}



public class Main {

    /**
     *
     * @param numeroDeSensores Numero de sensores que serao utilizados na execucao atual, conforme passado pela linha de comando.
     * @return Retorna 1 caso a inicializacao das threads e do programa tenha dado certo.
     */
    private static int iniciaTarefa(int numeroDeSensores) {
        Gerente gerente = new Gerente();
        Thread atuadores[] = new Thread[numeroDeSensores];
        Thread sensores[] = new Thread[numeroDeSensores];

        for(int i = 0; i < numeroDeSensores; i++){
            Sensor sensor = new Sensor(i, gerente);
            sensores[i] = sensor;
            atuadores[i] = new Atuador(sensor, gerente);
        }
        //iniciando as threads
        for(int i = 0; i < numeroDeSensores; i++){
            sensores[i].start();
            atuadores[i].start();
        }
            return 1;
        }


    public static void main(String[] args){
        int numeroDeSensores = 0;

        /*
        Processamento da entrada do programa pela linha de comando
         */
        try {
            numeroDeSensores = Integer.parseInt(args[0]);
        }
        catch(NumberFormatException e) {
            System.out.println("Insira o numero de threads.");
        }
        catch(ArrayIndexOutOfBoundsException e) {
            System.out.println("Insira exatamente 1 argumento na entrada, por favor.");
        }
        if (args.length != 1)
            throw new IllegalArgumentException("Favor inserir exatamente 1 argumento de entrada.");

        if(iniciaTarefa(numeroDeSensores) == 1) {
            System.out.println("--- SISTEMA DE MONITORAMENTO DE TEMPERATURA INICIADO COM SUCESSO! ---");
        }
    }
}
