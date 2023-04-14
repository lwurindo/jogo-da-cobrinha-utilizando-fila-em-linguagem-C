// Jogo feito por Eric Kennedy Laurindo //

#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include<conio.h>
#include<time.h>
#include <locale.h>

// Variáveis Globais
int c[300][2], pontos=1, cx=2, cy=2;
int comida[2], velo=150;
int i, velx, vely, num_colisoes = 0;


// ----------------------------------- Começo do Menu ----------------------------------- \\

void linhaCol(int lin, int col);
void box(int lin1, int col1, int lin2, int col2);
int menu(int lin1, int col1, int qtd, char lista[3][40]);
void textColor(int letras, int fundo);

    // Cores designadas para a Letra.
enum {BLACK,
      BLUE,
      GREEN,
      CYAN,
      RED,
      MAGENTA,
      BROWN,
      LIGHTGRAY,
      DARKGRAY,
      LIGHTBLUE,
      LIGHTGREEN,
      LIGHTCYAN,
      LIGHTRED,
      LIGHTMAGENTA,
      YELLOW,
      WHITE
};

     // Cores designadas para o Fundo.
enum {_BLACK=0,
      _BLUE=16,
      _GREEN=32,
      _CYAN=48,
      _RED=64,
      _MAGENTA=80,
      _BROWN=96,
      _LIGHTGRAY=112,
      _DARKGRAY=128,
      _LIGHTBLUE=144,
      _LIGHTGREEN=160,
      _LIGHTCYAN=176,
      _LIGHTRED=192,
      _LIGHTMAGENTA=208,
      _YELLOW=224,
      _WHITE=240
};


void textColor(int letra, int fundo)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), letra + fundo);
}

void linhaCol(int lin, int col)
{
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),(COORD)
    {
        col-1,lin-1
    });

    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}


void box(int lin1, int col1, int lin2, int col2)
{
    int i,j, tamlin, tamcol;

    tamlin = lin2 - lin1;
    tamcol = col2 - col1;


    for (i=col1; i<=col2; i++)
    {
        linhaCol(lin1,i);
        printf("%c",196);
        linhaCol(lin2,i);
        printf("%c",196);
    }

    for (i=lin1; i<=lin2; i++)
    {
        linhaCol(i,col1);
        printf("%c",179);
        linhaCol(i,col2);
        printf("%c",179);
    }

    for (i=lin1+1; i<lin2; i++)
    {
        for(j=col1+1; j<col2; j++)
        {
            linhaCol(i,j);
            printf(" ");
        }
    }

    linhaCol(lin1,col1);
    printf("%c",218);
    linhaCol(lin1,col2);
    printf("%c",191);
    linhaCol(lin2,col1);
    printf("%c",192);
    linhaCol(lin2,col2);
    printf("%c",217);

}

int menu(int lin1, int col1, int qtd, char lista[3][40])
{
    int opc=1, lin2, col2, linha,i,tamMaxItem, tecla;

    tamMaxItem = strlen(lista[0]);
    for(i=1; i<qtd; i++)
    {
        if(strlen(lista[i])>tamMaxItem)
        {
            tamMaxItem = strlen(lista[i]);
        }
    }
    lin2 = lin1+(qtd*2+2);
    col2 = col1+tamMaxItem+4;

    while(1)
    {

        linha=lin1+2;
        for(i=0; i<qtd; i++)
        {
            if(i+1==opc)textColor(BLACK, _LIGHTGREEN);
            else textColor(WHITE, _BLUE);
            linhaCol(linha,col1+2);
            printf("%s",lista[i]);
            linha +=2;
        }

        linhaCol(1,1);
        tecla= getch();
        linhaCol(22,1);
        if(tecla==27)
        {
            opc=0;
            break;
        }
        else if(tecla==13)
        {
            break;
        }

        else if(tecla==72)
        {
            if(opc>1)opc--;

        }
        else if(tecla==80 )
        {
            if (opc<qtd)opc++;

        }
    }
    return opc;
}

// ----------------------------------- Fim do Menu ----------------------------------- \\

/* Função para posicionar o cursos em uma coordenada específica da tela. */
void gotoxy(int x, int y)
{
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),(COORD)
    {
        x,y
    });
}

/* Define uma estrutura No, que representa um nó de uma lista encadeada. O campo proximo é usado para encadear os nós em uma lista, permitindo que se percorra todos eles a partir de um ponto inicial. */
typedef struct no
{
    int num;
    int x, y;
    int cor;
    struct no *proximo;
} No;

No *fila = NULL;

No* inserir_cabeca(No **fila, int x, int y, int pontos)
{
    No *novo = malloc(sizeof(No));
    if(novo)
    {
        novo->num = pontos;
        novo->x = x;
        novo->y = y;
        novo->cor = rand() % 7 + 31;  // atribui uma cor aleatória
        novo->proximo = *fila;
        *fila = novo;
        return novo;
    }
    else
    {
        printf("Erro ao alocar memoria.\n");
        return NULL;
    }
}

void desenha(No *fila)
{
    No *no = fila;
    while (no)
    {
        printf("\033[%dm", no->cor);  // define a cor atual
        gotoxy(no->x, no->y);
        printf("%c",219);
        no = no->proximo;
    }
    printf("\033[0m");  // restaura a cor padrão do terminal
}

void atualiza()
{
    // Atualiza a posição da cobrinha
    int x = fila->x, y = fila->y;

    gotoxy(fila->x, fila->y);
    printf(" ");

    No *no = fila;
    while (no->proximo)
    {
        no->x = no->proximo->x;
        no->y = no->proximo->y;
        no = no->proximo;
    }
    no->x = cx;
    no->y = cy;

    cx += velx;
    cy += vely;
}


void remover_cabeca(No **fila)
{
    No *no = *fila;
    No *anterior = NULL;
    if (no)
    {
        while (no->proximo != NULL)
        {
            anterior = no;
            no = no->proximo;
        }
        cx = no->x;
        cy = no->y;
        if (anterior != NULL)
        {
            anterior->proximo = no->proximo;
        }
        else
        {
            *fila = no->proximo;
        }
        free(no);
    }
}

void verifica_colisao_comida()
{
    if (cx == comida[0] && cy == comida[1])
    {
        geraComida();
        inserir_cabeca(&fila, cx, cy, pontos);
        pontos++;
    }
}


int analiza()
{
    // Verifica se a cobrinha encostou em seu próprio corpo
    No *no = fila->proximo; // Começa a partir do segundo nó
    while (no)
    {
        if (cx == no->x && cy == no->y)
        {
            num_colisoes++;
            if (num_colisoes == 3)
            {
                return 1;
            }
            // Remove a cabeça da cobra
            remover_cabeca(&fila);
            break;
        }
        no = no->proximo;
    }
    return 0;
}


void geraComida()
{
    // Gera comida em local aleatório dentro do retângulo
    gotoxy(comida[0], comida[1]);
    printf(" ");
    srand(time(NULL));
    do
    {
comida[0] = (rand() % 49) + 31; // coordenada x dentro do retângulo
        comida[1] = (rand() % 17) + 4; // coordenada y dentro do retângulo
    }
    while (comida[0] <= 20 || comida[0] >= 70 || comida[1] <= 4 || comida[1] >= 20);   // verifica se está dentro do retângulo
    gotoxy(comida[0], comida[1]);
    printf("%c", 4);
}

void posicionar_mensagem(int linha, int coluna, char *mensagem, char *variavel) {
    // Definir a posição do cursor na tela usando ANSI escape codes
    printf("\x1b[%d;%dH", linha, coluna);
    // Imprimir a mensagem na posição especificada
    printf("%s", mensagem);

    // Verificar se a variável está presente e imprimi-la junto com a mensagem
    if (variavel != NULL) {
        printf("%s", variavel);
    }
}

void snakeASCI()
{
    printf("\n\n");

printf("                      _             \n");
printf("      ___ _ __   __ _| | _____ \n");
printf("     / __| '_ \\ / _` | |/ / _ \\ \n");
printf("     \\__ \\ | | | (_| |   <  __/ \n");
printf("     |___/_| |_|\\__,_|_|\\_\\___| \n");
printf("    \n\n");

    printf("               /^\\/^\\\n");
    printf("             _|__|  O|\n");
    printf("    \\/     /~     \\_/ \\\n");
    printf("     \\____|__________/  \\\n");
    printf("            \\_______      \\\n");
    printf("                    `\\     \\                 \\\n");
    printf("                      |     |                  \\\n");
    printf("                     /      /                    \\\n");
    printf("                    /     /                       \\\\\n");
    printf("                  /      /                         \\ \\\n");
    printf("                 /     /                            \\  \\\n");
    printf("               /     /             _----_            \\   \\\n");
    printf("              /     /           _-~      ~-_         |   |\n");
    printf("             (      (        _-~    _--_    ~-_     _/   |\n");
    printf("              \\      ~-____-~    _-~    ~-_    ~-_-~    /\n");
    printf("                ~-_           _-~          ~-_       _-~\n");
    printf("                   ~--______-~                ~-___-~\n");
}

int main()
{
    int opc, opcSnake;
    int gameover=0;
    int tecla;

    char listaCobra[4][40]= {"Comecar o jogo", "Como Jogar", "Creditos", "Sair"};

            do
            {
                textColor(WHITE, _BLACK);
                system("cls");
                snakeASCI();
                opcSnake = menu(14,70,4,listaCobra);
                int tamanhoFila = inserir_cabeca(&fila, cx, cy, pontos);

                switch (opcSnake)
                {
                case 1:
                    textColor(WHITE, _BLACK);
                    system("cls");
                    tecla='d'; // O jogo começa com a direção para a direita.
                    for(i=30; i<=80; i++)  // Linha superior
                    {
                        gotoxy(i,4);
                        printf("%c",219);
                        Sleep(5); // Pausa execução por 5 milissegunos
                    }
                    for(i=5; i<=20; i++)  // Coluna da direita
                    {
                        gotoxy(80,i);
                        printf("%c",219);
                        Sleep(5); // Pausa execução por 5 milissegunos
                    }
                    for(i=80; i>=30; i--)  // Linha inferior
                    {
                        gotoxy(i,20);
                        printf("%c",219);
                        Sleep(5); // Pausa execução por 5 milissegunos
                    }
                    for(i=19; i>=5; i--)  // Coluna da esquerda
                    {
                        gotoxy(30,i);
                        printf("%c",219);
                        Sleep(5); // Pausa execução por 5 milissegunos
                    }
                    geraComida(); // Gera a primeira comida
                    desenha(fila); // Desenha a cobra
                    cx=35;
                    cy=6;

                    while(gameover==0)  // Enquanto o usuário não perder
                    {
                        gotoxy(88,2); // Move o cursor para c: 52, l: 2
                        printf("Tamanho da fila: %d\t",pontos);
                        gotoxy(88,4);
                        printf("Vidas: %d", (3-num_colisoes));


                        c[0][0]=cx;
                        c[0][1]=cy;


                        if(kbhit()) // Se alguma tecla for apertada, o valor
                            tecla=getch(); // vai para a variável 'tecla'

                        if(tecla=='w' || tecla=='W')
                        {
                            cy--;
                            if(cy==4) break; // Se a cabeça da cobra estiver na parede superior,
                        }                    // O jogo acaba
                        if(tecla=='a' || tecla=='A')
                        {
                            cx--;
                            if(cx==30) break; // Se a cabeça da cobra estiver na parede da esquerda,
                        }                    // O Jogo acaba
                        if(tecla=='s' || tecla=='S')
                        {
                            cy++;
                            if(cy==20) break; // Se a cabeça da cobra estiver na parede de baixo,
                        }                     // O jogo acaba
                        if(tecla=='d' || tecla=='D')
                        {
                            cx++;
                            if(cx>=80) break; // Se a a cabeça da cobra estiver na parede da direida,// O jogo acaba
                        }

                        if(cx==comida[0] && cy==comida[1])  // Se a cobra comer a comida
                        {
                            pontos++;
                            inserir_cabeca(&fila, cx, cy, pontos);
                            if(velo>50) velo-=10; // Velocidade em milissegundos abaixa
                            geraComida();
                        }
                        gameover=analiza();
                        atualiza(); // Atualiza a cobra
                        desenha(fila); // Desenha a cobra
                        gotoxy(50,20);
                        Sleep(velo);
                    }
                    textColor(WHITE, _BLACK);
                    system("cls"); // Quando o usuário perder, limpa a tela e exibe uma mensagem final
                    printf("\n\n\n\n\t\t\tVoce perdeu! Sua fila chegou no tamanho de %d.\n",pontos);
                    posicionar_mensagem(6, 25, "             ____", NULL);
                    posicionar_mensagem(7, 25, "            / . .\\", NULL);
                    posicionar_mensagem(8, 25, "            \  ---<", NULL);
                    posicionar_mensagem(9, 25, "             \  /", NULL);
                    posicionar_mensagem(10, 25, "  __________/ /", NULL);
                    posicionar_mensagem(11, 25, "-=___________/", NULL);

                    pontos = 0;
                    for(i = 0; i < tamanhoFila; i++)
                    {
                        remover_cabeca(&fila);
                        tamanhoFila--;
                        pontos = pontos - pontos + 1;
                        num_colisoes = 0;
                        velo = 150;

                    }

                    system("pause");

                    break;
                case 2:
                    textColor(WHITE, _BLACK);
                    system("cls");
                    posicionar_mensagem(10, 50, "Como jogar: ", NULL);
                    posicionar_mensagem(12, 22, "Seu objetivo eh controlar uma cobra que se move pela tela em busca de comida.", NULL);
                    posicionar_mensagem(14, 18, "A cada vez que a cobra come, ela cresce um pouco mais e a velocidade do jogo tambem aumenta.", NULL);
                    posicionar_mensagem(16, 22, "Para jogar, voce deve usar as setas: W - Cima, D - Direita, S - Baixo, A - Esquerda.", NULL);
                    posicionar_mensagem(18, 34, "Voce possui 3 vidas. A colisao com a parede eh GAMEOVER!", NULL);
                    posicionar_mensagem(20, 28, "Porem a colisao com seu proprio corpo remove 1 de vida e 1 de tamanho.", NULL);
                    posicionar_mensagem(22, 28, "A sua pontuacao eh baseada no numero de alimentos coletados, BOA SORTE!", NULL);

                    getch();
                    break;
                case 3:
                        textColor(WHITE, _BLACK);
                        system("cls");
                        posicionar_mensagem(10, 43, "Jogo desenvolvido pelos alunos: ", NULL);
                        posicionar_mensagem(11, 43, "Eric Kennedy", NULL);
                        posicionar_mensagem(12, 43, "Mateus Brocatto", NULL);
                        posicionar_mensagem(13, 43, "Pedro", NULL);
                        posicionar_mensagem(15, 43, "Prof. Orientador: ", NULL);
                        posicionar_mensagem(16, 43, "Alexandre Donizetti", NULL);
                        getch();
                        break;

                case 4:
                    textColor(WHITE, _BLACK);
                    system("cls");
                    posicionar_mensagem(15, 50, "Encerrando...", NULL);
                    Sleep(1000);
                    return 0;
                    break;
                }
            }while(opcSnake !=4);

}

