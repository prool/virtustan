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

// static variables
int Codetable;
char *CodetableName[] = {"UTF","KOI","WIN","LAT"};
char out[MAXUTF];

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
q, quit, exit, конец - exit\n\
\n");
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

printf("\nVirtustan 0.2\n\n");
printf("Current codetable is %s. ",CodetableName[Codetable]);
print("Ktulhu ФХТАГН!!\n\n");
printf("Use `help' command for help and `quit' for quit.\n\n");

Codetable=UTF;

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
	else printf("Unknown command `%s'\n", cmd);
	}
return 0;
}
