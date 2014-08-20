#include <stdio.h>
#include <string.h>

#define MAXLEN 255

void help (void)
{
printf("Command list:\n\
help, помощь - help\n\
quit, exit, конец - exit\n\
\n");
}

int main(void)
{
char cmd[MAXLEN];
char *cc;

printf("\nVirtustan 0.2\n\n");
printf("Use `help' command for help and `quit' for quit.\n\n");

while(1)
	{
	printf("> ");
	fgets(cmd,MAXLEN,stdin);
	// printf("cmd=`%s'\n", cmd);
	cc=strchr(cmd,'\n');
	if (cc) *cc=0;
	if (!strcmp(cmd,"quit")) break;
	if (!strcmp(cmd,"exit")) break;
	if (!strcmp(cmd,"конец")) break;
	if (!strcmp(cmd,"help")) help();
	else if (!strcmp(cmd,"помощь")) help();
	else printf("Invalid command `%s'\n", cmd);
	}
return 0;
}
