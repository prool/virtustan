#include "proollib.h"

char *ptime(void)
	{
	char *tmstr;
	time_t mytime;

	mytime = time(0);

	tmstr = asctime(localtime(&mytime));
	*(tmstr + strlen(tmstr) - 1) = '\0';

	return tmstr;
	}
