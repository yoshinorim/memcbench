#define APPMAIN 1
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <time.h>
#include <limits.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <memcached.h>
#include <string.h>
#include <unistd.h>
#include "opt.h"

unsigned long long timediff(struct timeval tv0, struct timeval tv1)
{  // sec -> micro sec
  return (unsigned long long)llabs( (long long)tv1.tv_sec - (long long)tv0.tv_sec ) * (unsigned long long)1000000 +
         (unsigned long long)llabs( (long long)tv1.tv_usec - (long long)tv0.tv_usec );
}

struct Tls
{
  Tls(){}
  pthread_t ptid;
  static unsigned long trans_per_thread;
  struct memcached_st *memc;
  struct memcached_server_st *servers;

  void init() {
    return;
  }

  ~Tls(){
  }
};

unsigned long Tls::trans_per_thread = 0;

void* do_process(void*);
int do_select(Tls*);
int do_real_select(memcached_st *memc);

void* do_process(void *tls){
  int ret = 0;
  char host[1024];
  sprintf(host, "%s:%d", opt_hostname, opt_port);
  struct memcached_st *memc = memcached_create(NULL);
  struct memcached_server_st *servers= memcached_servers_parse(host);
  ((Tls*)tls)->memc= memc;
  ((Tls*)tls)->servers= servers;
  memcached_return rc = memcached_server_push(memc, servers);
  if(rc) {
    fprintf(stderr, "Error occured during memcached push=%d\n", rc);
    pthread_exit(0);
  }

  for(unsigned long l=0; l<Tls::trans_per_thread; l++){
    ret = do_select((Tls*)tls);
    if(ret){
      fprintf(stderr, "Error occured during process. ret=%d\n", ret);
    }
    if(opt_sleep_millis > 0){
      usleep(opt_sleep_millis);
    }
  }
  memcached_server_list_free(servers);
  memcached_free(memc);
  pthread_exit(0);  
}


int do_select(Tls* tls){
  int ret = 0;
  ret = do_real_select(tls->memc);
  return ret;
}


int do_real_select(memcached_st *memc){
  //random value between 1..opt_rows
  memcached_return rc;
  int rand_id = rand() % opt_rows + 1;
  char key[256];
  sprintf(key,"%s%d", opt_key_suffix, rand_id);
  int keylen=strlen(key);
  uint32_t flags;
  size_t reslen; 
  char *res = NULL;
  res = memcached_get(memc, key, keylen, &reslen, &flags, &rc);
  if(opt_debug)
    printf("key: %s, value: %s, rc: %d\n", key, res, rc);
  return 0;
}


int main(int argc, char **argv)
{
  struct timeval t_begin, t_initend,t_end;
  struct tm *tm_begin,*tm_initend,*tm_end;
  time_t tt_begin,tt_initend,tt_end;
  char s_begin[20],s_initend[20],s_end[20];
  unsigned long long init_time = 0;
  unsigned long long exec_time = 0;
  unsigned long long total_time = 0;

  parg(argc, argv);

  get_options(argc, argv, "hPSrnRkd");

  std::cout << "Row range: 0.." << opt_rows << std::endl;
  std::cout << "Total Requests:" << opt_requests << std::endl;
  std::cout << "Concurrent Threads:" << opt_nclient << std::endl;

  gettimeofday(&t_begin, 0);
  tt_begin = t_begin.tv_sec;
  tm_begin = localtime(&tt_begin);
  sprintf(s_begin,"%04d-%02d-%02d %02d:%02d:%02d", tm_begin->tm_year+1900, tm_begin->tm_mon+1, tm_begin->tm_mday, tm_begin->tm_hour, tm_begin->tm_min, tm_begin->tm_sec);

  srand((unsigned)time(NULL)); 

  Tls* tls = new Tls[opt_nclient];
  for (int i=0; i<opt_nclient; i++)
  {
     tls[i].init();
  }
  Tls::trans_per_thread = opt_requests / opt_nclient;

  gettimeofday(&t_initend, 0);
  tt_initend = t_initend.tv_sec;
  tm_initend = localtime(&tt_initend);
  sprintf(s_initend,"%04d-%02d-%02d %02d:%02d:%02d", tm_initend->tm_year+1900, tm_initend->tm_mon+1, tm_initend->tm_mday, tm_initend->tm_hour, tm_initend->tm_min, tm_initend->tm_sec);

  for (int i=0; i<opt_nclient; i++)
  {
    pthread_create(&(tls[i].ptid), NULL, do_process, (void*)&(tls[i]) );
  }

  for (int i=0; i<opt_nclient; i++)
  {
    pthread_join(tls[i].ptid, NULL);
  }

  gettimeofday(&t_end,0);
  tt_end = t_end.tv_sec;
  tm_end = localtime(&tt_end);
  sprintf(s_end,"%04d-%02d-%02d %02d:%02d:%02d", tm_end->tm_year+1900, tm_end->tm_mon+1, tm_end->tm_mday, tm_end->tm_hour, tm_end->tm_min, tm_end->tm_sec);

  init_time = timediff(t_begin,t_initend);
  exec_time = timediff(t_initend,t_end);
  total_time = timediff(t_begin,t_end);

  std::cout << std::endl;
  std::cout << "----Bench Report----" << std::endl;
  std::cout << "Total Transactions:" << opt_requests << std::endl;
  std::cout << "Concurrent Threads:" << opt_nclient << std::endl;
  std::cout << "Total Time:" << total_time/1000 << "ms" << std::endl;
  std::cout << "Connection Time:" << init_time/1000 << "ms" << std::endl;
  std::cout << "Execution Time:" << exec_time/1000 << "ms" << std::endl;

  delete[] tls;
  return 0;
}

