# MDminimum
```
Molecular Dynamics Program written in C. Based on Molecular Physics 87 (5), 1117-1157.
```

###Installation
``
make
``

###How to use
``
MDminimun  --nums 1000 ../input/ALADv.crd ../input/ALADv.top ../output/ALADv.out ../output/ALADv.trj
``

###Options
``
 [--nve]
 [--log <logfilename>]
 [--vMode]
 [--nums <numsteps>]
 [--temp <temperature>]
 [--tau <tau>]
 [--int <output freaquency>]
 [--rst <restart file name>]
 [--rstvel <restart file name velocity>]
 [--dt <time step>]
 [-h] help

``
