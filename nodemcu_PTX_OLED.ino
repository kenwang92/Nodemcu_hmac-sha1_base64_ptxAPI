#include <ESP8266HTTPClient.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <time.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <avr/pgmspace.h>
#include <sha1.h>
#include <WiFiClientSecure.h>
//#include <Orbitron_Medium_35.h>
//#include <Orbitron_Medium_25.h>
//#include <Orbitron_Medium_20.h>
#include <SSD1306.h>
#include <Wire.h>

#define ssid "" //AP ssid
#define passwd "" //AP passwd
SSD1306  display(0x3c, D2, D1);
const int httpsPort = 443;
const char* host = "ptx.transportdata.tw";
String url = "/MOTC/v2/Bus/EstimatedTimeOfArrival/City/NewTaipei/99?$select=EstimateTime&$filter=StopID%20eq%20%27178614%27&$format=JSON";
const char fingerprint[] = "CB 58 99 2F CA CA 7B AA 17 58 C5 8B 57 70 D1 FC A6 4C CA C7";
String id = "";//APP ID get by click here https://ptx.transportdata.tw/PTX/ and you have need an account
const long zone = 0;
char *wday[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
char *month[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 0);
WiFiClientSecure client;
#define appkey ""//APPkey

/*
String printHash(uint8_t* hash) {
  char a1[20];
  char a2[20];
  char a3[42];
  for (int j = 0; j < 20; j++) {
    Serial.print("0123456789abcdef"[hash[j] >> 4]);
    Serial.print("0123456789abcdef"[hash[j] & 0xf]);
    a1[j] = ("0123456789abcdef"[hash[j] >> 4]);
    a2[j] = ("0123456789abcdef"[hash[j] & 0xf]);
  }
  for (int s = 0; s < 20; s++) {
    a3[2 * s] = a1[s];
    a3[2 * s + 1] = a2[s];
  }
  Serial.println();
  a3[40] = '\0';
  String outp(a3);
  return outp;
}
*///debug for hmac-sha1
void setup() {
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, passwd);
  Serial.begin(115200);
  timeClient.begin();
  display.init();
  display.flipScreenVertically();
  display.clear();
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("wait...");
    display.clear();
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    //display.setFont(Orbitron_Medium_35);
    display.drawString(1,30,"Wait...");
    display.display();
  }//wait for WIFI connect
  Serial.println("success!");
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  //display.setFont(Orbitron_Medium_20);
  display.drawString(1,30,"Success!");
  display.display();
  
  client.setFingerprint(fingerprint);
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }
}

String getHeader(String input) {
  //format:Wed, 19 Apr 2017 08:37:50 GMT
  timeClient.update();
  time_t rawtime = timeClient.getEpochTime();
  struct tm * ti = localtime (&rawtime);
  char basestring[40];

  char b[3];
  strcpy(b, String(", ").c_str());

  char week[10];
  strcpy(week, wday[timeClient.getDay()]);

  char c[2];
  strcpy(c, String(" ").c_str());

  char d[2];
  strcpy(d, String(":").c_str());

  char e[5];
  strcpy(e, String(" GMT").c_str());

  char day[35];
  strcpy(day, String(ti->tm_mday).c_str());

  char mon[15];
  strcpy(mon, String(month[ti->tm_mon]).c_str());

  char year[6];
  strcpy(year, String(ti->tm_year + 1900).c_str());

  char hours[3];
  strcpy(hours, String(timeClient.getHours() < 10 ? "0" + String(timeClient.getHours()) : String(timeClient.getHours())).c_str());
  

  char mins[3];
  strcpy(mins, String(timeClient.getMinutes() < 10 ? "0" + String(timeClient.getMinutes()) : String(timeClient.getMinutes())).c_str());

  char secs[5];
  strcpy(secs, String(timeClient.getSeconds() < 10 ? "0" + String(timeClient.getSeconds()) : String(timeClient.getSeconds())).c_str());

  strcat(basestring, week);
  strcat(basestring, b);
  strcat(basestring, day);
  strcat(basestring, c);
  strcat(basestring, mon);
  strcat(basestring, c);
  strcat(basestring, year);
  strcat(basestring, c);
  strcat(basestring, hours);
  strcat(basestring, d);
  strcat(basestring, mins);
  strcat(basestring, d);
  strcat(basestring, secs);
  strcat(basestring, e);
  //Serial.print("basestring:");//debug for GMT
  //Serial.println(basestring);//debug for hmac-sha1
  uint8_t *in, out, i;
  char b64[29];
  char key[] = appkey;
  static const char PROGMEM b64chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  String times = basestring;
  String ts = "x-date: " + times;
  Sha1.initHmac((uint8_t*)key, 27);
  Sha1.print(ts);
  //Serial.println(printHash(Sha1.resultHmac()));//debud use
  for (in = Sha1.resultHmac(), out = 0; ; in += 3) { 
    b64[out++] =   in[0] >> 2;
    b64[out++] = ((in[0] & 0x03) << 4) | (in[1] >> 4);
    if (out >= 26) break;
    b64[out++] = ((in[1] & 0x0f) << 2) | (in[2] >> 6);
    b64[out++] =   in[2] & 0x3f;
  }
  b64[out] = (in[1] & 0x0f) << 2;
  
  for (i = 0; i <= out; i++) b64[i] = pgm_read_byte(&b64chars[b64[i]]);
  b64[i++] = '=';
  b64[i++] = 0;
  String b64out(b64);
  if (input == "v1") {
    return b64out;
  } else {
    return times;
  }
}
void loop() {
  String v1 = getHeader("v1");
  String v2 = getHeader("v2");
  String au = "hmac username=\"" + id + "\", algorithm=\"hmac-sha1\", headers=\"x-date\", signature=\"" + v1 + "\"";
  String xd = v2;
  /*
   * Serial.print("au:");
   * Serial.println(au);
   * Serial.print("xd:");
   * Serial.println(xd);
   */
  client.setFingerprint(fingerprint);
  client.print("GET " + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Authorization: hmac username=\"" + id + "\", algorithm=\"hmac-sha1\", headers=\"x-date\", signature=\"" + v1 + "\"\r\n" +
               "x-date: " + v2 + "\r\n\r\n");
  if (client.connected()) {
    String aldata = client.readString();
    /*
      Serial.println("reply was:");
      Serial.println("==========");
      Serial.println(aldata);
      Serial.println("==========");
    */
    //for test
    /*
      reply was:
      ==========
      HTTP/1.1 200 OK
      Server: nginx
      Date: Sat, 10 Aug 2019 04:53:16 GMT
      Content-Type: application/json; charset=utf-8
      Content-Length: 152
      Connection: keep-alive
      X-RateLimit-Limit-day: 20000
      X-RateLimit-Remaining-day: 19968
      Cache-Control: no-cache
      Pragma: no-cache
      Expires: -1
      X-Powered-By: ASP.NET
      Access-Control-Allow-Origin: *
      X-Kong-Upstream-Latency: 23
      X-Kong-Proxy-Latency: 2
      Via: kong/0.14.1
      Strict-Transport-Security: max-age=63072000; includeSubdomains; preload

      [{"Direction":0,"EstimateTime":364,"StopStatus":0,"MessageType":0,"SrcUpdateTime":"2019-08-10T12:53:00+08:00","UpdateTime":"2019-08-10T12:53:05+08:00"}]

    */
    //aldata content
    int elocate = aldata.indexOf("\"E", 480); //find estimate location
    String st = aldata.substring(elocate);
    char* firstFilter = const_cast<char *>(st.c_str());
    Serial.println(firstFilter);//example:"EstimateTime":174,"StopStatus":0,"......
    
    const char * const delim = ":,";
    char* stchar = new char[st.length()+1];
    char* str = NULL;
    str = strtok(firstFilter,delim);
    Serial.println(str);//example:"EstimateTime"
    str = strtok(NULL,delim);
    Serial.println(str);//example:364
    int displayMins = floor(atoi(str)/60);
    display.clear();
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    //display.setFont(Orbitron_Medium_35);
    display.drawString(0,1,"NO.99");
    //display.setFont(Orbitron_Medium_25);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.drawString(0,37,String(displayMins)+" MINS");
    display.display();
  }
  delay(15000);//update time
}
