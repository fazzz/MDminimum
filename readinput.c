#define _GNU_SOURCE  

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* Options
    mode MODE
    vmode VMODE
    nums NUMS
    temp TEMP
    tau TAU
    int INT
    dt DT
*/

void readinput(FILE *inpfile) {
  int i,j,k;
  int n;
  int c;

  int status;

  double f;

  nl=0;
  ncmax=1;
  status=SPACE;
  d=0.0;
  f=0.0;
  nf=0;
  n=0;
  while ( (c=getc(inpfile))!=-1 ) {
    i=0;
    nc=0;
    if(isspace(c) && c!='\n') {
      if (status==INTEGER || status==DECIMAL ) {
	++n;
	v=sign*(f+d);
	d=0.0;
	f=0.0;
	nf=0;
	status=WRITE;
      }
      else {
	status=SPACE;
      }
    }
    else if (isdigit(c)) {
      if (status==INTEGER)
	d=d*10+c-'0';
      else if (status==DECIMAL) {	
	++nf;
	f=f+(c-'0')*pow(0.1,nf);
      }
      else if (status==SPACE || status==WRITE) {
	status=INTEGER;
	d=c-'0';
      }	
    }
    else if (isalpha(c)) {

    }
    else if (c=='.') {
      status=DECIMAL;
    }
    else if (c=='+') {
      if (status==INTEGER || status==DECIMAL ) {
	++n;
	v=sign*(f+d);
	d=0.0;
	f=0.0;
	nf=0.0;
	status=WRITE;
      }
      else {
	status=INTEGER;
      }
      sign=1.0;
    }
    else if (c=='-') {
      if (status==INTEGER || status==DECIMAL ) {
	++n;
	v=sign*(f+d);
	d=0.0;
	f=0.0;
	nf=0.0;
	status=WRITE;
      }
      else {
	status=INTEGER;
      }
      sign=-1.0;
    }
    else if (c==',') {

    }
    else if (c=='\n') {
      if (n>ncmax) ncmax=n;
      n=0;
      ++nl;
      status=SPACE;
    }
    if (status==WRITE) {
      if (nl==0) {
	ave=(double *)gcerealloc(ave,sizeof(double)*n);
	if ( vflag == ON )
	  var=(double *)gcerealloc(var,sizeof(double)*n);
      }
      ave[n-1]=(nl*ave[n-1]+v)/(nl+1);
      if ( vflag == ON )
	var[n-1]=(nl*var[n-1]+v*v)/(nl+1);
    }
  }
}
