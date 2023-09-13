# Sousvide client
## Setup
Create a new file `Config.h`
```
pragma once

#include<string>

class Config{
  public:
    const char* remoteIP = "SERVER_IP";
    std::string ssid = WLAN_SSID"
    std::string pwd = "WLAN_PWD";
} config;
```
