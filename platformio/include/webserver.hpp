#include <WebServer.h>
#include <ArduinoJson.h>

String setApplicationIp(WebServer server)
{
    String postBody = server.arg("plain");
    Serial.println(postBody);

    DynamicJsonDocument doc(512);
    DeserializationError error = deserializeJson(doc, postBody);

    if(error) {
        Serial.print(F("Error parsing JSON "));
        Serial.println(error.c_str());
 
        String msg = error.c_str();
 
        server.send(400, F("text/html"),
                    "Error in parsin json body! <br>" + msg);
    } else {
        JsonObject obj = doc.as<JsonObject>();
        if (server.method() == HTTP_POST) {
            if (obj.containsKey("name") && obj.containsKey("type")) {
 
                Serial.println(F("done."));
 
                // Here store data or doing operation
 
 
                // Create the response
                // To get the status of the result you can get the http status so
                // this part can be unusefully
                DynamicJsonDocument doc(512);
                doc["status"] = "OK";
 
                Serial.print(F("Stream..."));
                String buf;
                serializeJson(doc, buf);
 
                server.send(201, F("application/json"), buf);
                Serial.print(F("done."));
 
            } else {
                DynamicJsonDocument doc(512);
                doc["status"] = "KO";
                doc["message"] = F("No data found, or incorrect!");
 
                Serial.print(F("Stream..."));
                String buf;
                serializeJson(doc, buf);
 
                server.send(400, F("application/json"), buf);
                Serial.print(F("done."));
            }
        }
    }
}

void restServerRouting(WebServer server) {
    server.on("/", HTTP_GET, []() {
        server.send(200, F("text/html"),
            F("Welcome to the REST Web Server"));
    });
    // handle post request
    server.on(F("/setRoom"), HTTP_POST, setRoom);
}