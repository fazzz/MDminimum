#define _GNU_SOURCE  

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <netcdf.h>
#include <getopt.h>

#include "const.h"
#include "integrater.h"
#include "readParameters.h"
#include "force.h"
#include "BoxMuller.h"
#include "efunc.h"
#define NVT 1
#define NVE 0

#define ON 1
#define OFF 0

void USAGE(char *progname);

int main(int argc, char *argv[]) {
  int i,j,k,l=0,d;
  int numatom,numheavyatom,numres,numstep=10000,interval=100;
  double dt=0.001,dt2,wdt2[3],wdt4[3];
  double PE;
  double pi;

  int vMode=OFF,MODE=NVT,NCmode=3,nibnum=1;

  int nc=1;
  double T0=300,T,K0,KE;
  double NfKT,KT;
  double zeta=0.0,V_zeta=0.0,Q,tau=0.01,tau2;
  double PEv,KEv;

  int nump;

  double avePE=0.0,varPE=0.0,aveKE=0.0,varKE=0.0,aveT=0.0,varT=0.0;

  double *crd,*mass,*vel;

  double summass,COM[3];

  struct AmberParmL AP;
  struct potential e;
  struct force f;
  double p_t=0.0,Etot;

  double x[3];

  char *line;
  size_t len=0;

  int c;
  extern char *optarg;
  extern int optind,opterr,optopt;

  char *inputfilename,*velfilename,*parmfilename;
  char *trjfilename,*outputfilename,*rstfilename="rstcrd",*rstvelfilename="rstvel";
  char *logfilename="mdlog";

  FILE *inputfile,*velfile,*parmfile;
  FILE *outputfile,*trjfile,*rstfile,*rstvelfile;
  FILE *logfile;

  char *progname;

  int opt_idx=1;

  pi=acos(-1.0);

  struct option long_opt[] = {
    {"nve",0,NULL,'*'},
    {"log",1,NULL,'l'},
    {"vMode",1,NULL,'v'},
    {"nums",1,NULL,'s'},
    {"temp",1,NULL,'t'},
    {"tau",1,NULL,'a'},
    {"int",1,NULL,'i'},
    {"rst",1,NULL,'{'},
    {"rstvel",1,NULL,'}'},
    {"dt",1,NULL,'x'},
    {"h",0,NULL,'h'},
    {0,0,0,0}
  };

  while((c=getopt_long(argc,argv,"*hl:s:v:t:a:i:x:N:{:}:",long_opt,&opt_idx))!=-1) {
    switch(c) {
    case '*':
      MODE=NVE;
      break;
    case 'l':
      logfilename=optarg;
      break;
    case 's':
      numstep=atoi(optarg);
      break;
    case 'v':
      vMode=ON;
      velfilename=optarg;
      break;
    case 't':
      T0=atof(optarg);
      break;
    case 'a':
      tau=atof(optarg);
      break;
    case 'i':
      interval=atoi(optarg);
      break;
    case '{':
      rstfilename=optarg;
      break;
    case '}':
      rstvelfilename=optarg;
      break;
    case 'x':
      dt=atof(optarg);
      break;
    case 'h':
      USAGE(progname);
      exit(1);
    default:
      USAGE(progname);
      exit(1);
    }
  }

  progname=*argv;

  argc-=optind;
  argv+=optind;

  if (argc < 4) {
    USAGE(progname);
    exit(1);
  }
  inputfilename     = *argv;
  parmfilename      = *++argv;
  outputfilename    = *++argv;
  trjfilename       = *++argv;

  parmfile=efopen(parmfilename,"r");
  readParmtopL(parmfile,&AP);
  fclose(parmfile); 
  numatom=AP.NATOM;
  j=0;
  for (i=0;i<numatom;++i) if (strncmp(AP.IGRAPH[i],"H",1)==0)  ++j;
  numheavyatom=numatom-j;
  numres=AP.NRES;
  mass=(double *)emalloc(sizeof(double)*numatom);
  for (i=0;i<numatom;++i) mass[i]=AP.AMASS[i];
  
  crd=(double *)emalloc(sizeof(double)*numatom*3);
  vel=(double *)emalloc(sizeof(double)*numatom*3);

  inputfile=efopen(inputfilename,"r");
  getline(&line,&len,inputfile);
  fscanf(inputfile,"%d",&d);
  for (i=0;i<numatom;++i) for (j=0;j<3;++j) fscanf(inputfile,"%lf",&crd[i*3+j]);
  fclose(inputfile);

  if ( vMode==OFF ) {
    MD_Generate_inivelo(vel,mass,numatom,k_B*T0*UNIT);

    zeta=0.0;
    V_zeta=0.0;
  }
  else {
    velfile=efopen(velfilename,"r");
    for (i=0;i<numatom;++i) for (j=0;j<3;++j) fscanf(velfile,"%lf",&vel[i*3+j]);
    fclose(velfile);
  }
  K0=0.0;
  for (i=0;i<numatom;++i) 
    for (j=0;j<3;++j) 
      K0+=0.5*mass[i]*vel[i*3+j]*vel[i*3+j];
  T=K0/((3*numatom)*k_B)*2.0/UNIT;

  tau=tau/2.0/pi;         
  tau2=tau*tau;          
  KT=k_B*T0;
  NfKT=(3.0*numatom+1)*KT*UNIT;
  Q=tau2*KT*UNIT*(3.0*numatom);

  summass=0.0;
  for (i=0;i<numatom;++i) summass+=mass[i];
  for (i=0;i<3;++i) COM[i]=0.0;
  for (i=0;i<numatom;++i) 
    for (j=0;j<3;++j) 
      COM[j]+=mass[i]*crd[i*3+j]/summass;
  for (i=0;i<numatom;++i) 
    for (j=0;j<3;++j) 
      crd[i*3+j]-=COM[j];

  ffL_set_calcffandforce(&e,&f,AP);
  ffL_calcffandforce(crd,numatom,&e,&f,AP);
  
  MD_Propagetor_NH_Single_set_MP1996(nc,dt,&dt2,wdt2,wdt4);
  trjfile=efopen(trjfilename,"w");
  outputfile=efopen(outputfilename,"w");
  for (i=0;i<numstep;++i) {
    if (MODE==NVT) {
      KE=MD_Propagetor_NH_MP1998_AAFF_Amber(crd,vel,mass,&zeta,&V_zeta,Q,NfKT,numatom,&KEv,&PEv,dt,dt2,nc,wdt4,wdt2,&e,&f,AP);
    }
    else {
      KE=MD_Propagetor_vV_NVE_AAFF_Amber(crd,vel,mass,numatom,dt,&e,&f,AP);
    }
      
    if (i%interval==0) {
      KE=KE/UNIT;
      T=KE/((3*numatom)*k_B)*2.0;

      PEv=PEv/UNIT;
      KEv=KEv/UNIT;

      p_t=0.0;
      /* if (esflag==ON)    */p_t+=0.5*e.p_e_t;
      /* if (LJflag==ON)    */p_t+=0.5*e.p_LJ_t;
      /* if (es14flag==ON)	 */p_t+=0.5*e.p_e_14_t;
      /* if (LJ14flag==ON)	 */p_t+=0.5*e.p_LJ_14_t;
      /* if (dihflag==ON)	 */p_t+=e.p_d_t;
      /* if (angflag==ON)	 */p_t+=e.p_a_t;
      /* if (bodflag==ON)	 */p_t+=e.p_b_t;

      Etot=p_t+KE+KEv+PEv;
      fprintf(outputfile,"E_t    = %e \n",Etot);
      fprintf(outputfile,"KE     = %e \n",KE);
      fprintf(outputfile,"KEv    = %e \n",KEv);
      fprintf(outputfile,"PEv    = %e \n",PEv);
      fprintf(outputfile,"p_tot  = %e \n",p_t);
      fprintf(outputfile,"T      = %e \n",T);

      for (j=0;j<3;++j) COM[j]=0.0;
      for (j=0;j<numatom;++j) 
	for (k=0;k<3;++k) 
	  COM[k]+=mass[j]*crd[j*3+k]/summass;
      for (j=0;j<numatom;++j) 
	for (k=0;k<3;++k) 
	  crd[j*3+k]-=COM[k];

      avePE=(i*avePE+e.p_t)/(i+1);
      varPE=(i*varPE+e.p_t*e.p_t)/(i+1);

      aveKE=(i*aveKE+KE)/(i+1);
      varKE=(i*varKE+KE*KE)/(i+1);

      aveT=(i*aveT+T)/(i+1);
      varT=(i*varT+T*T)/(i+1);

      l=1;
      for (j=0;j<numatom;++j) {
	for (k=0;k<3;++k) {
	  ++l;
	  fprintf(trjfile,"-%5.3lf  ",crd[j*3+k]);
	  if (l%10==0) fprintf(trjfile,"\n");
	}
      }
    }
  }
  fclose(outputfile);
  fclose(trjfile);

  //  free(e);
  
  //  free(f);

  free(mass);
  free(crd);
  free(vel);

  //  free(AP);

  rstfile=efopen(rstfilename,"w");
  fprintf(rstfile,"PRO\n");
  fprintf(rstfile,"%d\n",numatom);
  for (i=0;i<numatom;++i) {
    for (k=0;k<3;++k) fprintf(rstfile,"%e ",crd[i*3+k]);
    fprintf(rstfile,"\n");
  }
  fclose(rstfile);

  rstvelfile=efopen(rstvelfilename,"w");
  for (i=0;i<numatom;++i) {
    for (j=0;j<3;++j) fprintf(rstvelfile,"%e ",vel[i*3+j]);
    if (MODE==NVT) fprintf(rstvelfile,"%lf %lf",zeta,V_zeta);
    fprintf(rstvelfile,"\n");
  }
  fclose(rstvelfile);

  logfile=efopen(logfilename,"w");
  varPE=sqrt(varPE);
  fprintf(logfile,"Potential energy = %10.5lf kcal/mol +- %10.5lf kcal/mol\n",avePE,varPE);
  varKE=sqrt(varKE);
  fprintf(logfile,"Kinetic   energy = %10.5lf kcal/mol +- %10.5lf kcal/mol\n",aveKE,varKE);
  varT=sqrt(varT);
  fprintf(logfile,"Temperature      = %10.5lf K        +- %10.5lf K\n",aveT,varT);
  fclose(logfile);

  return 0;
}

void USAGE(char *progname) {
  printf("USAGE:\n");
  printf("[--nve] ");
  printf("[--log <logfilename>] ");
  printf("[--vMode] ");
  printf("[--nums <numsteps>]\n");
  printf("[--temp <temperature>] ");
  printf("[--tau <tau>] ");
  printf("[--int <output freaquency>] \n");
  printf("[--rst <restart file name>] ");
  printf("[--rstvel <restart file name velocity>]\n");
  printf("[--dt <time step>] ");
  printf("[-h] help \n");
  printf("%s inputfilename parmfilename outputfilename trjfilename\n",progname);
}
