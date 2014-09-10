#include <stdio.h>
#include <string.h>
#include <iconv.h>

#define MAXLEN 255
#define MAXUTF 1024

#define UTF 0
#define KOI 1
#define WIN 2
#define LAT 3
#define ALFAVIT "В ЧАЩАХ ЮГА ЖИЛ БЫ ЦИТРУС ДА НО ФАЛЬШИВЫЙ ЭКЗЕМПЛЯР в чащах юга жил бы цитрус да но фальшивый экземпляр Ёё Ъъ\n"

#define MAX_X 100
#define MAX_Y 100

// static variables
int Codetable;
char *CodetableName[] = {"UTF","KOI","WIN","LAT"};
char out[MAXUTF];
struct
	{
	char *descr;
	int room_type;
	}
	world[MAX_X][MAX_Y];

int global_x, global_y;

// func. definitions
void outhex(char *);
void print (char *);

void help (void)
{
print("Command list:\n\
koi, win, utf, lat - switch codetable\n\
codetable - print current codetable\n\
alf - print alfavit\n\
ascii - print ascii table\n\
help, помощь - help\n\
test - test\n\
look - look\n\
directions: n, s, w, e\n\
q, quit, exit, конец - exit\n\
\n");
}

void init_world(void)
{
world[50][50].descr="Вы находитесь перед воротами Виртустана (ворота на севере)";
world[50][51].descr="Вы находитесь на пограничном посту Виртустана. Отсюда на север тянется Виртустан";
world[50][52].descr="Вы находитесь на Виртустанской улице, идущей в меридиональном направлении";
world[50][53].descr="Вы находитесь на Виртустанской площади";
world[50][54].descr="Вы находитесь на Виртустанском проспекте";
world[49][53].descr="Вы находитесь во Дворце Короля Виртустана. Вокруг лепота";
world[49][52].descr="Вы находитесь в Виртустанской Библиотеке. ПОТОМУ ЧТО ТИШИНА ДОЛЖНА БЫТЬ В БИБЛИОТЕКЕ!!";
world[51][52].descr="Вы находитесь в Виртустанской Гостинице";
world[51][53].descr="Вы находитесь в Виртустанском Банке. Слышен звон пересчитываемых монет, шелест купюр и звяканье кассового аппарата";
world[50][48].descr="Вы находитесь в неглубокой яме. На дне лежат чьи-то кости";
}

void look(void)
{
#define FOREST "Вы находитесь в светлом сосновом лесу, который охватывает Виртустан со всех сторон"
printf("(%i,%i)\n",global_x,global_y);
if (world[global_x][global_y].descr) print(world[global_x][global_y].descr);
else print(FOREST);
printf("\n");
}

void move(int dx, int dy)
{
global_x+=dx;
global_y+=dy;
print("Вы переместились ");
if (dx==1) print("на восток");
else if (dx==-1) print("на запад");
else if (dy==1) print("на север");
else if (dy==-1) print("на юг");
else print("в неизвестном направлении");
printf("\n");
look();
}

void test (void)
{
printf("\
-----------------------------------------------------------------------------\n\
[0;40;37;1m\
-----------------------------------------------------------------------------\n\
[0m\n\
");
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
	case UTF: printf(str); break;
	case KOI: printf(utf2koi(str)); break;
	case WIN: printf(utf2win(str)); break;
	case LAT: printf(utf2lat(str)); break;
	default: ;
	}
}

void ascii (void)
{int i;
for (i=32; i<256; i++)
	{
	printf ("%i=`%c' ", i, i);
	}
printf("\n");
}

int main(void)
{
char cmd[MAXLEN];
char *cc;
int i, j;

printf("\nVirtustan 0.2\n\n");
printf("Current codetable is %s. ",CodetableName[Codetable]);
print("Ktulhu ФХТАГН!!\n\n");
printf("Use `help' command for help and `quit' for quit.\n\n");


printf("init stated\n");
Codetable=UTF;

for (i=0; i<MAX_X; i++) for (j=0; j<MAX_Y; j++) {world[i][j].descr=0; world[i][j].room_type=0;}

global_x=50; global_y=50;

init_world();

printf("init ended\n\n");

look();

while(1)
	{
	printf("> ");
	fgets(cmd,MAXLEN,stdin);
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
	else if (!strcmp(cmd,"помощь")) help();
	else if (!strcmp(cmd,"test")) test();
	else if (!strcmp(cmd,"alf")) print(ALFAVIT);
	else if (!strcmp(cmd,"ascii")) ascii();
	else if (!strcmp(cmd,"koi")) {Codetable=KOI; printf("Codetable switch to KOI\n");}
	else if (!strcmp(cmd,"utf")) {Codetable=UTF; printf("Codetable switch to UTF\n");}
	else if (!strcmp(cmd,"win")) {Codetable=WIN; printf("Codetable switch to WIN\n");}
	else if (!strcmp(cmd,"lat")) {Codetable=LAT; printf("Codetable switch to LAT: yet not implemented!\n");}
	else if (!strcmp(cmd,"codetable")) printf("Current codetable is %s\n",CodetableName[Codetable]);
	else if (!strcmp(cmd,"look")) look();
	else if (!strcmp(cmd,"n")) move(0,+1);
	else if (!strcmp(cmd,"s")) move(0,-1);
	else if (!strcmp(cmd,"w")) move(-1,0);
	else if (!strcmp(cmd,"e")) move(+1,0);
	else printf("Unknown command `%s'\n", cmd);
	}
return 0;
}
