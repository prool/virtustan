/* Prool foolish translator (interpreter) */
#include <string.h>
#include <stdio.h>

#define MAXBUF 4096

int tron;

char *slovarb[]=
{
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
")", ")" // END
};

char buffer [MAXBUF];
char clipboard [MAXBUF];

char *prooltran(char *si)
{char *pp, *p0;
int ii;

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
