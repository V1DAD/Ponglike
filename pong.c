#include <stdlib.h>
#include <unistd.h>
#include <curses.h>
#include <time.h>
#include <stdbool.h>

#define MAXBOLAS (RESOLUCAO*2)
#define RESOLUCAO 50
#define CENTRO (RESOLUCAO/2)

typedef struct {int x, y, xa, ya, dirX, dirY, passos; bool ativo;} Bola;

char tela[RESOLUCAO][RESOLUCAO];
int dir = 0;
int ch, rodando = 1;
int posx = CENTRO, posy = ((RESOLUCAO/10)*2), posxa = CENTRO, dirplat;

int angmax = 2;
int tamplat = 4, velplat = 2;
int numbolas;
int pontos = 0;

void inverterDirecaoAleatoria(int *dir,int angmax,int *passos);
void deteccaoColisaoBola(int *dirEixo,int *eixo,int dir,int *pontos,int angmax, char *proxCasa, Bola *bolaAtual);

int main()
{
Bola bolas[MAXBOLAS];

//definir as bolas
for (int i = 0; i < MAXBOLAS; i++)
	{
	bolas[i].x = CENTRO; bolas[i].y = CENTRO; bolas[i].xa = CENTRO;
	bolas[i].ya = CENTRO; bolas[i].dirX = 1; bolas[i].dirY = 1;
	bolas[i].passos = 0; bolas[i].ativo = false;
	}

srand(time(NULL));
initscr();

//definir a matriz
for (int j = 0; j < RESOLUCAO; j++) { for (int k = 0; k <RESOLUCAO; k++)
	{
	if(j == 0) {tela[j][k] = '#';}
	else if(j == (RESOLUCAO - 1)) {tela[j][k] = '#';}
	else if(k == 0) {tela[j][k] = '#';}
	else if(k == (RESOLUCAO - 1)) {tela[j][k] = '#';}
	else {tela[j][k] = ' ';}}
	}
//criar blocos
for (int i = (RESOLUCAO/4); i < (RESOLUCAO - (RESOLUCAO/4)); i++) {for (int j = 0; j < (RESOLUCAO/10); j++) {tela[i][j + (RESOLUCAO-(RESOLUCAO/5))] = 'B';}}

//criar espinhos
for (int i = 1; i < (RESOLUCAO-1); i++) {tela[i][1] = '^';}

//setup tela
cbreak();
noecho();
curs_set(0);
nodelay(stdscr, TRUE);
keypad(stdscr, TRUE);

bolas[0].ativo = true;

//loop principal
while(rodando)
{
numbolas = 0;
for (int i = 0; i < MAXBOLAS; i++) {if(bolas[i].ativo == true) {numbolas++;}}

//input
ch = getch();
if(ch != ERR)
{
	switch(ch)
	{
	case 27 : rodando = 0; break;
	case 'w': dirplat = -1; break;
	case 's': dirplat = 1; break;
	case 'e': tamplat = tela[posx+tamplat][posy] == ' ' ? tamplat+1 : tamplat; break;
	case 'q': if(tamplat > 1){tela[posx+tamplat-1][posy] = ' ';tamplat--;} break;
	case 'a': velplat++;break;
	case 'd': if(velplat>0) {velplat--;}break;
	case 'f': for(int i = 0; i<MAXBOLAS; i++){if(bolas[i].ativo == false){bolas[i].ativo = true;i = MAXBOLAS+1;}} break;
	case 'c': for(int i = MAXBOLAS-1; i>0; i--){if(bolas[i].ativo == true){bolas[i].ativo = false;i = 0;}} break;
	}
}
//checagem de colisao e movimento da plataforma
for(int i = 0; i<velplat;i++)
{
posxa = posx;
if(dirplat == 1)
{
	switch(tela[posx+tamplat][posy])
	{
	case ' ': posx += dirplat; break;
	case '|': posx += dirplat; break;
	case '#': break;
	case 'o': posx += dirplat; tamplat = tela[posx+tamplat][posy] == ' ' ? tamplat+1 : tamplat; break;
	}
	tela[posxa][posy] = ' ';
}
else if(dirplat == -1)
{
	switch(tela[posx-1][posy])
	{
	case ' ': posx += dirplat; break;
	case '|': posx += dirplat; break;
	case '#': break;
	case 'o': posx += dirplat; tamplat = tela[posx+tamplat][posy] == ' ' ? tamplat+1 : tamplat; break;
	}
	tela[posxa+tamplat-1][posy] = ' ';
}
else {break;}
}
dirplat = 0;
for(int j = 0; j < tamplat; j++) {tela[posx+j][posy] = '|';}

//checagem de colisao e movimento da bola
for(int i = 0; i < MAXBOLAS ; i++)
{
	bolas[i].xa = bolas[i].x; bolas[i].ya= bolas[i].y;
	if((bolas[i].passos < abs(bolas[i].dirX)) && bolas[i].ativo == true)
		{
		dir = bolas[i].dirX < 0 ? -1 : 1;
		deteccaoColisaoBola(&bolas[i].dirX,&bolas[i].x,dir,&pontos,angmax,&tela[bolas[i].x + dir][bolas[i].y],&bolas[i]);
		}
	if( (bolas[i].passos < (abs(bolas[i].dirX) + abs(bolas[i].dirY))) && (bolas[i].passos >= abs(bolas[i].dirX))&& bolas[i].ativo == true )
		{
		dir = bolas[i].dirY < 0 ? -1 : 1;
		deteccaoColisaoBola(&bolas[i].dirY,&bolas[i].y,dir,&pontos,angmax,&tela[bolas[i].x][bolas[i].y + dir],&bolas[i]);
		}

	if(bolas[i].passos == (abs(bolas[i].dirX)+abs(bolas[i].dirY))) {bolas[i].passos = 0;}

	tela[bolas[i].xa][bolas[i].ya] = ' '; tela[bolas[i].x][bolas[i].y] = 'o';
	if(bolas[i].ativo == false)
	{
	tela[bolas[i].xa][bolas[i].ya] = ' '; tela[bolas[i].x][bolas[i].y] = ' ';
	bolas[i].x = CENTRO; bolas[i].y = CENTRO; bolas[i].xa = CENTRO;
	bolas[i].ya = CENTRO; bolas[i].dirX = 1; bolas[i].dirY = 1;
	bolas[i].passos = 0;
	}
}

//print tela


printw("Sair[esc] cima[w] baixo[s] encolher/extender plataforma[q][e] acelerar/desacelerar plataforma[a][d]\n adicionar/remover bola[f][c]\n\n");
for (int j = 0; j < RESOLUCAO; j++) { for (int k = 0; k <RESOLUCAO; k++) { printw("%c ",tela[j][k]); }printw("\n"); }
printw("\nnumbolas : %d ch : %c velplat : %d tamplat : %d pontos : %d",numbolas,ch,velplat,tamplat,pontos);
refresh(); usleep(40000); clear();
}
endwin();
return(0);
}

void inverterDirecaoAleatoria(int *dir,int angmax,int *passos)
{
*dir = *dir < 0 ? ((rand() % angmax) + 1) : -((rand() % angmax) + 1); *passos = 0;
}

void deteccaoColisaoBola(int *dirEixo,int *eixo,int dir,int *pontos,int angmax, char *proxCasa, Bola *bolaAtual)
{
switch(*proxCasa)
{
case '#': inverterDirecaoAleatoria(dirEixo,angmax,&bolaAtual->passos); break;
case '|': inverterDirecaoAleatoria(dirEixo,angmax,&bolaAtual->passos); *pontos = *pontos + 1;break;
case 'B': inverterDirecaoAleatoria(dirEixo,angmax,&bolaAtual->passos); *proxCasa = ' '; *pontos = *pontos + 1;break;
case 'o': inverterDirecaoAleatoria(dirEixo,angmax,&bolaAtual->passos);break;
case '^': bolaAtual->ativo = false; bolaAtual->x = CENTRO;bolaAtual->y = CENTRO;break;
case ' ': *eixo += dir;break;
}
bolaAtual->passos++;
}

//tela[bolas[i].x + dir][bolas[i].y]
//tela[bolas[i].x][bolas[i].y + dir]
