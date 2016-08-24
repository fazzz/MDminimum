#ifndef INCLUDE_PTL
#define INCLUDE_PTL

struct AmberParmL{
  char ITITLE[4];

  int NATOM;
  int NTYPES;
  int NBONH;
  int MBONA;
  int NTHETH;
  int MTHETA;
  int NPHIH;
  int MPHIA;
  int NHPARM;
  int NPARM;
  int NNB;
  int NEXT;
  int NRES;
  int NBONA;
  int NTHETA;
  int NPHIA;
  int NUMBND;
  int NUMANG;
  int NPTRA;
  int NATYP;
  int NPHB;
  int IFPERT;
  int NBPER;
  int NGPER;
  int NDPER;
  int MBPER;
  int MGPER;
  int MDPER;
  int IFBOX;
  int NMXPS;
  int IFCAP; 
  int NEXTRA;
  
  char **IGRAPH;

  double *CHRG;
  double *AMASS;
  int *IAC;
  int *NUMEX;
  int *ICO;
  char **LABERES;
  int *IPRES;
  double *RK;
  double *REQ;
  double *TK;
  double *TEQ;
  double *PK;
  double *PN;
  double *PHASE;
  double *SOLTY;
  double *CN1;
  double *CN2;
  int **BH;
  int **BA;
  int **TH;
  int **TA;
  int **PH;
  int **PA;
  int *NATEX;
  double *ASOL;
  double *BSOL;
  double *HBCUT;
  char **ISYMBL;
  char **ITREE;
  int *JOIN;
  int *IROTAT;
  int IPTRES;
  int NSPM;
  int NSPSOL;
  int *NSP;
  double BETA;
  double BOX[3];
  int NATCAP;
  double CUTCAP;
  double XCAP;
  double YCAP;
  double ZCAP;
  int **BPER;
  int *ICBPER;
  int **TPER;
  int *ICTPER;
  int **PPER;
  int *ICPPER;
  char **IGRPER;
  char **ISMPER;
  double *ALMPER;
  double *IAPER;
  int *IACPER;
  double *CGPER;
  double *ATPOL;
  double *ATPOL1;
  int NLES_NTYP;
  int *LES_TYPE;
  double *LES_FAC;
  double *LES_CNUM;
  double *LES_ID;

};

void readParmtopL(FILE *parmfile, struct AmberParmL *AP);

#endif
