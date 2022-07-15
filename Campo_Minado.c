#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <conio.h>
#include <locale.h>

#define l_constante 10
#define c_constante 20


#define ANSI_COLOR_RED      "\x1b[31m" 
#define ANSI_COLOR_GRAY     "\e[0;37m"
#define ANSI_COLOR_RESET    "\x1b[0m"
#define ANSI_COLOR_YELLOW    "\033[1;33m"
#define ANSI_COLOR_GREEN    "\033[1;32m"
#define ANSI_COLOR_BLUE     "\033[1;34m"
#define ANSI_COLOR_CYAN     "\033[1;36m"
#define ANSI_COLOR_MAGENTA  "\033[1;35m"
#define ANSI_COLOR_WHITE    "\033[1;97m"

// Guia para codificação
// 0 matriz com bombas 
// 1 matriz exibida para o usuário
// 2 matriz campo aberto ou fechado
// 3 matriz bombas vizinhas

// Definição da struct que armazena a matriz
typedef struct{
    char ** matriz;
}campo;


// Essa função cria a matriz e faz a alocação dinâmica dos campos
void cria_campo(campo * c){
    c[0].matriz = calloc(l_constante, sizeof(char*));
    c[1].matriz = calloc(l_constante, sizeof(char*));
    c[2].matriz = calloc(l_constante, sizeof(char*));
    c[3].matriz = calloc(l_constante, sizeof(char*));

    for (int linha = 0; linha < 10; linha++)
    {
       c[0].matriz[linha] = calloc(c_constante, sizeof(char));
       c[1].matriz[linha] = calloc(c_constante, sizeof(char));
       c[2].matriz[linha] = calloc(c_constante, sizeof(char));
       c[3].matriz[linha] = calloc(c_constante, sizeof(char));
    }

    //Adiciona um espaço inicial em todas as posições 
    for(int linha=0; linha<10 ; linha++){
        for (int coluna = 0; coluna < 20; coluna++)
        {
            c[0].matriz[linha][coluna] = ' ';
            c[1].matriz[linha][coluna] = ' ';
            c[2].matriz[linha][coluna] = ' ';
            c[3].matriz[linha][coluna] = ' ';
        }
    }
    // Define que tordos os campos inicialmente estão fechados 
    for (int linha = 0; linha < l_constante; linha++)
    {
        for(int coluna=0; coluna < c_constante; coluna++)     
        {
            c[2].matriz[linha][coluna] = '0';
        }
    }
}

// Função para preencher a matriz com as minas
void preenche_campo_minas(campo * c){
    //Essa parte do código gera os índices dos campos que ficaram as minas.
    int indices_aleatorios[40][2];
    int sn=0;
    int linha_coordenada, coluna_coordenada=0;

    srand(time(NULL));
    
    // limpa a matriz de índices aleatórios colocando -1 em todas as posições  
    for(int linha = 0; linha < 40; linha++){
        for(int coluna = 0; coluna < 2; coluna++){
            indices_aleatorios[linha][coluna] = -1;
        }
    }
    //-----------------------------------------------------------------------

    //Gera os 40 índices aleatórios das minas
    for(int i = 0; i < 40; i++){
        linha_coordenada = rand()%10;
        coluna_coordenada = rand()%20;    

        for(int linha = 0; linha < 40; linha++){
            if(linha_coordenada == indices_aleatorios[linha][0] && coluna_coordenada == indices_aleatorios[linha][1]){
                sn=1;
            }
        }
        if(sn == 1){
            i--;
        }else{
            indices_aleatorios[i][0]=linha_coordenada;
            indices_aleatorios[i][1]=coluna_coordenada;
        }
        sn=0;
    }
    //----------------------------------------------------------------------------------

    //Preenche a matriz com as minas

    for (int i = 0; i < 40; i++)
    {
        c[0].matriz[indices_aleatorios[i][0]][indices_aleatorios[i][1]] = 'x';
    }
    //----------------------------------------------------------------------
}


//Função para mostrar a matriz
void exibir_campo(campo * c){

    // 0 matriz com bombas 
    // 1 matriz exibida para o usuário
    // 2 matriz campo aberto ou fechado
    // 3 matriz bombas vizinhas

    int cont_linha = 0;
    printf("    0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17  18  19 \n");
    printf("  ---------------------------------------------------------------------------------\n");
    for (int i = 0; i < 10; i++)
    {
        printf("%d |", cont_linha);
        for (int a = 0; a < 20; a++)
        {
            printf(" %c |", c[3].matriz[i][a]);
        }
        printf("\n");
        printf("  ---------------------------------------------------------------------------------\n");
        cont_linha++;
    }

    // int cont_linha = 0;
    // printf(ANSI_COLOR_WHITE "    0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17  18  19 \n" ANSI_COLOR_RESET);
    // printf("  ---------------------------------------------------------------------------------\n");
    // for (int linha = 0; linha < 10; linha++)
    // {
    //     printf(ANSI_COLOR_WHITE "%d |", cont_linha);
    //     printf(ANSI_COLOR_RESET "");
    //     for (int coluna = 0; coluna < 20; coluna++)
    //     {
    //         if(c[2].matriz[linha][coluna] == '1'){
    //             if(c[0].matriz[linha][coluna] == 'x'){
    //                 printf(ANSI_COLOR_RED " %c ", c[0].matriz[linha][coluna]);
    //                 printf(ANSI_COLOR_RESET "|");
    //             }else{
    //                 // muda de cor de acordo com o número de minas ao redor
    //                 if(c[3].matriz[linha][coluna] == '4' || c[3].matriz[linha][coluna] == '3' || c[3].matriz[linha][coluna] == '2' && c[3].matriz[linha][coluna] == '1'){
    //                     printf(ANSI_COLOR_YELLOW " %c ", c[3].matriz[linha][coluna]);
    //                     printf(ANSI_COLOR_RESET "|");
    //                 }else if(c[3].matriz[linha][coluna] == '0'){
    //                     printf(ANSI_COLOR_GREEN " %c ", c[3].matriz[linha][coluna]);
    //                     printf(ANSI_COLOR_RESET "|"); 
    //                 }else{
    //                     printf(ANSI_COLOR_YELLOW " %c ", c[3].matriz[linha][coluna]);
    //                     printf(ANSI_COLOR_RESET "|");
    //                 }    
    //             }
    //         }else{  
    //             printf(" %c |", c[1].matriz[linha][coluna]);
    //         }
            
    //     }
    //     printf("\n");
    //     printf("  ---------------------------------------------------------------------------------\n");
    //     cont_linha++;
    // }
    
}

// Essa função vai verificar se todos os campos que não são bombas 
// estão abertos, retorna 1 para "ganhou", 0 para "perdeu"
// e -1 para "O jogo continua". Essa função também verifica
// se a coordenada é uma bomba, caso seja a função retorna 0 e o jogo acaba. 

int ganhou_perdeu(campo *c){

    return -1;
    // retorna 1 ou 0;
}

// Função que controla o jogo no modo casual.
void modo_casual(campo * c)
{

    //Além da coordenada ser válida o campo deve está fechado!!!
    //Criar uma lógica sem recursividade!

    int opcao;
    int coordenada1, coordenada2;
    printf("\n\n");
    printf("\t\t\t\t    CAMPO MINADO\n\n");

    exibir_campo(c);
    printf("\nInforme uma coordenada: ");
    scanf("%i", &opcao);
    if (opcao == 1)
    scanf("%i %i", &coordenada1, &coordenada2);
    if (coordenada_valida(coordenada1, coordenada2) == 1)
    {
        // usuário escolhe a posição 
        printf("verdadeiro");
        abri_coordena(c, coordenada1, coordenada2);
        exibir_campo(c);
    }
    else
    {
        printf("\nOpção inválida! (%i)\n\n", opcao);
        modo_casual(c);
        printf("falso");
    }
    modo_casual(c);
    
}


// Função que controla o jogo no modo autônomo.
void modo_autonomo(campo * c){

    estatisticas(c);
}

// Gera o menu inicial do jogo
void menu(campo * c)
{
    int opcao;
    // setlocale(LC_ALL, "Portuguese_Brazil");
    printf("Olá! Seja bem-vindo(a) ao Campo Minado.\n Por favor, escolha uma opção de 1 a 3. \n 1 - Jogar: Modo Casual\n 2 - Modo Autônomo\n 3 - Sair.\n\n");
    scanf("%i", &opcao);
    if (opcao > 3 || opcao < 1)
    {
        printf("\nOpção inválida! (%i)\n\n", opcao);
        menu(c);
    }
    else
    {
        // Adicionar o vizualizar tempo da partida!
        if (opcao == 1)
        {
            printf("\nVocê selecionou a opção (%i). Iniciando Modo Casual... \n\n", opcao);
            modo_casual(c);
        }
        else if(opcao == 2)
        {
            printf("\nVocê selecionou a opção (%i). Iniciando Modo Autônomo... \n\n", opcao);
            modo_autonomo(c);
        }
        else if(opcao == 3)
        {
            printf("\nVocê selecionou a opção (%i). Encerrando programa... \n\n", opcao);
        }
    }
}

// verifica se as coordenadas são válidas 
int coordenada_valida(int coordenada1, int coordenada2)
{
    if (coordenada1 >= 0 && coordenada1 < l_constante && coordenada2 >= 0 && coordenada2 < c_constante)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

//Conta quantas bombas existem ao redor da coordenada
int quatidade_bombas_vizinhas(campo * c, int linha, int coluna){

    int cont=0;

    if(coordenada_valida(linha-1, coluna) && c[0].matriz[linha-1][coluna] == 'x'){
        cont++;
    }
    if(coordenada_valida(linha+1, coluna) && c[0].matriz[linha+1][coluna] == 'x'){
        cont++;
    }
    if(coordenada_valida(linha, coluna-1) && c[0].matriz[linha][coluna-1] == 'x'){
        cont++;
    }
    if(coordenada_valida(linha, coluna+1) && c[0].matriz[linha][coluna+1] == 'x'){
        cont++;
    }

    if(coordenada_valida(linha-1, coluna-1) && c[0].matriz[linha-1][coluna-1] == 'x'){
        cont++;
    }
    if(coordenada_valida(linha-1, coluna+1) && c[0].matriz[linha-1][coluna+1] == 'x'){
        cont++;
    }
    if(coordenada_valida(linha+1, coluna-1) && c[0].matriz[linha+1][coluna-1] == 'x'){
        cont++;
    }
    if(coordenada_valida(linha+1, coluna+1) && c[0].matriz[linha+1][coluna+1] == 'x'){
        cont++;
    }

    return cont;
}

// Recebe a quantidade de bombas nas vizinhança da coordenada e adiciona na matriz "quantidade de bombas"
void contar_bombas_vizinhas(campo * c){
    char converte_int_char;
    for(int linha = 0; linha < l_constante; linha++){
        for(int coluna = 0; coluna < c_constante; coluna++){
            converte_int_char = quatidade_bombas_vizinhas(c, linha, coluna) + '0';
            c[3].matriz[linha][coluna] = converte_int_char;
        }   
    }
}

//Abri todos os campos que não tem bombas ao redor 
void abri_coordena(campo * c, int linha, int coluna){

    // 0 matriz com bombas 
    // 1 matriz exibida para o usuário
    // 2 matriz campo aberto ou fechado
    // 3 matriz bombas vizinhas
    

}
// Gera no final da partida as estatísticas do jogo
void estatisticas(campo * c){

}



int main (void){

    setlocale(LC_ALL, "Portuguese");
    campo *c;
    c = malloc (sizeof(campo)*4);
    cria_campo(c);
    preenche_campo_minas(c);
    contar_bombas_vizinhas(c);
    menu(c);

    system("Pause");
    return 0;
}