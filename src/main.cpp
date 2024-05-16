#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <DataProcess.h>
#include <FansControl.h>



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

DataProcess *DP = new DataProcess();
FansControl *FC = new FansControl(2,5000,0,8);


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

  FC->Setup();

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
      DP->WriteData(recvdata);
      DP->PrintData();
      //Serial.println(recvdata);
    }
    
    
  }
  Serial.printf("RSSI:");
  Serial.println(WiFi.RSSI());
  //FC->Start();
  delay(1000);
}