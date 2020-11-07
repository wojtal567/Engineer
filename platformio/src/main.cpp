#include <Arduino.h>
#include <WiFi.h>
#include <rtc.hpp>
#include <HTTPClient.h>
#include <time.h>
#include <Adafruit_GFX.h>
#include <Adafruit_BusIO_Register.h>

#include <LVGLInits.hpp>

// ! --------------------------------------------REST WebServer config
void setAppIp()
{
    String postBody = server.arg("plain");
    Serial.println(postBody);
    DynamicJsonDocument doc(512);
    DeserializationError error = deserializeJson(doc, postBody);
    if (error)
    {
        Serial.print(F(error.c_str()));

        server.send(400, F("text/html"),
                    "Error while parsing json body! <br>" + (String)error.c_str());
    }
    else
    {
        JsonObject postObj = doc.as<JsonObject>();

        Serial.print(F("HTTP Method: "));
        Serial.println(server.method());

        if (server.method() == HTTP_POST)
        {
            if (postObj.containsKey("ip"))
            {

                appIpAddress = postObj["ip"].as<String>();

                DynamicJsonDocument doc(512);
                doc["status"] = "OK";
                String buf;
                serializeJson(doc, buf);

                server.send(201, F("application/json"), buf);
            }
            else
            {
                DynamicJsonDocument doc(512);
                doc["status"] = "OK";
                doc["message"] = F("No data found or incorrect!");

                String buf;
                serializeJson(doc, buf);

                server.send(400, F("application/json"), buf);
            }
        }
    }
}

void restServerRouting()
{
    server.on("/", HTTP_GET, []() {
        server.send(200, F("text/html"),
                    F("You have entered the wrong neighbourhood"));
    });
    server.on(F("/setAppIp"), HTTP_POST, setAppIp);
}

void handleNotFound()
{
    String message = "File Not Found \n\n" + (String) "URI: " + server.uri() + "\n Method: " + (server.method() == HTTP_GET) ? "GET" : "POST" + (String) "\n Arguments: " + server.args() + "\n";

    for (uint8_t i = 0; i < server.args(); i++)
        message += " " + server.argName(i) + ": " + server.arg(i) + "\n";

    server.send(404, "text/plain", message);
}

void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    uint16_t c;

    tft.startWrite();                                                                            /* Start new TFT transaction */
    tft.setAddrWindow(area->x1, area->y1, (area->x2 - area->x1 + 1), (area->y2 - area->y1 + 1)); /* set the working window */
    for (int y = area->y1; y <= area->y2; y++)
    {
        for (int x = area->x1; x <= area->x2; x++)
        {
            c = color_p->full;
            tft.writeColor(c, 1);
            color_p++;
        }
    }
    tft.endWrite();            /* terminate TFT transaction */
    lv_disp_flush_ready(disp); /* tell lvgl that flushing is done */
}

bool my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{
    uint16_t touchX, touchY;

    bool touched = tft.getTouch(&touchX, &touchY, 600);

    if (!touched)
    {
        return false;
    }

    if (touchX > SCREEN_WIDTH || touchY > SCREEN_HEIGHT)
    {
        //Serial.println("Y or y outside of expected parameters..");
        //Serial.print("y:");
        //Serial.print(touchX);
        //Serial.print(" x:");
        //Serial.print(touchY);
    }
    else
    {

        data->state = touched ? LV_INDEV_STATE_PR : LV_INDEV_STATE_REL;

        /*Save the state and save the pressed coordinate*/
        //if(data->state == LV_INDEV_STATE_PR) touchpad_get_xy(&last_x, &last_y);

        /*Set the coordinates (if released use the last pressed coordinates)*/
        data->point.x = touchX;
        data->point.y = touchY;

        //Serial.print("Data x");
        //Serial.println(touchX);
        //
        //Serial.print("Data y");
        //Serial.println(touchY);
    }

    return false; /*Return `false` because we are not buffering and no more data to read*/
}

int getDDListIndexBasedOnLcdLockTime(int lcdLockTime)
{
    switch (lcdLockTime)
    {
    case -1:
    {
        return 7;
        break;
    }

    case 30000:
    {
        return 0;
        break;
    }
    case 60000:
    {
        return 1;
        break;
    }
    case 120000:
    {
        return 2;
        break;
    }
    case 300000:
    {
        return 3;
        break;
    }
    case 600000:
    {
        return 4;
        break;
    }
    case 1800000:
    {
        return 5;
        break;
    }
    case 3600000:
    {
        return 6;
        break;
    }
    default:
    {
        return 1;
    }
    }
}

void setup()
{
    pinMode(FAN_PIN, OUTPUT);
    digitalWrite(FAN_PIN, LOW);
    sqlite3_initialize();
    //Serial debug
    Serial.begin(115200);
    Serial2.begin(9600, SERIAL_8N1, 16, 17);
    //PMS sensor initialization
    pmsSensor = new PMS5003(&Serial2, &Serial);

    lv_init();
    tft.begin(); /* TFT init */
    tft.setRotation(3);

    uint16_t calData[5] = {275, 3620, 264, 3532, 1};
    tft.setTouch(calData);

    lv_disp_buf_init(&disp_buf, buf, NULL, LV_HOR_RES_MAX * 10);
    //Initialize the display
    lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = SCREEN_WIDTH;
    disp_drv.ver_res = SCREEN_HEIGHT;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.buffer = &disp_buf;
    lv_disp_drv_register(&disp_drv);

    //Initialize the input device driver
    lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);          /*Descriptor of a input device driver*/
    indev_drv.type = LV_INDEV_TYPE_POINTER; /*Touch pad is a pointer-like device*/
    indev_drv.read_cb = my_touchpad_read;   /*Set your driver function*/
    lv_indev_drv_register(&indev_drv);      /*Finally register the driver*/

    //Set theme
    lv_theme_t *th = lv_theme_material_init(LV_THEME_DEFAULT_COLOR_PRIMARY, LV_THEME_DEFAULT_COLOR_SECONDARY, LV_THEME_DEFAULT_FLAG, LV_THEME_DEFAULT_FONT_SMALL, LV_THEME_DEFAULT_FONT_NORMAL, LV_THEME_DEFAULT_FONT_SUBTITLE, LV_THEME_DEFAULT_FONT_TITLE);
    lv_theme_set_act(th);

    //Styles initialization functions
    containerStyleInit();
    font12StyleInit();
    font16StyleInit();
    font20StyleInit();
    font22StyleInit();
    transparentButtonStyleInit();
    whiteButtonStyleInit();
    tinySymbolStyleInit();
    lineStyleInit();
    toastListStyleInit();

    main_scr = lv_cont_create(NULL, NULL);
    lv_obj_set_style_local_bg_color(main_scr, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    settings_scr = lv_cont_create(NULL, NULL);
    lv_obj_set_style_local_bg_color(settings_scr, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    info_scr = lv_cont_create(NULL, NULL);
    lv_obj_set_style_local_bg_color(info_scr, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    time_settings_scr = lv_cont_create(NULL, NULL);
    lv_obj_set_style_local_bg_color(time_settings_scr, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    wifi_scr = lv_cont_create(NULL, NULL);
    lv_obj_set_style_local_bg_color(wifi_scr, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    wifilist_scr = lv_cont_create(NULL, NULL);
    lv_obj_set_style_local_bg_color(wifilist_scr, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lock_scr = lv_cont_create(NULL, NULL);
    lv_obj_set_style_local_bg_color(lock_scr, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);

    //Screens initialization function
    main_screen();
    wifi_screen();
    lock_screen();
    settings_screen();
    info_screen();
    timesettings_screen();
    wifiList_screen();
    lv_disp_load_scr(main_scr);

    lv_dropdown_set_selected(lockScreenDDlist, getDDListIndexBasedOnLcdLockTime(config.lcdLockTime));

    date = lv_task_create(dateTimeStatusFunc, 800, LV_TASK_PRIO_MID, NULL);
    syn_rtc = lv_task_create_basic();
    lv_task_set_cb(syn_rtc, config_time);
    lv_task_set_period(syn_rtc, 3600000);
    lv_spinbox_set_value(measure_period_hour, ((config.timeBetweenSavingSample / 60000) / 60));
    lv_spinbox_set_value(measure_av_period, (config.measurePeriod / 1000));
    lv_spinbox_set_value(measure_number, config.countOfSamples);
    lv_spinbox_set_value(measure_period_minute, ((config.timeBetweenSavingSample / 60000) % 60));

    getSample = lv_task_create(getSampleFunc, config.timeBetweenSavingSample, LV_TASK_PRIO_HIGH, NULL);
    turnFanOn = lv_task_create(turnFanOnFunc, config.timeBetweenSavingSample - 299999, LV_TASK_PRIO_HIGHEST, NULL);
    inactive_time = lv_task_create(inactive_screen, 1, LV_TASK_PRIO_HIGH, NULL);
    listNetwork_task = lv_task_create(list_networks, 10000, LV_TASK_PRIO_OFF, NULL);

    getAppLastRecordAndSynchronize = lv_task_create_basic();
    lv_task_set_cb(getAppLastRecordAndSynchronize, fetchLastRecordAndSynchronize);
    lv_task_set_period(getAppLastRecordAndSynchronize, 300);
    mySDCard.loadConfig(config, configFilePath);
    if (config.ssid != "")
    {
        mySDCard.printConfig(configFilePath);
        Serial.print(getMainTimestamp(Rtc).c_str());
        WiFi.begin(config.ssid.c_str(), config.password.c_str());
        volatile int attempts = 0;
        while (WiFi.status() != WL_CONNECTED and attempts != 20)
        {
            delay(500);
            Serial.print(".");
            attempts++;
        }
        if (WiFi.status() == WL_CONNECTED)
        {
            Rtc.SetIsRunning(true);
            restServerRouting();
            server.onNotFound(handleNotFound);
            server.begin();
        }
    }
    delay(500);
    lv_task_ready(syn_rtc);
}

void loop()
{
    lv_task_handler(); /* let the GUI do its work */
    server.handleClient();
    delay(5);
}
