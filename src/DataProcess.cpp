#include <DataProcess.h>

DataProcess::DataProcess()
{
    
}

DataProcess::~DataProcess()
{

}

void DataProcess::WriteData(String recvdata)
{
  int pData = 0;
  int pre_found = -1;
  int found = 0;
  for(int i = 0;i<ArrayLen;i++)
  {

    int k = 0;
    found = recvdata.indexOf('|');

    // Serial.printf("pre_found:");
    // Serial.println(pre_found);
    // Serial.printf("found:");
    // Serial.println(found);

    if(found != -1)
    {
      String data_tmp = "";
      for(k = pre_found;k<found-1;k++)
      {
        data_tmp += recvdata[k+1];
        recvdata[k+1] = ' ';
        //Serial.printf("data_tmp:");
        //Serial.println(data_tmp);
      }
      *DataArray[pData] = data_tmp.toInt();
      //Serial.printf("*DataArray[pData]:");
      //Serial.println(*DataArray[pData]);
      pData++;
      recvdata[found] = ' ';
      pre_found = found;

    }
  }
}

void DataProcess::PrintData()
{
    Serial.printf("cpu_tmp:");
    Serial.println(cpu_tmp);
    Serial.printf("cpu_usage:");
    Serial.println(cpu_usage);
    Serial.printf("net_usage:");
    Serial.println(net_usage);
    Serial.printf("disk_usage:");
    Serial.println(disk_usage);
    Serial.printf("ram_usage:");
    Serial.println(ram_usage);
}