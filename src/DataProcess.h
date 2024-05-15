#ifndef __DATAPROCESS__
#define  __DATAPROCESS__
#include <Arduino.h>

class DataProcess
{
private:
    const int _arrlen = 4;

public:
    DataProcess();
    ~DataProcess();

    int cpu_tmp = -1;
    int cpu_usage = -1;
    int net_usage = -1;
    int disk_usage = -1;
    int * DataArray[4] = {&cpu_tmp,&cpu_usage,&net_usage,&disk_usage};
    void WriteData(String recvdata);

};
#endif
