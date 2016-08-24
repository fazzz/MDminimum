#include <stdio.h>
#include <stdlib.h>

#include "efunc.h"

FILE *efopen(char *filename,char *flag){
  FILE *file;

  if((file=fopen(filename,flag))==NULL) {
    printf("open error about %s\n",filename);
    exit(1);
  }
  return file;
}

void *emalloc(size_t n){
  void *p;

  if ((p=malloc(n))==NULL) {
    printf("open about allocation\n");
    exit(1);
  }

  return p;
}




