#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

// Definição da struct que armazena a matriz
typedef struct{
    char ** matriz;
}campo;

void matriz_esta_aberta(campo * c)
{
    for (int i=0; i<10; i++)
    {
        for(int j=0; j<20; j++)
        {
            c[2].matriz[i][j] = '0';
        }
    }
}

// Essa função cria a matriz e faz a alocação dinâmica dos ca6mpos
void cria_campo(campo * c){
    c[0].matriz = calloc(10, sizeof(char*));
    c[1].matriz = calloc(10, sizeof(char*));

    for (int l = 0; l < 10; l++)
    {
       c[0].matriz[l] = calloc(20, sizeof(char));
       c[1].matriz[l] = calloc(20, sizeof(char));
    }
    //Adiciona um espaço inicial em todas as posições 
    for(int linha=0; linha<10 ; linha++){
        for (int coluna = 0; coluna < 20; coluna++)
        {
            c[0].matriz[linha][coluna] = ' ';
            c[1].matriz[linha][coluna] = ' ';
        }
    }
}

// Função para preencher a matriz com as minas
void preencher_campo(campo * c){
    //Essa parte do código gera os índices dos campos que ficaram as minas.
    int indices_aleatorios[40][2];
    int sn=0;
    int linha, coluna=0;

    srand(time(NULL));
    
    // limpa a matriz de índices aleatórios colocando -1 em todas as posições  
    for(int l = 0; l < 40; l++){
        for(int c = 0; c < 2; c++){
            indices_aleatorios[l][c] = -1;
        }
    }
    //-----------------------------------------------------------------------

    //Gera os 40 índices aleatórios das minas
    for(int i = 0; i < 40; i++){
        linha = rand()%10;
        coluna = rand()%20;    

        for(int l = 0; l < 40; l++){
            if(linha == indices_aleatorios[l][0] && coluna == indices_aleatorios[l][1]){
                sn=1;
            }
        }
        if(sn == 1){
            i--;
        }else{
            indices_aleatorios[i][0]=linha;
            indices_aleatorios[i][1]=coluna;
        }
        sn=0;
    }
    //----------------------------------------------------------------------------------

    //Preenche a matriz com as minas

    for (int i = 0; i < 40; i++)
    {
        c[0].matriz[indices_aleatorios[i][0]][indices_aleatorios[i][1]] = 3;
    }
    //-----------------------------------------------------------
}
//-----------------------------fim da função--------------------------------

//Função para mostrar a matriz
void print_campo(campo * c){
    int cont_linha = 0;
    printf("    0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17  18  19 \n");
    printf("  ---------------------------------------------------------------------------------\n");
    for (int i = 0; i < 10; i++)
    {
        printf("%d |", cont_linha);
        for (int a = 0; a < 20; a++)
        {
            printf(" %c |", c[2].matriz[i][a]);
        }
        printf("\n");
        printf("  ---------------------------------------------------------------------------------\n");
        cont_linha++;
    }
}
//----------------------------fim da função----------------------------------



int coordenada_valida(int coordenada1, int coordenada2)
{
    if (coordenada1 >= 0 && coordenada1<=10 && coordenada2 >= 0 && coordenada2 <=20)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void modo_casual(campo * c)
{
    int coordenada1, coordenada2;
    printf("\n\n");
    printf("\t\t\t\t    CAMPO MINADO\n\n");
    print_campo(c);
    printf("\nInforme uma coordenada: ");
    scanf("%i %i", &coordenada1, &coordenada2);
    if (coordenada_valida(coordenada1, coordenada2) == 1)
    {
        printf("verdadeiro");
    }
    else
    {
        printf("falso");
    }
    modo_casual(c);
}

void menu(campo * c)
{
    int opcao;
    printf("Olá! Seja bem-vindo(a) ao Campo Minado.\n Por favor, escolha uma opção de 1 a 3. \n 1 - Jogar: Modo Casual\n 2 - Modo Autônomo\n 3 - Sair.\n\n");
    scanf("%i", &opcao);
    if (opcao>3 || opcao<1)
    {
        printf("\nOpção inválida! (%i)\n\n", opcao);
        menu(c);
    }
    else
    {
        if (opcao == 1)
        {
            printf("\nVocê selecionou a opção (%i). Iniciando Modo Casual... \n\n", opcao);
            modo_casual(c);
        }
        else if(opcao == 2)
        {
            printf("\nVocê selecionou a opção (%i). Iniciando Modo Autônomo... \n\n", opcao);
        }
        else if(opcao == 3)
        {
            printf("\nVocê selecionou a opção (%i). Encerrando programa... \n\n", opcao);
        }
    }
}
int main (){

    campo *c;
    c = malloc (sizeof(campo)*4);
    cria_campo(c);
    preencher_campo(c);
    menu(c);
    return 0;
}