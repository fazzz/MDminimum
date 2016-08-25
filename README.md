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
MDminimun  --nums 1000 ALADv.crd ALADv.top ALADv.out ALADv.trj
``

###Options
[--nve] `set ensemble NVE (default nvt)`

[--log <logfilename>] `set log file name (default mdlog)`

[--vMode] `set restart mode ON (default OFF)`

[--nums <numsteps>] `set number of total calculation step (default 10000)`

[--temp <temperature>]`set temperature at K (default 300K)`

[--tau <tau>] `set tau parametr of Nose-Hoover chain at ps (default 0.01ps)`

[--int <output freaquency>] `set output frequency (default 100)`

[--rst <restart file name>] `set restart coordinate file name (default rstcrd)`

[--rstvel <restart file name velocity>] `set restart velocity file name (default rstvel)`

[--dt <time step>] `set time step width at ps (default 0.001ps)`

[-h] help `show the help message`
