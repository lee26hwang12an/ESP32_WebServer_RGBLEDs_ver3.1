#pragma once

const char *WIFI_SSID = "Darstellar UV";
const char *WIFI_PASS = "an.hdl195754@sis.hust.edu.vn";

/*
================================================================
    STATIC IP ADDRESS CONFIG - YES 1 | 0 NO
================================================================
*/
#define useStaticIP 1
const IPAddress WIFI_STATIC_IP(192, 168, 0, 200);
const IPAddress WIFI_DEFAULT_GATEWAY(192, 168, 0, 1);
const IPAddress WIFI_SUBNET_MASK(255, 255, 255, 0); 