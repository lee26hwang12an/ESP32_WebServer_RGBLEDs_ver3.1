#include <WiFi.h>
#include <esp_now.h>

typedef struct ESPNowPackage
{
    byte OFF;
    byte PREV_MODE;
    byte NEXT_MODE;
} __ESPNowPackage;

__ESPNowPackage ESPNowCommand;

esp_now_peer_info_t peerInfo;

uint8_t __targetMAC[6] = {0x48, 0xE7, 0x29, 0x97, 0xF2, 0xCC};

String receivedCommand = "";
byte CB_unregistered = 0;

void __ESPNOW_RCV_CB(const uint8_t *originMAC,
                     const uint8_t *incomingCommand,
                     int len)
{
  esp_now_unregister_recv_cb();
  CB_unregistered = 1;
  memcpy(&ESPNowCommand, incomingCommand, sizeof(incomingCommand));
  if (ESPNowCommand.OFF)
    receivedCommand = "OFF";
  else if (!ESPNowCommand.OFF &&
           ESPNowCommand.PREV_MODE &&
           !ESPNowCommand.NEXT_MODE)
    receivedCommand = "PREVIOUS MODE";
  else if (!ESPNowCommand.OFF &&
           !ESPNowCommand.PREV_MODE &&
           ESPNowCommand.NEXT_MODE)
    receivedCommand = "NEXT MODE";
  
  // Serial.println(receivedCommand);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  esp_now_init();
  memcpy(peerInfo.peer_addr, __targetMAC, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  esp_now_add_peer(&peerInfo);
  esp_now_register_recv_cb(__ESPNOW_RCV_CB);
}

void loop() {
  // put your main code here, to run repeatedly:
  // Serial.println(receivedCommand);
  if (receivedCommand == "OFF")
  {
    // Do something
    Serial.println(receivedCommand);
    delay(500);
    receivedCommand = "";

    // Re-register CB
    esp_now_register_recv_cb(__ESPNOW_RCV_CB);
    CB_unregistered = 0;
  }
  else if (receivedCommand == "PREVIOUS MODE")
  {
    // Do something
    Serial.println(receivedCommand);
    delay(500);
    receivedCommand = "";

    // Re-register CB
    esp_now_register_recv_cb(__ESPNOW_RCV_CB);
    CB_unregistered = 0;
  }
  else if (receivedCommand == "NEXT MODE")
  {
    // Do something
    Serial.println(receivedCommand);
    delay(500);
    receivedCommand = "";

    // Re-register CB
    esp_now_register_recv_cb(__ESPNOW_RCV_CB);
    CB_unregistered = 0;
  }
}
