#include <FansControl.h>

FansControl::FansControl(int pin,int freq,int Channel,int resolution)
{
    _freq = freq;
    _Channel = Channel;
    _resolution = resolution;
}

FansControl::~FansControl()
{

}

int FansControl::Start()
{
    for (int dutyCycle = 0; dutyCycle <= 255; dutyCycle++) {
    ledcWrite(_Channel, dutyCycle);
    delay(15);
    }
  // 逐渐减小 PWM 的占空比，使 LED 亮度逐渐减小
    for (int dutyCycle = 255; dutyCycle >= 0; dutyCycle--) {
    ledcWrite(_Channel, dutyCycle);
    delay(15);
    }
    return 1;
}
int FansControl::Setup()
{
  ledcSetup(_Channel, _freq, _resolution);
  // 将 PWM 通道连接到引脚
  ledcAttachPin(_Pin, _Channel);
  return 1;
}