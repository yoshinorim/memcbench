#include <getopt.h>
#include "opt.h"

struct option long_options[] =
{
    {"sleep" , required_argument, 0, 'S'},
    {"host",  required_argument, 0, 'h'},
    {"port",   required_argument, 0, 'P'},
    {"requests",   required_argument, 0, 'r'},
    {"nclient",  required_argument, 0, 'n'},
    {"rows",  required_argument, 0, 'R'},
    {"key_suffix", required_argument, 0, 'k'},
    {"debug", required_argument, 0, 'd'},
    {0,0,0,0}
};

void usage(char *shortopt)
{
  if (strchr(shortopt, 'n')) {
    printf("  --nclient= ,      -n  : NumberOfThreads\n");
  }
  if(strchr(shortopt,'h')) {
    printf("  --host= ,      -h :Hostname\n");
  }
  if(strchr(shortopt,'P')) {
    printf("  --port= ,      -P :Port\n");
  }
  if (strchr(shortopt, 'S')){
    printf("  --sleep= ,       -S : Sleep Millisecond\n");
  }
  if (strchr(shortopt, 'r')) {
    printf("  --requests= ,       -r  : Transactions\n");
  }
  if (strchr(shortopt, 'k')) {
    printf("  --key_suffix= ,       -k  : Key Suffix\n");
  }
  if (strchr(shortopt, 'd')) {
    printf("  --debug= ,       -d  : Debug\n");
  }
}

void get_options(int glob_argc, char **glob_argv, char *shortopt)
{
  int c, opt_ind = 0;
  while((c = getopt_long(glob_argc, glob_argv, "h:P:S:r:n:R:k:d:", long_options, &opt_ind)) != EOF)
  {
    switch(c)
    {
      case 'h': opt_hostname = optarg; break;
      case 'P': opt_port = atoi(optarg); break;
      case 'S': opt_sleep_millis = atoi(optarg)*1000; break;
      case 'r': opt_requests = atoi(optarg);
                if (opt_requests < 1) { opt_requests = 1; }
                break;
      case 'n': opt_nclient = atoi(optarg);
                if (opt_nclient < 1) { opt_nclient = 1; }
                break;
      case 'R': opt_rows= atoi(optarg); break;
      case 'k': opt_key_suffix = optarg; break;
      case 'd': opt_debug = atoi(optarg);
                break;
    }
  }

}

/*==========================================
 */
void parg(int argc, char **argv)
{
  int i;
  printf("\n==== %s ", argv[0]);
  for (i=1; i<argc; i++) {
    printf(" %s", argv[i]);
  }
  printf(" ====\n");
}

