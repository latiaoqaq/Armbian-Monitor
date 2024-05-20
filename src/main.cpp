// #include <Arduino.h>
// #include <WiFi.h>
// #include <HTTPClient.h>
// #include <ArduinoJson.h>
// #include <DataProcess.h>
// #include <FansControl.h>
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


// DataProcess *DP = new DataProcess();
// FansControl *FC = new FansControl(11,5000,0,8);


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

#if LV_USE_TFT_ESPI
#include <TFT_eSPI.h>
#endif
static void value_changed_event_cb(lv_event_t * e);
void lv_example_arc_1(void);
/*To use the built-in examples and demos of LVGL uncomment the includes below respectively.
 *You also need to copy `lvgl/examples` to `lvgl/src/examples`. Similarly for the demos `lvgl/demos` to `lvgl/src/demos`.
 *Note that the `lv_examples` library is for LVGL v7 and you shouldn't install it for this version (since LVGL v8)
 *as the examples and demos are now part of the main LVGL library. */

//#include <examples/lv_examples.h>
//#include <demos/lv_demos.h>

/*Set to your screen resolution*/
#define TFT_HOR_RES   240
#define TFT_VER_RES   240

/*LVGL draw into this buffer, 1/10 screen size usually works well. The size is in bytes*/
#define DRAW_BUF_SIZE (TFT_HOR_RES * TFT_VER_RES / 10 * (LV_COLOR_DEPTH / 8))
uint32_t draw_buf[DRAW_BUF_SIZE / 4];

#if LV_USE_LOG != 0
void my_print( lv_log_level_t level, const char * buf )
{
    LV_UNUSED(level);
    Serial.println(buf);
    Serial.flush();
}
#endif

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

/*Read the touchpad*/
void my_touchpad_read( lv_indev_t * indev, lv_indev_data_t * data )
{
    /*For example  ("my_..." functions needs to be implemented by you)
    int32_t x, y;
    bool touched = my_get_touch( &x, &y );

    if(!touched) {
        data->state = LV_INDEV_STATE_RELEASED;
    } else {
        data->state = LV_INDEV_STATE_PRESSED;

        data->point.x = x;
        data->point.y = y;
    }
     */
}

void setup()
{
    String LVGL_Arduino = "Hello Arduino! ";
    LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();

    Serial.begin( 115200 );
    Serial.println( LVGL_Arduino );

    lv_init();

    /*Set a tick source so that LVGL will know how much time elapsed. */
    lv_tick_set_cb(millis);

    /* register print function for debugging */
#if LV_USE_LOG != 0
    lv_log_register_print_cb( my_print );
#endif

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

    /*Initialize the (dummy) input device driver*/
    lv_indev_t * indev = lv_indev_create();
    lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER); /*Touchpad should have POINTER type*/
    lv_indev_set_read_cb(indev, my_touchpad_read);

    /* Create a simple label
     * ---------------------
     lv_obj_t *label = lv_label_create( lv_scr_act() );
     lv_label_set_text( label, "Hello Arduino, I'm LVGL!" );
     lv_obj_align( label, LV_ALIGN_CENTER, 0, 0 );

     * Try an example. See all the examples
     *  - Online: https://docs.lvgl.io/master/examples.html
     *  - Source codes: https://github.com/lvgl/lvgl/tree/master/examples
     * ----------------------------------------------------------------

     lv_example_btn_1();

     * Or try out a demo. Don't forget to enable the demos in lv_conf.h. E.g. LV_USE_DEMOS_WIDGETS
     * -------------------------------------------------------------------------------------------

     
     */

    //lv_demo_widgets();
    lv_obj_t * src = lv_screen_active();
    lv_obj_set_pos(src,0,0);
    lv_obj_set_size(src,240,240);
    lv_obj_set_style_bg_color(src,lv_color_hex(0x3B2D23),0);
    //lv_obj_set_style_bg_opa(src,50,0);
    lv_example_arc_1();
    Serial.println( "Setup done" );
}

void loop()
{
    lv_task_handler(); /* let the GUI do its work */
    delay(5); /* let this time pass */
}

static void set_angle(void * obj, int32_t v)
{
    lv_arc_set_value((lv_obj_t*)obj, v);
}

void lv_example_arc_1(void)
{
    lv_obj_t * label = lv_label_create(lv_screen_active());

    /*Create an Arc*/
    lv_obj_t * arc = lv_arc_create(lv_screen_active());
    lv_obj_set_size(arc, 70, 70);
    lv_arc_set_rotation(arc, 135);
    lv_arc_set_bg_angles(arc, 0, 270);
    lv_arc_set_value(arc, 10);
    lv_obj_set_pos(arc,40,40);
    lv_obj_set_pos(label,40,40);
    lv_obj_remove_style(arc, NULL, LV_PART_KNOB);
    lv_obj_set_style_arc_width(arc,10,LV_PART_MAIN);
    lv_obj_set_style_arc_width(arc,10,LV_PART_INDICATOR);
    lv_obj_set_style_arc_color(arc, lv_color_hex(0x0f0f0f), LV_PART_MAIN);
    lv_obj_set_style_arc_rounded(arc, true, LV_PART_MAIN);
    //lv_obj_center(arc);
    lv_obj_add_event_cb(arc, value_changed_event_cb, LV_EVENT_VALUE_CHANGED, label);

    /*Manually update the label for the first time*/
    lv_obj_send_event(arc, LV_EVENT_VALUE_CHANGED, NULL);

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, arc);
    lv_anim_set_exec_cb(&a, set_angle);
    lv_anim_set_duration(&a, 1000);
    lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);    /*Just for the demo*/
    lv_anim_set_repeat_delay(&a, 500);
    lv_anim_set_values(&a, 0, 100);
    lv_anim_start(&a);

}

static void value_changed_event_cb(lv_event_t * e)
{
    lv_obj_t * arc = lv_event_get_target_obj(e);
    lv_obj_t * label =(lv_obj_t*)lv_event_get_user_data(e);

    lv_label_set_text_fmt(label, "%" LV_PRId32 "%%", lv_arc_get_value(arc));

    /*Rotate the label to the current position of the arc*/
    //lv_arc_rotate_obj_to_angle(arc, label, 25);
}


