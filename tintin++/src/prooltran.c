/* Prool foolish translator (English to Russian interpreter) and others Prool's functions

   Prool here: http://prool.kharkov.org http://mud.kharkov.org https://github.com/prool/virtustan

   E-mail proolix BIGLAZYDOG gmail . com

*/
#include <string.h>
#include <stdio.h>
#include <time.h>

#include "tintin.h"
#include "prool.h"

#define MAXBUF 4096

int tron;
int total_log;
long int start_time;

char *slovarb[]=
{
"You see exits leading", "Выходы",
"password", "пароль",
"shortsword", "короткий меч",
"sword", "меч",
"bow", "лук",
"vial", "фиал",
"oaken", "дубовый",
"tunic", "роба",
"road", "дорога",
"leggings", "панталоны",
"backpack", "рюкзак",
"canvas", "холст",
"brown", "коричневый",
"hatchet", "топорик",
")", ")" // END
};

char buffer [MAXBUF];
char clipboard [MAXBUF];

void prool_ident(void)
{
printf("Mod by Prool. 2014-2017. https://github.com/prool/virtustan http://mud.kharkov.org proolix@gmail.com\n");
}

char *prooltran(char *si)
{char *pp, *p0;
int ii;

if (total_log) prool_log(si);

if (tron==0) return si;

p0=buffer;
strcpy(buffer,si);

ii=0;
while(1)
{// цикл по всем словам
if (slovarb[ii][0]==')') break;
	while(1) // многократная замена одного слова
	{
	pp=strstr(buffer,slovarb[ii]);
	if (pp==0) break;
	if (pp!=p0) {memcpy(clipboard,buffer,(pp-p0)); clipboard[pp-p0]=0;}
	else clipboard[0]=0;
	//printf("clipboard=%s\n",clipboard);
	strcat(clipboard,slovarb[ii+1]);
	//printf("clipboard=%s\n",clipboard);
	strcat(clipboard,buffer+(pp-p0)+strlen(slovarb[ii]));
	//printf("clipboard=%s\n",clipboard);
	strcpy(buffer,clipboard);
	}
ii+=2;
}

/*
if (pp)
	{
	if (pp!=p0) {memcpy(buffer,si,(pp-p0)); buffer[pp-p0]=0;}
	else buffer[0]=0;
	strcat(buffer,"Пруль");
	strcat(buffer,si+(pp-p0)+strlen("prool"));
	}
else	strcpy(buffer,si);
*/

//printf("buffer=%s\n", buffer);
return buffer;
}

char *ptime(void) // Возвращаемое значение: ссылка на текстовую строку с текущим временем
	{
	char *tmstr;
	time_t mytime;

	mytime = time(0);

	tmstr = (char *) asctime(localtime(&mytime));
	*(tmstr + strlen(tmstr) - 1) = '\0';

	return tmstr+4;

	}

void prool_log(char *message)
{
FILE *fp;

fp=fopen("prooltin.log","a");
if (fp==NULL) printf("prooltin: can't open log\n");
fprintf(fp,"%i %s %s\r\n",getpid(),ptime(),message);
fclose(fp);
}

void uptime_(void)
{long uptime; char *tmstr;
uptime=time(0)-start_time;
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

void prooltranslate_init(void)
{
}
