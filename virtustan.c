// Virtustan application (small rogue-like game)
// by Prool
// www.prool.kharkov.org virtustan.kharkov.org

#include "virtustan.h"

#define KIOCSOUND	0x4B2F	/* start sound generation (0 for off) */
#define KDMKTONE	0x4B30	/* generate tone */

#define FILLER '.'

// static variables
int step;
int langton;
int langton_direct;
int langton_x, langton_y;
int langton_table_x[4] = {0, 1,  0, -1};
int langton_table_y[4] = {1, 0, -1,  0};

// static vars for realtime() module
char screen [MAX_L] [MAX_C];
char screen_old [MAX_L] [MAX_C];
char screen_color [MAX_L] [MAX_C];
char screen_color_old [MAX_L] [MAX_C];
char screen_bg [MAX_L] [MAX_C];
char screen_bg_old [MAX_L] [MAX_C];
int cur_l, cur_c;

#if 0
void tone(int fd, int hertz, int hundredths) {
 unsigned int ticks = hundredths * HZ / 100;
 /* ticks & 0xffff не будет работать, если ticks — 0xf0000;
  * вместо этого нужно округлить до наибольшего допустимого значения */
 if (ticks > 0xffff) ticks = 0xffff;
 /* еще одна ошибка округления */
 if (hundredths && ticks == 0) ticks = 1;
 ioctl(fd, KDMKTONE, (ticks << 16 | (1193180/hertz)));
}
#endif

void beep(void)
{
int hertz = 60;
ioctl(STDOUT_FILENO, KIOCSOUND, 1193180/hertz);
//tone(STDOUT_FILENO, 60, 100);
}

void esc(int code)
{
printf("\033[%im", code);
}

int mod(int x)
{if (x<0) return -x;
else return x;}

void setcolor(int color)
{
if (color_mode==NO_COLOR) return;
if (color_mode==CONTRAST)
	{
	if (color==0) color=14;
	else color+=7;
	}
if (color==0) printf("%s",NORM_COLOR);
else if (color<=7) printf("\033[3%im", color);
else printf("\033[1;3%im", color-7);
}

void uptime_(void)
{long uptime; char *tmstr;
uptime=unixtime()-start_time;
if (uptime<=60)printf("Uptime %li sec", uptime);
else
	{
	uptime=uptime/60;
	if (uptime<=60) printf ("Uptime %li min", uptime);
	else
		{
		uptime=uptime/60;
		if (uptime<=24) printf("Uptime %li hour", uptime);
		else printf("Uptime %li days", uptime/24);
		}
	}
	tmstr = asctime(localtime(&start_time));
	*(tmstr + strlen(tmstr) - 1) = '\0';
printf(" since %s\n", tmstr);
}

void date(void)
{long uptime; char *tmstr;

puts(ptime());
printf("unixtime = %li\n", unixtime());
uptime_();
print_holyday();
print("Время года: ");
switch(sezon)
	{
	case WINTER: print("Зима"); break;
	case SPRING: print("Весна"); break;
	case SUMMER: print("Лето"); break;
	case AUTUMN: print("Осень"); break;
	default: print("Пиздец");
	}
printf("\n");
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

void test3(void)
{
putchar(0xc2); putchar(0xa6);
putchar(0xc2); putchar(0xa8);
putchar(0xc2); putchar(0xac);
putchar(0xc2); putchar(0xaf);
putchar(0xc2); putchar(0xb1);
putchar(0xcb); putchar(0xa5);
putchar(0xd6); putchar(0x8d);
putchar(0xe1); putchar(0x9a); putchar(0xa0);
putchar(0xe2); putchar(0x9b); putchar(0xa7);
putchar(0xe2); putchar(0x8c); putchar(0x82);
putchar(0xe2); putchar(0x98); putchar(0xad);
printf("\n");
}

void test4(void)
{int i; unsigned char str[3];
char input[MAXLEN];

printf("test of UTF-8 2-bytes codes. Enter prefix (for cyrillic D0 or D1) ? ");
fgets(input,MAXLEN,stdin);
input[2]=0; // dirty hack! truncate to 2 digits!
printf("Input = '%s'\n", input);
str[0]=htoi(input);
printf("htoi=%i\n", str[0]);
if (str[0]==0) return;
str[2]=0;

for (i=0x80;i<0xC0;i++)
	{
	str[1]=i&0xFF;
	printf("%02X %02X %s | ",str[0], str[1], str);
	}
}

void utf_to_lat(unsigned char *utfstr, char *latstr)
{
if (utfstr==0) return;
if (latstr==0) return;
while (*utfstr)
	{
	switch (*utfstr)
		{
		case 0xD0:
			utfstr++;
			switch (*utfstr++)
				{
				case 0: *latstr=0; return;
				case 0x81: *latstr++='E'; break;
				case 0x84: *latstr++='E'; break;
				case 0x86: *latstr++='I'; break;
				case 0x87: *latstr++='I'; break;
				case 0x90: *latstr++='A'; break;
				case 0x91: *latstr++='B'; break;
				case 0x92: *latstr++='V'; break;
				case 0x93: *latstr++='G'; break;
				case 0x94: *latstr++='D'; break;
				case 0x95: *latstr++='E'; break;
				case 0x96: *latstr++='#'; break;
				case 0x97: *latstr++='Z'; break;
				case 0x98: *latstr++='I'; break;
				case 0x99: *latstr++='J'; break;
				case 0x9a: *latstr++='K'; break;
				case 0x9b: *latstr++='L'; break;
				case 0x9c: *latstr++='M'; break;
				case 0x9d: *latstr++='N'; break;
				case 0x9e: *latstr++='O'; break;
				case 0x9f: *latstr++='P'; break;

				case 0xa0: *latstr++='R'; break;
				case 0xa1: *latstr++='S'; break;
				case 0xa2: *latstr++='T'; break;
				case 0xa3: *latstr++='U'; break;
				case 0xa4: *latstr++='F'; break;
				case 0xa5: *latstr++='H'; break;
				case 0xa6: *latstr++='C'; break;
				case 0xa7: *latstr++='4'; break;
				case 0xa8: *latstr++='W'; break;
				case 0xa9: *latstr++='W'; break;
				case 0xaa: *latstr++= 39; break;
				case 0xab: *latstr++='Y'; break;
				case 0xac: *latstr++= 39; break;
				case 0xad: *latstr++='E'; break;
				case 0xae: *latstr++='Y'; break;
				case 0xaf: *latstr++='9'; break;

				case 0xB0: *latstr++='a'; break;
				case 0xB1: *latstr++='b'; break;
				case 0xB2: *latstr++='v'; break;
				case 0xB3: *latstr++='g'; break;
				case 0xB4: *latstr++='d'; break;
				case 0xB5: *latstr++='e'; break;
				case 0xB6: *latstr++='#'; break;
				case 0xB7: *latstr++='z'; break;
				case 0xB8: *latstr++='i'; break;
				case 0xB9: *latstr++='j'; break;
				case 0xBa: *latstr++='k'; break;
				case 0xBb: *latstr++='l'; break;
				case 0xBc: *latstr++='m'; break;
				case 0xBd: *latstr++='n'; break;
				case 0xBe: *latstr++='o'; break;
				case 0xBf: *latstr++='p'; break;
				default: *latstr++='*';
				}
			break;
		case 0xD1:
			utfstr++;
			switch (*utfstr++)
				{
				case 0: *latstr=0; return;
				case 0x80: *latstr++='r'; break;
				case 0x81: *latstr++='s'; break;
				case 0x82: *latstr++='t'; break;
				case 0x83: *latstr++='u'; break;
				case 0x84: *latstr++='f'; break;
				case 0x85: *latstr++='h'; break;
				case 0x86: *latstr++='c'; break;
				case 0x87: *latstr++='4'; break;
				case 0x88: *latstr++='w'; break;
				case 0x89: *latstr++='w'; break;
				case 0x8a: *latstr++= 39; break;
				case 0x8b: *latstr++='y'; break;
				case 0x8c: *latstr++= 39; break;
				case 0x8d: *latstr++='e'; break;
				case 0x8e: *latstr++='y'; break;
				case 0x8f: *latstr++='9'; break;

				case 0x91: *latstr++='e'; break;
				case 0x93: *latstr++='g'; break;
				case 0x94: *latstr++='e'; break;
				case 0x96: *latstr++='i'; break;
				case 0x97: *latstr++='i'; break;

				default: *latstr++='*';
				}
			break;
		default:
			*latstr++=*utfstr++;
		}
	}
*latstr=0;
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

printf("Q - quit; M - mouse mode on; Z - mouse mode off; N - mouse on from mc; X - mouse off from mc;\n");

while(1)
	{
	usleep(100000);
	c=getchar(); putchar('.');
	if (c!=-1) printf ("test2. Code=%02X\r\n",(unsigned char)c);
	fflush(0);
	switch (c)
		{
		case 'Q':
		ioctl(0, TCGETA, &tstdin);
		printf("tstdin.c_lflag=%X\n", tstdin.c_lflag);
    		tstdin.c_lflag |= (ICANON|ECHO);
		printf("tstdin.c_lflag=%X\n", tstdin.c_lflag);
    		ioctl(0, TCSETA, &tstdin);
		fcntl(STDIN_FILENO, F_SETFL, oldf);
		return;
		case 'M': printf("\nMouse mode on%c[?9h\n",ESC); break;
		case 'Z': printf("\nMouse mode off%c[?9l\n",ESC); break;
		case 'N':
#if 1 // mouse code from midnight commander
//    case MOUSE_XTERM_NORMAL_TRACKING:
        /* save old highlight mouse tracking */
        printf (ESC_STR "[?1001s");

        /* enable mouse tracking */
        printf (ESC_STR "[?1000h");

        /* enable SGR extended mouse reporting */
//        printf (ESC_STR "[?1006h");

        fflush (stdout);

//    case MOUSE_XTERM_BUTTON_EVENT_TRACKING:
        /* save old highlight mouse tracking */
        printf (ESC_STR "[?1001s");

        /* enable mouse tracking */
        printf (ESC_STR "[?1002h");

        /* enable SGR extended mouse reporting */
//        printf (ESC_STR "[?1006h");

        fflush (stdout);
#endif
			  break;
	case 'X':
#if 1 // mouse code from midnight commander

// case MOUSE_XTERM_NORMAL_TRACKING:
        /* disable SGR extended mouse reporting */
        printf (ESC_STR "[?1006l");

        /* disable mouse tracking */
        printf (ESC_STR "[?1000l");

        /* restore old highlight mouse tracking */
        printf (ESC_STR "[?1001r");

        fflush (stdout);
//    case MOUSE_XTERM_BUTTON_EVENT_TRACKING:
        /* disable SGR extended mouse reporting */
        printf (ESC_STR "[?1006l");

        /* disable mouse tracking */
        printf (ESC_STR "[?1002l");

        /* restore old highlight mouse tracking */
        printf (ESC_STR "[?1001r");

        fflush (stdout);
#endif
			  break;
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

#define NBUF 16
void hexfile (char *filename)
{
int i, n, line;
unsigned char buf[NBUF];
unsigned char c;
unsigned int addr;

FILE *fp=fopen(filename,"r");
line=1;
addr=0;

if (!fp) {printf("Can't open file %s\n", filename); return;}
set_terminal_raw();
while (!feof(fp))
	{
	n=fread(buf, 1, NBUF, fp);
	printf("%04X ", addr);
	for(i=0;i<n;i++) printf("%02X ", buf[i]);
	if (n<NBUF) for (i=0;i<((NBUF-n)*3);i++) printf(" ");
	printf(" ");
	for(i=0;i<n;i++) {if ((buf[i]>=32)&&(buf[i]<127)) c=buf[i]; else c='.'; printf("%c", c);}
	printf("\n");
	addr+=NBUF;
	if (++line>=lines) {c=pressanykey(); line=1; if (c=='q') break; printf("\r"); }
	}
fclose(fp);
set_terminal_no_raw();
printf("\n");
}

void printfile_basepath(char *filename)
{char full_path[MAXLEN];
strcpy(full_path, base_path);
strcat(full_path,"/");
strcat(full_path,filename);
printfile(full_path);
}

void printfile(char *filename)
{
FILE *fp;
char str[MAXLEN];
char c;
int i;

//printf("printfile() filename=%s\n", filename);

fp = fopen (filename,"r");
                                 
if (fp==NULL) {printf("Can't open file `%s'\n", filename); return;}

i=0;
set_terminal_raw();
while(!feof(fp))
	{char *cc;
	str[0]=0;
	cc=fgets(str,MAXLEN,fp);
	if (cc==NULL) break;
	//if (*cc=='#') continue; // this is comment
	if (*cc==']')
		{// this is command
		if (!memcmp(cc,"]COLOR",strlen("]COLOR"))) setcolor(atoi(cc+strlen("]COLOR")));
		if (!memcmp(cc,"]ESC",strlen("]ESC"))) esc(atoi(cc+strlen("]ESC")));
		continue;
		}
	if (str[0])
		{int ii=0;
		while(str[ii]) putchar(str[ii++]);
		if (++i>=(lines-1)) {i=0;c=pressanykey(); if (c=='q') {printf("QUIT\n");break;} }
		}
	}
fclose(fp);
set_terminal_no_raw();
setcolor(0);
}

#if 0
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
fclose(fp);
}
#endif

void save_world(void)
{int i,j,ii; char *cc;
FILE *fp;

fp=fopen("world.h", "w");
if (fp==NULL) {printf("Can't open world file\n"); return;}
fprintf(fp,"// world file for Virtustan application. created %s\n", ptime());
fprintf(fp,"// www.prool.kharkov.org\n");
for (i=0; i<MAX_X; i++) for (j=0; j<MAX_Y; j++)
	{
	if (cc=world[i][j].descr) fprintf(fp, "world[%i][%i].descr=\"%s\";\n", i, j, cc);
	if (ii=world[i][j].room_type) fprintf(fp, "world[%i][%i].room_type=%i;\n", i, j, ii);
	if (world[i][j].room_type==SOWED) fprintf(fp, "world[%i][%i].timer=time(0);\n", i, j);
	if (ii=world[i][j].object) fprintf(fp, "world[%i][%i].object=%i;\n", i, j, ii);
	if (ii=world[i][j].mob) fprintf(fp, "world[%i][%i].mob=%i;\n", i, j, ii);
	if ((ii=world[i][j].color)!=DEFAULT_COLOR) fprintf(fp, "world[%i][%i].color=%i;\n", i, j, ii);
	if ((ii=world[i][j].bg)!=DEFAULT_BG) fprintf(fp, "world[%i][%i].bg=%i;\n", i, j, ii);
	if ((ii=world[i][j].symbol)!=DEFAULT_SYMBOL) fprintf(fp, "world[%i][%i].symbol=%i;\n", i, j, ii);
	}
fprintf(fp,"// end of world file\n");
fclose(fp);
print("Мир сохранен. ");
if (updated)
	print("Не забудьте перекомпилировать его командой make\n");
else
	print("Но по нашим данным в мире вроде ничего не менялось. Но автоматика отслеживает не все транзакции, например взятие и ложение предметов не отслеживается. Но сохраняется по команде save! В любом случае после сохранения и выхода не забудьте сделать make\n");
updated=0;
}

void init_world(void)
{int i,j;

for (i=0; i<MAX_X; i++) for (j=0; j<MAX_Y; j++)
{
world[i][j].descr=0;
world[i][j].room_type=0;
world[i][j].symbol=DEFAULT_SYMBOL;
world[i][j].color=DEFAULT_COLOR;
world[i][j].bg=DEFAULT_BG;
world[i][j].object=0;
world[i][j].mob=0;
world[i][j].timer=0;
}

#include "world.h"

#if 0

world[50][50].descr="Вы находитесь перед воротами Виртустана (ворота на севере, идти на север надо командой n от слова north). На воротах висит объявление. Для просмотра объявления наберите vorotaob";
world[50][50].object=1;
world[50][51].descr="Вы находитесь на пограничном посту Виртустана. Отсюда на север тянется Виртустан"; world[50][51].symbol='!';
world[50][52].descr="Вы находитесь на Виртустанской улице, идущей в меридиональном направлении";
world[50][53].descr="Вы находитесь на Виртустанской площади";
world[50][54].descr="Вы находитесь на Виртустанском проспекте";
world[49][53].descr="Вы находитесь во Дворце Короля Виртустана. Вокруг лепота";
world[49][52].descr="Вы находитесь в Виртустанской Библиотеке. ТИШИНА ДОЛЖНА БЫТЬ В БИБЛИОТЕКЕ!!";
world[51][52].descr="Вы находитесь в Виртустанской Гостинице";
world[51][53].descr="Вы находитесь в Виртустанском Банке. Слышен звон пересчитываемых монет, шелест купюр и звяканье кассового аппарата";
world[50][48].descr="Вы находитесь в неглубокой яме"; world[50][48].object=2;
for (i=0; i<MAX_X; i++) for (j=0; j<MAX_Y; j++)
	{
	if ((world[i][j].descr) && (world[i][j].symbol=='"')) world[i][j].symbol='W';
	}

#endif

#if 0
for(j=1;j<MAX_Y-1;j++)
	{
	world[0][j].color=3;
	world[0][j].symbol='|';
	world[MAX_X-1][j].color=3;
	world[MAX_X-1][j].symbol='|';
	}
for(i=1;i<MAX_X-1;i++)
	{
	world[i][0].color=3;
	world[i][0].symbol='_';
	world[i][MAX_Y-1].color=3;
	world[i][MAX_Y-1].symbol='_';
	}
#endif

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

int print_object(int o)
// Return value: -1 - default item (invalid item number); else return 0
{int i;
i=0;
if (o==0) return 0;
printf("%s",ZHELT1);
switch (o)
	{
	case 1: print("Гранитный камешек"); break;
	case 2: print("Кости"); break;
	case 3: print("Семя"); break;
	case 4: print("Бесцветный камушек"); break;
	case 5: print("Синий камушек"); break;
	case 6: print("Огромный ржавый гвоздь"); break;
	case 7: print("Булыжник"); break;
	case 8: print("Дырявый котелок"); break;
	case 9: print("Ослиная челюсть"); break;
	case 10: print("Рваный сапог"); break;
	case 11: print("Чешуйка от былинного доспеха"); break;
	default: print("Сгусток темноты"); i=-1;
	}
printf("%s\n",NORM_COLOR);
return i;
}

void inv(void)
{
if (inv_o) {print("У вас в инвентаре находится:\n"); print_object(inv_o);}
else print("Ваш инвентарь пуст\n");
}

void get(void)
{int i;
if (world[global_x][global_y].room_type==SOWED)
	{
	print("Здесь нечего взять, только растет растение\n");
	}
else if ((i=world[global_x][global_y].object)==0) print("Что вы хотите взять? Тут ничего нет\n");
else
	if (inv_o) print("Ваш инвентарь заполнен, вы не можете ничего взять\n");
	else 
		{
		inv_o=i;
		world[global_x][global_y].object=0;
		print("Вы взяли предмет: ");
		print_object(inv_o);
		}
}

void drop(void)
{int i;
if ((i=world[global_x][global_y].object)) print("Сюда ничего нельзя положить, тут нет места\n");
else
	if (!inv_o) print("Вы ничего не можете положить, у вас же ничего нет\n");
	else 
		{
		world[global_x][global_y].object=inv_o;
		i=inv_o;
		inv_o=0;
		print("Вы положили предмет: ");
		print_object(i);
		}
}

void dup_(void)
{int i, j;
if ((i=world[global_x][global_y].object)==0)
	if (inv_o)	{
			world[global_x][global_y].object=inv_o;
			print("Вы дуплицировали и бросили на землю: "); print_object(inv_o);
			}
	else		print("У вас ничего нет. На земле ничего нет. Ничего не делаем\n");
else
	if (inv_o)	{
			print("Инвентарь и комната заполнены, вы ничего не можете дуплицировать\n");
			}
	else		{
			inv_o=i;
			print("Вы создали себе дубликат того, что лежало на земле: "); print_object(i);
			}
}

void swap(void)
{int i, j;
if ((i=world[global_x][global_y].object)==0)
	if (inv_o)	drop();
	else		print("У вас ничего нет. На земле ничего нет. Ничего не делаем\n");
else
	if (inv_o)	{
			j=inv_o;
			world[global_x][global_y].object=j;
			inv_o=i;
			print("Вы бросили: "); print_object(j);
			print("А взамен вы подняли: "); print_object(i);
			}
	else		get();
}

void roomcolor(void)
{int i;
char str[MAXLEN];

for (i=0;i<15;i++)
	{
	setcolor(i);
	printf("[%i] ",i);
	}
setcolor(0);

printf("\nColor number (1-)? ");
str[0]=0;i=0;
fgets(str,MAXLEN,stdin);
i=atoi(str);
if (i)	{
	updated=1;
	world[global_x][global_y].color=i;
	setcolor(i);
	printf("set color %i\n", i);
	setcolor(0);
	}
}

void roombg(void)
{int i;
char str[MAXLEN];

for (i=40;i<=47;i++)
	{
	esc(i);
	if (i==47) setcolor(2);
	printf("[bg %i] ",i);
	}
setcolor(0);

printf("\nBackground number (40-47)? ");
str[0]=0;i=0;
fgets(str,MAXLEN,stdin);
i=atoi(str);
if (i)	{
	updated=1;
	world[global_x][global_y].bg=i;
	setcolor(0);
	printf("set bg %i ", i);
	esc(i);
	printf("set bg %i\n", i);
	setcolor(0);
	}
}

void roomsymbolcode(void)
{int i;
char str[MAXLEN];

printfile("ascii.txt");

printf("\nRoom symbol code? ");
str[0]=0;i=0;
fgets(str,MAXLEN,stdin);
i=atoi(str);
if (i)	{
	updated=1;
	world[global_x][global_y].symbol=i;
	printf("set symbol '%c'\n", i);
	}
}

void roomtype(void)
{int i;
char str[MAXLEN];

printfile("roomtypes.h");

printf("\nCurrent room type %i\n",world[global_x][global_y].room_type);

printf("\nNew room type? (0 - no modif.) ");
str[0]=0;i=0;
fgets(str,MAXLEN,stdin);
i=atoi(str);
if (i)	{
	updated=1;
	world[global_x][global_y].room_type=i;
	printf("set type = %i\n", i);
	}
else printf("no set type\n");
}

void roomdescr(void)
{int i;
char str[MAXLEN];
char *c, *mm;

printf("\nRoom description? ");
str[0]=0;i=0;
fgets(str,MAXLEN,stdin);

c=strchr(str,'\n');
if (c) *c=0;

mm=malloc(MAXLEN);
if (!mm) {printf("All your memory is belong to us\n"); return;}
strcpy(mm,str);
if (1)	{
	updated=1;
	world[global_x][global_y].descr=mm;
	printf("set descr '%s'\n", mm);
	}
}

void roomsymbol(void)
{
char c;

printf("\nSymbol? ");
c=getchar();
if (c!=10)	{
	updated=1;
	world[global_x][global_y].symbol=c;
	printf("set symbol '%c'\n", c);
	}
}

void resetroom (void)
{
	updated=1;
	world[global_x][global_y].descr=0;
	world[global_x][global_y].room_type=0;
	world[global_x][global_y].symbol=DEFAULT_SYMBOL;
	world[global_x][global_y].color=DEFAULT_COLOR;
	world[global_x][global_y].bg=DEFAULT_BG;
	print("Комната очищена!!\n");
}

void till (void)
{
if (world[global_x][global_y].object)
	{
	print("Здесь пахать нельзя. Здесь лежит ");
	print_object (world[global_x][global_y].object);
	return;
	}
if ((world[global_x][global_y].descr==0) && (world[global_x][global_y].object==0))
	{
	world[global_x][global_y].descr="Вспахано";
	world[global_x][global_y].room_type=TILLED;
	world[global_x][global_y].symbol=' ';
	world[global_x][global_y].color=1;
	world[global_x][global_y].bg=43;
	print("Мы вспахали!\n");
	}
else
	print("Здесь нельзя пахать!\n");
}

void sow (void)
{
if (world[global_x][global_y].room_type==TILLED)
	{
	world[global_x][global_y].descr="Засеяно";
	world[global_x][global_y].room_type=SOWED;
	world[global_x][global_y].color=9; // green
	world[global_x][global_y].bg=DEFAULT_BG;
	world[global_x][global_y].object=3; // Семя
	world[global_x][global_y].timer=time(0);
	print("Мы посеяли!\n");
	}
else
	print("Здесь не вспахано, сеять нельзя\n");
}

void harvest (void)
{
if (world[global_x][global_y].room_type==SOWED)
	{
	if (plant_symbol(global_x, global_y)=='W')
		{
		world[global_x][global_y].descr=0;
		world[global_x][global_y].room_type=0;
		world[global_x][global_y].color=9; // green
		world[global_x][global_y].bg=DEFAULT_BG;
		world[global_x][global_y].object=0;
		world[global_x][global_y].timer=0;
		crop++;
		print("Урожай убран. Всего в закромах:"); printf(" %i\n", crop);
		}
	else
		{
		print("Урожай еще не созрел!\n");
		return;
		}
	}
else
	print("Здесь не посеяно: убирать нечего\n");
}

void score(void)
{
print("Закрома: "); printf("%i\n", crop);
}

void create(void)
{
if (inv_o) print("Вы не можете создать объект, у вас заполнен инвентарь\n");
else 	{
	inv_o=CREATED_OBJECT;
	print("Вы создали объект: ");
	print_object(inv_o);
	}	
}

void dig (void)
{int o;
if (random()<(RAND_MAX/10))
	{// мы что-то откопали
	o=random()%10;
	print("Вы откопали предмет: ");
	print_object(o);
	if (inv_o)
		{
		print("Но у вас заполнен инвентарь и вы потеряли его\n");
		if (world[global_x][global_y].object) print("Он упал обратно в яму и его засыпало землей\n");
			else
				{
				print("Он упал на землю");
				world[global_x][global_y].object=o;
				}
		}
	else
		inv_o=o;
	}
else print("Вы копали-копали, но ничего не нашли\n");
}

void destroy (void)
{int i;
if (!inv_o) print("Вы ничего не можете уничтожить, у вас же ничего нет\n");
else	{
	i=inv_o;
	inv_o=0;
	print("Объект уничтожен: ");
	print_object(i);
	}
}

void look(void)
{int i;

map();
printf("%s(%i,%i)%s step=%i roomtype=%i energy=%i\n",
GOLUB1,global_x,global_y,NORM_COLOR, step, world[global_x][global_y].room_type,energy);
if (world[global_x][global_y].descr) print(world[global_x][global_y].descr);
else
	{
	switch(sezon)
	{
	case WINTER: print("Зимняя снежная равнина"); break;
	case SPRING: print("Грязюка"); break;
	case SUMMER: print("Луг"); break;
	case AUTUMN: print("Грязюка. Опавшие листья"); break;
	default: print("Вокруг пиздец");
	}
	//world[global_x][global_y].descr="Тут был Пруль";
	}
printf("\n");

if (world[global_x][global_y].room_type==SOWED)
	{// засеяно
	printf("Timer %li Item age %li\n",
	world[global_x][global_y].timer, time(0) - world[global_x][global_y].timer);
	}
else
	{// не засеяно
	if (i=world[global_x][global_y].object) print_object(i);
	}
}

int try_move_to(int x, int y)
{
if ((x>MAX_X-1)||(y>MAX_Y-1)||(x<0)||(y<0)) return 1;
if (world[x][y].room_type==-1) return 2;
return 0;
}

void move_(int dx, int dy)
{int try_x, try_y;
int i;

try_x=global_x+dx;
try_y=global_y+dy;

i=try_move_to(try_x, try_y);

switch (i)
	{
	case 0: break;
	case 1: print("В этом направлении переместиться невозможно. Там край Мира\n"); return;
	default: print("В этом направлении переместиться невозможно\n"); return;
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

void testesc (void)
{int i;
char str[MAXLEN];

//printf("7m");

#if 0
printf("\nESC code? ");
str[0]=0;i=0;
fgets(str,MAXLEN,stdin);
i=atoi(str);
printf("entered ESC code %i\n", i);
esc(i);
printf("test text test text test text\n");
#endif

#if 1 
printf("\
-------------------------------------\
[7m\n");
#endif
#if 0
printf("---------------------------------------\n\
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

esc(21); printf("xxxxxxxxxxxxxxxxxxxxxxxx\n");
#endif

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
#ifdef ANDROID
strcpy(str_o, str_i);
#else
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
#endif
}

void koi_to_utf8(char *str_i, char *str_o)
{
#ifdef ANDROID
strcpy(str_o, str_i);
#else
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
#endif
}

void fromkoi (char *str)
{char buf [MAXUTF];
koi_to_utf8(str,buf);
strcpy(str,buf);
}

void utf8_to_win(char *str_i, char *str_o)
{
#ifdef ANDROID
strcpy(str_o, str_i);
#else
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
#endif
}

void win_to_utf8(char *str_i, char *str_o)
{
#ifdef ANDROID
strcpy(str_o, str_i);
#else
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
#endif
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

char *utf2lat (unsigned char *str)
{int i;
utf_to_lat(str,out);
return out;
#if 0
for (i=0; i<strlen(str); i++) printf("%02X ", *(str+i));
return "lat not implemented yet\n";
#endif
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
for (i=22/*33*/; i<128; i++)
	{
	printf ("%c ", i);
	if (!(i%16)) printf("\n");
	}
printf("\n");
}

void filler_do(int x,int y,int x1,int y1)
{
if (x1<0) return;
if (y1<0) return;
if (x1>=MAX_X) return;
if (y1>=MAX_Y) return;
if (world[x1][y1].descr) return;
if (world[x1][y1].room_type==-1) return; /*
	char symbol;
	char color; // color of foreground
	char bg; // color of background
	int object; // object in room
	int mob; // mob in room
	long int timer; */

world[x1][y1].room_type=FILLER2_ROOM;
world[x1][y1].timer=UPDATED_ROOM;
world[x1][y1].symbol=world[x][y].symbol;
}

void tempora_fugit(void)
{int x,y;
// array world[MAX_X][MAX_Y];

if (energy) energy--;

for (x=0;x<MAX_X;x++)
for (y=0;y<MAX_Y;y++)
	{
	// что-то делаем
	if (world[x][y].timer!=UPDATED_ROOM)
	{
	switch (world[x][y].room_type)
	{
	case INC_ROOM: world[x][y].symbol++; break;
	case FILLER_ROOM:
			world[x][y].room_type=FILLER2_ROOM;
			filler_do(x,y,x+1,y);
			filler_do(x,y,x-1,y);
			filler_do(x,y,x,y+1);
			filler_do(x,y,x,y-1);
#if 1
			filler_do(x,y,x-1,y-1);
			filler_do(x,y,x-1,y+1);
			filler_do(x,y,x+1,y-1);
			filler_do(x,y,x+1,y+1);
#endif
			world[x][y].timer=UPDATED_ROOM;
			break;
	case FILLER2_ROOM: world[x][y].room_type=FILLER_ROOM;
			world[x][y].timer=UPDATED_ROOM;
			break;
	case FILLER3_ROOM: world[x][y].room_type=0;
			world[x][y].timer=UPDATED_ROOM;
			break;
	}
	}
	}
for (x=0;x<MAX_X;x++)
for (y=0;y<MAX_Y;y++)
	{
	if (world[x][y].timer==UPDATED_ROOM) world[x][y].timer=0;
	}

// Langton ant
if (langton)
	{
	if (world[langton_x][langton_y].room_type==0)
		{ // 0 = langton black
		// 90 grad left
		langton_direct++;if (langton_direct==4) langton_direct=0;
		// set color black
		world[langton_x][langton_y].room_type=LANGTON_WHITE;
		world[langton_x][langton_y].symbol=' ';
		}
	else	{ // !0 = langton white
		// 90 grad right
		langton_direct--;if (langton_direct==-1) langton_direct=3;
		// set color white
		world[langton_x][langton_y].room_type=0;
		world[langton_x][langton_y].symbol='#';
		}
	// Langton move
	langton_x+=langton_table_x[langton_direct];
	langton_y+=langton_table_y[langton_direct];
	}
}

void map(void)
{
int x, y;
int local_max_y, local_max_x, local_min_y, local_min_x;

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
	setcolor((world[x][y].color));
	if (color_mode!=NO_COLOR) esc(world[x][y].bg); // background
	if ((x==global_x)&&(y==global_y)) {setcolor((PLAYER_COLOR)); printf("@");}
	else if (world[x][y].room_type==SOWED)
		putchar(plant_symbol(x,y));
	else putchar(world[x][y].symbol);
	printf(" ");
	setcolor(0);
	}
    printf("\n");
    }
printf(NORM_COLOR);
}

#define SEKADI 30
char plant_symbol(int x, int y /*int obj, long int timer*/)
{long int age;
int obj;
long int timer;

obj=world[x][y].object;
timer=world[x][y].timer;

age=time(0)-timer;
// . , | V W
if (age<SEKADI) return '.';
if (age<SEKADI*2) return ',';
if (age<SEKADI*3) return '|';
if (age<SEKADI*4) return 'V';
if (age<SEKADI*5) return 'W';
	world[x][y].descr="Сухое дерево";
	world[x][y].room_type=0;
	world[x][y].symbol='Y';
	world[x][y].color=3; // brown
	world[x][y].bg=DEFAULT_BG;
	world[x][y].object=0;
	world[x][y].timer=0;
return 'Y';
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

#define MAX_HOLYDAY 10
void print_holyday(void)
{
int i, day, month;
time_t mytime;
struct tm *timestruct;
struct holyday_stru
	{
	int day;
	int month;
	char *text;
	}
	holyday[MAX_HOLYDAY];

for (i=0;i<MAX_HOLYDAY;i++) holyday[i].text=0;

holyday[0].day=1;
holyday[0].month=12;
holyday[0].text="Первый день зимы";

holyday[1].day=2;
holyday[1].month=12;
holyday[1].text="Второй день зимы";

holyday[2].day=31;
holyday[2].month=12;
holyday[2].text="Новый Год";

holyday[3].day=1;
holyday[3].month=2;
holyday[3].text="Новый Год";

holyday[4].day=7;
holyday[4].month=4;
holyday[4].text="День рождения Пруля";

holyday[5].day=1;
holyday[5].month=9;
holyday[5].text="День знаний";

	mytime = time(0);
	timestruct=localtime(&mytime);
	day=timestruct->tm_mday;
	month=timestruct->tm_mon+1;
	//printf("day = %i\n", day);
	//printf("month = %i\n", month);

if ((month>=3) && (month<=5)) sezon=SPRING;
else if ((month>=6) && (month<=8)) sezon=SUMMER;
else if ((month>=9) && (month<=11)) sezon=AUTUMN;
else sezon=WINTER;

for (i=0;i<MAX_HOLYDAY;i++)
	{
	if (holyday[i].text==0) break;
	if (holyday[i].day==day)
		if (holyday[i].month==month)
			{
			sprintf (recode_buffer,"Сегодня праздник: %s\n", holyday[i].text);
			print(recode_buffer);
			break;
			}
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

long int unixtime(void)
	{
	return time(0);
	}

void log_(char *str)
{
FILE *fp;

fp=fopen(logfilename,"a");
if (!fp) {printf("Can't open logfile `%s'\n",logfilename); return;}
fprintf(fp,"%s %s\n",ptime(), str);
fflush(0);
fclose(fp);
}

int exec(char *file)
{FILE *fp;
fp=fopen(file,"r");
if (fp==NULL) return 1;
fclose(fp);
printfile(file);
return 0;
}

void sysinfo(char *envp[])
{
char terminal [MAXLEN];
char **envpp;

ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

printf("size of int %li",sizeof(int));
printf("\r\nsize of long int %li",sizeof(long int));
printf("\r\nsize of short int %li",sizeof(short int));
printf("\r\nsize of char %li",sizeof(char));
printf("\r\nsize of char* %li",sizeof(char*));
printf("\r\nsize of int* %li\n",sizeof(int*));
printf ("lines %d\n", lines);
printf ("columns %d\n", COLUMNS);
printf("base path %s\n", base_path);
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
printf("Color mode = ");
setcolor(7);
switch(color_mode)
	{
	case 0: printf("normal color\n"); break;
	case NO_COLOR: printf("no color\n"); break;
	case CONTRAST: printf("contrast color\n"); break;
	default: printf("ЖОПА\n");
	}
setcolor(0);
switch(have_config)
	{
	case 0: printf("No config file\n"); break;
	case 1: printf("Have config file '%s' in current dir\n", CONFIG_FILE); break;
	case 2: printf("Have priority config file '%s' in up dir\n", CONFIG_FILE); break;
	default: printf("Config internal logic error!\n");
	}
if (updated) print("\nМир был изменен!\n");
}

int file_no=0;

void ls(void)
{DIR *dir;
struct dirent *entry;
int i=0;
int counter=1;
char c;
int line=0;
struct stat struktura;

dir = opendir(".");

if (dir==0) {printf("Can't open current directory\n"); return;}

printf("\n");

set_terminal_raw();

while(1)
	{
	entry=readdir(dir);
	if (entry==0) break;
	if (i++ == file_no) printf("%2i. %s%s%s", counter++, REVERSE, entry->d_name, NORM_COLOR);
	else printf("%2i. %s", counter++, entry->d_name);
	if (!stat(entry->d_name, &struktura))
		{
		if (struktura.st_mode&0100000U) {/*printf("=regular file\n");*/}
		else if (struktura.st_mode&040000U) printf("/"); // directory
		else {/*print("=не файл и не каталог\n");*/}
		}
	printf("\n");
	if (++line>=(lines-1))
		{line=0;c=pressanykey();
		if (c=='q') {printf("QUIT\n");break;}else printf("\r                          \r");}
	}
set_terminal_no_raw();
closedir(dir);
}

void cat(void)
{DIR *dir;
struct dirent *entry;
int i=0;

dir = opendir(".");

if (dir==0) {printf("Can't open current directory\n"); return;}

while(1)
	{
	entry=readdir(dir);
	if (entry==0) break;
	if (i++ == file_no) {printfile(entry->d_name); return;}
	}
}

void hexcat(void)
{DIR *dir;
struct dirent *entry;
int i=0;

dir = opendir(".");

if (dir==0) {printf("Can't open current directory\n"); return;}

while(1)
	{
	entry=readdir(dir);
	if (entry==0) break;
	if (i++ == file_no) {hexfile(entry->d_name); return;}
	}
}

void delete_file(void)
{DIR *dir;
struct dirent *entry;
int i=0;

dir = opendir(".");

if (dir==0) {printf("Can't open current directory\n"); return;}

while(1)
	{
	entry=readdir(dir);
	if (entry==0) break;
	if (i++ == file_no)
		{
		if (remove(entry->d_name)==0) printf("File '%s' deleted\n", entry->d_name);
		else printf("File '%s' not deleted\n", entry->d_name);
		return;
		}
	}
}

void cmd_chdir(void)
{DIR *dir;
struct dirent *entry;
int i=0;

dir = opendir(".");

if (dir==0) {printf("Can't open current directory\n"); return;}

while(1)
	{
	entry=readdir(dir);
	if (entry==0) break;
	if (i++ == file_no) {if (chdir(entry->d_name)==0) file_no=0; pwd(); return;}
	}
pwd();
}

void create_file(void)
{
char str[MAXLEN]; char *cc;
FILE *fp;

str[0]=0;
cc=0;

printf("Filename > ");
cc=fgets(str,MAXLEN,stdin);

cc=strchr(str,'\r'); if (cc) *cc=0;
cc=strchr(str,'\n'); if (cc) *cc=0;

//printf("Try create file '%s'\n", str);

if (str[0]==0) {printf("create file: error: empty filename\n"); return;}

fp=fopen(str,"r");
if (fp)
	{
	printf("File '%s' already exits! Use 'delfile' if need\n", str);
	return;
	}

fp=fopen(str,"w");

if (fp==0) {printf("create file: error: can't create file\n"); return;}

printf("Enter lines for file\n. - end of input\n");

while(1)
	{
	printf(" > ");
	cc=fgets(str,MAXLEN,stdin);
	if (*cc=='.') break;
	//printf("inputted string = '%s'\n", str);
	fprintf(fp,"%s",str);
	}

fclose(fp);

}

void pwd(void)
{char buf[MAXLEN];
getcwd(buf,MAXLEN);
printf("%s\n", buf);
}

void filestatus(void)
{DIR *dir;
struct dirent *entry;
struct stat struktura;

int i=0;

dir = opendir(".");

if (dir==0) {printf("Can't open current directory\n"); return;}

while(1)
	{
	entry=readdir(dir);
	if (entry==0) break;
	if (i++ == file_no)
		{
		if (!stat(entry->d_name, &struktura))
			{
/* struct stat {
               dev_t     st_dev;         // ID of device containing file
               ino_t     st_ino;         // inode number
               mode_t    st_mode;        // protection
               nlink_t   st_nlink;       // number of hard links
               uid_t     st_uid;         // user ID of owner
               gid_t     st_gid;         // group ID of owner
               dev_t     st_rdev;        // device ID (if special file)
               off_t     st_size;        // total size, in bytes
               blksize_t st_blksize;     // blocksize for filesystem I/O
               blkcnt_t  st_blocks;      // number of 512B blocks allocated
		} */
/*The following mask values are defined for the file type component of the st_mode field:

           S_IFMT     0170000   bit mask for the file type bit fields

           S_IFSOCK   0140000   socket
           S_IFLNK    0120000   symbolic link
           S_IFREG    0100000   regular file
           S_IFBLK    0060000   block device
           S_IFDIR    0040000   directory
           S_IFCHR    0020000   character device
           S_IFIFO    0010000   FIFO */
	       printf("\nStatus of current file\n\n");
	       printf("Name %s ", entry->d_name);
		if (struktura.st_mode&0100000U) printf("=regular file\n");
		else if (struktura.st_mode&040000U) printf("=directory\n");
		else print("=не файл и не каталог\n");
               printf("device major/minor %04lX\n",struktura.st_dev); // ID of device containing file
               printf("inode %li\n",struktura.st_ino);         // inode number
               printf("mode %o\n",struktura.st_mode);        // protection
               printf("hardlinks %li\n",struktura.st_nlink);       // number of hard links
               printf("uid %i\n",struktura.st_uid);         // user ID of owner
               printf("gid %i\n",struktura.st_gid);         // group ID of owner
               printf("device id %li\n",struktura.st_rdev);        // device ID (if special file)
               printf("size %li bytes\n",struktura.st_size);        // total size, in bytes
               printf("blocks allocated %li (bytes allocated %li)\n",
		struktura.st_blocks, struktura.st_blocks*512); // number of 512B blocks allocated
               printf("blksize %li\n",struktura.st_blksize);     // blocksize for filesystem I/O
			}
		return;
		}
	}

}

void dir_up(void)
{
file_no--;
ls();
}

void dir_down(void)
{
file_no++;
ls();
}

void dir_move(void)
{char str[MAXLEN]; int i;
printf("Move directory cursor by lines ? ");
str[0]=0;i=0;
fgets(str,MAXLEN,stdin);
i=atoi(str);
file_no+=i;
ls();
}

void olist (void)
{int i;
// print list of objects
for (i=1;;i++)
	{
	printf("%i ", i);
	if (print_object(i)==-1) break;
	}
}

void random_flood(void)
{int i, j, r;

for (i=0; i<MAX_X; i++) for (j=0; j<MAX_Y; j++)
{
if (random()<(RAND_MAX/100))
	{
	if (world[i][j].descr==0)
		{
		world[i][j].symbol='R';
		r=random() % 127;
		if (r>=32) world[i][j].symbol=r;
		/*
		world[i][j].descr=0;
		world[i][j].room_type=0;
		world[i][j].color=DEFAULT_COLOR;
		world[i][j].bg=DEFAULT_BG;
		world[i][j].object=0;
		world[i][j].mob=0;
		world[i][j].timer=0;
		*/
		}
	}
}
look();
}

void reset(void) // reset terminal
{
printf("%cc",ESC); // ESC c - reset terminal
}

void point (void)
{
printf("point!\n");
}

void go_home(void)
{
//global_x=50; global_y=50;
//global_x=0; global_y=MAX_Y-1;
global_x=30; global_y=40;
}

void version (void)
{
printf("Virtustan application\nCopyleft by Prool, 2015-2016\nThis program comes with ABSOLUTELY NO WARRANTY; for details type `gpl3'.\
 Compile %s %s\n",__DATE__,__TIME__);
}

void system_(void)
{
system("sh");
}

void /*RETSIGTYPE*/ hupsig(int sig)
{
printf("\n\nExit by HUP signal = %i\n\n", sig);
log_("exit by HUP signal");
fflush(0);
exit(1);
}

///////////////////////////////////////////////
int main (int argc, char *argv[], char *envp[])
{
unsigned char cmd[MAXLEN_CMD];
char *cc;
int i, j;
FILE *fconfig;
char string[PROOL_MAX_STRLEN];
char config_filename_2[PROOL_MAX_STRLEN];
char buffer_string[PROOL_MAX_STRLEN];

start_time=unixtime();
step=0;
langton=0;
langton_direct=1;

energy=100; // начальное значение энергии

signal(SIGHUP, hupsig);
signal(SIGINT, hupsig);
signal(SIGTERM, hupsig);

getcwd(base_path, MAXLEN);

strcpy(logfilename, base_path);
strcat(logfilename, "/virtustan-app.log");

//printf("base path =%s\n", base_path);

srandom(unixtime());

// prool: config file processing

strcpy(config_filename_2, "../");
strcat(config_filename_2, CONFIG_FILE);

fconfig=fopen(config_filename_2,"r");
if (fconfig)
	{
	printf("Using %s\n", config_filename_2);
	have_config=2;
	}
else 	{ fconfig=fopen(CONFIG_FILE, "r");
	if (fconfig)
			{	
			printf("Using %s\n", CONFIG_FILE);
			have_config=1;
			}
	}
if (fconfig)
	{
	while (!feof(fconfig))
		{char *pp;
		string[0]=0;
		fgets(string,PROOL_MAX_STRLEN,fconfig);
		pp=strchr(string,'\n');
		if (pp) *pp=0;
		// printf("`%s'\n", string); // debug print
		if (!strcmp(string,"test")) printf("TEST OK!\n");
		}
	fclose(fconfig);
	}
else
	{
	printf("%s not found\n", CONFIG_FILE);
	have_config=0;
	}
// end of config file processing

if (random()<(RAND_MAX/20))
	{// fortune
	print("\n\n\n                         На пальмовой набережной он получил всё, что ему причиталось\n");
	//printf("random = %i\n", random());
	//printf("max random = %i\n", RAND_MAX);
	printf("\n\npress any key\n");
	getchar();
	}

updated=0;
HALF_X=10; // полуразмеры карты от центра карты по измерениям: (10,10) by default
HALF_Y=10; 

ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
MAX_I=lines-8; // 20
if ((COLUMNS-1)>MAX_X) MAX_J=MAX_X;
else MAX_J=COLUMNS-1;
i_c=MAX_I-2; j_c=MAX_J-2; // cursor loc. for realtime mod

clearscreen();

version();

//sysinfo(envp);

printf("%sUse `help' command for help and `quit' for quit.%s\n", BEL1, NORM_COLOR);

print_holyday();

log_("Virtustan application started. http://prool.kharkov.org https://github.com/prool/virtustan");

//printf("init started\n");
Codetable=UTF;

go_home();

init_world();

// init realtime()
for (i=0;i<MAX_L;i++) for (j=0;j<MAX_C;j++) {screen[i][j]=FILLER; screen_color[i][j]=0; screen_bg[i][j]=0;}
cur_l=0; cur_c=0;
//i=0; j=0; screen[i][j]='0'; screen_color[i][j]=2; screen_bg[i][j]=0;

HALF_X = COLUMNS/4-1;
HALF_Y = lines/2-3;

//printf("init ended\n\n");

look();

if (argc==2)
	{
	// printf("arg %s\n", argv[1]);
	if (!strcmp(argv[1],"realtime")) realtime();
	else
		{
		printf("\nusage: virtustan [realtime]\n");
		return 1;
		}
	}

while(1)
	{
	step++;
	printf("virtustan app> ");
	fgets(cmd,MAXLEN_CMD,stdin); // нафига я тут использовал fgets, а не gets ? наверное из-за проверки длины
	switch (Codetable)
		{
		case KOI: fromkoi(cmd); break;
		case WIN: fromwin(cmd); break;
		}
	// printf("cmd=`%s'\n", cmd);
	cc=strchr(cmd,'\n');
	if (cc) *cc=0;
	if (cmd[0]==0) continue;
	if ((!strcmp(cmd,"Q")) || (!strcmp(cmd,"q")) || (!strcmp(cmd,"quit")) || (!strcmp(cmd,"exit")) ||
		(!strcmp(cmd,"конец"))) 
		{
		if (updated==0) break;
		print("Мир был изменен! Сохраните мир командой save. Или выйдите командой непосредственного выхода Quit\n");
		}
	else if (!strcmp(cmd,"Quit")) break;
	else if ((!strcmp(cmd,"help"))||(!strcmp(cmd,"h"))||(!strcmp(cmd,"?"))||(!strcmp(cmd,"помощь")))
		printfile_basepath("texts/help.txt"); 
	else if (!strcmp(cmd,"alf")) print(ALFAVIT);
	else if (!strcmp(cmd,"ver")) version();
	else if (!strcmp(cmd,"ascii")) ascii();
	else if (!strcmp(cmd,"koi")) {Codetable=KOI; printf("Codetable switch to KOI\n");}
	else if (!strcmp(cmd,"utf")) {Codetable=UTF; printf("Codetable switch to UTF\n");}
	else if (!strcmp(cmd,"win")) {Codetable=WIN; printf("Codetable switch to WIN\n");}
	else if (!strcmp(cmd,"lat")) {Codetable=LAT; printf("Codetable switch to LAT: beta\n");}
	else if (!strcmp(cmd,"codetable")) printf("Current codetable is %s\n",CodetableName[Codetable]);
	else if ((!strcmp(cmd,"look")) || (!strcmp(cmd,"см")) || (!strcmp(cmd,"."))) {look(); energy++;}
	else if (!strcmp(cmd,"rest")) {print("Вы отдохнули и восстановили немного энергии\n"); energy+=5;}

	else if (energy<=0) printf("У вас нет сил сделать что-либо!\n");

	else if (!strcmp(cmd,"n")) move_(0,+1);
	else if (!strcmp(cmd,UP_ARROW)) move_(0,+1);
	else if (!strcmp(cmd,"N")) move_(0,MAX_Y-1-global_y);
	else if (!strcmp(cmd,DOWN_ARROW)) move_(0,-1);
	else if (!strcmp(cmd,"s")) move_(0,-1);
	else if (!strcmp(cmd,"S")) move_(0,-global_y);
	else if (!strcmp(cmd,"w")) move_(-1,0);
	else if (!strcmp(cmd,LEFT_ARROW)) move_(-1,0);
	else if (!strcmp(cmd,"W")) move_(-global_x,0);
	else if (!strcmp(cmd,"e")) move_(+1,0);
	else if (!strcmp(cmd,RIGHT_ARROW)) move_(+1,0);
	else if (!strcmp(cmd,"E")) move_(MAX_X-1-global_x,0);
	else if (!strcmp(cmd,"gohome")) {go_home(); look();}
	else if (!strcmp(cmd,"map")) map();
	else if (!strcmp(cmd,"vorotaob")) printfile("texts/vorotaob.txt");
	else if (!strcmp(cmd,"gpl3")) printfile("LICENSE");
	else if (!strcmp(cmd,"constitution")) printfile("texts/constitution.txt");
	else if (!strcmp(cmd,"pledge")) printfile("texts/pledge.txt");
	else if (!strcmp(cmd,"env")) env(envp);
	else if (!strcmp(cmd,"testesc")) testesc();
	else if (!strcmp(cmd,"test2")) test2();
	else if (!strcmp(cmd,"test3")) test3();
	else if (!strcmp(cmd,"test4")) test4();
	else if (!strcmp(cmd,"rog")) rogalik();
	else if (!strcmp(cmd,"cls")) clearscreen();
	else if (!strcmp(cmd,"date")) date();
	else if (!strcmp(cmd,"rt")) realtime();
	else if (!strcmp(cmd,"rto")) realtime_old();
	else if (!strcmp(cmd,"inv")) inv();
	else if (!strcmp(cmd,"get")) get();
	else if (!strcmp(cmd,"drop")) drop();
	else if (!strcmp(cmd,"swap")) swap();
	else if (!strcmp(cmd,"dup")) dup_();
	else if (!strcmp(cmd,"create")) create();
	else if (!strcmp(cmd,"random")) random_flood();
	else if (!strcmp(cmd,"destroy")) destroy();
	else if (!strcmp(cmd,"roomcolor")) roomcolor();
	else if (!strcmp(cmd,"roombg")) roombg();
	else if (!strcmp(cmd,"roomsymbol")) roomsymbol();
	else if (!strcmp(cmd,"roomsymbolcode")) roomsymbolcode();
	else if (!strcmp(cmd,"roomdescr")) roomdescr();
	else if (!strcmp(cmd,"roomtype")) roomtype();
	else if (!strcmp(cmd,"resetroom")) resetroom();
	else if (!strcmp(cmd,"save")) save_world();
	else if (!strcmp(cmd,"ls")) ls();
	else if (!strcmp(cmd,"holyday")) print_holyday();
	else if (!strcmp(cmd,"blog")) printfile("texts/blog.txt");
	else if (!strcmp(cmd,"source")) printfile("virtustan.c");
	else if (!strcmp(cmd,"sysinfo")) sysinfo(envp);
	else if (!strcmp(cmd,"reset")) reset();
	else if (!strcmp(cmd,"dir-up")) dir_up();
	else if (!strcmp(cmd,L_UP)) dir_up();
	else if (!strcmp(cmd,"dir-down")) dir_down();
	else if (!strcmp(cmd,L_DOWN)) dir_down();
	else if (!strcmp(cmd,"dir-move")) dir_move();
	else if (!strcmp(cmd,"cd")) cmd_chdir();
	else if ((!strcmp(cmd,"cd..")) || (!strcmp(cmd,".."))) {chdir(".."); pwd();}
	else if (!strcmp(cmd,"cd/")) chdir("/");
	else if (!strcmp(cmd,"pwd")) pwd();
	else if (!strcmp(cmd,"delfile")) delete_file();
	else if (!strcmp(cmd,"createfile")) create_file();
	else if (!strcmp(cmd,"cat")) cat();
	else if (!strcmp(cmd,"hcat")) hexcat();
	else if (!strcmp(cmd,"stat")) filestatus();
	else if (!strcmp(cmd,"skript")) skript();
	else if (!strcmp(cmd,"till")) till();
	else if (!strcmp(cmd,"пахать")) till();
	else if (!strcmp(cmd,"sow")) sow();
	else if (!strcmp(cmd,"сеять")) sow();
	else if (!strcmp(cmd,"harvest")) harvest();
	else if (!strcmp(cmd,"убрать")) harvest();
	else if (!strcmp(cmd,"score")) score();
	else if (!strcmp(cmd,"счет")) score();
	else if (!strcmp(cmd,"сч")) score();
	else if (!strcmp(cmd,"dig")) dig();
	else if (!strcmp(cmd,"sh")) system_();
	else if (!strcmp(cmd,"contrast")) {color_mode=CONTRAST; printf("set contrast color mode\n");}
	else if (!strcmp(cmd,"nocolor")) {color_mode=NO_COLOR; reset(); printf("set nocolor mode\n");}
	else if (!strcmp(cmd,"normcolor")) {color_mode=0;printf("set normal color mode\n");}
	else if (!strcmp(cmd,"шире")) {HALF_X++; look(); }
	else if (!strcmp(cmd,"уже")) {HALF_X--; look(); }
	else if (!strcmp(cmd,"выше")) {HALF_Y++; look(); }
	else if (!strcmp(cmd,"ниже")) {HALF_Y--; look(); }
	else if (!strcmp(cmd,"olist")) olist();
	else if (!strcmp(cmd,"beep")) beep();
	else if (!strcmp(cmd,"langton")) { 	if (langton==0)
							{langton=1;langton_x=global_x;langton_y=global_y;
							printf("Langton ant started\n");
							}
						else
							{langton=0;
							printf("Langton ant stopped\n");
							}
					 }
	else if ((cmd[0]>='0') && (cmd[0]<='9'))
			{char c;
			// digit command
			i=atoi(cmd);
			c=cmd[strlen(cmd)-1];
			printf("digit %i, last symbol '%c'\n", i, c);
			switch (c)
				{
				case 'n': while (i--) move_(0,+1); break;
				case 's': while (i--) move_(0,-1); break;
				case 'w': while (i--) move_(-1,0); break;
				case 'e': while (i--) move_(+1,0); break;
				default: printf("Unknown multiply command\n");
				}
			}
	else 	{// No internal command. External command:
		if (exec(cmd))
			{
			printf("\nBad command or file name `%s'\n\n( ", cmd);
			i=0;
			while(cmd[i]) printf("%02X ", cmd[i++]);
			printf(")\n\nUse help for help. Use quit for quit\nИспользуйте команду помощь для получения помощи, а команду конец для выхода из программы\n");
			}
		}
	tempora_fugit();
	}
log_("Virtustan application finished");
printf("%sVirtustan application finished. Final statistics:%s\n", BEL1, NORM_COLOR);
printf("Steps %i\n", step);
uptime_();
return 0;
}
/******************************************************************************************************/
#define CLR for(jj=0;jj<MAX_J; jj++) putch(' ');putch('\r')
/******************************************************************************************************/
void enable_mouse(void)
{
#if 1 // mouse code from midnight commander
//    case MOUSE_XTERM_NORMAL_TRACKING:
        /* save old highlight mouse tracking */
        printf (ESC_STR "[?1001s");

        /* enable mouse tracking */
        printf (ESC_STR "[?1000h");

        /* enable SGR extended mouse reporting */
//        printf (ESC_STR "[?1006h");

        fflush (stdout);

//    case MOUSE_XTERM_BUTTON_EVENT_TRACKING:
        /* save old highlight mouse tracking */
        printf (ESC_STR "[?1001s");

        /* enable mouse tracking */
        printf (ESC_STR "[?1002h");

        /* enable SGR extended mouse reporting */
//        printf (ESC_STR "[?1006h");

        fflush (stdout);
#endif
}

void disable_mouse(void)
{

#if 1 // mouse code from midnight commander

// case MOUSE_XTERM_NORMAL_TRACKING:
        /* disable SGR extended mouse reporting */
        printf (ESC_STR "[?1006l");

        /* disable mouse tracking */
        printf (ESC_STR "[?1000l");

        /* restore old highlight mouse tracking */
        printf (ESC_STR "[?1001r");

        fflush (stdout);
//    case MOUSE_XTERM_BUTTON_EVENT_TRACKING:
        /* disable SGR extended mouse reporting */
        printf (ESC_STR "[?1006l");

        /* disable mouse tracking */
        printf (ESC_STR "[?1002l");

        /* restore old highlight mouse tracking */
        printf (ESC_STR "[?1001r");

        fflush (stdout);
#endif
}

/******************************************************************************************************/

void realtime (void)
{int i,j,c,quit, to_os;
int oldf;
long int ll;
char podkursor_save[3];
int tick_status;
int life_on;
int life_step;
int r_l, r_c; // realtime lines & realtime columns

ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

tick_status=0;
langton=0;
life_on=0;
life_step=0;

r_l=lines;
r_c=COLUMNS;

//printf("lines %i columns %i\n",r_l,r_c);
if (r_l>MAX_L) {printf("realtime module: array is small: lines=%i MAX_L=%i\n",r_l,MAX_L); return; }
if (r_c>MAX_C) {printf("realtime module: array is small: COLUMNS=%i MAX_C=%i\n",r_c,MAX_C); return; }

#if 0
i=0; j=0; screen[i][j]='0'; screen_color[i][j]=2; screen_bg[i][j]=0;
i=2; j=2; screen[i][j]='#'; screen_color[i][j]=2; screen_bg[i][j]=0;
#endif

for (i=0;i<MAX_L;i++) for (j=0;j<MAX_C;j++)
	{screen_old[i][j]=screen[i][j]; screen_color_old[i][j]=screen_color[i][j]; screen_bg_old[i][j]=screen_bg[i][j];}

to_os=0;


/*  Set stdin (file descriptor=0) to NOraw mode and echo */
ioctl(0, TCGETA, &tstdin);
tstdin.c_lflag &= ~(ICANON|ECHO);
ioctl(0, TCSETA, &tstdin);

oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

// скрыть курсор
putchar(27);
printf("[?25l");

enable_mouse();

podkursor_save[0]=screen[cur_l][cur_c];
podkursor_save[1]=screen_color[cur_l][cur_c];
podkursor_save[2]=screen_bg[cur_l][cur_c];

screen[cur_l][cur_c]='@';
screen_color[cur_l][cur_c]=CURSOR_COLOR;

quit=0;
while(!quit)
	{
	// refresh screen
	clearscreen();
	gotoxy(0,0);
	printf("Virtustan realtime mode. cur_l=%i cur_c=%i ? - help\n", cur_l, cur_c);
	for (i=0;i<(r_l-2);i++)
		{
		for (j=0;j<r_c;j++)
			{
			/*
			if (j==0) printf("0");
			else if (j==(r_c-1)) printf("!");
			else
				if (i==0) printf("-");
				else if (i==(r_l-3)) printf("=");
				else printf(".");
			*/
			if (screen_bg_old[i][j]) esc(screen_bg_old[i][j]);
			if (screen_color_old[i][j]) setcolor(screen_color_old[i][j]);
			printf("%c", screen_old[i][j]);
			if (screen_color_old[i][j]) setcolor(0);
			}
		if (r_c<COLUMNS) printf("\n");
		}
	while (1) // main realtime loop
		{
		usleep(10000);
		// обновляем screen
		ll=unixtime();
		i=ll%10;
		screen[0][r_c-1]='0'+i;
		screen_color[0][r_c-1]=2;
		screen_bg[0][r_c-1]=41;

		// ticks
		if (tick_status==i)
			{
			tick_status++;
			tick_status=tick_status%10;

			life_step=1;
#if 1
			if (screen[1][1]++=='~') screen[1][1]='!';
			//screen_color[1][1]++;
			//screen_bg[1][1]++;
#endif
			}
#if 1 // Langton ant
if (langton)
	{
	if (screen[langton_x][langton_y]==' ')
		{ // 0 = langton black
		// 90 grad left
		langton_direct++;if (langton_direct==4) langton_direct=0;
		// set color black
		screen[langton_x][langton_y]='*';
		}
	else	{ // !0 = langton white
		// 90 grad right
		langton_direct--;if (langton_direct==-1) langton_direct=3;
		// set color white
		screen[langton_x][langton_y]=' ';
		}
	// Langton move
	langton_x+=langton_table_x[langton_direct];
	langton_y+=langton_table_y[langton_direct];
	if ((langton_x<0) || (langton_x>=MAX_L) || (langton_y<0) || (langton_y>=MAX_C)) langton=0;
	}

#endif

#if 1 // life
int count;
if (life_on && life_step)
	{
	life_step=0;
	// первый проход
	for (i=0+1;i<((r_l-2)-1);i++)
		{
		for (j=0+1;j<(r_c-1);j++)
			{
			if (screen[i][j]=='*')
				{// заполненная клеточка. проверяем, будет ли жить
				count=0;
				if (screen[i+1][j]=='*') count++;
				if (screen[i-1][j]=='*') count++;
				if (screen[i][j+1]=='*') count++;
				if (screen[i][j-1]=='*') count++;
				if (screen[i-1][j-1]=='*') count++;
				if (screen[i-1][j+1]=='*') count++;
				if (screen[i+1][j-1]=='*') count++;
				if (screen[i+1][j+1]=='*') count++;
				if (!((count==2)||(count==3)))
					{//death
					screen_color[i][j]=6; // ставим пометку измнения
					}
				}
			else	{// пустая клеточка, проверяем, зародится ли там жизнь
				count=0;
				if (screen[i+1][j]=='*') count++;
				if (screen[i-1][j]=='*') count++;
				if (screen[i][j+1]=='*') count++;
				if (screen[i][j-1]=='*') count++;
				if (screen[i-1][j-1]=='*') count++;
				if (screen[i-1][j+1]=='*') count++;
				if (screen[i+1][j-1]=='*') count++;
				if (screen[i+1][j+1]=='*') count++;
				if (count==3) screen_color[i][j]=6; // ставим пометку
				}
			}
		}
	// второй проход
	for (i=0+1;i<((r_l-2)-1);i++)
		{
		for (j=0+1;j<(r_c-1);j++)
			{
			if (screen_color[i][j]==6) // стоит пометка
				{
				if (screen[i][j]=='*') screen[i][j]='.';
				else screen[i][j]='*';
				screen_color[i][j]=0;
				}
			}
		}
	}
#endif
		setpos(1,1); printf("debug: (%02i,%02i) (%i,%i,%i) ? - Help                                       ",
		cur_l, cur_c, podkursor_save[0],podkursor_save[1],podkursor_save[2]); // debug print
		// вывод разницы
		for (i=0;i<MAX_L;i++) for (j=0;j<MAX_C;j++)
			{
			if ( (screen[i][j]!=screen_old[i][j]) || (screen_color[i][j]!=screen_color_old[i][j]) ||
			     (screen_bg[i][j]!=screen_bg_old[i][j]))
				{
				setpos(i+2,j+1);
				if (screen_bg[i][j]) esc(screen_bg[i][j]);
				if (screen_color[i][j]) setcolor(screen_color[i][j]);
				printf("%c", screen[i][j]);
				if (screen_color[i][j]) setcolor(0);
				}
			}
		// screen_old <- screen
		for (i=0;i<MAX_L;i++) for (j=0;j<MAX_C;j++) {screen_old[i][j]=screen[i][j];
			screen_color_old[i][j]=screen_color[i][j]; screen_bg_old[i][j]=screen_bg[i][j];}
		
		c=getchar();


		if (c==27 /* ESC */) {	c=getchar();
				if (c==91 /* 0x5B */)
					{
					c=getchar();
					if (c==66 /*0x42*/)	goto l_s;
					else if (c==65)	goto l_n;
					else if (c==68)	goto l_w;
					else if (c==67)	goto l_e;
					else if (c==0x35)
						{
						c=getchar();
						if (c==0x7E)
							{ // PgUp
							//podkursor_save[0]='U';
							podkursor_save[1]++;
							}
						}
					else if (c==0x36)
						{
						c=getchar();
						if (c==0x7E)
							{ // PgDn
							//podkursor_save[0]='D';
							podkursor_save[2]++;
							}
						}
					else if (c==0x46)
						{
						//podkursor_save[0]='E';
						podkursor_save[2]=40;
						}
//нажатие лев. кн. мыши 1b 5b 4d 20 x y
//отпускание            1b 5b 4d 23 x y
//нажатие прав. кн.     1b 5b 4d 22 x y
//отпускание            1b 5b 4d 23 x y
//нажатие колесика      1b 5b 4d 21 x y
//отпускание            1b 5b 4d 23 x y
//шаг колесика вперед   1b 5b 4d 60 x y
//шаг колесика назад    1b 5b 4d 61 x y
//движ при наж лев.кн.  1b 5b 4d 40 x y
//движ при наж прав.кн. 1b 5b 4d 42 x y
					else if (c==0x4D) // mouse
						{int x,y; // mouse coordinates
						c=getchar();
						if (c==0x20) // press left button
							{
							x=getchar()-0x21;
							y=getchar()-0x21-1;
							if (screen[y][x]=='*') screen[y][x]=FILLER;
							else screen[y][x]='*';
							}
						else if (c==0x23) // release left button
							{
							c=getchar(); // x
							c=getchar(); // y
							}
						else if (c==0x22) // press right button
							{
							x=getchar();
							y=getchar();
							screen[y-0x21-1][x-0x21]='R';
							}	
						else if (c==0x21) // press wheel
							{
							x=getchar();
							y=getchar();
							screen[y-0x21-1][x-0x21]='W';
							}	
						else    { // unknown code
							c=getchar(); // x
							c=getchar(); // y
							}
						}
					}
		}

		else if (c=='r') break;
		else if (c=='<') {r_c--; clearscreen(); break;}
		else if (c=='>') {r_c++; clearscreen(); break;}
		else if (c==',') {r_l--; clearscreen(); break;}
		else if (c=='.') {r_l++; clearscreen(); break;}
		else if (c=='Q') {quit=1; to_os=1; break;}
		else if (c=='q')		{
					quit=1;
					screen[cur_l][cur_c]=podkursor_save[0];
					screen_color[cur_l][cur_c]=podkursor_save[1];
					screen_bg[cur_l][cur_c]=podkursor_save[2];
					break;
					}
		else if (c=='e') {l_e:if (cur_c<(r_c-1))
					{
					screen[cur_l][cur_c]=podkursor_save[0];
					screen_color[cur_l][cur_c]=podkursor_save[1];
					screen_bg[cur_l][cur_c]=podkursor_save[2];
					cur_c++;
					podkursor_save[0]=screen[cur_l][cur_c];
					podkursor_save[1]=screen_color[cur_l][cur_c];
					podkursor_save[2]=screen_bg[cur_l][cur_c];
					screen[cur_l][cur_c]='@';
					screen_color[cur_l][cur_c]=CURSOR_COLOR;
					} 
				 else screen[cur_l][cur_c]='*';
				 }
		else if (c=='w') {l_w:if (cur_c>0)
					{
					screen[cur_l][cur_c]=podkursor_save[0];
					screen_color[cur_l][cur_c]=podkursor_save[1];
					screen_bg[cur_l][cur_c]=podkursor_save[2];
					cur_c--;
					podkursor_save[0]=screen[cur_l][cur_c];
					podkursor_save[1]=screen_color[cur_l][cur_c];
					podkursor_save[2]=screen_bg[cur_l][cur_c];
					screen[cur_l][cur_c]='@';
					screen_color[cur_l][cur_c]=CURSOR_COLOR;
					} 
				 else screen[cur_l][cur_c]='*';
				 }
		else if (c=='s') {l_s:if (cur_l<(r_l-2)) // нижняя граница
					{
					screen[cur_l][cur_c]=podkursor_save[0];
					screen_color[cur_l][cur_c]=podkursor_save[1];
					screen_bg[cur_l][cur_c]=podkursor_save[2];
					cur_l++;
					podkursor_save[0]=screen[cur_l][cur_c];
					podkursor_save[1]=screen_color[cur_l][cur_c];
					podkursor_save[2]=screen_bg[cur_l][cur_c];
					screen[cur_l][cur_c]='@';
					screen_color[cur_l][cur_c]=CURSOR_COLOR;
					} 
				 else screen[cur_l][cur_c]='*';
				 }
		else if (c=='n') {l_n:if (cur_l>0)
					{
					screen[cur_l][cur_c]=podkursor_save[0];
					screen_color[cur_l][cur_c]=podkursor_save[1];
					screen_bg[cur_l][cur_c]=podkursor_save[2];
					cur_l--;
					podkursor_save[0]=screen[cur_l][cur_c];
					podkursor_save[1]=screen_color[cur_l][cur_c];
					podkursor_save[2]=screen_bg[cur_l][cur_c];
					screen[cur_l][cur_c]='@';
					screen_color[cur_l][cur_c]=CURSOR_COLOR;
					} 
				 else screen[cur_l][cur_c]='*';
				 }
		else if (c==' ') podkursor_save[0]++;
		else if (c=='0') podkursor_save[0]=' ';
		else if (c=='?')	{
					setpos(1,1);
					printf("\n\nlines %i columns %i\n\n",r_l,r_c);
					printf("\n\nHelp:\n\n");
					printf("n s w e or arrows - move\n");
					printf("r - refresh screen\n");
					printf("0 - clear cell\n");
					printf("Spacebar - symbol++\n");
					printf("PgUp - color++\nPgDn - background++\nEnd - background=40\n");
					printf("Mouse: left button - on/off cell\n");
					printf("R - random L - Langton ant\n");
					printf("l - Conway life start\n");
					printf("G - get data from main world S - save data to main world\n");
					printf("q - quit to app., Q - quit to OS shell\n");
					printf("? - this help\n\n");
					printf("Press any key for exit from help\n");
					while (getchar()==-1) usleep(10000);
					break;
					}
		else if (c=='R')
				{// random
				for (i=0;i<(r_l-2);i++)
				{
				for (j=0;j<r_c;j++)
				{
				if (random()<(RAND_MAX/100))screen[i][j]='*';
				}
				}
				}
		else if (c=='G')
				{
				for (i=0;i<MAX_L;i++) for (j=0;j<MAX_C;j++)
				{int ii, jj;
				ii=j;
				jj=MAX_Y-1-i;
				if ((ii<MAX_X) && (jj<MAX_Y))
					{
					if (world[ii][jj].symbol) screen[i][j]=world[ii][jj].symbol;
					screen_color[i][j]=world[ii][jj].color;
					if (world[ii][jj].bg) screen_bg[i][j]=world[ii][jj].bg;
					}
				}
				podkursor_save[0]=screen[cur_l][cur_c];
				podkursor_save[1]=screen_color[cur_l][cur_c];
				podkursor_save[2]=screen_bg[cur_l][cur_c];
					screen[cur_l][cur_c]='@';
					screen_color[cur_l][cur_c]=CURSOR_COLOR;
				}
		else if (c=='S')
				{
				for (i=0;i<MAX_L;i++) for (j=0;j<MAX_C;j++)
				{int ii, jj;
				ii=j;
				jj=MAX_Y-1-i;
					screen[cur_l][cur_c]=podkursor_save[0];
					screen_color[cur_l][cur_c]=podkursor_save[1];
					screen_bg[cur_l][cur_c]=podkursor_save[2];
				if ((ii<MAX_X) && (jj<MAX_Y))
					{
					if(screen[i][j]) world[ii][jj].symbol=screen[i][j];
					world[ii][jj].color=screen_color[i][j];
					if (screen_bg[i][j]) world[ii][jj].bg=screen_bg[i][j];
					}
				}
				}
		else if (c=='L')
				{
				 if (langton==0) {
				 langton=1;
				 langton_x=cur_l;
				 langton_y=cur_c;
				 }
				 else langton=0;
				}
		else if (c=='l') // life
				{
				if (life_on==0) life_on=1; else life_on=0;
				}
		else {if (c!=-1) screen[cur_l][cur_c]='#';}
		}
	}
langton=0;
ioctl(0, TCGETA, &tstdin);
tstdin.c_lflag |= (ICANON|ECHO);
ioctl(0, TCSETA, &tstdin);
fcntl(STDIN_FILENO, F_SETFL, oldf);
setpos(r_l-2,1);
printf("\n\nexit from realtime\n");
// показать курсор
putchar(27);
printf("[?25h");
disable_mouse();
if (to_os) {reset(); exit(0);}
}
// end of realtime()
///////////////////////////////////////////////////////////////////////////////

void realtime_old (void)
{char c; int i, j; int x,y;
int oldf;
int online_help=0;
int cursor_blink=0;
int save_x, save_y;
int jj;
int MAX_I, MAX_J;

ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
MAX_I=lines-8; // 20

if ((COLUMNS-1)>MAX_X) MAX_J=MAX_X;
else MAX_J=COLUMNS-1;

MAX_J=COLUMNS-0; // prool fool

//i_c=MAX_I-2; j_c=MAX_J-2; // cursor loc. for realtime mod

/*  Set stdin (file descriptor=0) to NOraw mode and echo */
ioctl(0, TCGETA, &tstdin);
tstdin.c_lflag &= ~(ICANON|ECHO);
ioctl(0, TCSETA, &tstdin);

oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

clearscreen();
// скрыть курсор
putchar(27);
printf("[?25l");

save_x=global_x; save_y=global_y;

while(1)
	{
	// refresh screen
	gotoxy(0,0);
	printf("Virtustan realtime application MAX_J=%i q - quit to virtustan app, Q - quit to OS, ? - help ", MAX_J);
	//setcolor(2);
	//printf("%s\n\n", ptime()+4);
	//setcolor(0);

	printf("\r\n");
	global_x=j_c; global_y=i_c+MAX_Y-MAX_I;
	for (i=MAX_I-1;i>=0;i--)
		{
		for (j=0;j<MAX_J;j++)
			{
			if ((i==i_c) && (j==j_c)) // cursor
				{
				#ifdef CURSOR_BLINKED
				if (time(0)&1) setcolor(8);
				else setcolor(1);
				#endif
				putchar('*');
				#ifdef CURSOR_BLINKED
				setcolor(0);
				#endif
				}
			else
			 {
			x=j; y=i+MAX_Y-MAX_I;
			if ((x>=MAX_X)||(y>=MAX_Y)) putchar('~');
			else
				{
				setcolor(world[x][y].color);
				if (color_mode!=NO_COLOR) esc(world[x][y].bg); // background
				if (world[x][y].room_type==SOWED)
				putchar(plant_symbol(x,y));
				else putchar(world[x][y].symbol);
				setcolor(0);
				}
			 }
			}
		putchar('\r');
		putchar('\n');
		}

	if (online_help)
		{
		CLR;
printf("Help:\n");
setcolor(14);
printf("?");
setcolor(0);
printf(" help ");
setcolor(14);
printf("Q");
setcolor(0);
printf(" quit to OS ");
setcolor(14);
printf("q");
setcolor(0);
printf(" quit to app.\n");
setcolor(14);
printf("n s w e N S W E ");
setcolor(0);
printf("or ");
setcolor(14);
printf("arrows");
setcolor(0);
printf(" move\n");
setcolor(14);
printf("/");
setcolor(0);
printf(" till ");
setcolor(14);
printf(".");
setcolor(0);
printf(" sow ");
setcolor(14);
printf(",");
setcolor(0);
printf(" harvest ");
setcolor(14);
printf("backspace");
setcolor(0);
printf(" refresh screen");
		online_help=0;
		}
	usleep(100000);
	c=getchar();
	//if (c!=-1) {}
	//fflush(0);
	if (c=='Q')
		{
		ioctl(0, TCGETA, &tstdin);
    		tstdin.c_lflag |= (ICANON|ECHO);
    		ioctl(0, TCSETA, &tstdin);
		fcntl(STDIN_FILENO, F_SETFL, oldf);
		printf("\n\nexit from realtime\n");
		// показать курсор
		putchar(27);
		printf("[?25h");
		exit(0);
		}
	if (c=='q')
		{
		ioctl(0, TCGETA, &tstdin);
    		tstdin.c_lflag |= (ICANON|ECHO);
    		ioctl(0, TCSETA, &tstdin);
		fcntl(STDIN_FILENO, F_SETFL, oldf);
		printf("\n\nexit from realtime\n");
		// показать курсор
		putchar(27);
		printf("[?25h");
		global_x=save_x; global_y=save_y;
		return;
		}
	switch (c)
		{
		case '?': online_help=1; break;
		case '/': CLR; till(); break;
		case '.': CLR; sow(); break;
		case ',': CLR; harvest(); break;
		case 'B': clearscreen(); ls(); while(getchar()==-1) ; break; // boss key
		case 'N': i_c=MAX_I-1; CLR; print("Мы переместились на крайний север"); break;
		case 'S': i_c=0; CLR; print("Мы переместились на крайний юг"); break;
		case 'W': j_c=0; CLR; print("Мы переместились на крайний запад"); break;
		case 'E': j_c=MAX_J-1; CLR; print("Мы переместились на крайний восток"); break;
		case 'n': /* north */ goto l_n;
		case 's': /* south */ goto l_s;
		case 'w': /* west */  goto l_w;
		case 'e': /* east */  goto l_e;
		case 27 :	c=getchar();
				if (c==91)
					{
					c=getchar();
					if (c==66)	{l_s: // s
							if (i_c>0) i_c--;
							CLR; print("Мы переместились на юг"); 
							break;
							}
					else if (c==65)	{l_n: // n
							if (i_c<MAX_I-1) i_c++;
							CLR; print("Мы переместились на север"); 
							break;
							}
					else if (c==68)	{l_w: // west
							if (j_c>0) j_c--;
							CLR; print("Мы переместились на запад");
							break;
							}
					else if (c==67)	{l_e: // east
							if (j_c<MAX_J-1) j_c++;
							CLR; print("Мы переместились на восток"); 
							break;
							}
					}
				break;
		case 127: // backspace - clrscr
				clearscreen();
				break;
		case -1: break;
		default : CLR; printf("Invalid key = %i", c);
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

#define puts0(STR) printf("%s",STR)
#define putdec(DIGIT) printf("%i", DIGIT)
#define puthex(DIGIT) printf("%04X", DIGIT)
#define puthex_l(DIGIT) printf("%08X", DIGIT)
#define puthex_b(DIGIT) printf("%02X", DIGIT)
#define peek(ADR) 0
#define peek2(SEG,OFFSET) 0
#define putch(a) putchar(a)

#include "readw.c"

char *getsn(char *buf, int len)
{char *c;
fgets(buf, len, stdin);
c=strchr(buf,'\n');
if (c) *c=0;
}

int reads(int fd, char *buf, int count) // read string from file
{
int i, j; char c;

for (i=0;i<(count-1);i++)
	{
	j=read(fd,&c,1);
	if ((j==0)||(c=='\r')||(c=='\n')) {*buf=0; return 1;}
	*buf++=c;
	}
*buf=0;
return 0;
}

int htoi(const char  *s)
{
int i; char c;
i=0;
while (*s)
  {
  i<<=4;
  c=toupper(*s++);
  if ((c>='0')&&(c<='9')) i+=c-'0';
  else if ((c>='A')&&(c<='F')) i+=c+10-'A';
  else return 0;
  }
return i;
}

#include "proolskript.c"

// END OF FILE. КОНЕЦ ФАЙЛА
