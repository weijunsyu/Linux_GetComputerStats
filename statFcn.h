#ifndef STATFCN_H
#define STATFCN_H

//returns a dynamically allocated char array (string) of the kernel version
extern char* findKernel(FILE* file);
//returns the processor type as a string
extern char* findProcessor(FILE* file);
//returns the time system was last booted as a string
extern char* findLastBoot(FILE* file);
//return the uptime of the system as a double
extern double findUpTime(FILE* file);
//return the number of processes as a double
extern double findProcesses(FILE* file);
//find the total system memory
extern double findMemTotal(FILE* file);
//find the available memory
extern double findMemAvail(FILE* file);
//find the number of context switches
extern double findCtxt(FILE* file);
//find the time CPU has spent in user, system, and idle
extern double* findCPUtimes(FILE* file);
//find the number of disk read and write requests
extern double* findIORequests(FILE* file);

#endif
