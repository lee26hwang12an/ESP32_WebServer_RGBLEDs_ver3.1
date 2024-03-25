#include "usrDef_webFunctions.h"
#include "usrDef_LEDsFunctions.h"
#include "usrDef_defaultColors.h"
#include <WiFi.h>
#include <esp_now.h>


/*
================================================================
    MISCELLANEOUS
================================================================
*/
uint8_t modeChanged;

uint32_t __prevModeColor;
uint32_t __prevModeGradient[__LED_amount];

uint32_t __colorBank2[4] = {
    0xFF0000, 0x0000FF, 0x00FF00, 0xFF0000
};



/*
================================================================
    WEB SERVER INSTANCING
================================================================
*/
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

String serverCommand = "efxviii-0-150-100-fe04d1-150";
String __commandFXID;
String __commandAttr1;
String __commandAttr2;
String __commandAttr3;
String __commandAttr4;
String __commandAttr5;


/*
================================================================
    LEDS INSTANCING
================================================================
*/
CRGB LEDs[__LED_amount];


/*
================================================================
    SETUP
================================================================
*/
void setup()
{
    __initLEDsNow("300 bulbs, brightness 100");
    __initServerNow("Server on port 80, websocket on /ws");

    modeChanged = 0;
    __prevModeColor = 0;
    for (int i = 0; i < __LED_amount; i++)
        __prevModeGradient[i] = 0;

    // Serial.begin(112500);
    // delay(1000);
    // Serial.println();
    // Serial.println("================");
    // Serial.println("    ALL DONE");
    // Serial.println("================");
    // delay(500);
    // Serial.end();
}


/*
================================================================
    LOOP
================================================================
*/
void loop()
{
    // SERVER LISTENING ----------------------------------------
    __serverAnalyzeCommandNow(0);

    if (__commandFXID == "efx0") // TURN OFF
    {
        modeChanged = 0;
        uint32_t __localColorBank[1] = {0};
        __transition2Colors(LEDs, 0, __LED_amount - 1,
                            __prevModeColor,
                            0);
        FastLED.setBrightness(0);
        __prevModeColor = 0;
        for (int i = 0; i < __LED_amount; i++)
            __prevModeGradient[i] = uint32_t(LEDs[i]);
    }
    else if (__commandFXID == "efxi") // SOLID COLOR
    {
        modeChanged = 0;
        FastLED.setBrightness(__commandAttr1.toInt());

        __transition2Colors(LEDs, 0, __LED_amount - 1,
                            __prevModeColor,
                            __serverCommandToHex(__commandAttr2));
        __prevModeColor = __serverCommandToHex(__commandAttr2);
        for (int i = 0; i <= 59; i++)
            __prevModeGradient[i] = __serverCommandToHex(__commandAttr2);
    }
    else if (__commandFXID == "efxii") // STATIC GRADIENT
    {
        modeChanged = 0;
        FastLED.setBrightness(__commandAttr1.toInt());
        uint32_t __localColorBank[] = {
            __serverCommandToHex(__commandAttr2),
            __serverCommandToHex(__commandAttr3)};
        __transitionToGradient(LEDs, 0, __LED_amount - 1,
                               __prevModeGradient,
                               __localColorBank,
                               2);
        for (int i = 0; i <= 59; i++)
            __prevModeGradient[i] = uint32_t(LEDs[i]);
    }
    else if (__commandFXID == "efxiii") // BREATHING FX
    {
        modeChanged = 0;
        __colorsBreathing(LEDs, 0, __LED_amount - 1,
                          DEFAULT_breathingColorBank, 11);
    }
    else if (__commandFXID == "efxiv") // CHAMELEON FX
    {
        modeChanged = 0;
        FastLED.setBrightness(100);
        __colorsShifting(LEDs, 0, __LED_amount - 1);
    }
    else if (__commandFXID == "efxv") // RAINBOW PARADE
    {
        modeChanged = 0;
        FastLED.setBrightness(__commandAttr1.toInt());
        __paintGradient_now(LEDs, 0, __LED_amount - 1,
                            DEFAULT_rainbowColorBank, 6);
        __rotateLEDs(LEDs,
                     __commandAttr2.toInt(),
                     __commandAttr3.toInt());
    }
    else if (__commandFXID == "cfxi")
    {
        modeChanged = 0;
        FastLED.setBrightness(100);
        __paintGradient_now(LEDs, 10, 49, __colorBank2, 2);
    }

    // FOR TESTING ---------------------------------------------
}


/*
================================================================
    CUSTOM FUNCTIONS
================================================================
*/