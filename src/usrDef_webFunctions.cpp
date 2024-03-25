#include "usrDef_webFunctions.h"
#include "usrDef_WiFiConfig.h"
#include "html.h"
#include "css.h"
#include "javascript.h"

#define __serverReadyLED 2

extern AsyncWebServer server;
extern AsyncWebSocket ws;

extern uint8_t modeChanged;

extern String serverCommand;
extern String __commandFXID;
extern String __commandAttr1;
extern String __commandAttr2;
extern String __commandAttr3;
extern String __commandAttr4;
extern String __commandAttr5;

void __initServerNow(char *message)
{
    if (!Serial.available())
        Serial.begin(115200);

    pinMode(__serverReadyLED, OUTPUT);

    // Attempt to conenct to wifi and display local IP ---------
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    Serial.println();
    while (WiFi.status() != WL_CONNECTED)
    {
        digitalWrite(__serverReadyLED, 1);
        delay(600);
        Serial.println("Connecting to WiFi..");
        digitalWrite(__serverReadyLED, 0);
    }
    if (useStaticIP)
        WiFi.config(WIFI_STATIC_IP, WIFI_DEFAULT_GATEWAY, WIFI_SUBNET_MASK);

    Serial.println();
    Serial.println("Successful. Current Wifi IP:");
    Serial.println(WiFi.localIP());
    delay(2000);
    digitalWrite(__serverReadyLED, 1);

    // Upload HTML, CSS, and JS to server ----------------------
    server.on(
        "/",
        HTTP_GET,
        [](AsyncWebServerRequest* request)
        {
            request->send_P(200, "text/html", index_html);
        }
    );
    server.on(
        "/style_css",
        HTTP_GET,
        [](AsyncWebServerRequest* request)
        {
            request->send_P(200, "text/css", style_css);
        }
    );
    server.on(
        "/script_js",
        HTTP_GET,
        [](AsyncWebServerRequest* request)
        {
            request->send_P(200, "text/js", script_js);
        }
    );
    server.on(
        "/data", 
        HTTP_POST, 
        [](AsyncWebServerRequest* request)
        {
            String command = request->arg("command");
            serverCommand = command;
            request->send(200);
            modeChanged = 1;
        }
    );

    // Confirm uploading and start the server ------------------
    Serial.println();
    Serial.println("Server commencing...");
    server.begin();
    delay(2500);
    digitalWrite(__serverReadyLED, 0);
    Serial.println("");
    Serial.println("Server ready!");
    delay(500);
    Serial.end();
}

uint32_t __serverCommandToHex(String inputCommand)
{
    uint32_t hexValue;

    hexValue = strtoul(inputCommand.c_str(), NULL, 16);

    return hexValue;
}

String __serverReadCommandUntilTerminator(String commandToRead,
                   uint8_t startFromWhere,
                   uint8_t *endPosition,
                   char terminator)
{
    uint8_t iterate;
    String  commandToReturn;

    for (iterate = startFromWhere;
         iterate < commandToRead.length();
         iterate++)
    {
        if (commandToRead[iterate] == terminator)
        {
            *endPosition = iterate;
            break;
        }
        commandToReturn += commandToRead[iterate];
    }

    return commandToReturn;
}

void __serverAnalyzeCommandNow(uint8_t enableSerial_commandDisplaying)
{
    uint8_t __commandIterate = 0;

    __commandFXID = "";
    __commandAttr1 = "";
    __commandAttr2 = "";
    __commandAttr3 = "";
    __commandAttr4 = "";
    __commandAttr5 = "";

    __commandFXID += __serverReadCommandUntilTerminator(serverCommand, __commandIterate, &__commandIterate);
    __commandIterate++;
    __commandAttr1 += __serverReadCommandUntilTerminator(serverCommand, __commandIterate, &__commandIterate);
    __commandIterate++;
    __commandAttr2 += __serverReadCommandUntilTerminator(serverCommand, __commandIterate, &__commandIterate);
    __commandIterate++;
    __commandAttr3 += __serverReadCommandUntilTerminator(serverCommand, __commandIterate, &__commandIterate);
    __commandIterate++;
    __commandAttr4 += __serverReadCommandUntilTerminator(serverCommand, __commandIterate, &__commandIterate);
    __commandIterate++;
    __commandAttr5 += __serverReadCommandUntilTerminator(serverCommand, __commandIterate, &__commandIterate);
    __commandIterate++;

    if (!enableSerial_commandDisplaying)
        return;

    if (!Serial.available())
        Serial.begin(115200);

    Serial.print(__commandFXID);
    Serial.print("    ");
    Serial.print(__commandAttr1);
    Serial.print("    ");
    Serial.print(__commandAttr2);
    Serial.print("    ");
    Serial.print(__commandAttr3);
    Serial.print("    ");
    Serial.print(__commandAttr4);
    Serial.print("    ");
    Serial.print(__commandAttr5);
    Serial.print("\n");
}