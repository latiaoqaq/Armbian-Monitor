#ifndef __DATAPROCESS__
#define  __DATAPROCESS__
#include <Arduino.h>

#define ArrayLen 5
class DataProcess
{
private:


public:
    DataProcess();
    ~DataProcess();
    int cpu_tmp = -1;
    int cpu_usage = -1;
    int net_usage = -1;
    int disk_usage = -1;
    int ram_usage = -1;
    int * DataArray[ArrayLen] = {&cpu_tmp,&cpu_usage,&ram_usage,&disk_usage,&net_usage};



    void WriteData(String recvdata);
    void PrintData();
};
#endif
