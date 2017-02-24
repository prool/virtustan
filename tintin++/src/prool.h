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

extern int total_log;
extern int tron;
extern int total_log;
extern long int start_time;
