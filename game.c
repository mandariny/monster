#include <stdio.h>
#include <ncurses.h>
#include <locale.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>

#define N 7

char map[N][N+1] = {
	"#######",
	"#     #",
	"#^##  #", 
	"#    a#",
	"#   ###",
	"#    Y#",
	"#######"
};
int ny = 1;
int nx = 1;
int hp = 100;
int dx[] = {1, 0, -1, 0};
int dy[] = {0, 1, 0, -1};
int my = 2;
int mx = 5;
int flag = 0;

void *monster()
{
	while(1)
	{
		int d = rand() % 4;
		int x = mx + dx[d];
		int y = my + dy[d];

		if(map[y][x] == '#' || map[y][x] == '^') continue;
		map[my][mx] = ' ';
		map[y][x] = 'M';
		my = y;
		mx = x;
		
		if(flag) break;

		usleep(500 * 1000);
	}	

	return 0;
}

void print()
{
	clear();
	for (int y = 0; y < N; y++)
	{
		for (int x = 0; x < N; x++)
		{
			if(y == ny && x == nx) printw("%c", '@');
			else printw("%c", map[y][x]);
		}
		printw("\n");
	}
	printw("HP : %d\n", hp);
	refresh();
}

int main()
{
	setlocale(LC_CTYPE, "ko_KR.utf8");
	initscr();
	curs_set(0);

	nodelay(stdscr, TRUE);
	keypad(stdscr, TRUE);

	pthread_t mon;

	pthread_create(&mon, NULL, monster, NULL);

	while(1)
	{	
		int yy = ny;
		int xx = nx;

		print();
		int ch = getch();
		if(ch == ERR) ch = 0;

		if(ch == KEY_LEFT) 
		{
			xx--;
		}
		else if(ch == KEY_RIGHT)
		{
			xx++;
		}
		else if(ch == KEY_UP)
		{
			yy--;
		}
		else if(ch == KEY_DOWN)
		{
			yy++;
		}

		if(map[yy][xx] == '#') continue;

		if(ch != 0 && map[yy][xx] == '^'){
			hp -= 10;
		}

		if(map[yy][xx] == 'M' || hp <= 0) {
			print();
			usleep(500 * 1000);
			clear();
			mvprintw(10, 30, "GAME OVER\n");
			refresh();
			flag = 1;
			break;
		}
		if(map[yy][xx] == 'Y'){
			print();
			usleep(500 * 1000);
			clear();
			mvprintw(10, 30, "WIN (%d)\n", hp);
			refresh();
			flag = 1;
			break;
		}

		if(map[yy][xx] == 'a'){
			hp += 10;
			if(hp > 100) hp = 100;
			map[yy][xx] = ' ';
		}

		nx = xx;
		ny = yy;
	}	
	
    pthread_join(mon, NULL);	
	sleep(1);

	endwin();
	return 0;
}
