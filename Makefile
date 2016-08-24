#
# Makefile for MD_NHC_MP1996
#

HOME=/Users/yamamoriyuu/Box Sync/Research (fromambertozen@gmail.com)/mac/works/programs/C/MDminimum/src
SRC=.
BIN=../bin

LIBDIR= #-L../lib
INCDIR= #-I../include

# definitions
#CC    = icc -pg -O3 -tpp6 -ipo
#CC    = icc -check=conversion,stack,uninit
#CC    = icc
CC    = gcc -g
#CC     = gcc

CFLAG = #-D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE -m64 -O2

OBJS =  MDrun.o integrater.o \
	readParameters.o force.o topology.o inner_outer_product.o bond_angle_dihedral.o \
	BoxMuller.o efunc.o

LIBS =  integrater.h readParameters.h force.h topology.h \
	inner_outer_product.h bond_angle_dihedral.h const.h BoxMuller.h efunc.h

EXLIBS=-lm

TARGET = MDminimum

.c.o:
	$(CC) $(INCDIR) -c $(CFLAG) $<;

all: MDminimum install

# rules of generations
MDminimum:  $(OBJS) $(LIBS)
	$(CC) $(CFLAG) -o $@ $(OBJS) $(LIBDIR) $(INCDIR) $(EXLIBS) ;

install: 
	cp $(TARGET) $(BIN) ; 

MDrun.o:               integrater.h force.h readParameters.h const.h BoxMuller.h efunc.h
integrater.o:          integrater.h const.h force.h efunc.h
force.o:               force.h bond_angle_dihedral.h readParameters.h const.h \
                       inner_outer_product.h topology.h efunc.h
readParmtop.o:         readParameters.h efunc.h
topology.o:            topology.h
bond_angle_dihedral.o: bond_angle_dihedral.h
inner_outer_product.o: inner_outer_product.h
BoxMuller.o:           BoxMuller.h
efunc.o:               efunc.h

clean: 
	rm $(OBJS); \
	rm $(TARGET);
	rm ../bin/$(TARGET);
