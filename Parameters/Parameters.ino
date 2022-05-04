
#define WIFI_SSID "xxxxxxxxxx"
#define WIFI_PSK  "yyyyyyyyyy"
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

bool is_changed=false;
String proto="",adres="",komenda="",kodes="",bity="",very_long_int="";
IRrecv irrecv(kRecvPin, kCaptureBufferSize, kTimeout, true);
const uint8_t kTolerancePercentage = 25;  // kTolerance is normally 25%
#define LEGACY_TIMING_INFO false
const uint16_t kMinUnknownSize = 32;



void setup() {
  pinMode(LED_BUILTIN,OUTPUT);
  //String tt[2]={"NEC;0x617048B7;32;ToggleOnOff","NEC;0x617014EB;32;Ulubione"};
  kody.push_back("dummy_code");
 // kody.push_back(tt[1].c_str());
  //kody.push_back(tt[0].c_str());
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
   if (irrecv.decode(&results)&&!is_changed) 
   {
    digitalWrite(LED_BUILTIN,HIGH);
    delay(500);
    proto=marka[results.decode_type + 1];
    adres=String(results.address, HEX);
    komenda=String(results.command, HEX);
    very_long_int=Uint64toPrint(results.value);
    bity=String(results.bits);
    digitalWrite(LED_BUILTIN,0);
    is_changed=true;
    }
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
    for (int i = 1; i < kody.size(); i++) {
      String w = "<p><input type=\"text\" size=\"50px\" onclick=\"send(this)\" readonly value=\"" + (String)kody[i].c_str();
      w.concat("\"/></p>");
      res->println(w);
    }
    res->println("</body></html>");
  }
  else handle404(req, res);
}
void handleRecive(HTTPRequest * req, HTTPResponse * res) {
    Serial.print("dekodowanie \n");
    res->setHeader("Content-Type", "text/html");
    res->println("<!DOCTYPE html>");
    res->println("<html>");
    res->println("<head><meta charset=\"utf-8\"/>");
    res->println("<title>Recived IR data</title>");
    res->println("<script>");
    res->println("function sender(){");
    res->println("xhttp=new XMLHttpRequest(); xhttp.open(\"GET\",\"/Add?x=\"+document.getElementById('model').value+\";\"+document.getElementById('value').value+\";\"+document.getElementById('len').value+\";\"+document.getElementById('koment').value+\";\"); ");
    res->println("xhttp.send();}  </script></head><body>");
    res->println("<p>Model/Protokol<input type=\"text\" id=\"model\" name=\"model\" readonly value=\"" + (proto + "\"></p>"));
    res->println("<p>Adres<input type=\"text\" id=\"address\" name=\"address\" readonly value=\"" + (adres + "\"></p>"));
    res->println("<p>Komenda<input type=\"text\" id=\"command\" name=\"command\" readonly value=\"" + (komenda + "\"></p>"));
    res->print("<p>Wartośc komendy<input type=\"text\" id=\"value\" name=\"command\" readonly value=\"");
    res->print(very_long_int);
    res->println("\"></p>");
    res->println("<p>Ile bitow<input type=\"text\" id=\"len\" name=\"bits\" readonly value=\"" + (bity + "\"></p><br>"));
    res->println("<p>Nazwa komendy do zapisania<input type=\"text\" id=\"koment\" name=\"nazwa\">");
    res->println("<button onclick=\"sender()\">Save Code</button><br>");
    res->println("<h3>Na potrzeby pracy zapisywane są tylko 3 pola model wartosc komendy i RAW oraz nazwa własna komendy</h3> ");
    res->println("Odswiezenie strony powoduje zdejcie blokady na odbieranie kodow IR. Dziala to jednorazowo(jeden kod IR na odswiezenie)</br>Otrzymanie kodu zostaje zasygnalizowane zaswieceniem sie wbudowanej diody LED<br>");
    res->println("</body></html>");
  is_changed=false;
  
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
   
    res->setHeader("Content/Type","text/plain");
    res->printf("wyslano wiadomosc model: %s kod: %s dlugosc: %s \n",prot.c_str(),war.c_str(),len.c_str()); 
     SendIR(prot,strtoull(war.c_str(),NULL,16),(uint16_t)strtol(len.c_str(),NULL,10));
    }
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
 delay(300);
 yield();
  for(int i=0;i<100;i++){
    Serial.printf("Oczekiwany model: :%s: otrzymany: :%s: \n",model.c_str(),marka[i].c_str());
    delay(50);
  if(model==(std::string)marka[i].c_str()){
    //if((decode_type_t)(i-1)==NEC)
    Serial.println(Uint64toPrint(irdata));
    Serial.print((decode_type_t)i-1);
    
  sender.send((decode_type_t)(i-1),irdata,len);
  break;
  }
  }//Serial.print("Nie udalo sie wyslac kodu");
  
  
  
  }
