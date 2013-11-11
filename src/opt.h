#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/time.h>
#include <time.h>
#include <sys/times.h>


#define MAXRECORD 100000
#define MAXCLIENT 1500


#ifndef __OPT_H_
#define __OPT_H_

#ifdef APPMAIN
unsigned long opt_requests = 0;
int opt_nclient = 1;
char *opt_hostname = "127.0.0.1";
int opt_port = 11211;
int opt_sleep_millis = 0;
int opt_rows= 100;
char *opt_key_suffix = "";
int opt_debug= 0;
#else
extern unsigned long opt_requests ;
extern int opt_nclient;
extern char* opt_hostname;
extern int opt_port;
extern int opt_sleep_millis;
extern int opt_rows;
extern char *opt_key_suffix;
extern int opt_debug;
#endif

void parg(int argc, char **argv);
void usage(char *shortopt);
void get_options(int glob_argc, char **glob_argv, char *shortopt);

#endif /* __OPT_H_ */


