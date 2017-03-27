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
long int startwatchtime;
int prool_loop_counter;
long int watchdog;

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

for(ii=0;ii<MAXWORD;ii++)
{// цикл по всем словам
if (English[ii][0]==0) continue;
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

fp=fopen("/tmp/prooltin.log","a");
if (fp==NULL) {printf("prooltin: can't open log\n"); total_log=0; return;}
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
			break;
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

DO_COMMAND(do_prool) // prool
{
prool_ident();

printf("\nCompile date %s %s\nCurrent date %s\n\nprool's remarks:\n\
Command for MSSP:\n#config {debug telnet} on\n\
\n\
Prool command\n\
#prool - prool help\n\
#tron - enable foolish translator\n\
#troff - disable foolish translator\n\
#totalon - enable total logging\n\
#totaloff - disable total logging\n\
#listdic - list of dictionary\n\
#writedic - write dictionary to file\n\
#addword english,russian - add word pair to dic\n\
#delword english - del word from dic\n\
#proolwatchdogtimer [n] - set watchdog to n seconds\n\
\n\
",__DATE__,__TIME__,ptime());

printf("Translator = %i\n", tron);
printf("Total log = %i\n", total_log);
printf("PID = %i\n", getpid());
printf("prool loop counter = %i\n", prool_loop_counter);
printf("watchdog = %li\n", watchdog);

printf("arg='%s'\n", arg);

return ses;
}

DO_COMMAND(do_totalon)
{
total_log=1;
prool_log("Total log enabled");
printf("Total log enabled\n");
return ses;
}

DO_COMMAND(do_totaloff)
{
prool_log("Total log disabled");
printf("Total log disabled\n");
total_log=0;
return ses;
}

DO_COMMAND(do_tron)
{
tron=1;
printf("Translator enabled\n");
return ses;
}

DO_COMMAND(do_troff)
{
tron=0;
printf("Translator disabled\n");
return ses;
}

DO_COMMAND(do_addword)
{
char buf[MAXBUF];
char buf2[MAXBUF];
char *cc;
int i;

//printf("arg=%s\n", arg);

strncpy(buf,arg,MAXBUF);

cc=strchr(buf,',');
if (cc==0) {printf("ERROR: No comma\n"); return ses;}
strcpy(buf2,cc+1);
*cc=0;
printf("addword 1 '%s' 2 '%s'\n", buf, buf2);

if ((strlen(buf)>=MAXWORDLEN) || (strlen(buf2)>=MAXWORDLEN))
			{
			printf("addword: Word length overflow\n");
			prool_log("addword: Word length overflow");
			return ses;
			}

for(i=0;i<MAXWORD;i++)
	{
	if (English[i][0]==0)
		{
		strcpy(English[i],buf);
		strcpy(Russian[i],buf2);
		return ses;
		}
	}

printf("ERROR: addword: word overflow\n");
prool_log("ERROR: addword: word overflow");

return ses;
}

DO_COMMAND(do_delword)
{int i;
	if (*arg==0) {
		printf("usage: #delword word\n");
		return ses;
	}

	for (i=0;i<MAXWORD;i++)
	{
		if (!strcmp(English[i],arg))
		{
			English[i][0]=0;
			printf("Found and delete\n");
			return ses;
		}
	}
printf("Word '%s' not found\n", arg);
return ses;
}

DO_COMMAND(do_listdic)
{
	int i,count;

	count=0;
	for (i=0;i<MAXWORD;i++) {
		if (English[i][0]) {printf("%s,%s\n",English[i],Russian[i]); count++;}
	}

	printf("Total words %i\n", count);

	return ses;
}

DO_COMMAND(do_writedic)
{
	int i,count;
	FILE *fp;

	count=0;
	fp=fopen("slovarb2.csv","w");
	if (fp==NULL) {printf("writedic: can't open file\n"); return ses;}
	for (i=0;i<MAXWORD;i++) {
		if (English[i][0]) {fprintf(fp,"%s,%s\n",English[i],Russian[i]); count++;}
	}
	fclose(fp);

	printf("Total words %i\n", count);

	return ses;
}

DO_COMMAND(do_proolwatchdogtimer)
{
watchdog=atoi(arg);

printf("watchdog set to %li sec\n", watchdog);
prool_log("proolwatchdog set");

if (watchdog) startwatchtime=time(0);

return ses;
}

int prool_loop(void)
{
long int i;
prool_loop_counter++;
if (watchdog)
	{
	i=time(0);
	if ((i-startwatchtime)>watchdog)
		{
		printf("proolwatchdog bzzzzz!!!\n");
		prool_log("proolwatchdog bzzzzz!!!");
		watchdog=0;
		printf("tintin++ quit\n");
		prool_log("tintin++ quit");
		do_zap(0,0);
		printf("zapped\n");
		}
	}
return 0;
}
