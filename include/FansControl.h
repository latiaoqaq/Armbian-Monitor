#ifndef __FANSCONTROL__
    #define __FANSCONTROL__
    #include <Arduino.h>
    #include <WiFi.h>

    class FansControl
    {
    private:
        /* data */
        // 定义 PWM 参数
        int _pin = 11;
        int _freq = 5000; // PWM 频率
        int _Channel = 0; // PWM 通道
        int _resolution = 8; // PWM 分辨率
        bool isStop = true;
    public:
        FansControl(int pin = 11,int freq =10000,int Channel=0,int resolution=10);
        ~FansControl();
        int Start();
        int Setup();
        int Stop();
        int SetSpeed(float percent);

    };
    
#endif