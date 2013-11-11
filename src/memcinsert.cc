#define APPMAIN 1
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <limits.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <memcached.h>
#include <string.h>
#include "opt.h"


int main(int argc, char** argv) {
  struct memcached_st *memc;
  struct memcached_server_st *servers;
  memcached_return rc;
  int num_rows;
  if(argc >= 3) {
    num_rows= atoi(argv[2]);
    printf("Using host:port %s %d\n", argv[1], num_rows);
  }else {
    printf("Set host:port num_rows\n");
    return 1;
  }

  memc = memcached_create(NULL);
  servers = memcached_servers_parse(argv[1]);
  rc = memcached_server_push(memc, servers);

  for(int i=0; i<num_rows ; i++){
    char key[20];
    char value[20];
    sprintf(key,"%d",i);
    int keylen=strlen(key);
    sprintf(value,"%d",i);
    int valuelen= strlen(value);
    rc = memcached_set(memc, key, keylen, value, valuelen, (time_t)0, (uint16_t)0);
  }

  return 0;
}

