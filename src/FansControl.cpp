#include <FansControl.h>

FansControl::FansControl(int pin,int freq,int Channel,int resolution)
{
    _pin = pin;
    _freq = freq;
    _Channel = Channel;
    _resolution = resolution;
}

FansControl::~FansControl()
{

}


int FansControl::Setup()
{
  ledcSetup(0,25000, 10);
  ledcAttachPin(13,0);
  //ledcWrite(0,1023);
  // analogReadResolution(12); // 12位分辨率
  // adcAttachPin(4);//将引脚连接到ADC
  return 1;
}

int FansControl::Start()
{  
  ledcWrite(0, 1023);
}


int FansControl::Stop()
{
  ledcWrite(0, 512);
}

