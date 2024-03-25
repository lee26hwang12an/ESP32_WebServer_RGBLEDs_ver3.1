#pragma once
#ifndef _WEBFUNCTIONS_H_
#define _WEBFUNCTIONS_H_

#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <WiFiClientSecure.h>

/*
    @brief Connect to wifi with the given credentials, then start the server with the defined HTML, CSS, and JS.
    @param message Put whatever of type string, as a message.
    @return Nothing.
*/
void __initServerNow(char *message);

/*
    @brief Convert command read in String type to color value.
    @param inputCommand The command to read.
    @return A value of type unsigned long.
*/
uint32_t __serverCommandToHex(String inputCommand);

/*
    @brief Read the received command, starting from specified position till the terminator.
    @param commandToRead The command to read.
    @param startFromWhere The position in the command to start reading.
    @param *endPosition This param is used to return the position at which the terminating character is found.
    @param terminator The terminating character. Default as '-' if not specified.
    @return A string that is a fraction of the original command.
*/
String __serverReadCommandUntilTerminator(String commandToRead,
                   uint8_t startFromWhere,
                   uint8_t *endPosition,
                   char terminator = '-');

/*
    @brief Analyze the entire command line received.
    @param enableSerial_commandDisplaying If set to 1, display the command decomposed. If set to 0, disable printing.
    @return Nothing.
*/
void __serverAnalyzeCommandNow(uint8_t enableSerial_commandDisplaying = 0);

#endif /* EOF _WEBFUNCTIONS_H_ */