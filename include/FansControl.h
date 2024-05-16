#ifndef __FANSCONTROL__
    #define __FANSCONTROL__
    #include <Arduino.h>

    class FansControl
    {
    private:
        /* data */
        // 定义 PWM 参数
        int _Pin = 0;
        int _freq = 5000; // PWM 频率
        int _Channel = 0; // PWM 通道
        int _resolution = 8; // PWM 分辨率
    public:
        FansControl(int pin,int freq,int Channel,int resolution);
        ~FansControl();
        int Start();
        int Setup();

    };
    
#endif