#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <locale.h>

#define l_constante 10
#define c_constante 20
#define q_bombas_constante 40

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GRAY "\e[0;37m"
#define ANSI_COLOR_RESET "\x1b[0m"
#define ANSI_COLOR_YELLOW "\033[1;33m"
#define ANSI_COLOR_GREEN "\033[1;32m"
#define ANSI_COLOR_BLUE "\033[1;34m"
#define ANSI_COLOR_CYAN "\033[1;36m"
#define ANSI_COLOR_MAGENTA "\033[1;35m"
#define ANSI_COLOR_WHITE "\033[1;97m"

// Guia para codificação
// 0 matriz com bombas
// 1 matriz exibida para o usuário
// 2 matriz campo aberto ou fechado
// 3 matriz bombas vizinhas
// Definição da struct que armazena a matriz

int cont_pedidos_ajuda = 0;
int l_cord=0;
int c_cord=0;

enum finaliza_jogo
{
    ganhou = 0,
    perdeu = 1,
    continua = -1
} jogo;

typedef struct
{
    char **matriz;
} campo;

void fechar_campos(campo *c)
{
    for (int linha = 0; linha < l_constante; linha++)
    {
        for (int coluna = 0; coluna < c_constante; coluna++)
        {
            c[2].matriz[linha][coluna] = '0';
        }
    }
}

// Essa função cria a matriz e faz a alocação dinâmica dos campos
void cria_campo(campo *c)
{
    c[0].matriz = calloc(l_constante, sizeof(char *));
    c[1].matriz = calloc(l_constante, sizeof(char *));
    c[2].matriz = calloc(l_constante, sizeof(char *));
    c[3].matriz = calloc(l_constante, sizeof(char *));
    c[4].matriz = calloc(l_constante, sizeof(char *));

    for (int linha = 0; linha < 10; linha++)
    {
        c[0].matriz[linha] = calloc(c_constante, sizeof(char));
        c[1].matriz[linha] = calloc(c_constante, sizeof(char));
        c[2].matriz[linha] = calloc(c_constante, sizeof(char));
        c[3].matriz[linha] = calloc(c_constante, sizeof(char));
        c[4].matriz[linha] = calloc(c_constante, sizeof(char));
    }

    // Adiciona um espaço inicial em todas as posições
    for (int linha = 0; linha < 10; linha++)
    {
        for (int coluna = 0; coluna < 20; coluna++)
        {
            c[0].matriz[linha][coluna] = ' ';
            c[1].matriz[linha][coluna] = ' ';
            c[2].matriz[linha][coluna] = ' ';
            c[3].matriz[linha][coluna] = ' ';
            c[4].matriz[linha][coluna] = ' ';
        }
    }
    // Define que tordos os campos inicialmente estão fechados
    for (int linha = 0; linha < l_constante; linha++)
    {
        for (int coluna = 0; coluna < c_constante; coluna++)
        {
            c[2].matriz[linha][coluna] = '0';
            c[4].matriz[linha][coluna] = '0';
        }
    }
}

// Função para preencher a matriz com as minas
void preenche_campo_minas(campo *c)
{
    // Essa parte do código gera os índices dos campos que ficaram as minas.
    int indices_aleatorios[q_bombas_constante][2];
    int sn = 0;
    int linha_coordenada, coluna_coordenada = 0;

    srand(time(NULL));

    // limpa a matriz de índices aleatórios colocando -1 em todas as posições
    for (int linha = 0; linha < q_bombas_constante; linha++)
    {
        for (int coluna = 0; coluna < 2; coluna++)
        {
            indices_aleatorios[linha][coluna] = -1;
        }
    }
    //-----------------------------------------------------------------------

    // Gera os 40 índices aleatórios das minas
    for (int i = 0; i < q_bombas_constante; i++)
    {
        linha_coordenada = rand() % 10;
        coluna_coordenada = rand() % 20;

        for (int linha = 0; linha < q_bombas_constante; linha++)
        {
            if (linha_coordenada == indices_aleatorios[linha][0] && coluna_coordenada == indices_aleatorios[linha][1])
            {
                sn = 1;
            }
        }
        if (sn == 1)
        {
            i--;
        }
        else
        {
            indices_aleatorios[i][0] = linha_coordenada;
            indices_aleatorios[i][1] = coluna_coordenada;
        }
        sn = 0;
    }
    //----------------------------------------------------------------------------------

    // Preenche a matriz com as minas

    for (int i = 0; i < q_bombas_constante; i++)
    {
        c[0].matriz[indices_aleatorios[i][0]][indices_aleatorios[i][1]] = 'x';
    }
    //----------------------------------------------------------------------
}

// Função para mostrar a matriz
void exibir_campo(campo *c)
{

    int cont_linha = 0;
    printf(ANSI_COLOR_WHITE "    0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17  18  19 \n" ANSI_COLOR_RESET);
    printf("  ---------------------------------------------------------------------------------\n");
    for (int linha = 0; linha < 10; linha++)
    {
        printf(ANSI_COLOR_WHITE "%d |", cont_linha);
        printf(ANSI_COLOR_RESET "");
        for (int coluna = 0; coluna < 20; coluna++)
        {
            if (c[2].matriz[linha][coluna] == '1')
            {
                if (c[0].matriz[linha][coluna] == 'x')
                {
                    printf(ANSI_COLOR_RED " %c ", c[0].matriz[linha][coluna]);
                    printf(ANSI_COLOR_RESET "|");
                }
                else
                {
                    // muda de cor de acordo com o número de minas ao redor
                    if (c[3].matriz[linha][coluna] == '4' || c[3].matriz[linha][coluna] == '3' || c[3].matriz[linha][coluna] == '2' && c[3].matriz[linha][coluna] == '1')
                    {
                        printf(ANSI_COLOR_YELLOW " %c ", c[3].matriz[linha][coluna]);
                        printf(ANSI_COLOR_RESET "|");
                    }
                    else if (c[3].matriz[linha][coluna] == '0')
                    {
                        printf(ANSI_COLOR_GREEN " %c ", c[3].matriz[linha][coluna]);
                        printf(ANSI_COLOR_RESET "|");
                    }
                    else
                    {
                        printf(ANSI_COLOR_YELLOW " %c ", c[3].matriz[linha][coluna]);
                        printf(ANSI_COLOR_RESET "|");
                    }
                }
            }
            else
            {
                printf(" %c |", c[1].matriz[linha][coluna]);
            }
        }
        printf(ANSI_COLOR_WHITE " %d", cont_linha);
        printf(ANSI_COLOR_RESET "");
        printf("\n");
        printf("  ---------------------------------------------------------------------------------\n");
        
        cont_linha++;
    }
    printf(ANSI_COLOR_WHITE "    0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17  18  19 \n" ANSI_COLOR_RESET);
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

// Conta quantas bombas existem ao redor da coordenada
int quatidade_bombas_vizinhas(campo *c, int linha, int coluna)
{

    int cont = 0;

    if (coordenada_valida(linha - 1, coluna) && c[0].matriz[linha - 1][coluna] == 'x')
    {
        cont++;
    }
    if (coordenada_valida(linha + 1, coluna) && c[0].matriz[linha + 1][coluna] == 'x')
    {
        cont++;
    }
    if (coordenada_valida(linha, coluna - 1) && c[0].matriz[linha][coluna - 1] == 'x')
    {
        cont++;
    }
    if (coordenada_valida(linha, coluna + 1) && c[0].matriz[linha][coluna + 1] == 'x')
    {
        cont++;
    }

    if (coordenada_valida(linha - 1, coluna - 1) && c[0].matriz[linha - 1][coluna - 1] == 'x')
    {
        cont++;
    }
    if (coordenada_valida(linha - 1, coluna + 1) && c[0].matriz[linha - 1][coluna + 1] == 'x')
    {
        cont++;
    }
    if (coordenada_valida(linha + 1, coluna - 1) && c[0].matriz[linha + 1][coluna - 1] == 'x')
    {
        cont++;
    }
    if (coordenada_valida(linha + 1, coluna + 1) && c[0].matriz[linha + 1][coluna + 1] == 'x')
    {
        cont++;
    }

    return cont;
}

// Recebe a quantidade de bombas nas vizinhança da coordenada e adiciona na matriz "quantidade de bombas"
void contar_bombas_vizinhas(campo *c)
{
    char converte_int_char;
    for (int linha = 0; linha < l_constante; linha++)
    {
        for (int coluna = 0; coluna < c_constante; coluna++)
        {
            converte_int_char = quatidade_bombas_vizinhas(c, linha, coluna) + '0';
            c[3].matriz[linha][coluna] = converte_int_char;
        }
    }
}

// Abri todos os campos que não tem bombas ao redor
void abrir_coordenada(campo *c, int linha, int coluna)
{

    // 0 matriz com bombas
    // 1 matriz exibida para o usuário
    // 2 matriz campo aberto ou fechado
    // 3 matriz bombas vizinhas

    if (coordenada_valida(linha, coluna) && c[2].matriz[linha][coluna] == '0')
    {

        c[2].matriz[linha][coluna] = '1';

        if (c[3].matriz[linha][coluna] == '0')
        {
            abrir_coordenada(c, linha - 1, coluna);
            abrir_coordenada(c, linha + 1, coluna);
            abrir_coordenada(c, linha, coluna - 1);
            abrir_coordenada(c, linha, coluna + 1);

            abrir_coordenada(c, linha - 1, coluna - 1);
            abrir_coordenada(c, linha - 1, coluna + 1);
            abrir_coordenada(c, linha + 1, coluna - 1);
            abrir_coordenada(c, linha + 1, coluna + 1);
        }
    }
}

void opcao_invalida(int opcao)
{
    printf("Opção inválida! (%i)\n", opcao);
}

void calcular_tempo_jogo(float *ref_tempo_inicial, float *ref_tempo_final, float *ref_tempo_jogo)
{
    *ref_tempo_jogo = *ref_tempo_final - *ref_tempo_inicial;
    float horas = *ref_tempo_jogo / 3600;
    float minutos = (*ref_tempo_jogo / 3600 - (int)horas) * 60;
    int segundos = lround((((*ref_tempo_jogo / 3600 - (int)horas) * 60) - (int)minutos) * 60);
    printf("\n Seu tempo de jogo até agora foi de:\n");
    if (horas == 1)
    {
        printf(" %i Hora,", (int)horas);
    }
    if (horas > 1 || horas < 1)
    {
        printf(" %i Horas,", (int)horas);
    }
    if (minutos == 1)
    {
        printf(" %i Minuto e", (int)minutos);
    }
    if (minutos > 1 || minutos < 1)
    {
        printf(" %i Minutos e", (int)minutos);
    }
    if (segundos == 1)
    {
        printf(" %i Segundo.", segundos);
    }
    if (segundos > 1 || segundos < 1)
    {
        printf(" %i Segundos.", segundos);
    }
}

// Função que retorna uma coordenada que possivelmente não tem uma bomba
void solicitar_ajuda(campo *c, int i)
{
    //   0 matriz com bombas
    //   1 matriz exibida para o usuário
    //   2 matriz campo aberto ou fechado
    //   3 matriz bombas vizinhas

    //   1 - perdeu
    //   0 - ganhou
    //   -1 continua

    //   Se todos os campos estiverem fechados as coodenadas são aleatórias

    srand(time(NULL));
    int g_p = -1;
    int repeticao = 0;
    int contador_campos_abertos = 0;
    if (i <= 6)
    {

        for (int linha = 0; linha < l_constante; linha++)
        {
            for (int coluna = 0; coluna < c_constante; coluna++)
            {
                if (c[2].matriz[linha][coluna] == '1')
                {
                    contador_campos_abertos++;
                }
            }
        }
        if (contador_campos_abertos >= 4)
        {
            solicitar_ajuda(c, 7);
        }
        else
        {
            srand(time(NULL));
            repeticao = 0;
            while (repeticao == 0)
            {

                int num_l_aleatorio = rand() % 10;
                int num_c_aleatorio = rand() % 20;

                if (c[2].matriz[num_l_aleatorio][num_c_aleatorio] == '0' && c[4].matriz[num_l_aleatorio][num_c_aleatorio] != '1')
                {
                    printf("A coordenada %d %d é provável não ter bomba\n", num_l_aleatorio, num_c_aleatorio);
                    l_cord= num_l_aleatorio;
                    c_cord= num_c_aleatorio;
                    repeticao = 1;
                }
            }
        }

    }
    else if (i > 6 && i <= 11)
    {
        identificar_campo_sem_bomba(c);
        int result = abrir_campos(c);
    }
    else
    {
        srand(time(NULL));
        repeticao = 0;
        while (repeticao == 0)
        {

            int num_l_aleatorio = rand() % 10;
            int num_c_aleatorio = rand() % 20;

            if (c[2].matriz[num_l_aleatorio][num_c_aleatorio] == '0' && c[4].matriz[num_l_aleatorio][num_c_aleatorio] != '1')
            {
                printf("A coordenada %d %d é provável não ter bomba\n", num_l_aleatorio, num_c_aleatorio);
                l_cord= num_l_aleatorio;
                c_cord= num_c_aleatorio;
                repeticao = 1;
            }
        }
    }
}


// Verifica se o jogador abriu todos os campos que não são bombas ou abriu uma bomba
int ganhou_perdeu(campo *c)
{
    // 0 matriz com bombas
    // 1 matriz exibida para o usuário
    // 2 matriz campo aberto ou fechado
    // 3 matriz bombas vizinhas

    // 1 - perdeu
    // 0 - ganhou
    // -1 continua

    for (int linha = 0; linha < l_constante; linha++)
    {
        for (int coluna = 0; coluna < c_constante; coluna++)
        {
            if (c[2].matriz[linha][coluna] == '1' && c[0].matriz[linha][coluna] == 'x')
            {
                return 1;
            }
        }
    }

    int cont = 0;
    for (int linha = 0; linha < l_constante; linha++)
    {
        for (int coluna = 0; coluna < c_constante; coluna++)
        {
            if (c[2].matriz[linha][coluna] == '1')
            {
                cont++;
            }
        }
    }
    if (cont == ((l_constante * c_constante) - q_bombas_constante))
    {
        return 0;
    }

    return -1;
}

//Função que auxilia a função "solicitar ajuda" identificando os campos que possivelmente
// tem bombas que, essa função constrói uma nova matriz [4] informando esses valores

void identificar_campo_sem_bomba(campo *c)
{
    // 0 matriz com bombas
    // 1 matriz exibida para o usuário
    // 2 matriz campo aberto ou fechado
    // 3 matriz bombas vizinhas
    int k = 0;
    int coordenada_bombas[40][2];

    for (int i = 0; i < l_constante; i++)
    {
        for (int j = 0; j < c_constante; j++)
        {
            int k_inicio = k;
            int cont = 0;
            int coordenada_validas = 0;
            if (c[2].matriz[i][j] == '1' && c[3].matriz[i][j] != '0')
            {
                if (coordenada_valida(i - 1, j - 1))
                {
                    coordenada_validas++;
                    if (c[2].matriz[i - 1][j - 1] == '1')
                    {
                        cont++;
                    }
                    else
                    {
                        coordenada_bombas[k][0] = i - 1;
                        coordenada_bombas[k][1] = j - 1;
                        k++;
                    }
                }
                if (coordenada_valida(i - 1, j))
                {
                    coordenada_validas++;
                    if (c[2].matriz[i - 1][j] == '1')
                    {
                        cont++;
                    }
                    else
                    {
                        coordenada_bombas[k][0] = i - 1;
                        coordenada_bombas[k][1] = j;
                        k++;
                    }
                }
                if (coordenada_valida(i - 1, j + 1))
                {
                    coordenada_validas++;
                    if (c[2].matriz[i - 1][j + 1] == '1')
                    {
                        cont++;
                    }
                    else
                    {
                        coordenada_bombas[k][0] = i - 1;
                        coordenada_bombas[k][1] = j + 1;
                        k++;
                    }
                }
                if (coordenada_valida(i, j - 1))
                {
                    coordenada_validas++;
                    if (c[2].matriz[i][j - 1] == '1')
                    {
                        cont++;
                    }
                    else
                    {
                        coordenada_bombas[k][0] = i;
                        coordenada_bombas[k][1] = j - 1;
                        k++;
                    }
                }
                if (coordenada_valida(i, j + 1))
                {
                    coordenada_validas++;
                    if (c[2].matriz[i][j + 1] == '1')
                    {
                        cont++;
                    }
                    else
                    {
                        coordenada_bombas[k][0] = i;
                        coordenada_bombas[k][1] = j + 1;
                        k++;
                    }
                }
                if (coordenada_valida(i + 1, j - 1))
                {
                    coordenada_validas++;
                    if (c[2].matriz[i + 1][j - 1] == '1')
                    {
                        cont++;
                    }
                    else
                    {
                        coordenada_bombas[k][0] = i + 1;
                        coordenada_bombas[k][1] = j - 1;
                        k++;
                    }
                }
                if (coordenada_valida(i + 1, j))
                {
                    coordenada_validas++;
                    if (c[2].matriz[i + 1][j] == '1')
                    {
                        cont++;
                    }
                    else
                    {
                        coordenada_bombas[k][0] = i + 1;
                        coordenada_bombas[k][1] = j;
                        k++;
                    }
                }
                if (coordenada_valida(i + 1, j + 1))
                {
                    coordenada_validas++;
                    if (c[2].matriz[i + 1][j + 1] == '1')
                    {
                        cont++;
                    }
                    else
                    {
                        coordenada_bombas[k][0] = i + 1;
                        coordenada_bombas[k][1] = j + 1;
                        k++;
                    }
                }
                cont = coordenada_validas - cont;
                char caractere = cont + '0';
                if (caractere == c[3].matriz[i][j])
                {
                    for (int l = k_inicio; l < k; l++)
                    {
                        c[4].matriz[coordenada_bombas[l][0]][coordenada_bombas[l][1]] = '1';
                    }
                }
                else
                {
                    k = k_inicio;
                }
            }
        }
    }
}

//função que avalia os campos próximos as bombas que a função "identificar_campo_sem_bomba" qualificou,
// verificando quais campos podem ser abertos. Essa função exibe para o usuário um desses valores. 
int abrir_campos(campo *c)
{
    // 0 matriz com bombas
    // 1 matriz exibida para o usuário
    // 2 matriz campo aberto ou fechado
    // 3 matriz bombas vizinhas
    // 4 matriz bombas descobertas

    for (int i = 0; i < l_constante; i++)
    {
        for (int j = 0; j < c_constante; j++)
        {
            int k = 0;
            int cont = 0;
            int coordenada_validas = 0;
            int coordenada_abrir[100][2];
            if (c[2].matriz[i][j] == '1' && c[3].matriz[i][j] != '0')
            {
                if (coordenada_valida(i - 1, j - 1))
                {
                    coordenada_validas++;
                    if (c[4].matriz[i - 1][j - 1] == '1')
                    {
                        cont++;
                    }
                    else
                    {
                        if (c[2].matriz[i - 1][j - 1] == '0')
                        {
                            coordenada_abrir[k][0] = i - 1;
                            coordenada_abrir[k][1] = j - 1;
                            k++;
                        }
                    }
                }
                if (coordenada_valida(i - 1, j))
                {
                    coordenada_validas++;
                    if (c[4].matriz[i - 1][j] == '1')
                    {
                        cont++;
                    }
                    else
                    {
                        if (c[2].matriz[i - 1][j] == '0')
                        {
                            coordenada_abrir[k][0] = i - 1;
                            coordenada_abrir[k][1] = j;
                            k++;
                        }
                    }
                }
                if (coordenada_valida(i - 1, j + 1))
                {
                    coordenada_validas++;
                    if (c[4].matriz[i - 1][j + 1] == '1')
                    {
                        cont++;
                    }
                    else
                    {
                        if (c[2].matriz[i - 1][j + 1] == '0')
                        {
                            coordenada_abrir[k][0] = i - 1;
                            coordenada_abrir[k][1] = j + 1;
                            k++;
                        }
                    }
                }
                if (coordenada_valida(i, j - 1))
                {
                    coordenada_validas++;
                    if (c[4].matriz[i][j - 1] == '1')
                    {
                        cont++;
                    }
                    else
                    {
                        if (c[2].matriz[i][j - 1] == '0')
                        {
                            coordenada_abrir[k][0] = i;
                            coordenada_abrir[k][1] = j - 1;
                            k++;
                        }
                    }
                }
                if (coordenada_valida(i, j + 1))
                {
                    coordenada_validas++;
                    if (c[4].matriz[i][j + 1] == '1')
                    {
                        cont++;
                    }
                    else
                    {
                        if (c[2].matriz[i][j + 1] == '0')
                        {
                            coordenada_abrir[k][0] = i;
                            coordenada_abrir[k][1] = j + 1;
                            k++;
                        }
                    }
                }
                if (coordenada_valida(i + 1, j - 1))
                {
                    coordenada_validas++;
                    if (c[4].matriz[i + 1][j - 1] == '1')
                    {
                        cont++;
                    }
                    else
                    {
                        if (c[2].matriz[i + 1][j - 1] == '0')
                        {
                            coordenada_abrir[k][0] = i + 1;
                            coordenada_abrir[k][1] = j - 1;
                            k++;
                        }
                    }
                }
                if (coordenada_valida(i + 1, j))
                {
                    coordenada_validas++;
                    if (c[4].matriz[i + 1][j] == '1')
                    {
                        cont++;
                    }
                    else
                    {
                        if (c[2].matriz[i + 1][j] == '0')
                        {
                            coordenada_abrir[k][0] = i + 1;
                            coordenada_abrir[k][1] = j;
                            k++;
                        }
                    }
                }
                if (coordenada_valida(i + 1, j + 1))
                {
                    coordenada_validas++;
                    if (c[4].matriz[i + 1][j + 1] == '1')
                    {
                        cont++;
                    }
                    else
                    {
                        if (c[2].matriz[i + 1][j + 1] == '0')
                        {
                            coordenada_abrir[k][0] = i + 1;
                            coordenada_abrir[k][1] = j + 1;
                            k++;
                        }
                    }
                }
                char caractere = cont + '0';
                if (caractere == c[3].matriz[i][j])
                {
                    for (int l = 0; l < k; l++)
                    {
                        printf("A coordenada %i %i possivelmente não tem bomba\n", coordenada_abrir[l][0], coordenada_abrir[l][1]);
                        l_cord = coordenada_abrir[l][0];
                        c_cord = coordenada_abrir[l][1];
                        return 1;
                        
                    }
                }
            }
        }
    }
}

// função que realiza o modo autônomo chamando a função "solicita ajuda" 
//até que o jogo finalize.
void modo_autonomo(campo *c)
{

    int cont =0;
    int g_p= -1;

    while(g_p == -1){
        // 1 - perdeu
        // 0 - ganhou
        // -1 continua

        solicitar_ajuda(c, cont);
        abrir_coordenada(c, l_cord, c_cord);
        exibir_campo(c);

        cont++;

        g_p=ganhou_perdeu(c);

        if(g_p == perdeu){
            printf("Você perdeu\n");
            gera_arquivo(cont);
            exit(0);
        }else if(g_p==ganhou){
            printf("Você ganhou =) \n");
            gera_arquivo(cont);
            exit(0);
        }
    }
}

//Função gera as variações do jogo de acordo com o que foi solicitado para o usuário
// 1 - começar com um campo vazio; 0 - campo com numero.
void variacao_inicia_numero(campo *c, int linha_cord, int coluna_cord, int variacao)
{

    if (variacao == 1)
    {

        while (c[0].matriz[linha_cord][coluna_cord] == 'x' || c[3].matriz[linha_cord][coluna_cord] == '0')
        {
            preenche_campo_minas(c);
            contar_bombas_vizinhas(c);
        }
    }
    else if (variacao == 2)
    {

        while (c[0].matriz[linha_cord][coluna_cord] == 'x' || c[3].matriz[linha_cord][coluna_cord] != '0')
        {
            preenche_campo_minas(c);
            contar_bombas_vizinhas(c);
        }
        printf("Você iniciou na coordenada 0 0 =)\n");
    }
}

//função controla o modo casual
void modo_casual(campo *c, float *ref_tempo_inicial)
{
    int coordenada1, coordenada2, opcao;
    int contador_jogadas = 0;
    printf("\n\n");
    // printf("\t\t\t\t    CAMPO MINADO\n\n");
    // exibir_campo(c);
    printf("1 - Informar uma coordenada; 2 - Tempo de jogo; 3 - Desistir do jogo; 4 - Solicitar ajuda;\n");
    scanf("\n%i", &opcao);
    if (opcao == 1)
    {

        int linha_cord, coluna_cord = 0;
        int contador_fechado = 0;
        int opcao_variacao = 0;

        for (int linha = 0; linha < l_constante; linha++)
        {
            for (int coluna = 0; coluna < c_constante; coluna++)
            {
                if (c[2].matriz[linha][coluna] == '0')
                {
                    contador_fechado++;
                }
            }
        }
        if (contador_fechado == (l_constante * c_constante))
        {
            printf("1 - Iniciar com um número. 2 - Iniciar com um campo vazio. 3 - Jogar normalmente\n");
            scanf("%d", &opcao_variacao);

            if (opcao_variacao == 1 || opcao_variacao == 2)
            {

                variacao_inicia_numero(c, 0, 0, opcao_variacao);
                coordenada1 = linha_cord;
                coordenada2 = coluna_cord;
            }
            else if (opcao_variacao == 3)
            {
                printf("\nInforme uma coordenada: ");
                scanf("%i %i", &coordenada1, &coordenada2);
            }
        }
        else
        {
            printf("\nInforme uma coordenada: ");
            scanf("%i %i", &coordenada1, &coordenada2);
        }

        // printf("\nInforme uma coordenada: ");
        // scanf("%i %i", &coordenada1, &coordenada2);
        if (coordenada_valida(coordenada1, coordenada2) == 1)
        {
            // printf("verdadeiro\n");

            abrir_coordenada(c, coordenada1, coordenada2);
            contador_jogadas++;
            exibir_campo(c);

            int g_p = ganhou_perdeu(c);

            if (g_p == perdeu)
            {
                printf("Você perdeu!\n");
                gera_arquivo(contador_jogadas);
                free(c);
                cria_campo(c);
                preenche_campo_minas(c);
                contar_bombas_vizinhas(c);
                fechar_campos(c);
                menu(c);
            }
            else if (g_p == ganhou)
            {
                printf("Você ganhou!\n");
                gera_arquivo(contador_jogadas);
                free(c);
                cria_campo(c);
                preenche_campo_minas(c);
                contar_bombas_vizinhas(c);
                fechar_campos(c);
                menu(c);
            }
            modo_casual(c, ref_tempo_inicial);
        }
        else
        {
            printf("Coordenada inválida! (%i %i) Tente novamente.", coordenada1, coordenada2);
            modo_casual(c, ref_tempo_inicial);
        }
    }
    else if (opcao == 2)
    {
        time_t tempo;
        struct tm *infoTempo;
        time(&tempo);
        infoTempo = localtime(&tempo);
        float tempo_final = infoTempo->tm_sec + (infoTempo->tm_min * 60) + (infoTempo->tm_hour - 3) * 3600;
        float *ref_tempo_final = &tempo_final;
        float tempo_jogo;
        float *ref_tempo_jogo = &tempo_jogo;
        calcular_tempo_jogo(ref_tempo_inicial, ref_tempo_final, ref_tempo_jogo);
        printf("\n");
        exibir_campo(c);
        modo_casual(c, ref_tempo_inicial);
    }
    else if (opcao == 3)
    {
        int opcao_saida = 5;
        while (opcao_saida < 1 || opcao_saida > 2)
        {
            printf("\nTem certeza que deseja sair do jogo? Todo seu progresso será apagado. Digite 1 para (Sair do Jogo) e 2 para (Voltar ao Jogo).\n\n");
            scanf("%i", &opcao_saida);
            if (opcao_saida == 1)
            {
                free(c);
                cria_campo(c);
                preenche_campo_minas(c);
                contar_bombas_vizinhas(c);
                fechar_campos(c);
                menu(c);
            }
            else if (opcao_saida == 2)
            {
                printf("Ok! Voltando para o mesmo ponto do jogo...\n");

                exibir_campo(c);
                modo_casual(c, ref_tempo_inicial);
                // modo_casual(c,ref_tempo_inicial);
            }
            else
            {
                opcao_invalida(opcao_saida);
            }
        }
    }
    else if (opcao == 4)
    {

        solicitar_ajuda(c, cont_pedidos_ajuda);
        cont_pedidos_ajuda++;

        // identificar_campo_sem_bomba(c);
        // exibir_campo(c);
        modo_casual(c, ref_tempo_inicial);
    }
    else
    {
        opcao_invalida(opcao);
        exibir_campo(c);
        modo_casual(c, ref_tempo_inicial);
    }
}

//função que controla a exibição do menu inicial
void menu(campo *c)
{
    int opcao;
    printf("*---------------------------------------------------------------------*\n");
    printf("*                                                                     *\n");
    printf("*                                                                     *\n");
    printf("*                            CAMPO MINADO                             *\n");
    printf("*                                                                     *\n");
    printf("*                        Alunos: Luiz e Lucas                         *\n");
    printf("*                                                                     *\n");
    printf("*                                                                     *\n");
    printf("*---------------------------------------------------------------------*\n");
    printf("\nOlá! Seja bem-vindo(a) ao Campo Minado.\n\n Por favor, escolha uma opção de 1 a 3. \n 1 - Jogar: Modo Casual\n 2 - Modo Autônomo\n 3 - Sair.\n\n");
    scanf("%i", &opcao);
    if (opcao > 3 || opcao < 1)
    {
        opcao_invalida(opcao);
        menu(c);
    }
    else
    {
        if (opcao == 1)
        {
            printf("\nVocê selecionou a opção (%i). Iniciando Modo Casual... \n\n", opcao);
            time_t tempo;
            struct tm *infoTempo;
            time(&tempo);
            infoTempo = localtime(&tempo);
            float tempo_inicial = infoTempo->tm_sec + (infoTempo->tm_min * 60) + (infoTempo->tm_hour - 3) * 3600;
            float *ref_tempo_inicial = &tempo_inicial;
            exibir_campo(c);
            modo_casual(c, ref_tempo_inicial);
        }
        else if (opcao == 2)
        {
            printf("\nVocê selecionou a opção (%i). Iniciando Modo Autônomo... \n\n", opcao);
            modo_autonomo(c);
        }
        else if (opcao == 3)
        {
            int opcao_saida = 5;
            while (opcao_saida < 1 || opcao_saida > 2)
            {
                printf("\nTem certeza que deseja sair do programa? Digite 1 para (Sair do Programa) e 2 para (Voltar ao Menu Inicial).\n\n");
                scanf("%i", &opcao_saida);
                if (opcao_saida == 1)
                {
                    printf("\nVocê selecionou a opção (%i). Encerrando programa... \n\n", opcao);
                    exit(0);
                }
                else if (opcao_saida == 2)
                {
                    printf("Ok! Voltando ao Menu Inicial...\n");
                    menu(c);
                }
                else
                {
                    opcao_invalida(opcao_saida);
                }
            }
        }
    }
}

//Função que gena no final da execução do código um arquivo com a quantidade de jogadas feitas
void gera_arquivo(int jogadas)
{
    FILE *pont_arq;

    pont_arq = fopen("numero_de_jogadas.txt", "a");

    fclose(pont_arq);

    fopen("numero_de_jogadas.txt", "w");

    fprintf(pont_arq, "Quantidade de jogadas:\n");
    fprintf(pont_arq, "%d", jogadas + 1);
    fclose(pont_arq);

    printf("O arquivo foi criado com sucesso!");
}

int main()
{

    setlocale(LC_ALL, "Portuguese");
    campo *c;
    c = malloc(sizeof(campo) * 5);
    cria_campo(c);
    preenche_campo_minas(c);
    contar_bombas_vizinhas(c);
    fechar_campos(c);
    menu(c);

    system("Pause");
    return 0;
}