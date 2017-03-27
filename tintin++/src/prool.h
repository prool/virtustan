DO_COMMAND(do_prool);
DO_COMMAND(do_proolwatchdogtimer);
DO_COMMAND(do_totalon);
DO_COMMAND(do_totaloff);
DO_COMMAND(do_addword);
DO_COMMAND(do_delword);
DO_COMMAND(do_listdic);
DO_COMMAND(do_writedic);

char *prooltran(char *si);
void prool_log(char *message);
void prool_ident(void);
char *ptime(void);
void uptime_(void);
void prooltranslate_init(void);
int prool_loop(void);

extern int total_log;
extern int tron;
extern int total_log;
extern long int start_time;
extern long int startwatchtime;
extern int prool_loop_counter;
extern long int watchdog;
