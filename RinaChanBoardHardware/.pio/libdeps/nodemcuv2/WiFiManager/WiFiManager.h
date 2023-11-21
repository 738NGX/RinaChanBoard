/**************************************************************
   WiFiManager is a library for the ESP8266/Arduino platform
   (https://github.com/esp8266/Arduino) to enable easy
   configuration and reconfiguration of WiFi credentials using a Captive Portal
   inspired by:
   http://www.esp8266.com/viewtopic.php?f=29&t=2520
   https://github.com/chriscook8/esp-arduino-apboot
   https://github.com/esp8266/Arduino/tree/master/libraries/DNSServer/examples/CaptivePortalAdvanced
   Built by AlexT https://github.com/tzapu
   Licensed under MIT license
 **************************************************************/

#ifndef WiFiManager_h
#define WiFiManager_h

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <memory>

extern "C" {
  #include "user_interface.h"
}

const char HTTP_HEADER[] PROGMEM          = "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1, user-scalable=no\"/><title>{v}</title>";
const char HTTP_STYLE[] PROGMEM           = "<style>.c{text-align: center;} div,input{padding:5px;font-size:1em;border-radius:0.5rem;} input{margin-top:0.5rem;margin-bottom:0.5rem;width:95%;} body{text-align: center;font-family:verdana;} button{border:0;border-radius:1rem;background-color:#9ca5b9;color:#fff;line-height:2.4rem;font-size:1.2rem;width:100%;} .q{float: right;width: 64px;text-align: right;} .l{background: url(\"data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAMgAAADICAYAAACtWK6eAAAACXBIWXMAAAsTAAALEwEAmpwYAAAGsmlUWHRYTUw6Y29tLmFkb2JlLnhtcAAAAAAAPD94cGFja2V0IGJlZ2luPSLvu78iIGlkPSJXNU0wTXBDZWhpSHpyZVN6TlRjemtjOWQiPz4gPHg6eG1wbWV0YSB4bWxuczp4PSJhZG9iZTpuczptZXRhLyIgeDp4bXB0az0iQWRvYmUgWE1QIENvcmUgNy4xLWMwMDAgMTE2LjM5YWJhZjcsIDIwMjIvMDIvMjUtMjE6NTc6MjEgICAgICAgICI+IDxyZGY6UkRGIHhtbG5zOnJkZj0iaHR0cDovL3d3dy53My5vcmcvMTk5OS8wMi8yMi1yZGYtc3ludGF4LW5zIyI+IDxyZGY6RGVzY3JpcHRpb24gcmRmOmFib3V0PSIiIHhtbG5zOnhtcD0iaHR0cDovL25zLmFkb2JlLmNvbS94YXAvMS4wLyIgeG1sbnM6ZGM9Imh0dHA6Ly9wdXJsLm9yZy9kYy9lbGVtZW50cy8xLjEvIiB4bWxuczpwaG90b3Nob3A9Imh0dHA6Ly9ucy5hZG9iZS5jb20vcGhvdG9zaG9wLzEuMC8iIHhtbG5zOnhtcE1NPSJodHRwOi8vbnMuYWRvYmUuY29tL3hhcC8xLjAvbW0vIiB4bWxuczpzdEV2dD0iaHR0cDovL25zLmFkb2JlLmNvbS94YXAvMS4wL3NUeXBlL1Jlc291cmNlRXZlbnQjIiB4bXA6Q3JlYXRvclRvb2w9IkFkb2JlIFBob3Rvc2hvcCAyMi41IChXaW5kb3dzKSIgeG1wOkNyZWF0ZURhdGU9IjIwMjMtMTEtMTRUMTg6NTM6MTUrMDg6MDAiIHhtcDpNb2RpZnlEYXRlPSIyMDIzLTExLTE1VDAwOjU3OjU3KzA4OjAwIiB4bXA6TWV0YWRhdGFEYXRlPSIyMDIzLTExLTE1VDAwOjU3OjU3KzA4OjAwIiBkYzpmb3JtYXQ9ImltYWdlL3BuZyIgcGhvdG9zaG9wOkNvbG9yTW9kZT0iMyIgcGhvdG9zaG9wOklDQ1Byb2ZpbGU9InNSR0IgSUVDNjE5NjYtMi4xIiB4bXBNTTpJbnN0YW5jZUlEPSJ4bXAuaWlkOjhhN2UxMDQzLTc0ZmItZGM0ZC05ZDU3LTU1MGMyYWMzOTIyNCIgeG1wTU06RG9jdW1lbnRJRD0ieG1wLmRpZDphZmEzYjYyMS0yY2I3LTkwNDEtYWVjMS0xMzc5ZGM5YTMxYWYiIHhtcE1NOk9yaWdpbmFsRG9jdW1lbnRJRD0ieG1wLmRpZDphZmEzYjYyMS0yY2I3LTkwNDEtYWVjMS0xMzc5ZGM5YTMxYWYiPiA8eG1wTU06SGlzdG9yeT4gPHJkZjpTZXE+IDxyZGY6bGkgc3RFdnQ6YWN0aW9uPSJjcmVhdGVkIiBzdEV2dDppbnN0YW5jZUlEPSJ4bXAuaWlkOmFmYTNiNjIxLTJjYjctOTA0MS1hZWMxLTEzNzlkYzlhMzFhZiIgc3RFdnQ6d2hlbj0iMjAyMy0xMS0xNFQxODo1MzoxNSswODowMCIgc3RFdnQ6c29mdHdhcmVBZ2VudD0iQWRvYmUgUGhvdG9zaG9wIDIyLjUgKFdpbmRvd3MpIi8+IDxyZGY6bGkgc3RFdnQ6YWN0aW9uPSJzYXZlZCIgc3RFdnQ6aW5zdGFuY2VJRD0ieG1wLmlpZDpiZWQ1MjI2Zi0zYjIxLTA4NDQtYTNjMi0yNzU3NjUwN2RlNTEiIHN0RXZ0OndoZW49IjIwMjMtMTEtMTRUMTg6NTY6MzIrMDg6MDAiIHN0RXZ0OnNvZnR3YXJlQWdlbnQ9IkFkb2JlIFBob3Rvc2hvcCAyMi41IChXaW5kb3dzKSIgc3RFdnQ6Y2hhbmdlZD0iLyIvPiA8cmRmOmxpIHN0RXZ0OmFjdGlvbj0ic2F2ZWQiIHN0RXZ0Omluc3RhbmNlSUQ9InhtcC5paWQ6OGE3ZTEwNDMtNzRmYi1kYzRkLTlkNTctNTUwYzJhYzM5MjI0IiBzdEV2dDp3aGVuPSIyMDIzLTExLTE1VDAwOjU3OjU3KzA4OjAwIiBzdEV2dDpzb2Z0d2FyZUFnZW50PSJBZG9iZSBQaG90b3Nob3AgMjIuNSAoV2luZG93cykiIHN0RXZ0OmNoYW5nZWQ9Ii8iLz4gPC9yZGY6U2VxPiA8L3htcE1NOkhpc3Rvcnk+IDwvcmRmOkRlc2NyaXB0aW9uPiA8L3JkZjpSREY+IDwveDp4bXBtZXRhPiA8P3hwYWNrZXQgZW5kPSJyIj8+2N+ZyAAAB9lJREFUeJzt3T2S20YQhuGm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\") no-repeat left center;background-size: 1em;}</style>";
const char HTTP_SCRIPT[] PROGMEM          = "<script>function c(l){document.getElementById('s').value=l.innerText||l.textContent;document.getElementById('p').focus();}</script>";
const char HTTP_HEADER_END[] PROGMEM      = "</head><body><div style='text-align:left;display:inline-block;min-width:260px;'>";
const char HTTP_PORTAL_OPTIONS[] PROGMEM  = "<form action=\"/wifi\" method=\"get\"><button>配置WiFi(扫描WiFi)</button></form><br/><form action=\"/0wifi\" method=\"get\"><button>配置WiFi(输入WiFi名)</button></form><br/><form action=\"/i\" method=\"get\"><button>硬件信息</button></form><br/><form action=\"/r\" method=\"post\"><button>硬件复位</button></form>";
const char HTTP_ITEM[] PROGMEM            = "<div><a href='#p' onclick='c(this)'>{v}</a>&nbsp;<span class='q {i}'>{r}%</span></div>";
const char HTTP_FORM_START[] PROGMEM      = "<form method='get' action='wifisave'><input id='s' name='s' length=32 placeholder='WiFi名称'><br/><input id='p' name='p' length=64 type='password' placeholder='密码'><br/>";
const char HTTP_FORM_PARAM[] PROGMEM      = "<br/><input id='{i}' name='{n}' maxlength={l} placeholder='{p}' value='{v}' {c}>";
const char HTTP_FORM_END[] PROGMEM        = "<br/><button type='submit'>保存</button></form>";
const char HTTP_SCAN_LINK[] PROGMEM       = "<br/><div class=\"c\"><a href=\"/wifi\">扫描</a></div>";
const char HTTP_SAVED[] PROGMEM           = "<div>Wifi信息已经上传<br />正在尝试将璃奈板连接到Wifi...<br />如果连接失败将会重新返回到配网页面</div>";
const char HTTP_END[] PROGMEM             = "</div></body></html>";

#ifndef WIFI_MANAGER_MAX_PARAMS
#define WIFI_MANAGER_MAX_PARAMS 10
#endif

class WiFiManagerParameter {
  public:
    /** 
        Create custom parameters that can be added to the WiFiManager setup web page
        @id is used for HTTP queries and must not contain spaces nor other special characters
    */
    WiFiManagerParameter(const char *custom);
    WiFiManagerParameter(const char *id, const char *placeholder, const char *defaultValue, int length);
    WiFiManagerParameter(const char *id, const char *placeholder, const char *defaultValue, int length, const char *custom);
    ~WiFiManagerParameter();

    const char *getID();
    const char *getValue();
    const char *getPlaceholder();
    int         getValueLength();
    const char *getCustomHTML();
  private:
    const char *_id;
    const char *_placeholder;
    char       *_value;
    int         _length;
    const char *_customHTML;

    void init(const char *id, const char *placeholder, const char *defaultValue, int length, const char *custom);

    friend class WiFiManager;
};


class WiFiManager
{
  public:
    WiFiManager();
    ~WiFiManager();

    boolean       autoConnect();
    boolean       autoConnect(char const *apName, char const *apPassword = NULL);

    //if you want to always start the config portal, without trying to connect first
    boolean       startConfigPortal();
    boolean       startConfigPortal(char const *apName, char const *apPassword = NULL);

    // get the AP name of the config portal, so it can be used in the callback
    String        getConfigPortalSSID();

    void          resetSettings();

    //sets timeout before webserver loop ends and exits even if there has been no setup.
    //useful for devices that failed to connect at some point and got stuck in a webserver loop
    //in seconds setConfigPortalTimeout is a new name for setTimeout
    void          setConfigPortalTimeout(unsigned long seconds);
    void          setTimeout(unsigned long seconds);

    //sets timeout for which to attempt connecting, useful if you get a lot of failed connects
    void          setConnectTimeout(unsigned long seconds);


    void          setDebugOutput(boolean debug);
    //defaults to not showing anything under 8% signal quality if called
    void          setMinimumSignalQuality(int quality = 8);
    //sets a custom ip /gateway /subnet configuration
    void          setAPStaticIPConfig(IPAddress ip, IPAddress gw, IPAddress sn);
    //sets config for a static IP
    void          setSTAStaticIPConfig(IPAddress ip, IPAddress gw, IPAddress sn);
    //called when AP mode and config portal is started
    void          setAPCallback( void (*func)(WiFiManager*) );
    //called when settings have been changed and connection was successful
    void          setSaveConfigCallback( void (*func)(void) );
    //adds a custom parameter, returns false on failure
    bool          addParameter(WiFiManagerParameter *p);
    //if this is set, it will exit after config, even if connection is unsuccessful.
    void          setBreakAfterConfig(boolean shouldBreak);
    //if this is set, try WPS setup when starting (this will delay config portal for up to 2 mins)
    //TODO
    //if this is set, customise style
    void          setCustomHeadElement(const char* element);
    //if this is true, remove duplicated Access Points - defaut true
    void          setRemoveDuplicateAPs(boolean removeDuplicates);

  private:
    std::unique_ptr<DNSServer>        dnsServer;
    std::unique_ptr<ESP8266WebServer> server;

    //const int     WM_DONE                 = 0;
    //const int     WM_WAIT                 = 10;

    //const String  HTTP_HEADER = "<!DOCTYPE html><html lang=\"en\"><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"/><title>{v}</title>";

    void          setupConfigPortal();
    void          startWPS();

    const char*   _apName                 = "no-net";
    const char*   _apPassword             = NULL;
    String        _ssid                   = "";
    String        _pass                   = "";
    unsigned long _configPortalTimeout    = 0;
    unsigned long _connectTimeout         = 0;
    unsigned long _configPortalStart      = 0;

    IPAddress     _ap_static_ip;
    IPAddress     _ap_static_gw;
    IPAddress     _ap_static_sn;
    IPAddress     _sta_static_ip;
    IPAddress     _sta_static_gw;
    IPAddress     _sta_static_sn;

    int           _paramsCount            = 0;
    int           _minimumQuality         = -1;
    boolean       _removeDuplicateAPs     = true;
    boolean       _shouldBreakAfterConfig = false;
    boolean       _tryWPS                 = false;

    const char*   _customHeadElement      = "";

    //String        getEEPROMString(int start, int len);
    //void          setEEPROMString(int start, int len, String string);

    int           status = WL_IDLE_STATUS;
    int           connectWifi(String ssid, String pass);
    uint8_t       waitForConnectResult();

    void          handleRoot();
    void          handleWifi(boolean scan);
    void          handleWifiSave();
    void          handleInfo();
    void          handleReset();
    void          handleNotFound();
    void          handle204();
    boolean       captivePortal();
    boolean       configPortalHasTimeout();

    // DNS server
    const byte    DNS_PORT = 53;

    //helpers
    int           getRSSIasQuality(int RSSI);
    boolean       isIp(String str);
    String        toStringIp(IPAddress ip);

    boolean       connect;
    boolean       _debug = true;

    void (*_apcallback)(WiFiManager*) = NULL;
    void (*_savecallback)(void) = NULL;

    int                    _max_params;
    WiFiManagerParameter** _params;

    template <typename Generic>
    void          DEBUG_WM(Generic text);

    template <class T>
    auto optionalIPFromString(T *obj, const char *s) -> decltype(  obj->fromString(s)  ) {
      return  obj->fromString(s);
    }
    auto optionalIPFromString(...) -> bool {
      DEBUG_WM("NO fromString METHOD ON IPAddress, you need ESP8266 core 2.1.0 or newer for Custom IP configuration to work.");
      return false;
    }
};

#endif
