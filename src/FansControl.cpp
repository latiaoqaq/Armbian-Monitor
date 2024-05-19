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
  pinMode(_pin,OUTPUT);
  ledcSetup(_Channel, _freq, _resolution);
  // 将 PWM 通道连接到引脚
  ledcAttachPin(_pin, _Channel);
  return 1;
}

int FansControl::Start()
{  
  ledcWrite(_Channel, 512);
}


int FansControl::Stop()
{
  ledcWrite(_Channel, 0);
}

