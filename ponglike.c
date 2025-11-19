#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <curses.h>
#include <stdbool.h>

#define BOLA 'o'
#define PLATAFORMA '|'
#define BARREIRA '#'
#define BLOCO 'B'
#define ESPINHO '^'

typedef struct {int x, y, xa, ya, dirx, diry, angmax, tamanho, passos, velocidade, vida; bool ativo; char icone;} Objeto;
typedef struct {Objeto plataforma[100], bolas[100]; char tela[36][64]; int pontos;} Jogo;

void definirBolas(Jogo *estadoJogo, int quantidade);
void iniciarBola(Jogo *estadoJogo, int i);
void definirTela(Jogo *estadoJogo, int altura, int largura);
int angRand(int max, bool positivo);

Jogo estadoJogo;

int main()
{
return(0);
}
void definirBolas(Jogo *estadoJogo, int quantidade)
{
for(int i = 0; i < quantidade; i++) {iniciarBola(estadoJogo, i);}
}

void iniciarBola(Jogo *estadoJogo, int i)
{
estadoJogo->bolas[i].x = 32; estadoJogo->bolas[i].y = 18; estadoJogo->bolas[i].xa = 1; estadoJogo->bolas[i].ya = 1;
estadoJogo->bolas[i].angmax = 2; estadoJogo->bolas[i].tamanho = 1; estadoJogo->bolas[i].passos = 0;
estadoJogo->bolas[i].dirx = angRand(estadoJogo->bolas[i].angmax, true);
estadoJogo->bolas[i].diry = angRand(estadoJogo->bolas[i].angmax, true);
estadoJogo->bolas[i].velocidade = 1; estadoJogo->bolas[i].vida = 1; estadoJogo->bolas[i].ativo = true;
estadoJogo->bolas[i].icone = 'o';
}

void definirTela(Jogo *estadoJogo, int altura, int largura)
{
	for(int y = 0; y < altura; y++)
	{
	for(int x = 0; x < largura; x++)
	{
	if(((x == 0) || (x == largura)) || ((y == 0) || (y = altura))) {estadoJogo->tela[y][x] = '#';}
	else {estadoJogo->tela[y][x] = ' ';}
	}
	}
}

int angRand(int max, bool positivo)
{
int val = (rand() % max) + 1;
if(positivo) {return(val);}
else {return(-(val));}
}
