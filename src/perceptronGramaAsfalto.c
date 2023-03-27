#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mem.h>
#include <time.h>

#define numeroPixels 1025

/// Matheus Amaral Moreira - 170080307

struct PerceptronOut{
    double *valor;
    double *peso;
    double bias;
};
struct PerceptronHidden{
    double valor[536];
    double peso[536];
    double bias;
    struct PerceptronOut *saida;
};
struct PerceptronIn{
    double valor;
    double peso;
    double bias;
    struct PerceptronHidden **camadaOculta;
};




typedef struct PerceptronIn Neuronio;
typedef struct PerceptronHidden Hidden;
typedef struct PerceptronOut Output;


    int contador = 0;
int RANDOMIZER = 0; ///gera a aleatoriedade continua
int ORDEM_ENUMERADA_ALEATORIA = 0; ///Faz com que a aleatoriedade controlada nao perca o fluxo

int geraValoresAleatorios(int, int);
void carregarImagem(int**, char *);
int* embaralhar(int *, int);
int* criarVetor(int);
double* criarVetor2(int);
int** criarMatriz(int, int);
double** criarMatriz2(int, int);
void destroiMatriz(int **, int);
void destroiMatriz2(double **, int);
void normalizarVetor(double *, int);
void ILBP(int **, double *);
void GLCM(int **, double *);


void carregarAmostral(int *, int **, double *, double **);
Neuronio* criarNeuronio(Neuronio *, int, int);
Hidden* criarNeuronio2(Hidden *, int);
void calcularAtivacao1(Neuronio **, double **);
double carregarNeuronio(Neuronio**, double*, int);
void criarRede(Neuronio***, int);



int main(int argc, char *argv[] ) {
    int **imagem = NULL, *ordem = NULL;
    double *parametros = NULL, **amostral = NULL;
    parametros = criarVetor2(536);
    imagem = criarMatriz(numeroPixels, numeroPixels);
    amostral = criarMatriz2(100, 536);
    int numeroDeCamadasOcultas = atoi(argv[1]);
    ordem = criarVetor(100);
    for (int i = 0; i < 100; ++i) {
        ordem[i] = i + 2;             /// 2/2 = 1; 3/2 = 1; 100/2 = 50; 101/2 = 50
    }
    embaralhar(ordem, 100);

    carregarAmostral(ordem, imagem, parametros, amostral);

    destroiMatriz(imagem, numeroPixels);
    free(parametros);

    ///agora comeca o projeto6
    Neuronio **inputs = NULL;
    inputs = (Neuronio **) malloc(536 * sizeof(Neuronio *));
    if(inputs == NULL){
        printf("erro de alocacao no inputs");
        exit(-1);
    }


    criarRede(&inputs, numeroDeCamadasOcultas);



    for (int n = 0; n < 50; ++n) {
        int auxGrama = 0, auxAsfalto = 0;

        double resultado = carregarNeuronio(inputs, amostral[n], numeroDeCamadasOcultas);
        if (ordem[n] == 0){             ///se e grama
            if(resultado > 0.5){
                auxGrama++;
            }
        }else{
            if(resultado <= 0.5){
                auxAsfalto++;
            }
        }

        ///resposta
        float taxaDeAcerto = 100.0 * (auxAsfalto + auxGrama) / 50.0;
        float taxaDeFalsaRejeicao = 100.0 * (25 - auxGrama) / 25.0;
        float taxaDeFalsaAceitacao = 100.0 * (25 - auxAsfalto) / 25.0;

        printf("\n\n\n\n\n");


        printf("Taxa de acerto: %lf\n", taxaDeAcerto);
        printf("Taxa de falsa aceitacao: %lf\n", taxaDeFalsaAceitacao);
        printf("Taxa de falsa Rejeicao: %lf\n", taxaDeFalsaRejeicao);




    }




    return 0;
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

///"sobrecarga de metodos"
int* criarVetor(int k){
    int *p;
    p = (int *) malloc( k * sizeof(int));
    if (p==NULL) {
        printf("Falha no vetor.\n");
        exit(1);
    }
    return p;
}
double* criarVetor2(int k){
    double *p;
    p = (double *) malloc( k * sizeof(double));
    if (p==NULL) {
        printf("Falha no vetor.\n");
        exit(1);
    }
    return p;
}

int potenciaBin(int y) {
    int aux=1;
    for (int k = 0; k < y; ++k)
        aux *= 2;


    return aux;

}

///"sobrecarga de metodos"
int** criarMatriz(int k, int l){
    int **M;
    M = (int **) malloc(k * sizeof(int*));
    if (M==NULL) {
        printf("Falha na matriz.\n");
        exit(1);
    }
    for (int i = 0; i < k; ++i) {
        *(M+i) = criarVetor(l);
        if (*(M+i) == NULL) {
            printf("Falha na matriz.\n");
            exit(1);
        }
    }
    return M;
}
double** criarMatriz2(int k, int l){
    double **M;
    M = (double **) malloc(k * sizeof(double*));
    if (M==NULL) {
        printf("Falha na matriz.\n");
        exit(1);
    }
    for (int i = 0; i < k; ++i) {
        *(M+i) = criarVetor2(l);
        if (*(M+i) == NULL) {
            printf("Falha na matriz.\n");
            exit(1);
        }
    }
    return M;
}

void carregarImagem(int** imagem, char *path){
    FILE *file = NULL;

    file = fopen(path, "r");
    if (file==NULL) {
        printf("Falha em carregar o arquivo.\n");
        exit(1);
    }

    for (int i = 0; i < numeroPixels; ++i) {
        for (int j = 0; j < numeroPixels; ++j) {
            fscanf(file, "%d", (*(imagem + i) + j));
            getc(file);
        }
    }

    fclose(file);
}

int* embaralhar(int *ordem, int n){
    int r , aux;
    for (int i = 0; i < n; i++)
    {
        r = geraValoresAleatorios(0, n);

        aux = ordem[i];
        ordem[i] = ordem[r];
        ordem[r] = aux;
    }

    return ordem;
}

void zerarVetor(unsigned int* num, int n){
    for (int i = 0; i < n; ++i)
        num[i] = 0;
}

///"sobrecarga de metodos"
void destroiMatriz(int **M, int k){
    for (int i = 0; i < k; ++i) {
        free(*(M+i));
    }
    free(M);
}
void destroiMatriz2(double **M, int k){
    for (int i = 0; i < k; ++i) {
        free(*(M+i));
    }
    free(M);
}

///"sobrecarga de metodos"
unsigned int pegarOMenor(const unsigned int* num){
    unsigned int aux = 0;

    aux = num[0];
    for (int i = 1; i < 9; ++i) {
        if(aux > num[i])
            aux = num[i];
        else;
    }

    return aux;
}
double pegarOMenor2(const double *vector, int n){
    double aux = 0;

    aux = vector[0];
    for (int i = 1; i < n; ++i) {
        if(aux > vector[i])
            aux = vector[i];
        else;
    }

    return aux;
}

double pegarOMaior(const double *vector, int n){
    double aux = 0;

    aux = vector[0];
    for (int i = 1; i < n; ++i) {
        if(aux < vector[i])
            aux = vector[i];
        else;
    }

    return aux;
}



void rotaciona9BitsE(unsigned int* quad){
    unsigned short int aux1, aux0;
    aux0 = quad[8];
    for (int i = 0; i < 9; ++i) {
        aux1 = quad[i];
        quad[i] = aux0;
        aux0 = aux1;
    }
}

void normalizarVetor(double *vetor, int n){
    double maior = 0, menor = 0;


    menor = pegarOMenor2(vetor, n);
    maior = pegarOMaior(vetor, n);

    for (int i = 0; i < n; ++i) {
        vetor[i] = (vetor[i] - menor)/(maior - menor);
    }
    ///printar vetor normalizado
}

double calcularDistancia(const double *parametros,const double *media, int n){
    double distancia = 0;
    for (int i = 0; i < n; ++i) {
        distancia +=((parametros[i] - media[i]) * (parametros[i] - media[i]));
    }
    distancia = sqrt(distancia);
    return distancia;
}


///funcoes principais

void ILBP(int **imagem, double *parametros) {
    unsigned int quadradoBin[9], bin[9];
    float mediaQuadrado = 0;

    for (int i = 1; i < numeroPixels-1; ++i) {
        for (int j = 1; j < numeroPixels-1; ++j) {


            mediaQuadrado = (float) (imagem[i-1][j-1] + imagem[i-1][j] + imagem[i-1][j+1] + imagem[i][j-1] + imagem[i][j] + imagem[i][j+1] + imagem[i+1][j-1] + imagem[i+1][j] + imagem[i+1][j+1])/9;

            for (short int k = 0; k < 3; ++k) {
                for (short int l = 0; l < 3; ++l) {
                    if(imagem[i + (k-1)][j + (l-1)] >= mediaQuadrado) {
                        quadradoBin[(k * 3) + l] = 1;
                    }
                    else
                        quadradoBin[(k * 3) + l] = 0;
                }
            }

            ///zerar o parametro
            zerarVetor(bin, 9);

            for (int x = 0; x < 9; ++x) {
                ///COMO A ORDEM NAO IMPORTA, USEI O CAMINHO PADRAO
                for (int y = 0; y < 9; ++y) {
                    bin[x] += (unsigned int) (quadradoBin[8 - y] * potenciaBin(y));
                }
                rotaciona9BitsE(quadradoBin);
            }
///inccrementar no menor bit
            parametros[pegarOMenor(bin)]++;
        }

    }

}

void GLCM(int **imagem, double *parametros){
    int matrizCinza[256][256];
// 0 1 2
// 3 O 4  (O de origem)
// 5 6 7


    for (int n = 0; n < 8; ++n) {

        ///zerar valores iniciais
        for (int i = 0; i < 256; ++i) {
            for (int j = 0; j < 256; ++j) {
                matrizCinza[i][j] = 0;
            }
        }

        ///matriz n
        switch(n) {
            case 0:
                for (int i = 1; i < numeroPixels; ++i) {
                    for (int j = 1; j < numeroPixels; ++j) {
                        matrizCinza[*(*(imagem + i) + j)][*(*(imagem + i - 1) + j-1)]++;
                    }
                }
                break;
            case 1:

                for (int i = 1; i < numeroPixels; ++i) {
                    for (int j = 0; j < numeroPixels; ++j) {
                        matrizCinza[*(*(imagem + i) + j)][*(*(imagem + i - 1) + j)]++;
                    }
                }
                break;
            case 2:
                for (int i = 1; i < numeroPixels; ++i) {
                    for (int j = 0; j < numeroPixels-1; ++j) {
                        matrizCinza[*(*(imagem + i) + j)][*(*(imagem + i - 1) + j + 1)]++;
                    }
                }
                break;
            case 3:
                for (int i = 0; i < numeroPixels; ++i) {
                    for (int j = 1; j < numeroPixels; ++j) {
                        matrizCinza[*(*(imagem + i) + j)][*(*(imagem + i) + j - 1)]++;
                    }
                }
                break;
            case 4:
                for (int i = 0; i < numeroPixels; ++i) {
                    for (int j = 0; j < numeroPixels-1; ++j) {
                        matrizCinza[*(*(imagem + i) + j)][*(*(imagem + i) + j + 1)]++;
                    }
                }
                break;
            case 5:
                for (int i = 0; i < numeroPixels-1; ++i) {
                    for (int j = 1; j < numeroPixels; ++j) {
                        matrizCinza[*(*(imagem + i) + j)][*(*(imagem + i + 1) + j - 1)]++;
                    }
                }
                break;
            case 6:
                for (int i = 0; i < numeroPixels-1; ++i) {
                    for (int j = 0; j < numeroPixels; ++j) {
                        matrizCinza[*(*(imagem + i) + j)][*(*(imagem + i + 1) + j)]++;
                    }
                }
                break;
            case 7:
                for (int i = 0; i < numeroPixels-1; ++i) {
                    for (int j = 0; j < numeroPixels-1; ++j) {
                        matrizCinza[*(*(imagem + i) + j)][*(*(imagem + i + 1) + j + 1)]++;
                    }
                }
                break;
        }
        ///fim matriz n

        double matrizAux;
        for (int i = 0; i < 256; ++i) {
            for (int j = 0; j < 256; ++j) {
                if(n == 1 || n == 3 || n == 4 || n == 6) {
                    matrizAux = ((double) (matrizCinza[i][j]))/(1025*1024);
                }
                else{
                    matrizAux = ((double) (matrizCinza[i][j]))/(1024*1024);
                }
                ///energia
                parametros[512 + (n * 3)] += (matrizAux * matrizAux);
                ///homogeneidade
                parametros[513 + (n * 3)] += matrizAux / (1 + abs(i - j));
                ///contraste
                parametros[514 + (n * 3)] += (abs(i - j) * abs(i - j)) * matrizAux;
            }
        }


    }


}

void carregarAmostral(int *ordem, int **imagem, double *parametros, double **amostral){
    char path[39], digito[3];

    for (int n = 0; n < 50; ++n) {
        if (ordem[n] % 2 != 0){
            strcpy(path, "DataSet\\DataSet\\asphalt\\asphalt_01.txt");    ///tamanho 38 + \0
            itoa(ordem[n]/2, digito, 10);
            if(ordem[n]/2 > 9){
                path[32] = digito[0];
                path[33] = digito[1];
            } else{
                path[32] = '0';
                path[33] = digito[0];
            }
            ordem[n] = 1;                    /// isso e pra sinalizar que e asfalto
        }else{
            strcpy(path,"DataSet\\DataSet\\grass\\grass_01.txt");     ///tamanho 34 + \0
            itoa(ordem[n]/2, digito, 10);
            if (ordem[n]/2 > 9) {
                path[28] = digito[0];
                path[29] = digito[1];
            }else{
                path[28] = '0';
                path[29] = digito[0];
            }
            ordem[n] = 0;                     /// isso e para sinalizar que e grama
        }

        zerarVetor((unsigned int *) parametros, 536);


        printf("%s\n", path);
        printf("%d%%\n", contador);
        carregarImagem(imagem, path);
        ILBP(imagem, parametros);
        GLCM(imagem, parametros);
        normalizarVetor(parametros, 536);
        for (int i = 0; i < 536; ++i) {
            amostral[n][i] = parametros[i];
//            printf("%d e %d\n",i,  parametros[i]);
        }
        contador++;
    }

}

Neuronio* criarNeuronio(Neuronio *inicial, int NOcultas, int flag){


    Neuronio *novoNeuronio = NULL;
    novoNeuronio = (Neuronio *) malloc(sizeof(Neuronio));
    if(novoNeuronio == NULL){
        printf("erro de alocacao na camada de input");
        exit(-1);
    }

    novoNeuronio->valor = 0;
    novoNeuronio->bias = (float) geraValoresAleatorios(0, 10000)/1000;
    novoNeuronio->peso = (float) geraValoresAleatorios(0, 10000)/1000;
    if (flag == 0){
        novoNeuronio->camadaOculta = (Hidden **) malloc(NOcultas * sizeof(Hidden *));
        if(novoNeuronio->camadaOculta == NULL){
            printf("erro na oculta");
            exit(-1);
        }
    }else{
        for (int i = 0; i < NOcultas; ++i) {
            novoNeuronio->camadaOculta[i] = inicial->camadaOculta[i];
        }
    }

    return novoNeuronio;
}

Hidden* criarNeuronio2(Hidden *inicial, int flag){
    Hidden *novoOculto = NULL;
    novoOculto = (Hidden *) malloc(sizeof(Hidden));
    if(novoOculto == NULL){
        printf("erro de alocacao na camada oculta");
        exit(-1);
    }
    for (int i = 0; i < 536; ++i) {
        novoOculto->valor[i] = 0;
        novoOculto->peso[i] = (float) geraValoresAleatorios(0, 10000)/1000;
    }
    novoOculto->bias = (float) geraValoresAleatorios(0, 10000)/1000;


    if(flag == 0){
        novoOculto->saida = (Output *) malloc(sizeof(Output));
        if(novoOculto->saida == NULL){
            printf("erro na camada oculta");
            exit(-1);
        }
    }else{
        novoOculto->saida = inicial->saida;
    }
    return novoOculto;
}


void calcularAtivacao1(Neuronio **inputs, double *valores[536]){
    for (int i = 0; i < 536; ++i) {
        *valores[i] = inputs[i]->valor * inputs[i]->peso + inputs[i]->bias;
    }
}

void calcularAtivacao2(Hidden **hidden, double *valores[536], int NOcultas) {
    for (int i = 0; i < NOcultas; ++i) {
        for (int j = 0; j < 536; ++j) {
            *valores[i] += hidden[i]->valor[j] * hidden[i]->peso[j];
        }
        *valores[i] +=  hidden[i]->bias;
    }
}


double calcularSaida(Output *saida, int NOcultas){
    double final = 0;
    for (int i = 0; i < NOcultas; ++i) {
        final += saida->valor[i] * saida->peso[i];
    }
    return final += saida->bias;
}

double carregarNeuronio(Neuronio **inputs, double *amostral, int NCamadaEscondida){
    double *resultadosDosInputs = NULL, *resultadosDosOcultos = NULL;
    resultadosDosInputs = (double *) malloc(536 * sizeof(double));
    resultadosDosOcultos = (double *) malloc(NCamadaEscondida * sizeof(double));

    if(resultadosDosInputs == NULL || resultadosDosOcultos == NULL){
        printf("erro de memoria");
        exit(-1);
    }

    for (int i = 0; i < 536; ++i) {
        inputs[i]->valor = amostral[i];
    }

    calcularAtivacao1(inputs, &resultadosDosInputs);


    for (int i = 0; i < NCamadaEscondida; ++i) {
        for (int j = 0; j < 536; ++j) {
            ((*inputs)->camadaOculta[i])->valor[j] = resultadosDosInputs[j];
        }
    }

    calcularAtivacao2((*inputs)->camadaOculta, &resultadosDosOcultos, NCamadaEscondida);

    return calcularSaida((*(*inputs)->camadaOculta)->saida, NCamadaEscondida);
}

void criarRede(Neuronio ***inputs, int NOcultas){
    for (int i = 0; i < 536; ++i) {

        *inputs[i] = criarNeuronio(**inputs, NOcultas, i);

    }
    for (int j = 0; j < NOcultas; ++j) {
        (**inputs)->camadaOculta[j] = criarNeuronio2(*(**inputs)->camadaOculta, j);

    }


}
