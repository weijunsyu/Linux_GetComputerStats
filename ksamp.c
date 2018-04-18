
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "statFcn.h"

/*
NAME: WEIJUN SYU

Write a C program that gathers the following information from the /proc directory:
- Processor Type.
- Kernel version.
- The amount of time since it was last booted.
- The amount of time the processor has spent in user mode, system mode, and idle time.
- The number of disk read/write requests made on the system.
- The number of context switches the kernel has performed.
- The time the system was last booted.
- The number of processes that have been created since the last boot.
- The amount of memory configured for this computer.
- The amount of memory currently available.
Your program (call it ksamp) will gather the above information every x seconds over a
period of y seconds, where x and y are specified on the command line when you run
your program. For example,
    ksamp 2 60
specifies that your program should gather this information every 2 seconds, over a period
of 60 seconds. Hence, the information would be gathered 30 times in total.

LIMITATIONS of program: Assumes no errors in opening and reading files, all files assumed
to have "correct" structure, static length arrays used to hold strings will fail if
file strings end up larger. Assumes command line arguments given are 2 non-zero, non-negative
integers.

*/

//get the data of unchanging values storing size of array in size
char** doUnchanging(int* size);
//get the data averages of changing values give interval and period
double* doAverages(int interval, int period);
//print the data to console given data
void printData(char** unChanging, double* averages);

int main (int argc, char* argv[]) {
    if (argc != 3) { //if other than 2 arguments are given error and exit
        printf("%s\n", "Please give 2 integer arguments");
        return 1;
    }//else there are 2 arguments given
    int interval = atoi(argv[1]);
    int period =  atoi(argv[2]);

    //get data
    int sizeUnC;
    char** unChanging = doUnchanging(&sizeUnC);
    double* averages = doAverages(interval, period);

    //print to console
    printData(unChanging, averages);

    //free memory
    int i;
    for(i=0; i < sizeUnC; i++) {
        free(unChanging[i]);
    }
    free(unChanging);
    free(averages);
    return 0;
}


char** doUnchanging(int *size) {
    FILE* file;
    char** ret = malloc(3);
    *size = 3; //pass the size of returned array out of function
    //find the kernel version
    file = fopen("/proc/version", "r"); //open file
    char* kernelVer = findKernel(file);
    fclose(file); //close file
    //find the processor Type
    file = fopen("/proc/cpuinfo", "r");
    char* cpuType = findProcessor(file);
    fclose(file);
    //find the time the system was last booted
    file = fopen("/proc/stat", "r");
    char* lastBoot = findLastBoot(file);
    fclose(file);

    ret[0] = kernelVer;
    ret[1] = cpuType;
    ret[2] = lastBoot;
    return ret;
}

double* doAverages(int interval, int period) {
    double* averages = calloc(10, sizeof(double)); //allocate memory
    FILE* file;
    int i = 0; //loop counter
    int c = 0; // number of elements (n) used to calc mean

    //containers
    double upTime, procs, memTot, memAvail, ctxt;
    double* cpu;
    double* io;
    //averages
    double avgUpTime = 0;
    double avgProcs = 0;
    double avgMemTot = 0;
    double avgMemAvail = 0;
    double avgCtxt = 0;
    double avgUser = 0;
    double avgSys = 0;
    double avgIdle = 0;
    double avgRead = 0;
    double avgWrite = 0;

    while(i <= period) {
        //find the upTime
        file = fopen("/proc/uptime", "r");
        upTime = findUpTime(file);
        avgUpTime += upTime;
        fclose(file);
        //find the number of processes
        file = fopen("/proc/stat", "r");
        procs = findProcesses(file);
        avgProcs += procs;
        fclose(file);
        //find the total amount of memory
        file = fopen("/proc/meminfo", "r");
        memTot = findMemTotal(file);
        avgMemTot += memTot;
        fclose(file);
        //find the available memory
        file = fopen("/proc/meminfo", "r");
        memAvail = findMemAvail(file);
        avgMemAvail += memAvail;
        fclose(file);
        //find the number of context switches
        file = fopen("/proc/stat", "r");
        ctxt = findCtxt(file);
        avgCtxt += ctxt;
        fclose(file);
        //find the time in user, system, and idle modes
        file = fopen("/proc/stat", "r");
        cpu = findCPUtimes(file);
        avgUser += cpu[0];
        avgSys += cpu[1];
        avgIdle += cpu[2];
        free(cpu);
        fclose(file);
        //find the number of read and write findIORequests
        file = fopen("/proc/diskstats", "r");
        io = findIORequests(file);
        avgRead += io[0];
        avgWrite += io[1];
        free(io);
        fclose(file);

        c++; //increment counter for n requred in mean calculation
        i += interval;
        if (i <= period) {
            sleep(interval);
        }
    }
    //find the mean after takeing sums
    avgUpTime /= c;
    avgProcs /= c;
    avgMemTot /= c;
    avgMemAvail /= c;
    avgCtxt /= c;
    avgUser /= c;
    avgSys /= c;
    avgIdle /= c;
    avgRead /= c;
    avgWrite /= c;

    averages[0] = avgUpTime;
    averages[1] = avgProcs;
    averages[2] = avgMemTot;
    averages[3] = avgMemAvail;
    averages[4] = avgCtxt;
    averages[5] = avgUser;
    averages[6] = avgSys;
    averages[7] = avgIdle;
    averages[8] = avgRead;
    averages[9] = avgWrite;

    return averages;
}

void printData(char** unChanging, double* averages) {
    //print output
    printf("The kernel version is: %s\n", unChanging[0]);
    printf("The %s", unChanging[1]);
    printf("The time since last boot is: %s seconds\n", unChanging[2]);

    printf("The average uptime is: %f seconds\n", averages[0]);
    printf("The average number of processes is: %f\n", averages[1]);
    printf("The average total memory is: %f kB\n", averages[2]);
    printf("The average available memory is: %f kB\n", averages[3]);
    printf("The average number of context switches is: %f\n", averages[4]);
    printf("The average time spent in user mode: %f, system: %f, idle: %f\n",
            averages[5], averages[6], averages[7]);
    printf("The average number of disk read requests: %f, write: %f\n",
            averages[8], averages[9]);
}
