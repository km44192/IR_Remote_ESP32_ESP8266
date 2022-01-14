
#define WIFI_SSID "Orange_Swiatlowod_9E6A"
#define WIFI_PSK  "C4E5TY7ASKJL"
// Include certificate data (see note above)
#include "cert.h"
#include "private_key.h"
#include <WiFi.h>
#include <IRrecv.h>
#include <IRsend.h>
#include <IRac.h>
#include <HTTPSServer.hpp>
#include <SSLCert.hpp>
#include <HTTPRequest.hpp>
#include <HTTPResponse.hpp>
#include "sites.h"
// The HTTPS Server comes in a separate namespace. For easier use, include it here.
using namespace httpsserver;

SSLCert cert = SSLCert(
                 example_crt_DER, example_crt_DER_len,
                 example_key_DER, example_key_DER_len
               );

HTTPSServer secureServer = HTTPSServer(&cert);

void handleRoot(HTTPRequest * req, HTTPResponse * res); //Root
void handleLogin(HTTPRequest * req, HTTPResponse * res); //Czy zalogowany
void handleRemote(HTTPRequest * req, HTTPResponse * res); //Pilot
void handleRecive(HTTPRequest * req, HTTPResponse * res); //Odbior
void handleAddCode(HTTPRequest * req, HTTPResponse * res); //Dodaniekodu_IR
void handleSend(HTTPRequest * req, HTTPResponse * res); //Wysylanie_kodu
void handle404(HTTPRequest * req, HTTPResponse * res); //404_not_found
IRsend sender(4);
decode_results results;

const uint16_t kRecvPin = 34;
const uint16_t kCaptureBufferSize = 128;
#if DECODE_AC
const uint8_t kTimeout = 50;
#else
const uint8_t kTimeout = 15;
#endif


IRrecv irrecv(kRecvPin, kCaptureBufferSize, kTimeout, true);
const uint8_t kTolerancePercentage = 25;  // kTolerance is normally 25%
#define LEGACY_TIMING_INFO false
const uint16_t kMinUnknownSize = 32;



void setup() {
  String tt[2]={"NEC;0x617048B7;32;ToggleOnOff","NEC;0x617014EB;32;Ulubione"};
  kody.push_back("dummy_code");
  kody.push_back(tt[1].c_str());
  kody.push_back(tt[0].c_str());
  users[0] = "admin";
  passwd[0] = "admin";
  Serial.begin(115200);
  sender.begin();
  irrecv.setUnknownThreshold(kMinUnknownSize);
  irrecv.setTolerance(kTolerancePercentage);  // Override the default tolerance.
  irrecv.enableIRIn();
  // Connect to WiFi
  Serial.println("Setting up WiFi");
  WiFi.begin(WIFI_SSID, WIFI_PSK);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.print("Connected. IP=");
  Serial.println(WiFi.localIP());
  //Dodanie zakotwiczeń zasobów
  ResourceNode * nodeRoot      = new ResourceNode("/", "GET", &handleRoot);
  ResourceNode * nodeRemote       = new ResourceNode("/Remote", "GET", &handleRemote);
  ResourceNode * nodeRecive = new ResourceNode("/Recive", "GET", &handleRecive);
  ResourceNode * nodeSend = new ResourceNode("/Send", "GET", &handleSend);
  ResourceNode * nodeAdd = new ResourceNode("/Add", "GET", &handleAddCode);
  ResourceNode * nodeLogin = new ResourceNode("/check", "GET", &handleLogin);
  ResourceNode * node404       = new ResourceNode("", "GET", &handle404);

  secureServer.registerNode(nodeRoot);
  secureServer.registerNode(nodeRemote);
  secureServer.registerNode(nodeRecive);
  secureServer.registerNode(nodeSend);
  secureServer.registerNode(nodeAdd);
  secureServer.registerNode(nodeLogin);
  secureServer.setDefaultNode(node404);

  Serial.println("Starting server...");
  secureServer.start();
  if (secureServer.isRunning()) {
    Serial.println("Server ready.");
  }
}

void loop() {
  secureServer.loop();
  delay(10);
}

void handleRoot(HTTPRequest * req, HTTPResponse * res) {
  // We will deliver an HTML page

  if (!is_auth(req->getClientIP())) {
    res->setHeader("Content-Type", "text/html");
    res->print(Login);
  }
  else
    handleRemote(req, res);

}
void handleRemote(HTTPRequest * req, HTTPResponse * res)
{
  if (is_auth(req->getClientIP())) {

    Serial.println("uzyskano autoryzacje");
    res->setHeader("Content-Type", "text/html");
    res->println("<html><head>");
    res->println("<meta charset=\"utf-8\"/>");
    res->println("<script>");
    res->print("function send(data) {xhttp=new XMLHttpRequest(); let params=data.value; xhttp.open(\"GET\",\"/Send?x=\"+params); xhttp.send();}");
    res->println("</script>");
    res->println("<title>Pilot kodów IR</title></head><body>");
    res->println("<h1>Pilot do kodow IR w postaci Listy</h1>");
    Serial.printf("Aktualny rozmiar wektora %i \n", kody.size());
    Serial.printf("Element zerowy wektora %s \n" , kody[0].c_str());
    for (int i = 0; i < kody.size(); i++) {
      String w = "<p><input type=\"text\" size=\"50px\" onclick=\"send(this)\" readonly value=\"" + (String)kody[i].c_str();
      w.concat("\"/></p>");
      res->println(w);
    }
    res->println("</body></html>");
  }
  else handle404(req, res);
}
void handleRecive(HTTPRequest * req, HTTPResponse * res) {

  yield();
  delay(100);
  //irrecv.enableIRIn();
  if (irrecv.decode(&results)) {
    Serial.print("dekodowanie \n");
    String values = Uint64toPrint(results.value);
    //String raw_data = "";
   // for (uint16_t i = 1; i < results.rawlen; i++) {
    //  if (i % 100 == 0) yield();
   //   if (i & 1)
   //     raw_data += String(results.rawbuf[i] * kRawTick, DEC);
    //  else
    //    raw_data += "," + String(results.rawbuf[i] * kRawTick, DEC);
    //}// Serial.println(raw_data);
    res->setHeader("Content-Type", "text/html");
    res->println("<!DOCTYPE html>");
    res->println("<html>");
    res->println("<head><meta charset=\"utf-8\"/>");
    res->println("<title>Recived IR data</title>");
    res->println("<script>");
    res->println("function sender(){");
    res->println("xhttp=new XMLHttpRequest(); xhttp.open(\"GET\",\"/Add?x=\"+document.getElementById('model').value+\";\"+document.getElementById('value').value+\";\"+document.getElementById('len').value+\";\"+document.getElementById('koment').value+\";\"); ");
    res->println("xhttp.send();}  </script></head><body>");
    res->println("<p>Model/Protokol<input type=\"text\" id=\"model\" name=\"model\" readonly value=\"" + (String(marka[results.decode_type + 1]) + "\"></p>"));
    res->println("<p>Adres<input type=\"text\" id=\"address\" name=\"address\" readonly value=\"" + (String(results.address, HEX) + "\"></p>"));
    res->println("<p>Komenda<input type=\"text\" id=\"command\" name=\"command\" readonly value=\"" + (String(results.command, HEX) + "\"></p>"));
    res->print("<p>Wartośc komendy<input type=\"text\" id=\"value\" name=\"command\" readonly value=\"");
    res->print(values);
    res->println("\"></p>");
    res->println("<p>Ile bitow<input type=\"text\" id=\"len\" name=\"bits\" readonly value=\"" + (String(results.bits) + "\"></p><br>"));
    // res->print("<p>Kod IR w postaci RAW <input type=\"text\" id=\"raw\" name=\"bits\" readonly value=\"");
    //res->print(raw_data);
    //res->println("\"></p>");
    res->println("<p>Nazwa komendy do zapisania<input type=\"text\" id=\"koment\" name=\"nazwa\">");
    res->println("<button onclick=\"sender()\">Save Code</button><br>");
    res->println("<h3>Na potrzeby pracy zapisywane są tylko 3 pola model wartosc komendy i RAW oraz nazwa własna komendy ");
    res->println("</body></html>");
    //    raw_data="";
    // res->finalize();
   // irrecv.disableIRIn();
   irrecv.resume();
  }
}
void handleSend(HTTPRequest * req, HTTPResponse * res) {
  if(is_auth(req->getClientIP())){
  ResourceParameters * params = req->getParams();
  std::string ss = "", prot = "", war = "", len = "";
  params->getQueryParameter("x", ss);
  byte counter = 0;
 // int cnt1 = 0, cnt2 = 0,cnt3=0;
    for (int i = 0; i < ss.length(); i++)
    { 
      if (ss[i] == ';')
      {
        counter++;
        i++;
      }
       if (counter == 0) //model
      {
        prot+= ss[i];
        
      }
       else if (counter == 1) //komenda
      {
        war+= ss[i];
       
      }
      else if(counter ==2){ //len
        len+=ss[i];
        
        }
        else if(counter==3)
      break;
    }
    Serial.printf("Protokol= %s \n",prot.c_str());
    Serial.printf("Wartosc= %s \n",war.c_str());
    Serial.printf("rozmiar= %s \n",len.c_str());
    SendIR(prot,strtoull(war.c_str(),NULL,10),(uint16_t)strtol(len.c_str(),NULL,10));
    res->setHeader("Content/Type","text/plain");
    res->printf("wyslano wiadomosc model: %s kod: %s dlugosc: %s \n",prot.c_str(),war.c_str(),len.c_str()); }
    else
    handle404(req,res);
}
void handleAddCode(HTTPRequest * req, HTTPResponse * res) {
  std::string ww = "";
  ResourceParameters * params = req->getParams();
  params->getQueryParameter("x", ww);
  kody.push_back(ww);
  
}
void handleLogin(HTTPRequest * req, HTTPResponse * res) {
  ResourceParameters * params = req->getParams();
  std::string xx = "", yy = "";
  //Serial.println(ESP.getFreeHeap());
  //Serial.println(req->getRequestString().c_str());
  params->getQueryParameter("login", xx);
  params->getQueryParameter("passwd", yy);
  int t = params->getQueryParameterCount();
  Serial.printf("Ile jest parametrow %i\n", t);
  Serial.printf("user %s\n", xx.c_str());
  Serial.printf("passwd %s\n" , yy.c_str());
  if (czek(xx.c_str(), yy.c_str()))
    {auth(req->getClientIP());
  handleRemote(req, res);}
}
void handle404(HTTPRequest * req, HTTPResponse * res) {
  req->discardRequestBody();
  res->setStatusCode(404);
  res->setStatusText("Not Found");
  res->setHeader("Content-Type", "text/html");
  res->println("<!DOCTYPE html>");
  res->println("<html>");
  res->println("<head><title>Not Found</title></head>");
  res->println("<body><h1>404 Not Found</h1><p>The requested resource was not found on this server.</p></body>");
  res->println("</html>");
}
void auth(IPAddress ip) {
  for (byte w = 0; w < 4; w++)


    if (klient[w].addr == ip && !klient[w].auth) {
      klient[w].auth = true;
      break;
    }

    else if (klient[w].addr[0] == 0) {
      klient[w].addr = ip;
      klient[w].auth = true;
      break;
    }
    else {
      break;
    }
}
bool is_auth(IPAddress ip) {
  for (byte w = 0; w < 4; w++)
    if (klient[w].addr == ip)
    { return true;
      break;
    }
  return false;
}

void SendIR(std::string model,uint64_t irdata,uint16_t len) {
 // int tmp;
  for(int i=0;i<100;i++){
    Serial.printf("Oczekiwany model: :%s: otrzymany: :%s: \n",model,marka[i].c_str());
    delay(50);
  if(model==(std::string)marka[i].c_str()){

    Serial.print((decode_type_t)(i-1));
  sender.send((decode_type_t)(i-1),irdata,len,2);
  break;
  }
  }Serial.print("Nie udalo sie wyslac kodu");
  
  
  
  }
