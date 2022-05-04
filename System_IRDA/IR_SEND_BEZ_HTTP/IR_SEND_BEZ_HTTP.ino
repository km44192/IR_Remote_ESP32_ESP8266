#include <ESP8266WiFi.h>
//#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Arduino.h>
#include <assert.h>
#include <IRrecv.h>
#include <IRac.h>
#include <IRsend.h>
#include <IRtext.h>
#include <IRutils.h>
#include <SPI.h>
#include <SD.h>
#include <cstring>
#include "md5.h"

using std::string;
#define SD_CS_PIN SS
File myFile;

ESP8266WebServer server(80);
const uint16_t kRecvPin = 4;
const uint32_t kBaudRate = 115200;
const uint16_t kCaptureBufferSize = 1024;

const char *ssid = "xxxxxxxxx";
const char *password ="yyyyyyy";


#if DECODE_AC
const uint8_t kTimeout = 50;
#else
const uint8_t kTimeout = 15;
#endif
void pointer_to_string(string &s);  
 void onPilot(){
    myFile=SD.open("Remote_Panel.html");
    String page=String(myFile.readString());
    myFile.close();
  server.send(200,"text/html",page);
  

  }
void handleRoot()
{
  myFile= SD.open("Login.html");
  //while(myFile.available())
String Root=myFile.readString();
myFile.close();
  server.send(200,"text/html",Root);
}
void Login(){
  myFile= SD.open("Registered_Users.txt");
  String usr=server.arg(0);
  String passwd=server.arg(1);
  String all=myFile.readString();
  myFile.close();
  
string w=md5(passwd.c_str());
  usr.concat(';'); 
  pointer_to_string(w);
  string s=w;
  usr+=(s.c_str());
  if(-1!=all.indexOf(usr)){
    onPilot();
  }}





IRsend sender(D5);
void command(){
  String w1="<meta http-equiv=\"refresh\" content=\"0; url=/\" />";
  String model=server.arg(0);
  String comm=server.arg(1);
  String oc="";
  if(model=="LED")
  {
    myFile=SD.open("IR_codes_NEC.txt");
    String codes=myFile.readString();
    oc=codes.substring(codes.indexOf(comm)+(comm.length()+1),8);
    sender.sendNEC((oc.toInt()));
    myFile.close();
  }
  if(model=="ORANGE"){
   myFile=SD.open("tv_orange.txt");
    String codes=myFile.readString();
    oc=codes.substring(codes.indexOf(comm)+(comm.length()+1),8);//substr tworzy nowy string
    sender.sendNEC((oc.toInt()));
    myFile.close();
  }
  
  server.send(200,"text/html","Sending "+comm+"Data "+oc);
 
  Serial.println("Recived command "+comm+" Data "+oc);
  }

 
  /*
// DECODE_AC
// Alternatives:
// const uint8_t kTimeout = 90;
// Suits messages with big gaps like XMP-1 & some aircon units, but can
// accidentally swallow repeated messages in the rawData[] output.
//
// const uint8_t kTimeout = kMaxTimeoutMs;
// This will set it to our currently allowed maximum.
// Values this high are problematic because it is roughly the typical boundary
// where most messages repeat.
// e.g. It will stop decoding a message and start sending it to serial at
//      precisely the time when the next message is likely to be transmitted,
//      and may miss it.
*/
const uint16_t kMinUnknownSize = 32;
/*
// How much percentage lee way do we give to incoming signals in order to match
// it?
// e.g. +/- 25% (default) to an expected value of 500 would mean matching a
//      value between 375 & 625 inclusive.
// Note: Default is 25(%). Going to a value >= 50(%) will cause some protocols
//       to no longer match correctly. In normal situations you probably do not
//       need to adjust this value. Typically that's when the library detects
//       your remote's message some of the time, but not all of the time.
*/
const uint8_t kTolerancePercentage = kTolerance;  // kTolerance is normally 25%
#define LEGACY_TIMING_INFO false
IRrecv irrecv(kRecvPin, kCaptureBufferSize, kTimeout, true);
decode_results results;  // Somewhere to store the results

void setup() {
  sender.begin();
  server.begin();
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid,password);
  

#if defined(ESP8266)
  Serial.begin(kBaudRate, SERIAL_8N1, SERIAL_TX_ONLY);
#endif
///Uruchamianie_karty_sd
Serial.print("Initializing SD card...");

  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
/////////////////////////////////////
while(WiFi.status()!=WL_CONNECTED){
    delay(500);
    Serial.print(".");}

   Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  
  while (!Serial)  
    delay(50);
  assert(irutils::lowLevelSanityCheck() == 0);
  Serial.printf("\n" D_STR_IRRECVDUMP_STARTUP "\n", kRecvPin);
  
#if DECODE_HASH
  irrecv.setUnknownThreshold(kMinUnknownSize);
#endif  // DECODE_HASH
  irrecv.setTolerance(kTolerancePercentage);  
  irrecv.enableIRIn();
   
  server.on("/",handleRoot);
  server.on("/x",command); 
///Obsługa komend
//  server.on("/send","text/html",(){})
}

void loop() {
server.handleClient();
  // Check if the IR code has been received.
  if (irrecv.decode(&results)) {
    // Display a crude timestamp.
    uint32_t now = millis();
  //  Serial.printf(D_STR_TIMESTAMP " : %06u.%03u\n", now / 1000, now % 1000);
 
    if (results.overflow)
      Serial.printf(D_WARN_BUFFERFULL "\n", kCaptureBufferSize);
  
    //Serial.println(D_STR_LIBRARY "   : v" _IRREMOTEESP8266_VERSION_ "\n");
   
    if (kTolerancePercentage != kTolerance)
      Serial.printf(D_STR_TOLERANCE " : %d%%\n", kTolerancePercentage);
  
    Serial.print(resultToHumanReadableBasic(&results));
    String description = IRAcUtils::resultAcToString(&results);
    if (description.length()) Serial.println(D_STR_MESGDESC ": " + description);
    yield();  
#if LEGACY_TIMING_INFO
    Serial.println(resultToTimingInfo(&results));
    yield();  // Feed the WDT (again)
#endif  // LEGACY_TIMING_INFO
  
    Serial.println(resultToSourceCode(&results));
    Serial.println();   
    yield();             
  }
}
