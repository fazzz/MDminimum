
#define _GNU_SOURCE  

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#include "BoxMuller.h"

double Uniform( void ) {
  return ((double)rand()+1.0)/((double)RAND_MAX+2.0);
}

double Box_Muller(int i,double mean,double vari) {
  double u1,u2,pi;
  double c1,c2;
  //  FILE *logfile;

  u1=Uniform();
  u2=Uniform();
  pi=acos(-1);

  /**************************************************************************/
  /* logfile=efopen("log.txt","a");					    */
  /* c1=sqrt(2.0)*cos(2.0*pi*u2);					    */
  /* c1=log(2.0);							    */
  /* c1=sqrt(-2.0*log(u1))*cos(2.0*pi*u2);				    */
  /* c2=sqrt(-2.0*log(u1))*sin(2.0*pi*u2);				    */
  /* fprintf(logfile,"%d %12.8lf %12.8lf %12.8lf %12.8lf\n",i,u1,u2,c1,c2); */
  /* fclose(logfile);							    */
  /**************************************************************************/

  if (i%2)
    return mean+sqrt(-2.0*log(u1))*cos(2.0*pi*u2)*sqrt(vari);
  else
    return mean+sqrt(-2.0*log(u1))*sin(2.0*pi*u2)*sqrt(vari);
}

