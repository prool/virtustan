// Virtustan application (small rogue-like game)
// by Prool
// www.prool.kharkov.org www.virtustan.net

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iconv.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
//#include <ncurses.h>
#include <termios.h>
#include <locale.h> // for UTF-8 under ncurses

#define MAXLEN 255
#define MAXLEN_CMD 4096
#define MAXUTF 1024

#define UTF 0
#define KOI 1
#define WIN 2
#define LAT 3
#define ALFAVIT "В ЧАЩАХ ЮГА ЖИЛ БЫ ЦИТРУС ДА НО ФАЛЬШИВЫЙ ЭКЗЕМПЛЯР в чащах юга жил бы цитрус да но фальшивый экземпляр Ёё Ъъ\n"

#define MAX_X 60
#define MAX_Y 60

#define lines w.ws_row
#define COLUMNS w.ws_col

#define ESC 033
#define NORM_COLOR "\033[0m"

#define KRASN "\033[31m"
#define ZELEN "\033[32m"
#define ZHELT "\033[33m"
#define SIN "\033[34m"
#define FIOL "\033[35m"
#define GOLUB "\033[36m"
#define BEL "\033[37m"

#define KRASN1 "\033[1;31m"
#define ZELEN1 "\033[1;32m"
#define ZHELT1 "\033[1;33m"
#define SIN1 "\033[1;34m"
#define FIOL1 "\033[1;35m"
#define GOLUB1 "\033[1;36m"
#define BEL1 "\033[1;37m"

#define HALF "\033[2m"
#define UNDERSCORE "\033[4m"
#define BLINK "\033[5m"
#define REVERSE "\033[7m"

#define DEFAULT_COLOR	2
#define DEFAULT_BG	40 // 40	
#define DEFAULT_SYMBOL	'.'
#define PLAYER_COLOR	0
#define CREATED_OBJECT	777

#define WINTER 0
#define SPRING 1
#define SUMMER 2
#define AUTUMN 3

#define UP_ARROW "\x1b\x5b\x41"
#define DOWN_ARROW "\x1b\x5b\x42"
#define RIGHT_ARROW "\x1b\x5b\x43"
#define LEFT_ARROW "\x1b\x5b\x44"

// static variables

int Codetable;
char *CodetableName[] = {"UTF","KOI","WIN","LAT"};
char out[MAXUTF];
struct
	{
	char *descr;
	int room_type;
	char symbol;
	char color; // color of foreground
	char bg; // color of background
	int object; // object in room
	int mob; // mob in room
	}
	world[MAX_X][MAX_Y];

int inv_o=0; // inventory

int max_x, max_y;
int min_x, min_y;

int global_x, global_y;

struct winsize w;
struct termio tstdin;

int sezon;

// function definitions (prototypes)

void outhex(char *);
void print (char *);
void print2 (char *);
void map(void);
char *ptime(void);
void log_(char *str);
void computation_boundaries(void);
void rogalik(void);
void rogalik_help(void);
void realtime (void);
void print_holyday(void);
