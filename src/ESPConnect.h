#ifndef ESPConnect_h
#define ESPConnect_h

#include <functional>
#include <Arduino.h>

#if defined(ESP8266)
#include "ESP8266WiFi.h"
#include "WiFiClient.h"
#include "ESPAsyncTCP.h"
#elif defined(ESP32)
#include "WiFi.h"
#include "WiFiClient.h"
#include "AsyncTCP.h"
#include "Preferences.h"
#endif

#include "ESPAsyncWebServer.h"
#include "DNSServer.h"
#include "espconnect_webpage.h"

/* Library Default Settings */
#define ESPCONNECT_DEBUG 1

#define DEFAULT_CONNECTION_TIMEOUT 30000
#define DEFAULT_PORTAL_TIMEOUT 180000

#if ESPCONNECT_DEBUG == 1
#define ESPCONNECT_SERIAL(x) Serial.print("[ESPConnect][" + String(millis()) + "] " + x)
#else
#define ESPCONNECT_SERIAL(x)
#endif

class ESPConnectClass
{

private:
  DNSServer *_dns = nullptr;
  AsyncWebServer *_server = nullptr;

  String _auto_connect_ssid = "";
  String _auto_connect_password = "";
  unsigned long _auto_connect_timeout = DEFAULT_PORTAL_TIMEOUT;

  String _sta_ssid = "";
  String _sta_password = "";

  IPAddress _staticIP;
  IPAddress _gateway;
  IPAddress _subnet;
  bool _useStaticIP = false; // Flag to indicate whether to use static IP or not

  IPAddress _dns1;
  IPAddress _dns2;
  bool _useStaticIPDNS = false; // Flag to indicate whether to use custom dns or not when using static ip

private:
  void load_sta_credentials();

  // Start Captive portal
  bool start_portal();

public:
  // Check if ESPConnect was configured before
  bool isConfigured();

  // Set Custom AP
  void autoConnect(const char *ssid, const char *password = "", unsigned long timeout = DEFAULT_PORTAL_TIMEOUT);

  // Connect to Saved WiFi Credentials
  bool begin(AsyncWebServer *server, unsigned long timeout = DEFAULT_CONNECTION_TIMEOUT);

  // Erase Saved WiFi Credentials
  bool erase();

  /*
    Data Getters
  */

  // Return true / false depending of connection status
  bool isConnected();

  // Method to set static IP, gateway, and subnet
  void setStaticIP(IPAddress ip, IPAddress gateway, IPAddress subnet)
  {
    _staticIP = ip;
    _gateway = gateway;
    _subnet = subnet;
    _useStaticIP = true;
    _useStaticIPDNS = false;
  }

  // Method to set static IP, gateway, and subnet
  void setStaticIP(IPAddress ip, IPAddress gateway, IPAddress subnet, IPAddress dns1, IPAddress dns2)
  {
    _staticIP = ip;
    _gateway = gateway;
    _subnet = subnet;
    _useStaticIP = true;

    _dns1 = dns1;
    _dns2 = dns2;
    _useStaticIPDNS = true;
  }

  // Gets SSID of connected endpoint
  String getSSID();
  String getPassword();
};

extern ESPConnectClass ESPConnect;

#endif