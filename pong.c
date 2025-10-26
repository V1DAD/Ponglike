#include <stdlib.h>
#include <unistd.h>
#include <curses.h>
#include <time.h>
#include <stdbool.h>

#define RESOLUCAO 50
#define CENTRO (RESOLUCAO/2)


char tela[RESOLUCAO][RESOLUCAO];
int x = CENTRO, y = CENTRO, xa = CENTRO, ya = CENTRO, dirX = 1, dirY = 1, passos = 0, dir = 0;
int ch, rodando = 1;
int posx = CENTRO, posy = ((RESOLUCAO/10)*2), posxa = CENTRO, dirplat;

int angmax = 2;
int tamplat = 3;

int main()
{
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

cbreak();
noecho();
curs_set(0);
nodelay(stdscr, TRUE);
keypad(stdscr, TRUE);

//loop principal
while(rodando)
{

ch = getch();
if(ch != ERR)
{
	switch(ch)
	{
	case 27 : rodando = 0; break;
	case 'w': dirplat = -1; if((posx-1 > 0) && (tela[posx-2][posy] != '#')) {posx -= 2;} else if(tela[posx-1][posy] != '#'){posx -= 1;} break;
	case 's': dirplat = 1; if((posx+tamplat < RESOLUCAO-1) && (tela[posx+tamplat+1][posy] != '#')) {posx += 2;} else if(tela[posx+tamplat][posy] != '#'){posx += 1;} break;
	case 'e': tamplat = tela[posx+tamplat][posy] == ' ' ? tamplat+1 : tamplat; break;
	case 'q': if(tamplat > 1){tamplat--; dirplat = 1; if(tela[posx+tamplat][posy] != '#'){posx += 1;}} break;
	}
}

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
xa = x; ya= y;

//checagem de colisao

if(passos < abs(dirX))
	{
	dir = dirX < 0 ? -1 : 1;
	if((tela[x + dir][y] == '#') || (tela[x+dir][y] == '|')) { dirX = dirX < 0 ? ((rand() % angmax) + 1) : -((rand() % angmax) + 1); passos = 0;} else {x += dir;}
	passos++;
	}

if((passos < (abs(dirX) + abs(dirY))) && (passos >= abs(dirX)) )
	{
	dir = dirY < 0 ? -1 : 1;
	if((tela[x][y + dir] == '#') || (tela[x][y+dir] == '|')) { dirY = dirY < 0 ? ((rand() % angmax) + 1) : -((rand() % angmax) + 1); passos = 0;} else {y += dir;}
	passos++;
	}

if(passos == (abs(dirX)+abs(dirY))) {passos = 0;}

tela[xa][ya] = ' '; tela[x][y] = 'o';


//print tela

//printw("x : %d y : %d dirX : %d dirY : %d ch : %c \n",x,y,dirX,dirY,ch);
printw("Sair[esc] cima[w] baixo[s] encolher plataforma[q] extender plataforma[e]\n\n");
for (int j = 0; j < RESOLUCAO; j++) { for (int k = 0; k <RESOLUCAO; k++) { printw("%c ",tela[j][k]); }printw("\n"); }

refresh(); usleep(40000); clear();
}
endwin();
return(0);
}
