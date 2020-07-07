#include <RtcDS1307.h>
#include <Wire.h>

void saveWiFiToRtcMemory(RtcDS1307<TwoWire> &Rtc, String ssid, String password)
{
    char _ssid[24] = "-----------------------";
    char _password[24] = "-----------------------";
	Rtc.SetMemory(3, (const uint8_t*)_ssid, sizeof(_ssid));
	Rtc.SetMemory(28, (const uint8_t*)_password, sizeof(_password));

    for(int i = 0; i < 24; i++)
    {
      _ssid[i] = ssid[i];
      _password[i] = password[i];
    }

	Rtc.SetMemory(53, 1);
    Rtc.SetMemory(3, (const uint8_t*)_ssid, sizeof(_ssid)-1);
    Rtc.SetMemory(28, (const uint8_t*)_password, sizeof(_password)-1);
}

String getCharArrrayFromRTC(RtcDS1307<TwoWire> &Rtc, int address)
{
  uint8_t buff[24];
  uint8_t gotten = Rtc.GetMemory(address, buff, sizeof(buff));
  String result = "";
  for (uint8_t ch = 0; ch < gotten; ch++)
	  if((char)buff[ch] != ' ' && (char)buff[ch] != '\0' && isAlphaNumeric((char)buff[ch]))
    	result.concat((char)buff[ch]);

  Serial.println(result.c_str());
  return result;
}

String getMainTimestamp(RtcDS1307<TwoWire> &Rtc)
{
  RtcDateTime timestamp = Rtc.GetDateTime();
  String time = (String)timestamp.Year()+"-";

  if(timestamp.Month()<10)
    time+="0"+(String)timestamp.Month()+"-";
  else
    time+=(String)timestamp.Month()+"-";

  if(timestamp.Day()<10)
    time+="0"+(String)timestamp.Day()+" ";
  else
    time+=(String)timestamp.Day()+" ";
  
  if(timestamp.Hour()<10)
    time+="0"+(String)timestamp.Hour()+":";
  else
    time+=(String)timestamp.Hour()+":";
  
  if(timestamp.Minute()<10)
    time+="0"+(String)timestamp.Minute()+":";
  else
    time+=(String)timestamp.Minute()+":";
  
  if(timestamp.Second()<10)
    time+="0"+(String)timestamp.Second();
  else
    time+=(String)timestamp.Second();

  return time;
}

void configTime(RtcDS1307<TwoWire> &Rtc)
{
    Rtc.Begin();
    time_t rawtime;
    struct tm* timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    char year[5];
    char month[5];
    strftime (year, sizeof(year), "%Y",timeinfo);
    strftime (month, sizeof(month), "%m", timeinfo);
    RtcDateTime date = RtcDateTime(atoi(year), atoi(month), timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
    Rtc.SetDateTime(date);
    Rtc.SetMemory(1, 1);
}

String getDate(RtcDS1307<TwoWire> &Rtc)
{
    RtcDateTime dt = Rtc.GetDateTime();
    char datestring[20];
    snprintf_P(datestring, 
            20,
            PSTR("%02u.%02u.%04u"),
            dt.Day(),
            dt.Month(),
            dt.Year());
    return String(datestring);
}

String getTime(RtcDS1307<TwoWire> &Rtc)
{
    RtcDateTime dt = Rtc.GetDateTime();
    char timestring[20];
    snprintf_P(timestring,
            20,
            PSTR("%02u:%02u:%02u"),
            dt.Hour(),
            dt.Minute(),
            dt.Second()
    );
    return String(timestring);
}