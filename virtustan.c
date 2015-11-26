#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iconv.h>
#include <sys/ioctl.h>
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

// static variables

int Codetable;
char *CodetableName[] = {"UTF","KOI","WIN","LAT"};
char out[MAXUTF];
struct
	{
	char *descr;
	int room_type;
	char symbol;
	char color;
	}
	world[MAX_X][MAX_Y];

int max_x, max_y;
int min_x, min_y;

int global_x, global_y;

struct winsize w;
struct termio tstdin;

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

// functions bodyes

int mod(int x)
{if (x<0) return -x;
else return x;}

void setcolor(int color)
{
if (color==0) printf("%s",NORM_COLOR);
else printf("\033[3%im", color);
}

void help (void)
{
print("Command list:\n\
koi, win, utf, lat - switch codetable\n\
codetable - print current codetable\n\
sysinfo - print sysinfo\n\
alf - print alfavit\n\
ascii - print ascii table\n\
help, помощь, ? - help\n\
test - test color\n\
test2 - test of keyboard\n\
cls - clearscreen\n\
look - look\n\
directions: 1 step: n, s, w, e (от слов north, south, etc)\n\
many steps: N, S, W, E\n\
env - print environment\n\
date - print date & time\n\
vorotaob - объявление на воротах\n\
constitution - print Virtustan constitution\n\
pledge - print Virtustan pledge\n\
rog - rogalik (use arrows; q, Q - quit from rogalik)\n\
rt - realtime rogalik (Q - quit from rogalik)\n\
q, quit, exit, конец - exit\n\
\n\
Sites: virtustan.net prool.kharkov.org github.com/prool/virtustan\n");
}

void date(void)
{
puts(ptime());
}

void setpos(int line, int col)
{
printf("\033[%i;%iH", line, col);
}

void gotoxy(int x, int y)
{
setpos(y,x);
}

void clearscreen(void)
{
printf("\033[2J");
setpos(1,1);
}

void set_terminal_raw(void)
{
/*  Set stdin (file descriptor=0) to raw mode, no echo */
    ioctl(0, TCGETA, &tstdin);
    tstdin.c_lflag &= ~(ICANON|ECHO);
    ioctl(0, TCSETA, &tstdin);
}

void set_terminal_no_raw(void)
{
/*  Set stdin (file descriptor=0) to NOraw mode and echo */
		ioctl(0, TCGETA, &tstdin);
    		tstdin.c_lflag |= (ICANON|ECHO);
    		ioctl(0, TCSETA, &tstdin);
}

void test2 (void)
{char c; int i;
struct termio tstdin;
int oldf;

/*  Set stdin (file descriptor=0) to NOraw mode and echo */
ioctl(0, TCGETA, &tstdin);
printf("tstdin.c_lflag=%X\n", tstdin.c_lflag);
tstdin.c_lflag &= ~(ICANON|ECHO);
printf("tstdin.c_lflag=%X\n", tstdin.c_lflag);
ioctl(0, TCSETA, &tstdin);

oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

printf("Q - quit\n");

while(1)
	{
	usleep(100000);
	c=getchar(); putchar('.');
	if (c!=-1) printf ("test2. Code=%i\r\n",(int)c);
	fflush(0);
	if (c=='Q')
		{
		ioctl(0, TCGETA, &tstdin);
		printf("tstdin.c_lflag=%X\n", tstdin.c_lflag);
    		tstdin.c_lflag |= (ICANON|ECHO);
		printf("tstdin.c_lflag=%X\n", tstdin.c_lflag);
    		ioctl(0, TCSETA, &tstdin);
		fcntl(STDIN_FILENO, F_SETFL, oldf);
		return;
		}
	}

}

char pressanykey()
{char c;
printf("---press any key---");

while(1)
	{
	c=getchar();
	if (c!=-1) return c;
	}
}

void printfile3(char *filename)
{
FILE *fp;
char str[MAXLEN];
char c;
int i;

//printf("lines=%i\n", lines);

fp = fopen (filename,"r");
                                 
if (fp==NULL) {printf("Can't open file `%s'\n", filename); return;}

i=0;
set_terminal_raw();
while(!feof(fp))
	{
	str[0]=0;
	fgets(str,MAXLEN,fp);
	if (str[0])
		{
		print(str);
		if (++i>=(lines-1)) {i=0;c=pressanykey(); if (c=='q') {printf("QUIT\n");fclose(fp);set_terminal_no_raw();return;} }
		printf("\r                                    \r");
		}
	}
fclose(fp);
set_terminal_no_raw();
}

void printfile(char *filename)
{FILE *fp; char str[MAXLEN];

fp = fopen (filename,"r");

if (fp==NULL) {printf("Can't open file `%s'\n", filename); return;}

while(!feof(fp))
	{
	str[0]=0;
	fgets(str,MAXLEN,fp);
	if (str[0]) print(str);
	}
}

void init_world(void)
{int i,j;

for (i=0; i<MAX_X; i++) for (j=0; j<MAX_Y; j++)
{
world[i][j].descr=0;
world[i][j].room_type=0;
world[i][j].symbol='"';
world[i][j].color=2;
}

world[50][50].descr="Вы находитесь перед воротами Виртустана (ворота на севере, идти на север надо командой n от слова north)\n\
На воротах висит объявление. Для просмотра объявления наберите vorotaob";
world[50][51].descr="Вы находитесь на пограничном посту Виртустана. Отсюда на север тянется Виртустан"; world[50][51].symbol='!';
world[50][52].descr="Вы находитесь на Виртустанской улице, идущей в меридиональном направлении";
world[50][53].descr="Вы находитесь на Виртустанской площади";
world[50][54].descr="Вы находитесь на Виртустанском проспекте";
world[49][53].descr="Вы находитесь во Дворце Короля Виртустана. Вокруг лепота";
world[49][52].descr="Вы находитесь в Виртустанской Библиотеке. ТИШИНА ДОЛЖНА БЫТЬ В БИБЛИОТЕКЕ!!";
world[51][52].descr="Вы находитесь в Виртустанской Гостинице";
world[51][53].descr="Вы находитесь в Виртустанском Банке. Слышен звон пересчитываемых монет, шелест купюр и звяканье кассового аппарата";
world[50][48].descr="Вы находитесь в неглубокой яме. На дне лежат чьи-то кости";

for (i=0; i<MAX_X; i++) for (j=0; j<MAX_Y; j++)
{
if ((world[i][j].descr) && (world[i][j].symbol=='"')) world[i][j].symbol='W';
}

for(i=0;i<MAX_X;i++)
{
world[i][0].color=3;
world[i][MAX_Y-1].color=3;
}

for(j=0;j<MAX_Y;j++)
{
world[0][j].color=3;
world[MAX_X-1][j].color=3;
}

computation_boundaries();

//printf("Boundaries of map: x %i %i, y %i %i\n", min_x, max_x, min_y, max_y);

}

void computation_boundaries(void)
{ int x, y;
// computation of boundaries
max_x=-1;
max_y=-1;
min_x=MAX_X;
min_y=MAX_Y;
for (y=0;y<MAX_Y;y++)
    for (x=0;x<MAX_X;x++)
	{
	if (world[x][y].descr)
	    {
	    if (x>max_x) max_x=x;
	    if (y>max_y) max_y=y;
	    if (x<min_x) min_x=x;
	    if (y<min_y) min_y=y;
	    }
	}

}

int min (int x, int y)
{
if (x<y) return x;
return y;
}

int max (int x, int y)
{
if (x>y) return x;
return y;
}

void look(void)
{
#define EMPTY "Вы находитесь в пустоте"
map();
printf("%s(%i,%i)%s\n",GOLUB1,global_x,global_y,NORM_COLOR);
if (world[global_x][global_y].descr) print(world[global_x][global_y].descr);
else
	{
	print(EMPTY);
	//world[global_x][global_y].descr="Тут был Пруль";
	}
printf("\n");
}

void move_(int dx, int dy)
{int try_x, try_y;

try_x=global_x+dx;
try_y=global_y+dy;

if ((try_x>MAX_X-1)||(try_y>MAX_Y-1)||(try_x<0)||(try_y<0))
    {
    printf("В этом направлении переместиться невозможно. Там край мира\n");
    return;
    }

global_x+=dx;
global_y+=dy;
print("Вы переместились ");
if (dx>0) print("на восток");
else if (dx<0) print("на запад");
else if (dy>0) print("на север");
else if (dy<0) print("на юг");
else print("хер знает куда");

if ((mod(dx)>1)||(mod(dy)>1)) print(" до упора");
printf("\n");
look();
}

void test (void)
{int i;
printf("\
-----------------------------------------------------------------------------\n\
[0;40;37;1m\
-----------------------------------------------------------------------------\n\
[0m\n\
");

printf("%s normcolor ",NORM_COLOR);

for (i=31;i<38;i++)
	{
	printf("\033[%im www ", i);
	}
for (i=31;i<38;i++)
	{
	printf("\033[1;%im WWW ", i);
	}
printf("\033[0m\n");
#if 0
unsigned char m [256*2]; int i;
//print("test --- ТЕСТ тест \n\n");
//outhex("тест");
for (i=0;i<256*2;i++) m[i]=' ';
for (i=144;i<192;i++) {m[2*i]=0xD0U; m[2*i+1]=i&0xFF;}
printf("'%s'\n", m);
printf("\n==================== #2 =======================\n");
for (i=0;i<256*2;i++) m[i]=' ';
for (i=128;i<144;i++) {m[2*i]=0xD1U; m[2*i+1]=i&0xFF;}
printf("'%s'\n", m);
// АБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдежзийклмноп рстуфхцчшщъыьэюя
#endif
}

void utf8_to_koi(char *str_i, char *str_o)
{
	iconv_t cd;
	size_t len_i, len_o = MAXUTF;
	size_t i;

	if ((cd = iconv_open("KOI8-R", "UTF-8")) == (iconv_t) - 1)
	{
		printf("utf8_to_koi: iconv_open error\n");
		return;
	}
	len_i = strlen(str_i);
	if ((i=iconv(cd, &str_i, &len_i, &str_o, &len_o)) == (size_t) - 1)
	{
		printf("utf8_to_koi: iconv error\n");
		// return;
	}
	if (iconv_close(cd) == -1)
	{
		printf("utf8_to_koi: iconv_close error\n");
		return;
	}
}

void koi_to_utf8(char *str_i, char *str_o)
{
	iconv_t cd;
	size_t len_i, len_o = MAXUTF;
	size_t i;

	if ((cd = iconv_open("UTF-8", "KOI8-R")) == (iconv_t) - 1)
	{
		printf("koi_to_utf8: iconv_open error\n");
		return;
	}
	len_i = strlen(str_i);
	if ((i=iconv(cd, &str_i, &len_i, &str_o, &len_o)) == (size_t) - 1)
	{
		printf("koi_to_utf8: iconv error\n");
		// return;
	}
	if (iconv_close(cd) == -1)
	{
		printf("koi_to_utf8: iconv_close error\n");
		return;
	}
}

void fromkoi (char *str)
{char buf [MAXUTF];
koi_to_utf8(str,buf);
strcpy(str,buf);
}

void utf8_to_win(char *str_i, char *str_o)
{
	iconv_t cd;
	size_t len_i, len_o = MAXUTF;
	size_t i;

	if ((cd = iconv_open("CP1251", "UTF-8")) == (iconv_t) - 1)
	{
		printf("utf8_to_win: iconv_open error\n");
		return;
	}
	len_i = strlen(str_i);
	if ((i=iconv(cd, &str_i, &len_i, &str_o, &len_o)) == (size_t) - 1)
	{
		printf("utf8_to_win: iconv error\n");
		// return;
	}
	if (iconv_close(cd) == -1)
	{
		printf("utf8_to_win: iconv_close error\n");
		return;
	}
}

void win_to_utf8(char *str_i, char *str_o)
{
	iconv_t cd;
	size_t len_i, len_o = MAXUTF;
	size_t i;

	if ((cd = iconv_open("UTF-8", "CP1251")) == (iconv_t) - 1)
	{
		printf("win_to_utf8: iconv_open error\n");
		return;
	}
	len_i = strlen(str_i);
	if ((i=iconv(cd, &str_i, &len_i, &str_o, &len_o)) == (size_t) - 1)
	{
		printf("win_to_utf8: iconv error\n");
		// return;
	}
	if (iconv_close(cd) == -1)
	{
		printf("win_to_utf8: iconv_close error\n");
		return;
	}
}

void fromwin (char *str)
{char buf [MAXUTF];
win_to_utf8(str,buf);
strcpy(str,buf);
}

void outhex(char *str)
{
while(*str)
	{
	printf("'%X' ", *str++&0xFFU);
	}
printf("\n");
}

char *utf2koi (char *str)
{int i;
for (i=0;i<MAXUTF;i++)out[i]=0;
utf8_to_koi(str, out);
return out;
}

char *utf2win (char *str)
{int i;
for (i=0;i<MAXUTF;i++)out[i]=0;
utf8_to_win(str, out);
return out;
}

char *utf2lat (char *str)
{
return "lat not implemented yet\n";
}

void print (char *str)
{
switch (Codetable)
	{
	case UTF: printf((const char *)str); break;
	case KOI: printf(utf2koi(str)); break;
	case WIN: printf(utf2win(str)); break;
	case LAT: printf(utf2lat(str)); break;
	default: ;
	}
}

void ascii (void)
{int i;
for (i=33; i<128; i++)
	{
	printf ("%c ", i);
	}
printf("\n");
}

void map(void)
{
int x, y;
int local_max_y, local_max_x, local_min_y, local_min_x;

//computation_boundaries();

#define HALF_X 10 // полуразмеры карты от центра карты по измерениям
#define HALF_Y 10

local_min_x=global_x-HALF_X; if (local_min_x<0) local_min_x=0;
local_min_y=global_y-HALF_Y; if (local_min_y<0) local_min_y=0;

local_max_x=global_x+HALF_X; if (local_max_x>=MAX_X) local_max_x=MAX_X-1;
local_max_y=global_y+HALF_Y; if (local_max_y>=MAX_Y) local_max_y=MAX_Y-1;

// write of map

setcolor(0);

for (y=local_max_y; y>=local_min_y; y--)
    {
    for (x=local_min_x; x<=local_max_x; x++)
	{
	setcolor(world[x][y].color);
	if ((x==global_x)&&(y==global_y)) printf("@");
	else
	    putchar(world[x][y].symbol);
	printf(" ");
	setcolor(0);
	}
    printf("\n");
    }
printf(NORM_COLOR);
}

void env(char *envp[])
{int i;
i=0;
printf("env\n");
while(*envp)
	{
	printf("%s\n",*envp++);
	//if (++i>lines-4) {i=0; printf("press any key"); getchar();}
	}
}

char *ptime(void)
	{
	char *tmstr;
	time_t mytime;

	mytime = time(0);

	tmstr = asctime(localtime(&mytime));
	*(tmstr + strlen(tmstr) - 1) = '\0';

	return tmstr;

	}
void log_(char *str)
{
FILE *fp;

#define LOGFILE "virtustan-app.log"
fp=fopen(LOGFILE,"a");
if (!fp) {printf("Can't open logfile `%s'\n",LOGFILE); return;}
fprintf(fp,"%s %s\n",ptime(), str);
fflush(0);
fclose(fp);
}

void sysinfo(char *envp[])
{
char terminal [MAXLEN];
char **envpp;

printf ("lines %d\n", lines);
printf ("columns %d\n", COLUMNS);
#define S_TERM "TERM="
envpp=envp;
while (*envp)
    {
	if (!memcmp(*envp,S_TERM,min(strlen(*envp),strlen(S_TERM))))
		{
		strcpy(terminal,*envp+strlen(S_TERM));
		//printf("%s\n", *envp);
		printf("terminal = %s\n", terminal);
		}
	envp++;
    }
envp=envpp;
printf("Current codetable is %s. ",CodetableName[Codetable]);print("Ktulhu ФХТАГН!!\n");
}

/************************************************************************************************************************/
/************************************************************************************************************************/
/************************************************************************************************************************/
int main (int argc, char *argv[], char *envp[])
{
char cmd[MAXLEN_CMD];
char *cc;
int i, j;

ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

clearscreen();

printf("Virtustan application\nCopyleft by Prool, 2015\nThis program comes with ABSOLUTELY NO WARRANTY; for details type `gpl3'.\
 Compile %s %s\n",__DATE__,__TIME__);

//sysinfo(envp);

printf("%sUse `help' command for help and `quit' for quit.%s\n", BEL1, NORM_COLOR);

log_("Virtustan application started");

//printf("init started\n");
Codetable=UTF;


global_x=50; global_y=50;

init_world();

//printf("init ended\n\n");

look();

while(1)
	{
	printf("virtustan app> ");
	fgets(cmd,MAXLEN_CMD,stdin); // и нафига я тут использовал fgets, а не gets ? :) prool
	switch (Codetable)
		{
		case KOI: fromkoi(cmd); break;
		case WIN: fromwin(cmd); break;
		}
	// printf("cmd=`%s'\n", cmd);
	cc=strchr(cmd,'\n');
	if (cc) *cc=0;
	if (cmd[0]==0) continue;
	if (!strcmp(cmd,"q")) break;
	if (!strcmp(cmd,"quit")) break;
	if (!strcmp(cmd,"exit")) break;
	if (!strcmp(cmd,"конец")) break;
	if (!strcmp(cmd,"help")) help();
	else if (!strcmp(cmd,"h")) help();
	else if (!strcmp(cmd,"?")) help();
	else if (!strcmp(cmd,"помощь")) help();
	else if (!strcmp(cmd,"alf")) print(ALFAVIT);
	else if (!strcmp(cmd,"ascii")) ascii();
	else if (!strcmp(cmd,"koi")) {Codetable=KOI; printf("Codetable switch to KOI\n");}
	else if (!strcmp(cmd,"utf")) {Codetable=UTF; printf("Codetable switch to UTF\n");}
	else if (!strcmp(cmd,"win")) {Codetable=WIN; printf("Codetable switch to WIN\n");}
	else if (!strcmp(cmd,"lat")) {Codetable=LAT; printf("Codetable switch to LAT: yet not implemented!\n");}
	else if (!strcmp(cmd,"codetable")) printf("Current codetable is %s\n",CodetableName[Codetable]);
	else if (!strcmp(cmd,"look")) look();
	else if (!strcmp(cmd,"n")) move_(0,+1);
	else if (!strcmp(cmd,"N")) move_(0,MAX_Y-1-global_y);
	else if (!strcmp(cmd,"s")) move_(0,-1);
	else if (!strcmp(cmd,"S")) move_(0,-global_y);
	else if (!strcmp(cmd,"w")) move_(-1,0);
	else if (!strcmp(cmd,"W")) move_(-global_x,0);
	else if (!strcmp(cmd,"e")) move_(+1,0);
	else if (!strcmp(cmd,"E")) move_(MAX_X-1-global_x,0);
	else if (!strcmp(cmd,"map")) map();
	else if (!strcmp(cmd,"vorotaob")) printfile("texts/vorotaob.txt");
	else if (!strcmp(cmd,"gpl3")) printfile3("LICENSE");
	else if (!strcmp(cmd,"constitution")) printfile3("texts/constitution.txt");
	else if (!strcmp(cmd,"pledge")) printfile3("texts/pledge.txt");
	else if (!strcmp(cmd,"env")) env(envp);
	else if (!strcmp(cmd,"test")) test();
	else if (!strcmp(cmd,"test2")) test2();
	else if (!strcmp(cmd,"rog")) rogalik();
	else if (!strcmp(cmd,"cls")) clearscreen();
	else if (!strcmp(cmd,"date")) date();
	else if (!strcmp(cmd,"rt")) realtime();
	else if (!strcmp(cmd,"sysinfo")) sysinfo(envp);
	else printf("   Unknown command `%s'\n", cmd);
	}
log_("Virtustan application finished");
return 0;
}

void realtime (void)
{char c; int i;
struct termio tstdin;
int oldf;

/*  Set stdin (file descriptor=0) to NOraw mode and echo */
ioctl(0, TCGETA, &tstdin);
tstdin.c_lflag &= ~(ICANON|ECHO);
ioctl(0, TCSETA, &tstdin);

oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

while(1)
	{
	clearscreen();
	gotoxy(0,0);
	printf("Q - quit");
	gotoxy(30,0);
	printf("%s", ptime());
	usleep(100000);
	c=getchar();
	if (c!=-1) {}
	fflush(0);
	if (c=='Q')
		{
		ioctl(0, TCGETA, &tstdin);
    		tstdin.c_lflag |= (ICANON|ECHO);
    		ioctl(0, TCSETA, &tstdin);
		fcntl(STDIN_FILENO, F_SETFL, oldf);
		printf("\n\nexit from realtime\n");
		return;
		}
	}
}

#define ROG_X 10
#define ROG_Y 10

void rogalik(void)
{
int quit=0;
int i, j, c, c2, c3, me_x=0, me_y=0;

set_terminal_raw();
clearscreen();

printf("Rogalik!\n\nQ - quit\n\n");

while(!quit)
	{// refresh screen
	clearscreen();
	for (j=0; j<ROG_Y; j++)
		{
		for (i=0; i<ROG_X; i++)
			{
			if ((i==me_x)&&(j==me_y)) putchar('@');
			else putchar('.');
			}
		puts("");
		}
	c=getchar();
	switch(c)
		{
		case 27:
			c2=getchar();
			if (c2!=91) break;
			c3=getchar();
			switch (c3)
				{
				case 65: goto l_up;
				case 66: goto l_down;
				case 68: goto l_left;
				case 67: goto l_right;
				default: c=600;
				}
		case 'l': l_right: if ((me_x+1) < ROG_X) me_x++; break;
		case 'h': l_left: if ((me_x-1) >= 0   ) me_x--; break;
		case 'j': l_down: if ((me_y+1) < ROG_Y) me_y++; break;
		case 'k': l_up: if ((me_y-1) >= 0   ) me_y--; break;
		case 'q': quit=1; break;
		case 'Q': set_terminal_no_raw(); printf("\nExit from rogalik to OS shell\n\n"); exit(2);
		case '?': rogalik_help(); break;
		default: ;
		}
	}
//printf("set terminal noraw:\n");
set_terminal_no_raw();
printf("Exit from rogalik to virtustan appication!\n");
}

void rogalik_help(void)
{int line=1;
gotoxy (ROG_X+2,line++); printf ("rogalik help. помошч");
gotoxy (ROG_X+2,line++); printf ("? - this help");
gotoxy (ROG_X+2,line++); printf ("arrows, j, k, h, l - move");
gotoxy (ROG_X+2,line++); printf ("q - quit to virtustan");
gotoxy (ROG_X+2,line++); printf ("Q - quit from virtustan");
gotoxy (ROG_X+2,line++); printf ("--Press any key--");
getchar();
}
