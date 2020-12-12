#include <LVGLFunctions.hpp>
void inactive_screen(lv_task_t *task)
{
    if (config.lcdLockTime != -1)
    {
        if (lv_disp_get_inactive_time(NULL) > config.lcdLockTime)
        {
            if (lv_scr_act() != lock_scr)
                lv_disp_load_scr(lock_scr);
        }
    }
}

void fetchLastRecordAndSynchronize(lv_task_t *task)
{

    if (WiFi.status() == WL_CONNECTED && appIpAddress != "")
    {
        Serial.println("Starting synchronizing process...");
        HTTPClient getHttp;
        String url = "http://" + appIpAddress + "/fetch/last";
        Serial.print(url);
        if (getHttp.begin(url.c_str()))
        {
            Serial.print(getHttp.getString());
            uint8_t responseCode = getHttp.GET();

            if (responseCode >= 200 and responseCode < 300)
            {
                Serial.println("GET /last succesful.");
                Serial.println("HTTP RESPONSE CODE: " + (String)responseCode);
                StaticJsonDocument<600> response, doc1;

                DeserializationError err = deserializeJson(response, getHttp.getString());

                JsonArray lastRecord = doc1.to<JsonArray>();

                mySDCard.getLastRecord(&sampleDB, &Serial, &lastRecord);
                DynamicJsonDocument doc(33000);
                if ((response[0]["timestamp"].as<String>() != lastRecord[0]["timestamp"].as<String>()) || (response[0]["timestamp"].as<String>() == "null"))
                {
                    Serial.println("Got last record that looks good. Parsing and sending data to Server App...");
                    JsonArray records = doc.to<JsonArray>();
                    mySDCard.select(&sampleDB, &Serial, response[0]["timestamp"].as<String>(), &records);
                    String json = "";

                    serializeJson(doc, json);
                    getHttp.begin("http://" + appIpAddress + "/submit");
                    getHttp.addHeader("Content-Type", "application/json");
                    getHttp.POST(json);
                    Serial.print("POST RESPONSE:" + getHttp.getString());
                    getHttp.end();
                    //lv_task_ready(getAppLastRecordAndSynchronize);
                }
            }
            else
            {
                Serial.println("ERROR FETCHING DATA. CODE: " + (String)responseCode);
            }
        }
        else
        {
            Serial.print("Wrong url");
        }
        getHttp.end();
    }
}

void config_time(lv_task_t *task)
{
    if (WiFi.status() == WL_CONNECTED)
    {
        for (int i = 0; i < 500; i++)
            dateTimeClient.update();
        configTime(Rtc, dateTimeClient);
        wasUpdated = true;
        Serial.println("Succesfully updated data on RTC.");
    }
    else
    {
        wasUpdated = false;
    }
}

void dateTimeFunc(lv_task_t *task)
{

    if (Rtc.GetIsRunning())
    {
        lv_label_set_text(dateAndTimeAtBar, getMainTimestamp(Rtc).c_str());
        lv_label_set_text(labelTimeLock, getTime(Rtc).c_str());
        lv_label_set_text(labelDateLock, getDate(Rtc).c_str());
        if (in_time_settings == false)
        {
            lv_spinbox_set_value(time_hour, getTime(Rtc).substring(0, getTime(Rtc).indexOf(":")).toInt());
            lv_spinbox_set_value(time_minute, getTime(Rtc).substring(3, 5).toInt());
            lv_label_set_text(date_btn_label, getDate(Rtc).c_str());
        }
    }
    else
    {
        if (in_time_settings == false)
            lv_label_set_text(date_btn_label, "01.01.2020");
    }
}

void statusFunc(lv_task_t *task)
{
    if (WiFi.status() == WL_CONNECTED)
    {
        lv_obj_set_hidden(wifiStatusAtLockWarning, true);
        lv_obj_set_hidden(wifiStatusAtMainWarning, true);
        lv_label_set_text(info_wifi_address_label, WiFi.localIP().toString().c_str());
    }
    else
    {
        lv_obj_set_hidden(wifiStatusAtLockWarning, false);
        lv_obj_set_hidden(wifiStatusAtMainWarning, false);
        lv_label_set_text(info_wifi_address_label, "No WiFi connection.");
    }

    if (mySDCard.start(&sampleDB, &Serial2))
    {
        lv_obj_set_hidden(sdStatusAtLockWarning, true);
        lv_obj_set_hidden(sdStatusAtMainWarning, true);
        if (config.ssid == "" && config.password == "")
        {
            mySDCard.loadWiFi(config, configFilePath);
            mySDCard.saveConfig(config, configFilePath);
        }
        if (config.ssid != "" && config.password != "")
        {
            WiFi.begin(config.ssid.c_str(), config.password.c_str());
        }
    }
    else
    {
        lv_obj_set_hidden(sdStatusAtLockWarning, false);
        lv_obj_set_hidden(sdStatusAtMainWarning, false);
    }
}