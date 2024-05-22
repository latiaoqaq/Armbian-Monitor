 #include <Arduino.h>
//#include <WiFi.h>
// #include <HTTPClient.h>
// #include <ArduinoJson.h>
//#include <DataProcess.h>
//#include <FansControl.h>
// #include <U8g2lib.h>

// U8G2_SSD1306_128X64_NONAME_F_HW_I2C oled(U8G2_R0,SDA,SCL,U8X8_PIN_NONE);
// int RetCode = -1;
// const char* ssd="WSGX";
// const char* pasd="wsgx88888";

// const char * dst_addr = "192.168.2.6";
// const int dst_port = 8000;
// String recvdata = "";



// int cpu_tmp = -1;
// int cpu_usage = -1;
// int net_usage = -1;
// int disk_usage = -1;





// WiFiClient client;

// hw_timer_t * tcp_timer = NULL;


// void WriteData(String recvdata,int *Data_array[]);

// void IRAM_ATTR TcpTimerEvent()
// {
//   if(client.connected())
//   {
    
//   }
//   else
//   {
//     client.stop();
//     if(!client.connect(dst_addr,dst_port))
//     {
//       Serial.println("RecvFailed,Connecting.....");
//     }
//     else
//     {
//       Serial.println("ReConnected");
//     }
//     //timerAlarmDisable(tcp_timer);
//   }
// }

// void setup() {

//   //timer Init
//   tcp_timer = timerBegin(0,80,true);
//   timerAttachInterrupt(tcp_timer,&TcpTimerEvent,true);
//   timerAlarmWrite(tcp_timer,1000000,true);


//   //Serial Init
//   Serial.begin(115200);

//   //Wifi Init
//   Serial.printf("开始连接WIFI");
//   Serial.printf("账号为：%d",ssd);
//   Serial.printf("密码为：%d",pasd);

//   WiFi.begin(ssd,pasd);
//   while(WiFi.status() != WL_CONNECTED)
//   {
//     delay(500);
//     Serial.print(".");
//   }
//   Serial.print("connected success!\n");

//   Serial.print("IP:");
//   Serial.print(WiFi.localIP());

  
//   client.connect(dst_addr,dst_port);

//   //timerAlarmEnable(tcp_timer);
//   pinMode(0,OUTPUT);
//   ledcSetup(0, 10000, 10);
//   // 将 PWM 通道连接到引脚
//   ledcAttachPin(0, 0);
//   ledcWrite(0, 0);
//   //oled.enableUTF8Print();
//   oled.setContrast(200);
//   oled.setBusClock(400000);
  
//   oled.begin();
//   oled.enableUTF8Print();
// }

// void loop() {
  
//   Serial.println("MAIN");
  
//   unsigned long duration  = pulseIn(7, HIGH, 9000000);
//   Serial.println(duration);
//   //检测TCP连接，如果断开那就重连
//   if(!client.connected())
//   {
//     client.stop();
//     client.connect(dst_addr,dst_port);
//     Serial.println("Connecting...");
//   }
//   else
//   {
    
//     client.write('1');
//     if(client.available()>0)
//     {
//       recvdata = client.readStringUntil('\n');
//       Serial.println(recvdata);
//       DP->WriteData(recvdata);
//       DP->PrintData();
//       //Serial.println(recvdata);
//     }
    
    
//   }

//   char buf[10];


//   static unsigned int start = millis();
//   oled.clearBuffer();
//   oled.setFont(u8g2_font_ncenB08_tr);
//   itoa(DP->cpu_tmp,buf,10);
//   oled.drawStr(0,10,"CPU TEMP:");
//   oled.drawStr(80,10,buf);
//   oled.drawStr(95,10,"C");
//   itoa(DP->cpu_usage,buf,10);
//   oled.drawStr(0,30,"CPU USAGE:");
//   oled.drawStr(80,30,buf);
//   oled.drawStr(95,30,"%");
//   Serial.println(DP->ram_usage/1000);
//   itoa(DP->ram_usage,buf,10);
//   oled.drawStr(0,40,"RAM USAGE:");
//   oled.drawStr(80,40,buf);
//   oled.drawStr(110,40,"MB");
//   itoa(DP->disk_usage,buf,10);
//   oled.drawStr(0,50,"DISK USAGE:");
//   oled.drawStr(80,50,buf);
//   oled.drawStr(95,50,"%");
//   itoa(WiFi.RSSI(),buf,10);
//   oled.drawStr(0,60,buf); 
//   oled.sendBuffer();
//   static unsigned int end = millis();

  
//   Serial.println(end-start);

//   delay(1000);
// }
/*Using LVGL with Arduino requires some extra steps:
 *Be sure to read the docs here: https://docs.lvgl.io/master/get-started/platforms/arduino.html  */

/*Using LVGL with Arduino requires some extra steps:
 *Be sure to read the docs here: https://docs.lvgl.io/master/get-started/platforms/arduino.html  */

#include <lvgl.h>
#include "DataProcess.h"
#include "FansControl.h"

#if LV_USE_TFT_ESPI
#include <TFT_eSPI.h>
#endif
lv_obj_t * cpu_arc;
lv_obj_t * ram_arc;
lv_obj_t * disk_arc;
lv_obj_t * bg;
lv_obj_t * bg_bottom;
lv_obj_t * chart;
lv_obj_t *net_up_label;
lv_obj_t *net_down_label;
lv_obj_t *cputempbar;
lv_obj_t *fansspeedbar;
lv_chart_series_t * ser1;
lv_chart_series_t * ser2;

DataProcess *DP = new DataProcess();
FansControl *FC = new FansControl(11,5000,0,8);


static void value_changed_event_cb(lv_event_t * e);
int TempControl(int mode,DataProcess * DP,FansControl *FC);
lv_obj_t * lv_example_arc_1(void);
lv_obj_t * lv_example_arc_2(void);
lv_obj_t * lv_example_arc_3(void);
lv_obj_t * lv_example_chart_1(void);
lv_obj_t * lv_net_label_up(void);
lv_obj_t * lv_net_label_down(void);
static void set_temp(void * bar, int32_t temp);
lv_obj_t * lv_example_bar_3(void);
lv_obj_t * lv_example_bar_4(void);


#define TFT_HOR_RES   240
#define TFT_VER_RES   240

#define DRAW_BUF_SIZE (TFT_HOR_RES * TFT_VER_RES / 10 * (LV_COLOR_DEPTH / 8))

uint32_t draw_buf[DRAW_BUF_SIZE / 4];



/* LVGL calls it when a rendered image needs to copied to the display*/
void my_disp_flush( lv_display_t *disp, const lv_area_t *area, uint8_t * px_map)
{
    /*Copy `px map` to the `area`*/

    /*For example ("my_..." functions needs to be implemented by you)
    uint32_t w = lv_area_get_width(area);
    uint32_t h = lv_area_get_height(area);

    my_set_window(area->x1, area->y1, w, h);
    my_draw_bitmaps(px_map, w * h);
     */

    /*Call it to tell LVGL you are ready*/
    lv_disp_flush_ready(disp);
}

void setup()
{
    lv_init();
    lv_tick_set_cb(millis);


    lv_display_t * disp;
#if LV_USE_TFT_ESPI
    /*TFT_eSPI can be enabled lv_conf.h to initialize the display in a simple way*/
    disp = lv_tft_espi_create(TFT_HOR_RES, TFT_VER_RES, draw_buf, sizeof(draw_buf));
#else
    /*Else create a display yourself*/
    disp = lv_display_create(TFT_HOR_RES, TFT_VER_RES);
    lv_display_set_flush_cb(disp, my_disp_flush);
    lv_display_set_buffers(disp, draw_buf, NULL, sizeof(draw_buf), LV_DISPLAY_RENDER_MODE_PARTIAL);
#endif

//////////////////////////////////////////////////////////////////////////////////////////
    lv_obj_t * src = lv_screen_active();
    lv_obj_set_pos(src,0,0);
    lv_obj_set_size(src,240,240);
    lv_obj_set_style_bg_color(src,lv_color_hex(0x3B2D23),0);
    bg = lv_obj_create(lv_screen_active());
    lv_obj_set_size(bg,230,100);
    lv_obj_align(bg,LV_ALIGN_TOP_MID,0,0);
    lv_obj_set_style_bg_color(bg,lv_color_make(193,193,197),LV_PART_MAIN);
    lv_obj_set_style_bg_opa(bg,100,LV_PART_MAIN);
    

    bg_bottom = lv_obj_create(lv_screen_active());
    lv_obj_set_size(bg_bottom,230,120);
    lv_obj_align(bg_bottom,LV_ALIGN_BOTTOM_MID,0,-10);
    lv_obj_set_style_bg_color(bg_bottom,lv_color_make(193,193,197),LV_PART_MAIN);
    lv_obj_set_style_bg_opa(bg_bottom,100,LV_PART_MAIN);
    //lv_obj_set_style_bg_opa(src,50,0);
    cpu_arc = lv_example_arc_1();
    ram_arc = lv_example_arc_2();
    disk_arc = lv_example_arc_3();
    chart = lv_example_chart_1();
    net_up_label = lv_net_label_up();
    net_down_label = lv_net_label_down();
    cputempbar = lv_example_bar_3();
    fansspeedbar = lv_example_bar_4();

    //Serial
    Serial.begin(115200);
    
    Serial.println( "Setup done" );
//////////////////////////////////////////////////////////////////////////////////////////
}

void loop()
{
    if(Serial.available())
    {
      String rcvdata = Serial.readStringUntil('\n');
      int cpu_data = rcvdata.toInt();
      Serial.println(cpu_data);
      lv_arc_set_value(cpu_arc,cpu_data);
      lv_obj_send_event(cpu_arc, LV_EVENT_VALUE_CHANGED,NULL);
      lv_arc_set_value(disk_arc,cpu_data);
      lv_obj_send_event(disk_arc, LV_EVENT_VALUE_CHANGED,NULL);
      lv_arc_set_value(ram_arc,cpu_data);
      lv_obj_send_event(ram_arc, LV_EVENT_VALUE_CHANGED,NULL);
      
      lv_chart_set_next_value(chart,ser1, cpu_data);
      lv_chart_set_next_value(chart,ser2, cpu_data+30);
      lv_label_set_text_fmt(net_up_label,"u%.1fMB",cpu_data);
      lv_label_set_text_fmt(net_down_label,"u%.1fMB",cpu_data+30);
      lv_chart_refresh(chart);
    }
    lv_task_handler(); /* let the GUI do its work */
    delay(5); /* let this time pass */
}

lv_obj_t* lv_example_arc_1(void)
{
    /*Create an Arc*/


    lv_obj_t * arc = lv_arc_create(bg);
    lv_obj_set_size(arc, 60, 60);
    //lv_arc_set_rotation(arc, 135);
    lv_arc_set_bg_angles(arc, 120, 60);
    lv_arc_set_range(arc,0,80);
    lv_arc_set_value(arc,23);
    lv_obj_align(arc,LV_ALIGN_TOP_LEFT,0,0);
    lv_obj_set_style_radius(arc,360,LV_PART_MAIN);
    lv_obj_set_style_bg_opa(arc,LV_OPA_COVER,LV_PART_MAIN);
    lv_obj_set_style_bg_color(arc,lv_color_make(30,35,45),LV_PART_MAIN);
    lv_obj_set_style_pad_all(arc,3,LV_PART_MAIN);
    
    lv_obj_t * panel3 = lv_obj_create(arc);
    lv_obj_set_size(panel3,42,42);
    lv_obj_align_to(panel3,arc,LV_ALIGN_CENTER,0,0);
    lv_obj_set_style_radius(panel3,360,LV_PART_MAIN);
    lv_obj_set_style_bg_color(panel3,lv_color_make(100,100,100),LV_PART_MAIN);
    lv_obj_set_style_bg_grad_color(panel3,lv_color_make(60,65,75),LV_PART_MAIN);
    lv_obj_set_style_bg_grad_dir(panel3,LV_GRAD_DIR_VER,LV_PART_MAIN);
    lv_obj_set_style_arc_width(panel3,0.2,LV_PART_MAIN);
    lv_obj_set_style_shadow_color(panel3,lv_color_make(5,10,15),LV_PART_MAIN);
    lv_obj_set_style_shadow_width(panel3,8,LV_PART_MAIN);
    lv_obj_set_style_shadow_spread(panel3,0,LV_PART_MAIN);
    lv_obj_set_style_shadow_offset_x(panel3,0,LV_PART_MAIN);
    lv_obj_set_style_shadow_offset_y(panel3,3,LV_PART_MAIN);
    // lv_obj_t * panel_value = lv_obj_create(panel3);
    // lv_obj_set_size(panel_value,30,30);
    // lv_obj_align_to(panel_value,arc,LV_ALIGN_CENTER,0,0);
    // lv_obj_set_style_radius(panel_value,360,LV_PART_MAIN);
    // lv_obj_set_style_bg_color(panel_value,lv_color_make(12,25,30),LV_PART_MAIN);
    // lv_obj_set_style_bg_grad_color(panel_value,lv_color_make(25,28,38),LV_PART_MAIN);
    // lv_obj_set_style_bg_grad_dir(panel_value,LV_GRAD_DIR_VER,LV_PART_MAIN);
    // lv_obj_set_style_arc_width(panel_value,0.2,LV_PART_MAIN);
    // lv_obj_set_style_border_color(panel3,lv_color_make(90,100,110),LV_PART_MAIN);
    
    lv_obj_t * label = lv_label_create(panel3);
    lv_label_set_text_fmt(label,"%d",23);
    //lv_obj_set_style_text_align(label,LV_TEXT_ALIGN_CENTER,LV_PART_MAIN);
    //lv_obj_set_style_pad_all(label,0,LV_PART_MAIN);

    lv_obj_align(label,LV_ALIGN_CENTER,0,0);
    lv_obj_set_style_bg_opa(label,0,LV_PART_MAIN);
    //lv_obj_set_style_border_width(label,0,LV_PART_MAIN);
    //lv_obj_set_style_border_color(label,0,LV_PART_MAIN);
    //lv_style_set_text_font(label,&LV_FONT_MONTSERRAT_18,LV_PART_MAIN);
    lv_obj_set_style_text_color(panel3,lv_color_make(255,255,255),LV_PART_MAIN);
    lv_obj_add_event_cb(arc, value_changed_event_cb, LV_EVENT_VALUE_CHANGED, label);


    lv_obj_remove_style(arc, NULL, LV_PART_KNOB);
    lv_obj_set_style_arc_width(arc,5,LV_PART_MAIN);
    lv_obj_set_style_arc_width(arc,5,LV_PART_INDICATOR);
    lv_obj_set_style_arc_color(arc, lv_color_hex(0x0f0f0f), LV_PART_MAIN);
    lv_obj_set_style_arc_rounded(arc, true, LV_PART_MAIN);


    
    //lv_obj_center(arc);

    /*Manually update the label for the first time*/
    //lv_obj_send_event(arc, LV_EVENT_VALUE_CHANGED,NULL);


    return arc;

}
lv_obj_t * lv_example_arc_2(void)
{
    /*Create an Arc*/
    lv_obj_t * arc = lv_arc_create(bg);
    lv_obj_set_size(arc, 60, 60);
    //lv_arc_set_rotation(arc, 135);
    lv_arc_set_bg_angles(arc, 120, 60);
    lv_arc_set_range(arc,0,4096);
    lv_arc_set_value(arc,23);
    lv_obj_align(arc,LV_ALIGN_TOP_MID,0,0);
    lv_obj_set_style_radius(arc,360,LV_PART_MAIN);
    lv_obj_set_style_bg_opa(arc,LV_OPA_COVER,LV_PART_MAIN);
    lv_obj_set_style_bg_color(arc,lv_color_make(30,35,45),LV_PART_MAIN);
    lv_obj_set_style_pad_all(arc,3,LV_PART_MAIN);
    
    lv_obj_t * panel3 = lv_obj_create(arc);
    lv_obj_set_size(panel3,42,42);
    lv_obj_align_to(panel3,arc,LV_ALIGN_CENTER,0,0);
    lv_obj_set_style_radius(panel3,360,LV_PART_MAIN);
    lv_obj_set_style_bg_color(panel3,lv_color_make(100,100,100),LV_PART_MAIN);
    lv_obj_set_style_bg_grad_color(panel3,lv_color_make(60,65,75),LV_PART_MAIN);
    lv_obj_set_style_bg_grad_dir(panel3,LV_GRAD_DIR_VER,LV_PART_MAIN);
    lv_obj_set_style_arc_width(panel3,0.2,LV_PART_MAIN);
    lv_obj_set_style_shadow_color(panel3,lv_color_make(5,10,15),LV_PART_MAIN);
    lv_obj_set_style_shadow_width(panel3,8,LV_PART_MAIN);
    lv_obj_set_style_shadow_spread(panel3,0,LV_PART_MAIN);
    lv_obj_set_style_shadow_offset_x(panel3,0,LV_PART_MAIN);
    lv_obj_set_style_shadow_offset_y(panel3,3,LV_PART_MAIN);
    // lv_obj_t * panel_value = lv_obj_create(panel3);
    // lv_obj_set_size(panel_value,30,30);
    // lv_obj_align_to(panel_value,arc,LV_ALIGN_CENTER,0,0);
    // lv_obj_set_style_radius(panel_value,360,LV_PART_MAIN);
    // lv_obj_set_style_bg_color(panel_value,lv_color_make(12,25,30),LV_PART_MAIN);
    // lv_obj_set_style_bg_grad_color(panel_value,lv_color_make(25,28,38),LV_PART_MAIN);
    // lv_obj_set_style_bg_grad_dir(panel_value,LV_GRAD_DIR_VER,LV_PART_MAIN);
    // lv_obj_set_style_arc_width(panel_value,0.2,LV_PART_MAIN);
    // lv_obj_set_style_border_color(panel3,lv_color_make(90,100,110),LV_PART_MAIN);
    
    lv_obj_t * label = lv_label_create(panel3);
    lv_label_set_text_fmt(label,"%d",23);
    lv_obj_align(label,LV_ALIGN_CENTER,0,0);
    lv_obj_set_style_bg_opa(label,0,LV_PART_MAIN);
    //lv_obj_set_style_border_width(label,0,LV_PART_MAIN);
    //lv_obj_set_style_border_color(label,0,LV_PART_MAIN);
    //lv_style_set_text_font(label,&LV_FONT_MONTSERRAT_18,LV_PART_MAIN);
    lv_obj_set_style_text_color(panel3,lv_color_make(255,255,255),LV_PART_MAIN);
    lv_obj_add_event_cb(arc, value_changed_event_cb, LV_EVENT_VALUE_CHANGED, label);


    lv_obj_remove_style(arc, NULL, LV_PART_KNOB);
    lv_obj_set_style_arc_width(arc,5,LV_PART_MAIN);
    lv_obj_set_style_arc_width(arc,5,LV_PART_INDICATOR);
    lv_obj_set_style_arc_color(arc, lv_color_hex(0x0f0f0f), LV_PART_MAIN);
    lv_obj_set_style_arc_rounded(arc, true, LV_PART_MAIN);


    //lv_obj_center(arc);

    /*Manually update the label for the first time*/
    lv_obj_send_event(arc, LV_EVENT_VALUE_CHANGED,NULL);
    return arc;

}
lv_obj_t * lv_example_arc_3(void)
{
    /*Create an Arc*/
    lv_obj_t * arc = lv_arc_create(bg);
    lv_obj_set_size(arc, 60, 60);
    //lv_arc_set_rotation(arc, 135);
    lv_arc_set_bg_angles(arc, 120, 60);
    lv_arc_set_range(arc,0,100);
    lv_arc_set_value(arc,23);
    lv_obj_align(arc,LV_ALIGN_TOP_RIGHT,0,0);
    lv_obj_set_style_radius(arc,360,LV_PART_MAIN);
    lv_obj_set_style_bg_opa(arc,LV_OPA_COVER,LV_PART_MAIN);
    lv_obj_set_style_bg_color(arc,lv_color_make(30,35,45),LV_PART_MAIN);
    lv_obj_set_style_pad_all(arc,3,LV_PART_MAIN);
    
    lv_obj_t * panel3 = lv_obj_create(arc);
     lv_obj_set_size(panel3,42,42);
     lv_obj_align(panel3,LV_ALIGN_CENTER,0,0);
    // lv_obj_set_style_radius(panel3,360,LV_PART_MAIN);
    // lv_obj_set_style_bg_color(panel3,lv_color_make(100,100,100),LV_PART_MAIN);
    // lv_obj_set_style_bg_grad_color(panel3,lv_color_make(60,65,75),LV_PART_MAIN);
    // lv_obj_set_style_bg_grad_dir(panel3,LV_GRAD_DIR_VER,LV_PART_MAIN);
    // lv_obj_set_style_arc_width(panel3,0.2,LV_PART_MAIN);
    // lv_obj_set_style_shadow_color(panel3,lv_color_make(5,10,15),LV_PART_MAIN);
    // lv_obj_set_style_shadow_width(panel3,8,LV_PART_MAIN);
    // lv_obj_set_style_shadow_spread(panel3,0,LV_PART_MAIN);
    // lv_obj_set_style_shadow_offset_x(panel3,0,LV_PART_MAIN);
    // lv_obj_set_style_shadow_offset_y(panel3,3,LV_PART_MAIN);
    // lv_obj_t * panel_value = lv_obj_create(panel3);
    // lv_obj_set_size(panel_value,30,30);
    // lv_obj_align_to(panel_value,arc,LV_ALIGN_CENTER,0,0);
    // lv_obj_set_style_radius(panel_value,360,LV_PART_MAIN);
    // lv_obj_set_style_bg_color(panel_value,lv_color_make(12,25,30),LV_PART_MAIN);
    // lv_obj_set_style_bg_grad_color(panel_value,lv_color_make(25,28,38),LV_PART_MAIN);
    // lv_obj_set_style_bg_grad_dir(panel_value,LV_GRAD_DIR_VER,LV_PART_MAIN);
    // lv_obj_set_style_arc_width(panel_value,0.2,LV_PART_MAIN);
    // lv_obj_set_style_border_color(panel3,lv_color_make(90,100,110),LV_PART_MAIN);
    
    lv_obj_t * label = lv_label_create(panel3);
    lv_label_set_text_fmt(label,"%d",23);
    lv_obj_align(label,LV_ALIGN_CENTER,0,0);
    lv_obj_set_style_bg_opa(label,0,LV_PART_MAIN);
    //lv_obj_set_style_border_width(label,0,LV_PART_MAIN);
    //lv_obj_set_style_border_color(label,0,LV_PART_MAIN);
    //lv_style_set_text_font(label,&LV_FONT_MONTSERRAT_18,LV_PART_MAIN);
    lv_obj_set_style_text_color(panel3,lv_color_make(255,255,255),LV_PART_MAIN);
    lv_obj_add_event_cb(arc, value_changed_event_cb, LV_EVENT_VALUE_CHANGED, label);


    lv_obj_remove_style(arc, NULL, LV_PART_KNOB);
    lv_obj_set_style_arc_width(arc,5,LV_PART_MAIN);
    lv_obj_set_style_arc_width(arc,5,LV_PART_INDICATOR);
    lv_obj_set_style_arc_color(arc, lv_color_hex(0x0f0f0f), LV_PART_MAIN);
    lv_obj_set_style_arc_rounded(arc, true, LV_PART_MAIN);


    //lv_obj_center(arc);

    /*Manually update the label for the first time*/
    //lv_obj_send_event(arc, LV_EVENT_VALUE_CHANGED,NULL);

    return arc;

}
static void value_changed_event_cb(lv_event_t * e)
{
    lv_obj_t * arc = lv_event_get_target_obj(e);
    lv_obj_t * label =(lv_obj_t*)lv_event_get_user_data(e);
    int32_t value = lv_arc_get_value(arc);
    //Serial.printf("value%llu\n",value);

    lv_label_set_text_fmt(label,"%d",value);
    LV_LOG_USER(value);
}
lv_obj_t * lv_net_label_up(void)
{
    lv_obj_t * label_up = lv_label_create(bg_bottom);
    lv_obj_align(label_up,LV_ALIGN_TOP_LEFT,-5,0);
    lv_label_set_text_fmt(label_up,"u%dMB",30);

    return label_up;
    
}
lv_obj_t * lv_net_label_down(void)
{
    lv_obj_t * label_down = lv_label_create(bg_bottom);
    lv_obj_align(label_down,LV_ALIGN_TOP_LEFT,-5,22);
    lv_label_set_text_fmt(label_down,"n%dMB",40);
    return label_down;
    
}
lv_obj_t * lv_example_chart_1(void)
{
    /*Create a chart*/
    chart = lv_chart_create(bg_bottom);
    lv_obj_set_size(chart, 150, 40);
    lv_obj_align(chart,LV_ALIGN_TOP_RIGHT,0,0);
    lv_chart_set_type(chart, LV_CHART_TYPE_LINE);   /*Show lines and points too*/
    lv_chart_set_update_mode(chart,LV_CHART_UPDATE_MODE_SHIFT);
    lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_Y, 0, 5000);
    /*Add two data series*/
    ser1 = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_GREEN), LV_CHART_AXIS_PRIMARY_Y);
    ser2 = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);

    uint32_t i;
    for(i = 0; i < 10; i++) {
        /*Set the next points on 'ser1'*/
        lv_chart_set_next_value(chart, ser1, lv_rand(10, 50));
        lv_chart_set_next_value(chart, ser2, lv_rand(10, 50));
    }

    lv_chart_refresh(chart); /*Required after direct set*/

    return chart;
}
lv_obj_t * lv_example_bar_3(void)
{
    static lv_style_t style_indic;

    lv_style_init(&style_indic);
    lv_style_set_bg_opa(&style_indic, LV_OPA_COVER);
    lv_style_set_bg_color(&style_indic, lv_palette_main(LV_PALETTE_BLUE));
    lv_style_set_bg_grad_color(&style_indic, lv_palette_main(LV_PALETTE_RED));
    lv_style_set_bg_grad_dir(&style_indic, LV_GRAD_DIR_HOR);


    lv_obj_t * bar = lv_bar_create(bg_bottom);
    lv_obj_add_style(bar, &style_indic, LV_PART_INDICATOR);
    lv_obj_set_size(bar, 130, 10);
    lv_obj_align(bar,LV_ALIGN_BOTTOM_LEFT,70,0);
    lv_bar_set_range(bar, 0, 60);
    lv_obj_t * barlabel = lv_label_create(bg_bottom);
    lv_label_set_text_fmt(barlabel,"CPU:%d°C",40);
    lv_obj_align_to(barlabel,bar,LV_ALIGN_OUT_LEFT_MID,-8,0);
    return bar;
}
lv_obj_t * lv_example_bar_4(void)
{
    static lv_style_t style_indic;

    // lv_style_init(&style_indic);
    // lv_style_set_bg_opa(&style_indic, LV_OPA_COVER);
    // lv_style_set_bg_color(&style_indic, lv_palette_main(LV_PALETTE_BLUE));
    // lv_style_set_bg_grad_color(&style_indic, lv_palette_main(LV_PALETTE_RED));
    // lv_style_set_bg_grad_dir(&style_indic, LV_GRAD_DIR_HOR);


    lv_obj_t * bar = lv_bar_create(bg_bottom);
    //lv_obj_add_style(bar, &style_indic, LV_PART_INDICATOR);
    lv_obj_set_size(bar, 130, 10);
    lv_obj_align(bar,LV_ALIGN_LEFT_MID,70,15);
    lv_bar_set_range(bar, 0, 60);
    lv_obj_t * barlabel = lv_label_create(bg_bottom);
    lv_label_set_text_fmt(barlabel,"Fans:%d%",40);
    lv_obj_align_to(barlabel,bar,LV_ALIGN_OUT_LEFT_MID,-8,0);
    return bar;
}




int TempControl(int mode,DataProcess * DP,FansControl *FC)
{
    // if(0 == mode)
    // {
    //     // if(DP->cpu_tmp <30)
    //     // {
    //     //     FC->SetSpeed(0);
    //     // }
    //     // else
    //     // {
    //     //     FC->SetSpeed(DP->cpu_tmp * 1.6);
    //     // }
    //     FC->SetSpeed(0);
    //     //Serial.printf("Fans Normal Mode.");
    //     return 1;

    // }
    // // mode = 0 normal mode
    // else if (1 == mode)
    // {
    //     //mode = 1 silent mode
    //     //Serial.printf("Fans Silent Mode.");
    //     return 1;
    // }
    
    // else if (2 == mode)
    // {
    //     //mode = 2 full mode 
    //     //Serial.printf("Fans Full Mode.");
    //     return 1;
    // }
    // else
    // {
    //     //Serial.printf("TempControlStartFailed.");
    //     return -1;
    // }

    Serial.printf("1");
    
    

   

}
