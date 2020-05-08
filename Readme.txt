This C++ application is used for benchmark the Multiple Core CPU. Based on number of cpu-cores, it creates a threads on each CPU core and runs binomial computation on each CPU core by creating an individual thread. Using CPU affinity, each thread runs on individual core.
It can also be called Bounded Mulitprocessing(BMP).

User parameter: Number of Iteration that Binomial multiplication runs on each core.
Example: Multiple_Core 10000

Currently this application ported to Linux and QNX.

Command to build for aarch-64:
/media/adas/BSP/19/build_ls2084abbmini/tmp/work/aarch64-fsl-linux/rti/1.0-r0/recipe-sysroot-native/usr/bin/aarch64-fsl-linux/aarch64-fsl-linux-g++ --sysroot=/media/adas/BSP/19/build_ls2084abbmini/tmp/work/aarch64-fsl-linux/rti/1.0-r0/recipe-sysroot  -std=c++11 -I. -pthread -o Multi_Core_LS2084_Linux Binomial_Multi.cpp Multi_Core.cpp

Command to build for aarch-64 in QNX:
qcc -lang-c++ -Vgcc/5.4.0,gcc_ntoaarch64le -Y_gpp -o Multi_Core_Qnx  -I. -l c Binomial_Multi.cpp Multi_Core.cpp 
Note: Library -l c is important. 

Application Output:
root# Multi_Core_Linux 1000000
Number of Cores : 8
Num of threads  : 9
With-Out Thread----> ( 9.03482e+07 microSec) ( 90348.2 milliSec) ( 90 Sec)
With Thread--------> ( 2.06013e+07 microSec) ( 20601.3 milliSec) ( 20 Sec)


