// Virtustan application (small rogue-like game)
// by Prool
// www.prool.kharkov.org

#define CONFIG_FILE "virtustan.cfg"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iconv.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
//#include <ncurses.h>
#include <locale.h> // for UTF-8 under ncurses
#include <ctype.h>
#include <termios.h>
#include <sys/param.h>

#include "roomtypes.h"

#define MAX_X 200 // 100 // 60 // 130
#define MAX_Y 200 // 60

#define MAXLEN 255
#define PROOL_MAX_STRLEN 255
#define MAXLEN_CMD 4096
#define MAXUTF 1024

#define UTF 0
#define KOI 1
#define WIN 2
#define LAT 3
#define ALFAVIT "В ЧАЩАХ ЮГА ЖИЛ БЫ ЦИТРУС ДА НО ФАЛЬШИВЫЙ ЭКЗЕМПЛЯР в чащах юга жил бы цитрус да но фальшивый экземпляр Ёё Ъъ\n"

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
#define DEFAULT_CONTRAST_COLOR	14
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

#define L_UP "l\x1b\x5b\x41"
#define L_DOWN "l\x1b\x5b\x42"

#define puts0(STR) printf("%s",STR)
#define putdec(DIGIT) printf("%i", DIGIT)
#define puthex(DIGIT) printf("%04X", DIGIT)
#define puthex_l(DIGIT) printf("%08X", DIGIT)
#define puthex_b(DIGIT) printf("%02X", DIGIT)
#define peek(ADR) 0
#define peek2(SEG,OFFSET) 0
#define putch(a) putchar(a)

#define UPDATED_ROOM 777

// for realtime() module
#define MAX_L 79 /* 57 */
#define MAX_C 257 /* 217 */
#define CURSOR_COLOR 14

// static variables

int have_config;
int updated; // =1 if world is update
int i_c, j_c; // cursor location for realtime mode
int MAX_I, MAX_J;
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
	long int timer;
	}
	world[MAX_X][MAX_Y];

char base_path [MAXLEN];
char logfilename [MAXLEN];

int inv_o=0; // inventory

int max_x, max_y;
int min_x, min_y;

int global_x, global_y;

struct winsize w;
struct termio tstdin;

int sezon;

int crop=0;

#define NORMAL_COLOR_MODE 0
#define CONTRAST 1
#define NO_COLOR 2
int color_mode=0;

long int start_time;

int HALF_X; // полуразмеры карты от центра карты по измерениям: (10,10) by default
int HALF_Y; 

char recode_buffer [1024];

// function definitions (prototypes)

void outhex(char *);
void print (char *);
void print2 (char *);
void map(void);
char *ptime(void);
long int unixtime(void);
void log_(char *str);
void computation_boundaries(void);
void rogalik(void);
void rogalik_help(void);
void realtime (void);
void realtime_old (void);
void print_holyday(void);
void skript(void);
char plant_symbol(int, int);
int exec (char *);
int htoi(const char  *s);
void printfile(char *filename);
void printfile_basepath(char *filename);
void pwd(void);

// defines for proolskript interpteter

#define MAXFAJL 2500

#ifdef FREEBSD

#define TCGETA          0x5405
#define TCSETA          0x5406

/*
 * Classic struct termio. More modern Unix versions
 * contain additional information. Unix versions who
 * support termio and termios often use the same
 * structure for termio and termios, so termio
 * contains the full termios data on this systems.
 */
#define NCC 8
struct termio {
    int c_iflag;         /* input modes   */
    int c_oflag;         /* output modes  */
    int c_cflag;         /* control modes */
    int c_lflag;         /* local modes   */
    char c_cc[NCC];      /* control chars */
};
#endif // FREEBSD
