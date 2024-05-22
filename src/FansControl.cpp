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
  ledcSetup(0,25000, 8);
  ledcAttachPin(13,0);
  isStop = false;
  //ledcWrite(0,1023);
  // analogReadResolution(12); // 12位分辨率
  // adcAttachPin(4);//将引脚连接到ADC
  return 1;
}

int FansControl::Start()
{
  
  ledcAttachPin(13,0);
  isStop = 0;
}

int FansControl::SetSpeed(float percent)
{
  if(!(percent >=0))
  {
    return -1;
  }
  if(percent == 0)
  {
    Stop();
  }
  if(percent < 30)
  {
    if(isStop)
    {
      Start();
    }
    ledcWrite(0, 70);
  }
  else if(percent > 100)
  {
    if(isStop)
    {
      Start();
    }
    
    ledcWrite(0, 255);
  }
  else
  {
    if(isStop)
    {
      Start();
    }
    int fre = percent/100.0*255.0;
    Serial.printf("%d",fre);
    ledcWrite(0, fre);
  }
  return 1;
  

}


int FansControl::Stop()
{

  //ledcAttachPin(13,0);
  ledcDetachPin(13);
  digitalWrite(13,0);
  isStop = 1;
}

