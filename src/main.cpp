#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <DataProcess.h>
#include <FansControl.h>
#include <U8g2lib.h>

U8G2_SSD1306_128X64_NONAME_F_HW_I2C oled(U8G2_R0,SDA,SCL,U8X8_PIN_NONE);
int RetCode = -1;
const char* ssd="latiao";
const char* pasd="guangqinfu332601";

const char * dst_addr = "192.168.2.6";
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
  FC->Stop();
  //oled.enableUTF8Print();
  oled.setContrast(200);
  oled.setBusClock(400000);
  
  oled.begin();
  oled.enableUTF8Print();
}

void loop() {
  Serial.println("MAIN");

  int duration  = pulseIn(7, HIGH, 9000000);
  Serial.println(duration);
  // //检测TCP连接，如果断开那就重连
  // if(!client.connected())
  // {
  //   client.stop();
  //   client.connect(dst_addr,dst_port);
  //   Serial.println("Connecting...");
  // }
  // else
  // {
  //   client.write('1');
  //   if(client.available()>0)
  //   {
  //     recvdata = client.readStringUntil('\n');
  //     Serial.println(recvdata);
  //     DP->WriteData(recvdata);
  //     DP->PrintData();
  //     //Serial.println(recvdata);
  //   }
    
    
  // }
  // //FC->Start();

  // char buf[10];


  // static unsigned int start = millis();
  // oled.clearBuffer();
  // oled.setFont(u8g2_font_ncenB08_tr);
  // itoa(DP->cpu_tmp,buf,10);
  // oled.drawStr(0,10,"CPU TEMP:");
  // oled.drawStr(80,10,buf);
  // oled.drawStr(95,10,"C");
  // itoa(DP->cpu_usage,buf,10);
  // oled.drawStr(0,30,"CPU USAGE:");
  // oled.drawStr(80,30,buf);
  // oled.drawStr(95,30,"%");
  // Serial.println(DP->ram_usage/1000);
  // itoa(DP->ram_usage,buf,10);
  // oled.drawStr(0,40,"RAM USAGE:");
  // oled.drawStr(80,40,buf);
  // oled.drawStr(110,40,"MB");
  // itoa(DP->disk_usage,buf,10);
  // oled.drawStr(0,50,"DISK USAGE:");
  // oled.drawStr(80,50,buf);
  // oled.drawStr(95,50,"%");
  // itoa(WiFi.RSSI(),buf,10);
  // oled.drawStr(0,60,buf); 
  // oled.sendBuffer();
  // static unsigned int end = millis();

  
  // Serial.println(end-start);

  delay(1000);
}