#include "usrDef_LEDsFunctions.h"

extern CRGB LEDs[];
extern uint8_t modeChanged;

void __initLEDsNow(char *message)
{
    if (!Serial.available())
        Serial.begin(115200);

    Serial.println();
    Serial.println("LEDs commencing...");
    FastLED.addLeds<__LED_type,
                    __dataPin,
                    __colorOrder>(LEDs,
                    __LED_amount).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(__brightness);

    modeChanged = 0;

    Serial.println();
    Serial.println("LEDs ready!");
    Serial.print("LEDs count: ");
    Serial.println(__LED_amount);

    __paintLEDs_lag(LEDs, 0, __LED_amount - 1, 0);
    delay(3000);
    Serial.end();
}

uint32_t __combineRGB(uint8_t red, uint8_t green, uint8_t blue)
{
    uint32_t RGB;

    RGB = (red * 0x010000) + (green * 0x000100) + (blue * 0x000001);

    return RGB;
}

uint8_t __separateRGB(uint32_t RGB, uint8_t whichColorToSeparate)
{
    uint8_t separatedColor;

    switch (whichColorToSeparate)
    {
        case 'r':
            separatedColor = RGB / 0x010000;
            break;
        case 'g':
            separatedColor = RGB / 0x000100;
            break;
        case 'b':
            separatedColor = RGB / 0x000001;
            break;
        case 'R':
            separatedColor = RGB / 0x010000;
            break;
        case 'G':
            separatedColor = RGB / 0x000100;
            break;
        case 'B':
            separatedColor = RGB / 0x000001;
            break;
        default:
            separatedColor = 0x000000;
            break;
    }

    return separatedColor;
}

void __paintLEDs_now(CRGB *LEDStrip,
                     uint16_t startLED, uint16_t endLED,
                     uint32_t color)
{
    uint16_t LEDsIteration;

    for (LEDsIteration = startLED; LEDsIteration <= endLED; LEDsIteration++)
        LEDStrip[LEDsIteration] = color;
    FastLED.show();
}

void __paintLEDs_lag(CRGB *LEDStrip,
                     uint16_t startLED, uint16_t endLED,
                     uint32_t color, uint32_t lagTime_ms)
{
    uint16_t LEDsIteration;
    uint32_t timeNow;

    for (LEDsIteration = startLED; LEDsIteration <= endLED; LEDsIteration++)
    {
        LEDStrip[LEDsIteration] = color;
        FastLED.show();
        timeNow = millis();
        while (millis() - timeNow <= lagTime_ms)
            if (modeChanged)
                return;
    }
}

void __paintBand_now(CRGB *LEDStrip,
                     uint16_t startLED, uint16_t endLED,
                     uint32_t *colorBank,
                     uint16_t howManyColors,
                     uint16_t LEDsPerColor)
{
    uint16_t LEDsIteration;
    uint16_t colorsIteration;
    uint16_t LEDsFilledWithThisColor;

    if (!LEDsPerColor)
        LEDsPerColor = (endLED - startLED + 1) / howManyColors;

    colorsIteration = 0;
    LEDsFilledWithThisColor = 0;
    for (LEDsIteration = startLED;
         LEDsIteration <= endLED;
         LEDsIteration++)
    {
        LEDStrip[LEDsIteration] = colorBank[colorsIteration];
        LEDsFilledWithThisColor = LEDsFilledWithThisColor + 1;
        if (LEDsFilledWithThisColor == LEDsPerColor)
        {
            LEDsFilledWithThisColor = 0;
            colorsIteration = colorsIteration + 1;
        }
        if (colorsIteration >= howManyColors)
            colorsIteration = 0;
    }
    FastLED.show();
}

void __paintBand_lag(CRGB *LEDStrip,
                     uint16_t startLED, uint16_t endLED,
                     uint32_t *colorBank, uint16_t howManyColors,
                     uint32_t lagTime_ms, uint16_t LEDsPerColor)
{
    uint16_t LEDsIteration;
    uint16_t colorsIteration;
    uint16_t LEDsFilledWithThisColor;
    uint32_t timeNow;

    if (!LEDsPerColor)
        LEDsPerColor = (endLED - startLED + 1) / howManyColors;

    colorsIteration = 0;
    LEDsFilledWithThisColor = 0;
    for (LEDsIteration = startLED;
         LEDsIteration <= endLED;
         LEDsIteration++)
    {
        LEDStrip[LEDsIteration] = colorBank[colorsIteration];
        LEDsFilledWithThisColor = LEDsFilledWithThisColor + 1;
        FastLED.show();
        if (LEDsFilledWithThisColor == LEDsPerColor)
        {
            LEDsFilledWithThisColor = 0;
            colorsIteration = colorsIteration + 1;
        }
        if (colorsIteration >= howManyColors)
            colorsIteration = 0;
        timeNow = millis();
        while (millis() - timeNow <= lagTime_ms)
            if (modeChanged)
                return;
    }
}

void __calculateGradient2Colors(CRGB *LEDStrip,
                                uint16_t startPos, uint16_t endPos,
                                uint32_t startColor, uint32_t endColor)
{
    int16_t  currentRed1;
    int16_t  currentGreen1;
    int16_t  currentBlue1;
    int16_t  currentRed2;
    int16_t  currentGreen2;
    int16_t  currentBlue2;
    uint16_t colorIterate;

    float    stepRed;
    float    stepGreen;
    float    stepBlue;

    colorIterate = 0;

    stepRed = (
        float(__separateRGB(startColor, 'r')) -
        float(__separateRGB(endColor, 'r'))
    ) / float(endPos - startPos);
    stepGreen = (
        float(__separateRGB(startColor, 'g')) -
        float(__separateRGB(endColor, 'g'))
    ) / float(endPos - startPos);
    stepBlue = (
        float(__separateRGB(startColor, 'b')) -
        float(__separateRGB(endColor, 'b'))
    ) / float(endPos - startPos);

    while (startPos <= endPos)
    {
        currentRed1 = __separateRGB(startColor, 'r') - colorIterate * stepRed;
        currentRed2 = __separateRGB(endColor, 'r') + colorIterate * stepRed;
        currentGreen1 = __separateRGB(startColor, 'g') - colorIterate * stepGreen;
        currentGreen2 = __separateRGB(endColor, 'g') + colorIterate * stepGreen;
        currentBlue1 = __separateRGB(startColor, 'b') - colorIterate * stepBlue;
        currentBlue2 = __separateRGB(endColor, 'b') + colorIterate * stepBlue;

        LEDStrip[startPos] = __combineRGB(currentRed1, currentGreen1, currentBlue1);
        LEDStrip[endPos] = __combineRGB(currentRed2, currentGreen2, currentBlue2);

        colorIterate++;
        startPos++;
        endPos--;
    }
}

void __paintGradient_now(CRGB *LEDStrip,
                         uint16_t startLED, uint16_t endLED,
                         uint32_t *colorBank, uint16_t howManyColors)
{
    uint16_t individualGradientLength;
    uint32_t storedGradient[endLED + 1];

    individualGradientLength = (endLED - startLED + 1) /
                               (howManyColors - 1);

    for (int iterateColors = 0; iterateColors < howManyColors - 1; iterateColors++)
        __calculateGradient2Colors(
            LEDStrip,
            startLED + iterateColors * individualGradientLength,
            startLED + (iterateColors + 1) * individualGradientLength - 1,
            colorBank[iterateColors],
            colorBank[iterateColors + 1]
        );
    
    for (int iterateLEDs = startLED; iterateLEDs <= endLED; iterateLEDs++)
        storedGradient[iterateLEDs] = uint32_t(LEDStrip[iterateLEDs]);

    FastLED.show();
}

void __transitionToGradient(CRGB *LEDStrip,
                            uint16_t startLED, uint16_t endLED,
                            uint32_t *prevGradient,
                            uint32_t *colorBank, uint16_t howManyColors,
                            uint32_t lagTime_ms)
{
    uint8_t  gradientIsDifferent;
    uint16_t individualGradientLength;
    uint32_t storedGradient[endLED + 1];
    uint32_t timeNow;

    gradientIsDifferent = 1;
    individualGradientLength = (endLED - startLED + 1) /
                               (howManyColors - 1);

    for (int iterateColors = 0; iterateColors < howManyColors - 1; iterateColors++)
        __calculateGradient2Colors(
            LEDStrip,
            startLED + iterateColors * individualGradientLength,
            startLED + (iterateColors + 1) * individualGradientLength - 1,
            colorBank[iterateColors],
            colorBank[iterateColors + 1]
        );

    for (int iterateLEDs = startLED; iterateLEDs <= endLED; iterateLEDs++)
    {
        storedGradient[iterateLEDs] = uint32_t(LEDStrip[iterateLEDs]);
        LEDStrip[iterateLEDs] = prevGradient[iterateLEDs];
    }

    while (gradientIsDifferent)
    {
        for (int iterateLEDs = startLED; iterateLEDs <= endLED; iterateLEDs++)
        {
            uint8_t currentRed = __separateRGB(uint32_t(LEDStrip[iterateLEDs]), 'r');
            uint8_t currentGreen = __separateRGB(uint32_t(LEDStrip[iterateLEDs]), 'g');
            uint8_t currentBlue = __separateRGB(uint32_t(LEDStrip[iterateLEDs]), 'b');
            if (currentRed < __separateRGB(storedGradient[iterateLEDs], 'r'))
                currentRed++;
            else if (currentRed > __separateRGB(storedGradient[iterateLEDs], 'r'))
                currentRed--;
            if (currentGreen < __separateRGB(storedGradient[iterateLEDs], 'g'))
                currentGreen++;
            else if (currentGreen > __separateRGB(storedGradient[iterateLEDs], 'g'))
                currentGreen--;
            if (currentBlue < __separateRGB(storedGradient[iterateLEDs], 'b'))
                currentBlue++;
            else if (currentBlue > __separateRGB(storedGradient[iterateLEDs], 'b'))
                currentBlue--;

            LEDStrip[iterateLEDs] = __combineRGB(currentRed, currentGreen, currentBlue);
        }
        FastLED.show();
        timeNow = millis();
        while (millis() - timeNow <= lagTime_ms) {}
        for (int iterateLEDs = startLED; iterateLEDs <= endLED; iterateLEDs++)
        {
            gradientIsDifferent = 1;
            if (uint32_t(LEDStrip[iterateLEDs]) != storedGradient[iterateLEDs])
                break;
            gradientIsDifferent = 0;
        }
    }
}

void __rotateLEDs(CRGB *LEDStrip,
                  uint8_t direction,
                  uint32_t duration_ms)
{
    uint32_t timeNow;
    uint32_t timePerLED;

    timePerLED = duration_ms / __LED_amount;

    while (!modeChanged)
    {
        if (direction)
            for (int iterateLEDs = __LED_amount - 1;
                iterateLEDs > 0; iterateLEDs--)
            {
                    LEDStrip[0] = LEDStrip[__LED_amount - 1];
                    LEDStrip[iterateLEDs] = LEDStrip[iterateLEDs - 1];
            }
        else if (!direction)
            for (int iterateLEDs = 0;
                iterateLEDs < __LED_amount - 1; iterateLEDs++)
            {
                    LEDStrip[__LED_amount - 1] = LEDStrip[0];
                    LEDStrip[iterateLEDs] = LEDStrip[iterateLEDs + 1];
            }

        FastLED.show();
        timeNow = millis();
        while (millis() - timeNow <= timePerLED)
            if (modeChanged)
                return;
    }
}

void __glowUp(CRGB *LEDStrip,
              uint16_t startLED, uint16_t endLED,
              uint32_t color, uint32_t lagTime_ms,
              uint8_t maxBrightness,
              uint8_t minBrightness)
{
    uint8_t  brightness;
    uint32_t timeNow;

    for (brightness = minBrightness;
         brightness <= maxBrightness;
         brightness++)
    {
        FastLED.setBrightness(brightness);
        __paintLEDs_now(LEDStrip,
                        startLED, endLED,
                        color);
        timeNow = millis();
        while (millis() - timeNow <= lagTime_ms)
            if (modeChanged)
                return;
    }

    timeNow = millis();
    while (millis() - timeNow <= 3 * lagTime_ms)
        if (modeChanged)
            return;
}

void __fadeAway(CRGB *LEDStrip,
                uint16_t startLED, uint16_t endLED,
                uint32_t color, uint32_t lagTime_ms,
                uint8_t maxBrightness,
                uint8_t minBrightness)
{
    int      brightness;
    uint32_t timeNow;

    for (brightness = maxBrightness;
         brightness >= minBrightness;
         brightness--)
    {
        FastLED.setBrightness(brightness);
        __paintLEDs_now(LEDStrip,
                        startLED, endLED,
                        color);
        timeNow = millis();
        while (millis() - timeNow <= lagTime_ms)
            if (modeChanged)
                return;
    }

    timeNow = millis();
    while (millis() - timeNow <= 3 * lagTime_ms)
        if (modeChanged)
            return;
}

void __colorsBreathing(CRGB *LEDStrip,
                       uint16_t startLED, uint16_t endLED,
                       uint32_t *colorBank, uint16_t howManyColors,
                       uint32_t lagTime_ms,
                       uint8_t maxBrightness,
                       uint8_t minBrightness)
{
    uint16_t breathingIteration;

    for (breathingIteration = 0;
         breathingIteration < howManyColors;
         breathingIteration++)
    {
        if (modeChanged)
            return;
        __glowUp(LEDStrip, startLED, endLED,
                 colorBank[breathingIteration], lagTime_ms,
                 maxBrightness, minBrightness);
        if (modeChanged)
            return;
        __fadeAway(LEDStrip, startLED, endLED,
                   colorBank[breathingIteration], lagTime_ms,
                   maxBrightness, minBrightness);
    }
}

void __transition2Colors(CRGB *LEDStrip,
                         uint16_t startLED, uint16_t endLED,
                         uint32_t color1, uint32_t color2,
                         uint32_t lagTime_ms)
{
    uint8_t  currentRed;
    uint8_t  currentGreen;
    uint8_t  currentBlue;
    uint8_t  targetRed;
    uint8_t  targetGreen;
    uint8_t  targetBlue;
    uint16_t colorIteration;
    uint32_t timeNow;

    currentRed = __separateRGB(color1, 'R');
    currentGreen = __separateRGB(color1, 'G');
    currentBlue = __separateRGB(color1, 'B');
    targetRed = __separateRGB(color2, 'R');
    targetGreen = __separateRGB(color2, 'G');
    targetBlue = __separateRGB(color2, 'B');

    while (currentRed != targetRed || currentGreen != targetGreen || currentBlue != targetBlue)
    {
        __paintLEDs_now(LEDStrip, startLED, endLED,
                __combineRGB(currentRed, currentGreen, currentBlue));
        if (currentRed < targetRed)
            currentRed++;
        else if (currentRed > targetRed)
            currentRed--;
        if (currentGreen < targetGreen)
            currentGreen++;
        else if (currentGreen > targetGreen)
            currentGreen--;
        if (currentBlue < targetBlue)
            currentBlue++;
        else if (currentBlue > targetBlue)
            currentBlue--;
        timeNow = millis();
        while (millis() - timeNow <= lagTime_ms) {}
    }
}

void __colorsShifting(CRGB *LEDStrip,
                      uint16_t startLED, uint16_t endLED,
                      uint8_t enableCustomColors,
                      uint32_t *customColorsBank,
                      uint16_t howManyColors,
                      uint32_t lagTime_ms)
{
    uint8_t  currentRed;
    uint8_t  currentGreen;
    uint8_t  currentBlue;
    uint8_t  targetRed;
    uint8_t  targetGreen;
    uint8_t  targetBlue;
    uint16_t colorIteration;
    uint32_t timeNow;

    if (!enableCustomColors)
    {
        currentRed = 0xFF;
        currentGreen = 0x00;
        currentBlue = 0x00;
        while (currentGreen != 0xFF)
        {
            __paintLEDs_now(LEDStrip, startLED, endLED,
                        __combineRGB(currentRed, currentGreen, currentBlue));
            currentGreen = currentGreen + 3;
            timeNow = millis();
            while (millis() - timeNow <= lagTime_ms)
                if (modeChanged)
                {
                    modeChanged = 0;
                    return;
                }
        }
        while (currentRed != 0x00)
        {
            __paintLEDs_now(LEDStrip, startLED, endLED,
                        __combineRGB(currentRed, currentGreen, currentBlue));
            currentRed = currentRed - 3;
            timeNow = millis();
            while (millis() - timeNow <= lagTime_ms)
                if (modeChanged)
                {
                    modeChanged = 0;
                    return;
                }
        }
        while (currentBlue != 0xFF)
        {
            __paintLEDs_now(LEDStrip, startLED, endLED,
                        __combineRGB(currentRed, currentGreen, currentBlue));
            currentBlue = currentBlue + 3;
            timeNow = millis();
            while (millis() - timeNow <= lagTime_ms)
                if (modeChanged)
                {
                    modeChanged = 0;
                    return;
                }
        }
        while (currentGreen != 0x00)
        {
            __paintLEDs_now(LEDStrip, startLED, endLED,
                        __combineRGB(currentRed, currentGreen, currentBlue));
            currentGreen = currentGreen - 3;
            timeNow = millis();
            while (millis() - timeNow <= lagTime_ms)
                if (modeChanged)
                {
                    modeChanged = 0;
                    return;
                }
        }
        while (currentRed != 0xFF)
        {
            __paintLEDs_now(LEDStrip, startLED, endLED,
                        __combineRGB(currentRed, currentGreen, currentBlue));
            currentRed = currentRed + 3;
            timeNow = millis();
            while (millis() - timeNow <= lagTime_ms)
                if (modeChanged)
                {
                    modeChanged = 0;
                    return;
                }
        }
        while (currentBlue != 0x00)
        {
            __paintLEDs_now(LEDStrip, startLED, endLED,
                        __combineRGB(currentRed, currentGreen, currentBlue));
            currentBlue = currentBlue - 3;
            timeNow = millis();
            while (millis() - timeNow <= lagTime_ms)
                if (modeChanged)
                {
                    modeChanged = 0;
                    return;
                }
        }
    }
    else if (enableCustomColors)
    {
        for (colorIteration = 0;
             colorIteration < howManyColors;
             colorIteration++)
        {
            currentRed = __separateRGB(customColorsBank[colorIteration], 'R');
            currentGreen = __separateRGB(customColorsBank[colorIteration], 'G');
            currentBlue = __separateRGB(customColorsBank[colorIteration], 'B');
            if (colorIteration == howManyColors - 1)
            {
                targetRed = __separateRGB(customColorsBank[0], 'R');
                targetGreen = __separateRGB(customColorsBank[0], 'G');
                targetBlue = __separateRGB(customColorsBank[0], 'B');
            }
            else
            {
                targetRed = __separateRGB(customColorsBank[colorIteration + 1], 'R');
                targetGreen = __separateRGB(customColorsBank[colorIteration + 1], 'G');
                targetBlue = __separateRGB(customColorsBank[colorIteration + 1], 'B');
            }
            while (currentRed != targetRed || currentGreen != targetGreen || currentBlue != targetBlue)
            {
                __paintLEDs_now(LEDStrip, startLED, endLED,
                        __combineRGB(currentRed, currentGreen, currentBlue));
                if (currentRed < targetRed)
                    currentRed++;
                else if (currentRed > targetRed)
                    currentRed--;
                if (currentGreen < targetGreen)
                    currentGreen++;
                else if (currentGreen > targetGreen)
                    currentGreen--;
                if (currentBlue < targetBlue)
                    currentBlue++;
                else if (currentBlue > targetBlue)
                    currentBlue--;
                timeNow = millis();
                while (millis() - timeNow <= lagTime_ms)
                    if (modeChanged)
                        return;
            }
        }
    }

}
//TODO: Improve this ---------------------------------------
void __FX_policeBeacon_circle(CRGB *LEDStrip,
                              uint16_t startLED, uint16_t endLED,
                              uint32_t color,
                              uint16_t beamLength,
                              uint32_t lagTime_ms)
{
    uint16_t beamStart;
    uint32_t timeNow;

    if (!beamLength)
        beamLength = (endLED - startLED + 1) / 6;
    if (!lagTime_ms)
        lagTime_ms = 500 / (endLED - startLED + 1);

    for (beamStart = startLED; beamStart <= endLED; beamStart++)
    {
        if (beamStart < beamLength)
            __paintLEDs_now(LEDStrip, startLED,
                            beamStart, color);
        else if (endLED - beamStart + 1 <= beamLength)
            __paintLEDs_now(LEDStrip, beamStart,
                            endLED, color);
        else
            __paintLEDs_now(LEDStrip, beamStart,
                            beamStart + beamLength - 1, color);
        if (beamStart >= 1)
            __paintLEDs_now(LEDStrip, startLED, beamStart - 1, 0);
        if (beamStart == endLED)
            __paintLEDs_now(LEDStrip, endLED, endLED, 0);
        timeNow = millis();
        while (millis() - timeNow <= lagTime_ms) {}
    }
}

void __FX_policeBeacon_bar1(CRGB *LEDStrip,
                            uint16_t startLED, uint16_t endLED,
                            uint8_t strobeNumber,
                            uint32_t lagTime_ms,
                            uint32_t color1,
                            uint32_t color2)
{
    uint8_t  strobes;
    uint16_t color1_endLED;
    uint32_t timeNow;

    strobes = strobeNumber;
    color1_endLED = (endLED - startLED + 1) / 2;

    // Color 1 strobe
    while (strobes != 0)
    {
        __paintLEDs_now(LEDStrip, startLED,
                        color1_endLED, color1);
        timeNow = millis();
        while (millis() - timeNow <= lagTime_ms)
            if (modeChanged)
                return;
        __paintLEDs_now(LEDStrip, startLED,
                        color1_endLED, 0x000000);
        strobes--;
        if (strobes != 0)
        {
            timeNow = millis();
            while (millis() - timeNow <= lagTime_ms)
                if (modeChanged)
                    return;
        }
    }

    strobes = strobeNumber;

    // Color 2 strobe
    while (strobes != 0)
    {
        __paintLEDs_now(LEDStrip, color1_endLED + 1,
                        endLED, color2);
        timeNow = millis();
        while (millis() - timeNow <= lagTime_ms)
            if (modeChanged)
                return;
        __paintLEDs_now(LEDStrip, color1_endLED + 1,
                        endLED, 0x000000);
        strobes--;
        if (strobes != 0)
        {
            timeNow = millis();
            while (millis() - timeNow <= lagTime_ms)
                if (modeChanged)
                    return;
        }
    }
}

void __FX_policeBeacon_bar2(CRGB *LEDStrip,
                            uint16_t startLED, uint16_t endLED,
                            uint8_t strobeNumber,
                            uint32_t lagTime_ms,
                            uint32_t color1,
                            uint32_t color2)
{
    uint8_t  strobes;
    uint16_t quarterLength;
    uint32_t timeNow;

    strobes = strobeNumber;
    quarterLength = (endLED - startLED + 1) / 4;

    while (strobes != 0)
    {
        __paintLEDs_now(LEDStrip,
                        quarterLength, 2 * quarterLength - 1,
                        color1);
        __paintLEDs_now(LEDStrip,
                        2 * quarterLength, 3 * quarterLength - 1,
                        color2);
        timeNow = millis();
        while (millis() - timeNow <= lagTime_ms)
            if (modeChanged)
                return;
        __paintLEDs_now(LEDStrip,
                        quarterLength, 2 * quarterLength - 1,
                        0x000000);
        __paintLEDs_now(LEDStrip,
                        2 * quarterLength, 3 * quarterLength - 1,
                        0x000000);
        strobes--;
        if (strobes != 0)
        {
            timeNow = millis();
            while (millis() - timeNow <= lagTime_ms)
                if (modeChanged)
                    return;
        }
    }

    strobes = strobeNumber;

    while (strobes != 0)
    {
        __paintLEDs_now(LEDStrip,
                        startLED, quarterLength - 1,
                        color1);
        __paintLEDs_now(LEDStrip,
                        endLED - quarterLength + 1,
                        endLED,
                        color2);
        timeNow = millis();
        while (millis() - timeNow <= lagTime_ms)
            if (modeChanged)
                return;
        __paintLEDs_now(LEDStrip,
                        startLED, quarterLength - 1,
                        0x000000);
        __paintLEDs_now(LEDStrip,
                        endLED - quarterLength + 1,
                        endLED,
                        0x000000);
        strobes--;
        if (strobes != 0)
        {
            timeNow = millis();
            while (millis() - timeNow <= lagTime_ms)
                if (modeChanged)
                    return;
        }
    }
}