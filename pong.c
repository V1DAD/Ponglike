#include <stdlib.h>
#include <unistd.h>
#include <curses.h>
#include <time.h>
#include <stdbool.h>

#define MAXBOLAS (RESOLUCAO*2)
#define RESOLUCAO 50
#define CENTRO (RESOLUCAO/2)

struct bola {int x, y, xa, ya, dirX, dirY, passos; bool ativo;};

struct bola bolas[MAXBOLAS];

char tela[RESOLUCAO][RESOLUCAO];
int dir = 0;
int ch, rodando = 1;
int posx = CENTRO, posy = ((RESOLUCAO/10)*2), posxa = CENTRO, dirplat;

int angmax = 2;
int tamplat = 4, velplat = 2;
int numbolas;
int pontos = 0;

int main()
{

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
	case 'w': dirplat = -1;if(posx-velplat<1){posx = 1;}
				else if((posx-1 > 0) && (tela[posx-velplat][posy] != '#')) {posx -= velplat;} break;
	case 's': dirplat = 1; if(posx+tamplat+velplat>RESOLUCAO-1){posx = RESOLUCAO-1-tamplat;}
				else if((posx+tamplat+velplat < RESOLUCAO-1) && (tela[posx+tamplat+velplat][posy] != '#')) {posx += velplat;} break;
	case 'e': tamplat = tela[posx+tamplat][posy] == ' ' ? tamplat+1 : tamplat; break;
	case 'q': if(tamplat > 1){tamplat--; dirplat = 1; if(tela[posx+tamplat][posy] != '#'){posx += 1;}} break;
	case 'a': velplat++;break;
	case 'd': if(velplat>0) {velplat--;}break;
	case 'f': for(int i = 0; i<MAXBOLAS; i++){if(bolas[i].ativo == false){bolas[i].ativo = true;i = MAXBOLAS+1;}} break;
	case 'c': for(int i = MAXBOLAS-1; i>0; i--){if(bolas[i].ativo == true){bolas[i].ativo = false;i = 0;}} break;
	}
}
//checagem de colisao e movimento da plataforma
if(dirplat == 1)
{
	for(int i = 0; posxa+i <= posx; i++)
	{tela[posxa+i][posy] = tela[posxa+i][posy] != '#' ? ' ' : '#';}
}
else if(dirplat == -1)
{
	for(int i = 0; posxa+tamplat-i >= posxa; i++)
	{tela[posxa+tamplat-i][posy] = tela[posxa+tamplat-i][posy] != '#' ? ' ' : '#';}
}

for(int i = 0; i < tamplat; i++) {tela[posx+i][posy] = tela[posx+i][posy] != '#' ? '|' : '#';}
posxa = posx;


//checagem de colisao e movimento da bola
for(int i = 0; i < MAXBOLAS ; i++)
{
	bolas[i].xa = bolas[i].x; bolas[i].ya= bolas[i].y;
	if((bolas[i].passos < abs(bolas[i].dirX)) && bolas[i].ativo == true)
		{
		dir = bolas[i].dirX < 0 ? -1 : 1;
		switch(tela[bolas[i].x + dir][bolas[i].y])
		{
		case '#': bolas[i].dirX = bolas[i].dirX < 0 ? ((rand() % angmax) + 1) : -((rand() % angmax) + 1); bolas[i].passos = 0; break;
		case '|': bolas[i].dirX = bolas[i].dirX < 0 ? ((rand() % angmax) + 1) : -((rand() % angmax) + 1); bolas[i].passos = 0; pontos++; break;
		case 'B': bolas[i].dirX = bolas[i].dirX < 0 ? ((rand() % angmax) + 1) : -((rand() % angmax) + 1); bolas[i].passos = 0; tela[bolas[i].x + dir][bolas[i].y] = ' '; pontos++; break;
		case 'o': bolas[i].dirX = bolas[i].dirX < 0 ? ((rand() % angmax) + 1) : -((rand() % angmax) + 1); bolas[i].passos = 0; break;
		case '^': bolas[i].ativo = false;bolas[i].x = CENTRO;bolas[i].y = CENTRO; break;
		case ' ': bolas[i].x += dir; break;
		}
		bolas[i].passos++;
		}

	if( (bolas[i].passos < (abs(bolas[i].dirX) + abs(bolas[i].dirY))) && (bolas[i].passos >= abs(bolas[i].dirX))&& bolas[i].ativo == true )
		{
		dir = bolas[i].dirY < 0 ? -1 : 1;
		switch(tela[bolas[i].x][bolas[i].y + dir])
		{
		case '#': bolas[i].dirY = bolas[i].dirY < 0 ? ((rand() % angmax) + 1) : -((rand() % angmax) + 1); bolas[i].passos = 0; break;
		case '|': bolas[i].dirY = bolas[i].dirY < 0 ? ((rand() % angmax) + 1) : -((rand() % angmax) + 1); bolas[i].passos = 0; pontos++; break;
		case 'B': bolas[i].dirY = bolas[i].dirY < 0 ? ((rand() % angmax) + 1) : -((rand() % angmax) + 1); bolas[i].passos = 0; tela[bolas[i].x][bolas[i].y + dir] = ' '; pontos++; break;
		case 'o': bolas[i].dirY = bolas[i].dirY < 0 ? ((rand() % angmax) + 1) : -((rand() % angmax) + 1); bolas[i].passos = 0; break;
		case '^': bolas[i].ativo = false; bolas[i].x = CENTRO; bolas[i].y = CENTRO; break;
		case ' ': bolas[i].y += dir; break;
		}
		bolas[i].passos++;
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
