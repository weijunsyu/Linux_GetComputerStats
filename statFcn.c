#include "stdio.h"
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "strFcn.h"

char* findKernel(FILE* file) {
    char kernelVer[30]; //temp container
    int i;
    //get the kernel version and store in kernelVer discarding unneeded strings
    for(i=0; i < 3; i++) {
        fscanf(file, "%s", kernelVer);
    }
    return allocStr(kernelVer);
}

char* findProcessor(FILE* file) {
    char cpuType[80];
    while (fgets(cpuType, sizeof(cpuType), file)) { //read by line
        if (strstr(cpuType, "model name") != NULL) { //if substring present:
            break; //end loop
        }
    }
    return allocStr(cpuType);
}

char* findLastBoot(FILE* file) {
    char btime[30];
    char* lastBoot;
    while (fgets(btime, sizeof(btime), file)) {
        if (strstr(btime, "btime") != NULL) {
            break;
        }
    }
    strtok(btime, "\n"); //remove newline
    lastBoot = strtok(btime, " ");
    lastBoot = strtok(NULL, " "); //obtain pointer to correct substring
    return allocStr(lastBoot);
}

double findUpTime(FILE* file) {
    double upTime;
    fscanf(file, "%lf", &upTime); //scan as double value
    return upTime;
}

double findProcesses(FILE* file) {
    char line[30];
    char* numProc;
    double procs;
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, "processes") != NULL) {
            strtok(line, "\n");
            numProc = strtok(line, " ");
            numProc = strtok(NULL, " ");
            procs = atoi(numProc); //convert from string to int
            break;
        }
    }
    return procs;
}

double findMemTotal(FILE* file) {
    char line[50];
    char* mem;
    double memNum;
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, "MemTotal:") != NULL) {
            mem = strtok(line, " ");
            mem = strtok(NULL, " ");
            memNum = atoi(mem);
            break;
        }
    }
    return memNum;
}

double findMemAvail(FILE* file) {
    char line[50];
    char* mem;
    double memNum;
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, "MemAvailable:") != NULL) {
            mem = strtok(line, " ");
            mem = strtok(NULL, " ");
            memNum = atoi(mem);
            break;
        }
    }
    return memNum;
}

double findCtxt(FILE* file) {
    char line[30];
    char* switches;
    double ctxt;
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, "ctxt") != NULL) {
            switches = strtok(line, " ");
            switches = strtok(NULL, " ");
            ctxt = atoi(switches);
            break;
        }
    }
    return ctxt;
}

double* findCPUtimes(FILE* file) {
    char line[80];
    char* tmp; //temp pointer to beginning of substrings
    double user, sys, idle; //store final values

    fgets(line, sizeof(line), file);

    tmp = strtok(line, " ");
    tmp = strtok(NULL, " "); //navigate to correct loc. in string
    user = atoi(tmp); // store value
    tmp = strtok(NULL, " ");
    tmp = strtok(NULL, " "); //continue further into string
    sys = atoi(tmp);
    tmp = strtok(NULL, " ");
    idle = atoi(tmp);

    double* ret = malloc(3); //allocate memory
    ret[0] = user;
    ret[1] = sys;
    ret[2] = idle;
    return ret;
}

double* findIORequests(FILE* file) {
    char line[80];
    char* tmp;
    double read, write;
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, "sda") != NULL) {
            tmp = strtok(line, " ");
            tmp = strtok(NULL, " ");
            tmp = strtok(NULL, " ");
            tmp = strtok(NULL, " ");
            read = atoi(tmp);
            tmp = strtok(NULL, " ");
            tmp = strtok(NULL, " ");
            tmp = strtok(NULL, " ");
            tmp = strtok(NULL, " ");
            write = atoi(tmp);
            double* ret = malloc(2);
            ret[0] = read;
            ret[1] = write;
            return ret;
        }

    }
    return NULL;
}
