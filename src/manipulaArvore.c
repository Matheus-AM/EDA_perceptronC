#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/// Matheus Amaral Moreira - 17/0080307
struct ArvoreBinaria
{
    int valor;
    struct ArvoreBinaria *esquerda;
    struct ArvoreBinaria *direita;
};
typedef struct ArvoreBinaria Arvore;

void loadTreeFromFile(Arvore **, char*);
void showTree(Arvore **, int);
int isFull(Arvore **, int, int, int);
int searchValue(Arvore **, int);
int getHeight(Arvore **, int, int);
void removeValue(Arvore **);
void printInOrder(Arvore **);
void printPreOrder(Arvore **);
void printPostOrder(Arvore **);
void balanceTree(Arvore *);

void inserirValor(Arvore **, int);
Arvore *criarArvoreVazia();
void printarOpcoes();
void limparTela();
void freeArvore(Arvore **);

int main() {
    Arvore *root;

    root = criarArvoreVazia();
    loadTreeFromFile(&root, "BSTs\\bst1.txt");
    int altura = getHeight(&root, 1, 1);
    int opcao = 1;
    while(opcao != 0) {
        printarOpcoes();
        scanf("%d", &opcao);
        getchar();
        limparTela();
        switch (opcao){
            case 1:
                printInOrder(&root);
                printf("\n");
                break;
            case 2:
                printPreOrder(&root);
                printf("\n");
                break;
            case 3:
                printPostOrder(&root);
                printf("\n");
                break;
            case 4:
            {
                int busca;
                scanf("%d", &busca);
                busca = searchValue(&root, busca);
                if(busca != 0)
                    printf("O valor foi encontrado.\n");
            }
                break;
            case 5:
            {
                char busca;
                char path[15];
                printf("Escolha o arquivo:\n1\n2\n3\n4\n5\n");
                scanf("%c", &busca);

                strcpy(path, "BSTs\\bst1.txt");
                path[8] = busca;

                loadTreeFromFile(&root, path);

                limparTela();
                printf("arquivo carregado!\n");

            }
                break;
            case 6:
                printf("A altura da arvore e: %d\n", altura);
                break;
            case 7:
                if(isFull(&root, 0, altura, 0) == 1){
                    printf("A arvore nao e cheia.\n");
                } else{
                    printf("A arvore e cheia.\n");
                }
                break;
        }
    }

    freeArvore(&root);
    return 0;
}

void printarOpcoes(){
    printf("1 - Print in-order\n"
           "2 - Print pre-order\n"
           "3 - Print post-order\n"
           "4 - Buscar elemento\n"
           "5 - Trocar de arquivo\n"
           "6 - Dar altura da arvore\n"
           "7 - dizer se e cheia\n"
           "0 - Sair.\n");
}

void limparTela(){
    printf("\n\n\n\n\n");
}


void loadTreeFromFile(Arvore **root, char *path){
    FILE *file = NULL;
    file = fopen(path, "r+");
    int num = 0;


    if (file == NULL) {
        printf("Falha em carregar o arquivo.\n");
        exit(1);
    }

    if(*root != NULL) {
        freeArvore(root);
        *root = NULL;
    }

    while(!feof(file)){
            fscanf(file, "%d", &num);
            inserirValor(root, num);
    }
    fclose(file);
}

void inserirValor(Arvore **atual, int valor){
    if(*atual == NULL){
        *atual = (Arvore *) malloc(sizeof(Arvore));
        (*atual)->valor = valor;
        (*atual)->esquerda = NULL;
        (*atual)->direita = NULL;
    }else {
        if(valor <= (*atual)->valor)
            inserirValor(&(*atual)->esquerda, valor);
        else
            inserirValor(&(*atual)->direita, valor);
    }
}

Arvore *criarArvoreVazia(){
    return NULL;
}

//void showTree(Arvore **atual, int altura){
//    for (int i = 0; i < altura*2; ++i) {
//        printf(" ");
//    }
//    printf("%d\n", (*atual)->valor);
//    for (int i = 0; i < (altura-1)*2; ++i) {
//        printf(" ");
//    }
//    printf("/");
//    for (int i = 0; i < altura; ++i) {
//        printf(" ");
//    }
//    printf("\\ \n");
//
//
//    if((*atual)->esquerda != NULL ){
//        showTree(&(*atual)->esquerda, altura-1);
//    }
//    if((*atual)->direita != NULL){
//        showTree(&(*atual)->direita, altura+1);
//    }
//
//
//}

int searchValue(Arvore **atual, int busca){
    if(*atual == NULL){
        printf("Valor nao encontrado.\n");
        return 0;
    }
    if(busca == (*atual)->valor){
        return busca;
    }else {
        if(busca < (*atual)->valor)
            return searchValue(&(*atual)->esquerda, busca);
        else
            return searchValue(&(*atual)->direita, busca);
    }
}

int isFull(Arvore **atual, int profundidade, int altura, int validacao){
    if((*atual)->esquerda != NULL ){
        profundidade++;
        validacao = isFull(&(*atual)->esquerda, profundidade, altura, validacao);
    }
    if((*atual)->direita != NULL){
        profundidade++;
        validacao = isFull(&(*atual)->direita, profundidade, altura, validacao);
    }

    if(profundidade != altura)
        return 1;
    else
        return validacao;
}

void printInOrder(Arvore **atual){
    if((*atual)->esquerda != NULL ){
        printInOrder(&(*atual)->esquerda);
    }
    printf("%d\t", (*atual)->valor);
    if((*atual)->direita != NULL){
        printInOrder(&(*atual)->direita);
    }
}

void printPreOrder(Arvore **atual){
    printf("%d\t", (*atual)->valor);
    if((*atual)->esquerda != NULL ){
        printPreOrder(&(*atual)->esquerda);
    }
    if((*atual)->direita != NULL){
        printPreOrder(&(*atual)->direita);
    }
}

void printPostOrder(Arvore **atual){
    if((*atual)->esquerda != NULL ){
        printPostOrder(&(*atual)->esquerda);
    }
    if((*atual)->direita != NULL){
        printPostOrder(&(*atual)->direita);
    }
    printf("%d\t", (*atual)->valor);
}

int getHeight(Arvore **atual, int alturaE, int alturaD){
    if((*atual)->esquerda != NULL ){
        alturaE++;
        alturaE = getHeight(&(*atual)->esquerda, alturaE, alturaE);
    }
    if((*atual)->direita != NULL){
        alturaD++;
        alturaD = getHeight(&(*atual)->direita, alturaD, alturaD);
    }

    if(alturaE >= alturaD)
        return alturaE;
    else
        return alturaD;
}


void freeArvore(Arvore **atual){
    if((*atual)->esquerda != NULL ){
        freeArvore(&(*atual)->esquerda);
    }
    if((*atual)->direita != NULL){
        freeArvore(&(*atual)->direita);
    }
    free(*atual);
}
