#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>




int RetCode = -1;
const char* ssd="WSGX";
const char* pasd="wsgx88888";

const char * dst_addr = "192.168.100.13";
const int dst_port = 8000;
String recvdata = "";



int cpu_tmp = -1;
int cpu_usage = -1;
int net_usage = -1;
int disk_usage = -1;

int *Data_Array[4] = {&cpu_tmp,&cpu_usage,&net_usage,&disk_usage};

WiFiClient client;

hw_timer_t * tcp_timer = NULL;


void WriteData(String recvdata,int *Data_array[]);

void IRAM_ATTR TcpTimerEvent()
{
  if(client.connected())
  {
    
  }
  else
  {
    client.stop();
    if(!client.connect(dst_addr,dst_port))
    {
      Serial.println("RecvFailed,Connecting.....");
    }
    else
    {
      Serial.println("ReConnected");
    }
    //timerAlarmDisable(tcp_timer);
  }
}

void setup() {

  //timer Init
  tcp_timer = timerBegin(0,80,true);
  timerAttachInterrupt(tcp_timer,&TcpTimerEvent,true);
  timerAlarmWrite(tcp_timer,1000000,true);


  //Serial Init
  Serial.begin(115200);

  //Wifi Init
  Serial.printf("开始连接WIFI");
  Serial.printf("账号为：%d",ssd);
  Serial.printf("密码为：%d",pasd);

  WiFi.begin(ssd,pasd);
  while(WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.print("connected success!\n");

  Serial.print("IP:");
  Serial.print(WiFi.localIP());

  
  client.connect(dst_addr,dst_port);

  //timerAlarmEnable(tcp_timer);




}

void loop() {
  Serial.println("MAIN");


  //检测TCP连接，如果断开那就重连
  if(!client.connected())
  {
    client.stop();
    client.connect(dst_addr,dst_port);
    Serial.println("Connecting...");
  }
  else
  {
    client.write('1');
    if(client.available()>0)
    {
      recvdata = client.readStringUntil('\n');
      WriteData(recvdata,Data_Array);
      
      Serial.println(recvdata);
    }
    
    
  }
  delay(1000);
}

void WriteData(String recvdata,int *DataArray[])
{
  int pData = 0;
  int pre_found = -1;
  int found = 0;
  for(int i = 0;i<4;i++)
  {

    int k = 0;
    found = recvdata.indexOf('|');

    Serial.printf("pre_found:");
    Serial.println(pre_found);
    Serial.printf("found:");
    Serial.println(found);

    if(found != -1)
    {
      String data_tmp = "";
      for(k = pre_found;k<found-1;k++)
      {
        data_tmp += recvdata[k+1];
        recvdata[k+1] = ' ';
        Serial.printf("data_tmp:");
        Serial.println(data_tmp);
      }
      *DataArray[pData] = data_tmp.toInt();
      Serial.printf("*DataArray[pData]:");
      Serial.println(*DataArray[pData]);
      pData++;
      recvdata[found] = ' ';
      pre_found = found;

    }
  }
  

}