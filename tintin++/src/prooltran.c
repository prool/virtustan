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
#define MAXWORD 200
#define MAXWORDLEN 30

int tron;
int total_log;
long int start_time;

char English [MAXWORD] [MAXWORDLEN];
char Russian [MAXWORD] [MAXWORDLEN];

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
if (English[ii][0]==0) break;
	while(1) // многократная замена одного слова
	{
	pp=strstr(buffer,English[ii]);
	if (pp==0) break;
	if (pp!=p0) {memcpy(clipboard,buffer,(pp-p0)); clipboard[pp-p0]=0;}
	else clipboard[0]=0;
	//printf("clipboard=%s\n",clipboard);
	strcat(clipboard,Russian[ii]);
	//printf("clipboard=%s\n",clipboard);
	strcat(clipboard,buffer+(pp-p0)+strlen(English[ii]));
	//printf("clipboard=%s\n",clipboard);
	strcpy(buffer,clipboard);
	}
ii+=1;
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
{FILE *fp;
char buf [MAXBUF];
char buf2 [MAXBUF];
char *cc;
int i,j;

for (i=0;i<MAXWORD;i++)
	{
	English[0][0] = 0;
	Russian[0][0] = 0;
	}

fp=fopen("slovarb.csv","r");
if (fp==NULL) {printf("Can't open Slovarb\n"); return;}
j=0;
while(!feof(fp))
	{
	buf[0]=0;
	fgets(buf,MAXBUF,fp);
	cc=strchr(buf,'\n');
	if (cc) *cc=0;
	if (buf[0])
		{
		printf("'%s' ", buf);
		cc=strchr(buf,',');
		if (cc==0) continue;
		strcpy(buf2,cc+1);
		*cc=0;
		printf("1 '%s' 2 '%s'\n", buf, buf2);
		if ((strlen(buf)>=MAXWORDLEN) || (strlen(buf2)>=MAXWORDLEN))
			{
			printf("Word length overflow\n");
			prool_log("Word length overflow");
			}
		strncpy(English[j],buf,MAXWORDLEN);
		strncpy(Russian[j],buf2,MAXWORDLEN);
		if (++j>=MAXWORD)
			{
			printf("Word overflow!\n");
			prool_log("Word overflow!");
			break;
			}
		}
	}

for (i=0;i<MAXWORD;i++)
	{
	if (English[i][0]==0) break;
	printf("%i) %s %s ", i, English[i], Russian[i]);
	}

fclose(fp);
}
