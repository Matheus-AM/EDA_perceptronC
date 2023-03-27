#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
/// MATHEUS AMARAL MOREIRA - 17/0080307

///
/// 1 e 2 sao in-out
/// 3 e out (pode ser in em emergencia)

///            .inicio                 .fimD                   .fimA
/// minha fila D> -> D> -> D> -> D> -> D> -> A> -> A> -> A> -> A> -> NULL


int RANDOMIZER = 0; ///gera a aleatoriedade continua
int ORDEM_ENUMERADA_ALEATORIA = 0; ///Faz com que a aleatoriedade controlada nao perca o fluxo


struct Avioes{
    char codigo[7];
    char modo;
    int combustivel;
    struct Avioes *prox;
};

typedef struct Avioes Aviao;

///insere o elemento sorteado no final da fila
Aviao* inserirFinal(Aviao *, int, char);

///essa funcao gera valores aleatorios num alcance numInferior <= range < numSuperior
int geraValoresAleatorios(int, int);

///return NULL
void *gerarListaVazia();

///responsavel por sortear e gerar as instancias iniciais de voos
void inicializaVoos(Aviao **, Aviao **, int, char);

///printa um elemento, seja decolagem ou aterrisagem
void printarLista(Aviao *);

///libera o primeiro elemento da lista, ja que lista e FIFO
Aviao* liberarInicio(Aviao *);

///funcao responsavel por colocar prioridade maxima
int priorizarSemCombustivel(Aviao **, Aviao **, int);

///formaliza o horario no formato 00:00
void modificaRelogio(char[6], int, int);

///apos ter as instancias ja geradas, essa funcao administra processos
void controlarVoos(Aviao **, Aviao **, Aviao **, Aviao **, int, int);

int main() {
    Aviao *inicioD, *inicioA, *fimD, *fimA;
    int numeroDecolagens = geraValoresAleatorios(10, 33);
    int numeroAproximacoes = geraValoresAleatorios(10, 33);

    inicioD = gerarListaVazia();
    fimD = gerarListaVazia();
    inicioA = gerarListaVazia();
    fimA = gerarListaVazia();

    inicializaVoos(&inicioD, &fimD, numeroDecolagens, 'D');
    inicializaVoos(&inicioA, &fimA, numeroAproximacoes, 'A');
    controlarVoos(&inicioD, &inicioA, &fimD, &fimA, numeroDecolagens, numeroAproximacoes);


    return 0;
}

Aviao* inserirFinal(Aviao *final, int posicao, char tipo){
    Aviao *novoElemento = NULL;
    const char nomesVoos[64][7] =  {"VG3001", "JJ4404", "LN7001", "TG1501", "GL7602", "TT1010", "AZ1009", "AZ1008", "AZ1010",
                                    "TG1506", "VG3002", "JJ4402", "GL7603", "RL7880", "AL0012", "TT4544", "TG1505", "VG3003",
                                    "JJ4403", "JJ4401", "LN7002", "AZ1002", "AZ1007", "GL7604", "AZ1006", "TG1503", "AZ1003",
                                    "JJ4405", "AZ1001", "LN7003", "AZ1004", "TG1504", "AZ1005", "TG1502", "GL7601", "TT4500",
                                    "RL7801", "JJ4410", "GL7607", "AL0029", "VV3390", "VV3392", "GF4681", "GF4690", "AZ1020",
                                    "JJ4435", "VG3010", "LF0920", "AZ1065", "LF0978", "RL7867", "TT4502", "GL7645", "LF0932",
                                    "JJ4434", "TG1510", "TT1020", "AZ1098", "BA2312", "VG3030", "BA2304", "KL5609", "KL5610",
                                    "KL5611"};


    novoElemento = (Aviao *) malloc(sizeof(Aviao));
    if(novoElemento == NULL){
        printf("nao ha espaco na memoria");
        exit(-1);
    }

    strcpy(novoElemento->codigo, nomesVoos[posicao]);
    novoElemento->modo = tipo;
    if(tipo == 'A')
        novoElemento->combustivel = geraValoresAleatorios(0, 13);
    else{
        novoElemento->combustivel = 100;
    }

    novoElemento->prox = NULL;
    if(final == NULL) {
        return novoElemento;
    }
    else{
        final->prox = novoElemento;
        return novoElemento;
    }
}

int geraValoresAleatorios(int numInferior, int numSuperior){
    int aux;
    srand((unsigned)time(NULL) + RANDOMIZER);
    RANDOMIZER = rand();
    while (1){
        aux = rand() % numSuperior;
        if (numInferior <= aux)
            return aux;
    }
}


void *gerarListaVazia(){
    return NULL;
}

void inicializaVoos(Aviao **Pinicio, Aviao **Pfim, int numeroDeElementos, char tipo){
    int ordemDosNomes[64];


    ///enfilera as 64 possibilidades de codigo de voo
    for (int i = 0; i < 64; ++i) {
        ordemDosNomes[i] = i;
    }

    ///embaralha as 64 possibilidades de codigo de voo
    for (int i = 0; i < 64; ++i) {
        int aux, posicao = geraValoresAleatorios(0, 64);
        aux = ordemDosNomes[i];
        ordemDosNomes[i] = ordemDosNomes[posicao];
        ordemDosNomes[posicao] = aux;
    }

    ///gera decolagens enfileradas aleatoriamente
    for (int i = 0; i < numeroDeElementos; ++i) {
        if (*Pinicio == NULL) {
            *Pfim = inserirFinal(*Pfim, ordemDosNomes[ORDEM_ENUMERADA_ALEATORIA], tipo);
            *Pinicio = *Pfim;
        } else {
            *Pfim = inserirFinal(*Pfim, ordemDosNomes[ORDEM_ENUMERADA_ALEATORIA], tipo);
        }
        ORDEM_ENUMERADA_ALEATORIA++;
    }

}

void printarLista(Aviao *atual){

    if(atual->modo == 'A')
        printf("Codigo do voo: %s\n"
               "Status: %c\n"
               "Nivel de combustivel: %d\n", atual->codigo, atual->modo, atual->combustivel);
    else
        printf("Codigo do voo: %s\n"
               "Status: %c\n", atual->codigo, atual->modo);
}

Aviao* liberarInicio(Aviao *inicio){
    Aviao *aux = NULL;
    if(inicio != NULL) {
        aux = inicio->prox;
        free(inicio);
    }
    return aux;
}

int priorizarSemCombustivel(Aviao **PinicioAterrisagem, Aviao **PfimAterrisagem, int numeroAproximacoes){
    Aviao *elementoAnterior = *PinicioAterrisagem;
    int i = 1, numZeros = 0;
        for (Aviao *atual = (*PinicioAterrisagem)->prox; i < numeroAproximacoes; atual = atual->prox) {
            if(atual == NULL){
                printf("lista menor do que devia\n");
                printf("numero de aproximacoes = %d\n", numeroAproximacoes);
                printf("i = %d\n", i);
                printarLista(*PfimAterrisagem);
                printarLista(atual);
                break;

            }
            if (atual->combustivel == 0) {
                elementoAnterior->prox = atual->prox;
                atual->prox = *PinicioAterrisagem;
                *PinicioAterrisagem = atual;
                atual = elementoAnterior;
                numZeros++;
            }
            elementoAnterior = atual;
            i++;
        }

    return numZeros;
}


void modificaRelogio(char horario[6], int horas, int minutos){
    if(horas < 10){
        horario[0] = '0';
        itoa(horas, &horario[1], 10);
    } else{
        itoa(horas, horario, 10);
    }
    horario[2] = ':';

    if(minutos < 10){
        horario[3] = '0';
        itoa(minutos, &horario[4], 10);
    } else{
        char aux[3];
        itoa(minutos, aux, 10);
        horario[3] = aux[0];
        horario[4] = aux[1];
    }

}


void controlarVoos(Aviao **PinicioD, Aviao **PinicioA, Aviao **PfimD, Aviao **PfimA, int numeroDecolagens, int numeroAproximacoes){
    int horas = 8, minutos = 0;
    char horario[6];
    modificaRelogio(horario, horas, minutos);
    int numeroVoos = numeroAproximacoes + numeroDecolagens;

    printf("-------------------------------------------------------\n");
    printf("Aeroporto Internacional de Brasilia\n"
           "Hora Inicial: %s\n", horario);
    {
        int i = 0;
        for (Aviao *atual = *PinicioA; atual != NULL; atual = atual->prox) {
            printf("Fila de Pedidos: [%s - %c - %d]\n", atual->codigo, atual->modo, i);
            i++;
        }
        i = 0;
        for (Aviao *atual = *PinicioD; atual != NULL; atual = atual->prox) {
            printf("Fila de Pedidos: [%s - %c - %d]\n", atual->codigo, atual->modo, i);
            i++;
        }
    }
    printf("NVoos: %d\n"
           "Naproximacoes: %d\n"
           "NDecolagens: %d\n", numeroVoos, numeroAproximacoes, numeroDecolagens);
    printf("-------------------------------------------------------\n");



    int verificador1 = 0, verificador2 = 0, verificador3 = 0, verificadorCombustivel = 0;
    char tipo1, tipo2, tipo3;
    while(numeroVoos != 0) {
        int numZeros;
        modificaRelogio(horario, horas, minutos);


        if((verificadorCombustivel % 10) == 0 && verificadorCombustivel != 0){
            for (Aviao *atual = *PinicioA; atual != NULL; atual = atual->prox) {
                atual->combustivel--;
                if (atual->combustivel < 0) {
                    atual = liberarInicio(atual);
                    printf("\n-------------------------------------------------------\n"
                           "AERONAVE CAIU!\n"
                           "-------------------------------------------------------\n\n");
                }

            }
        }

        numZeros = priorizarSemCombustivel(PinicioA, PfimA, numeroAproximacoes);
        if (numZeros >= 3) {
            printf("-------------------------------------------------------\n");
            printf("\nALERTA GERAL DE DESVIO DE AERONAVE!\n");
        }


        if(verificador1 == 0) {
            if(numeroDecolagens <= numeroAproximacoes || numZeros != 0) { //condicao da pista 1
                tipo1 = 'A';
            } else{
                tipo1 = 'D';
            }
        }
        if(verificador2 == 0) {
            if (numeroDecolagens <= numeroAproximacoes || numZeros != 0) {    //condicao da pista 2
                tipo2 = 'A';
            } else {
                tipo2 = 'D';
            }
        }
        if(verificador3 == 0) {
            if (numZeros >= 3) {                                             //condicao da pista 3
                tipo3 = 'A';
            } else {
                tipo3 = 'D';
            }
        }





            ///pista1
        if(verificador1 == 1 && tipo1 == 'A'){
            printf("-------------------------------------------------------\n");
            printf("Procedimento: Aproximacao\n");
            printf("-------------------------------------------------------\n");
            printarLista(*PinicioA);
            printf("Horario do inicio do procedimento: %s\n"
                   "Pista de pouso 1\n", horario);
            printf("-------------------------------------------------------\n\n");
        }
        if(verificador1 == 4 && tipo1 == 'A'){    //Fazer aterrissagem
            printf("-------------------------------------------------------\n");
            printf("Procedimento: Aterrissagem\n");
            printf("-------------------------------------------------------\n");
            printarLista(*PinicioA);
            printf("Horario do inicio do procedimento: %s\n"
                   "Pista de pouso 1\n", horario);
            *PinicioA = liberarInicio(*PinicioA);
            numeroAproximacoes--;
            verificador1 = -1;
            printf("-------------------------------------------------------\n\n");
        } else if(verificador1 == 2 && tipo1 == 'D'){//Fazer decolagem
            if(numeroDecolagens == 0){

            }else {
                printf("-------------------------------------------------------\n");
                printf("Procedimento: Decolagem\n");
                printf("-------------------------------------------------------\n");
                printarLista(*PinicioD);
                printf("Horario do inicio do procedimento %s\n"
                       "Pista de pouso 1\n", horario);
                *PinicioD = liberarInicio(*PinicioD);
                numeroDecolagens--;
                verificador1 = -1;
                printf("-------------------------------------------------------\n\n");
            }
        }

        if(numeroAproximacoes + numeroDecolagens == 0)
            break;


        ///pista2
        if(verificador2 == 1 && tipo2 == 'A'){
            printf("-------------------------------------------------------\n");
            printf("Procedimento: Aproximacao\n");
            printf("-------------------------------------------------------\n");
            printarLista(*PinicioA);
            printf("Horario do inicio do procedimento: %s\n"
                   "Pista de pouso 2\n", horario);
            printf("-------------------------------------------------------\n\n");
        }
        if(verificador2 == 4 && tipo2 == 'A'){    //Fazer aterrissagem
            printf("-------------------------------------------------------\n");
            printf("Procedimento: Aterrissagem\n");
            printf("-------------------------------------------------------\n");
            printarLista(*PinicioA);
            printf("Horario do inicio do procedimento: %s\n"
                   "Pista de pouso 2\n", horario);
            *PinicioA = liberarInicio(*PinicioA);
            numeroAproximacoes--;
            verificador2 = -1;
            printf("-------------------------------------------------------\n\n");
        } else if(verificador2 == 2 && tipo2 == 'D'){//Fazer decolagem
            if(numeroDecolagens == 0){

            }else {
                printf("-------------------------------------------------------\n");
                printf("Procedimento: Decolagem\n");
                printf("-------------------------------------------------------\n");
                printarLista(*PinicioD);
                printf("Horario do inicio do procedimento %s\n"
                       "Pista de pouso 2\n", horario);
                *PinicioD = liberarInicio(*PinicioD);
                numeroDecolagens--;
                verificador2 = -1;
                printf("-------------------------------------------------------\n\n");
            }
        }

        if(numeroAproximacoes + numeroDecolagens == 0)
            break;


        ///pista3
        if(verificador3 == 1 && tipo3 == 'A'){
            printf("-------------------------------------------------------\n");
            printf("Procedimento: Aproximacao\n");
            printf("-------------------------------------------------------\n");
            printarLista(*PinicioA);
            printf("Horario do inicio do procedimento: %s\n"
                   "Pista de pouso 3\n", horario);
            printf("-------------------------------------------------------\n\n");

        }
        if(verificador3 == 4 && tipo3 == 'A'){    //Fazer aterrissagem
            printf("-------------------------------------------------------\n");
            printf("Procedimento: Aterrissagem\n");
            printf("-------------------------------------------------------\n");
            printarLista(*PinicioA);
            printf("Horario do inicio do procedimento: %s\n"
                   "Pista de pouso 3\n", horario);
            *PinicioA = liberarInicio(*PinicioA);
            numeroAproximacoes--;
            verificador3 = -1;
            printf("-------------------------------------------------------\n\n");
        } else if(verificador3 == 2 && tipo3 == 'D'){//Fazer decolagem
            if(numeroDecolagens == 0){

            }else {
                printf("-------------------------------------------------------\n");
                printf("Procedimento: Decolagem\n");
                printf("-------------------------------------------------------\n");
                printarLista(*PinicioD);
                printf("Horario do inicio do procedimento %s\n"
                       "Pista de pouso 3\n", horario);
                *PinicioD = liberarInicio(*PinicioD);
                numeroDecolagens--;
                verificador3 = -1;
                printf("-------------------------------------------------------\n\n");
            }
        }



        minutos += 5;
        if(minutos == 60){
            horas++;
            minutos = 0;
        }
        numeroVoos = numeroAproximacoes + numeroDecolagens;
        verificador1++;
        verificador2++;
        verificador3++;
        verificadorCombustivel++;
    }

}

