
//#define WIFI_SSID "Orange_Swiatlowod_9E6A"
//#define WIFI_PSK  "C4E5TY7ASKJL"
// Include certificate data (see note above)
#include "cert.h"
#include "private_key.h"
#include <WiFi.h>
#include <IRrecv.h>
#include <IRsend.h>
//#include <IRac.h>
#include <HTTPSServer.hpp>
#include <SSLCert.hpp>
#include <HTTPRequest.hpp>
#include <HTTPResponse.hpp>
#include "sites.h"
#include <Preferences.h>
// The HTTPS Server comes in a separate namespace. For easier use, include it here.

using namespace httpsserver;
bool is_auth(std::string token) {
  for (byte w = 0; w < 4; w++)
    if (klient[w].token == token&&klient[w].expire>millis())
    { return true;
      break;
    }
    else if(klient[w].token==token&&klient[w].expire-millis()>millis())
    {//if time expires delete token
      //but how to prevent miscalculation when counter overflows
      klient[w].token="";klient[w].expire=0;
      return false;
      break;
      }
  return false;
}



 std::string auth() {
  for (byte w = 0; w < 4; w++)
  {
  if(klient[w].token=="")
   {
   byte s=random(2,10);//losowanie dlugosci klucza
   for(byte tt=0;tt<s;tt++)
    {
    if(tt<(s-1))
     klient[w].token+=String(esp_random()).c_str();//kodowanie random
    else if(tt==(s-1))
     klient[w].token+=String(millis()).c_str();//ostatnie bajty tokenu
    }
klient[w].expire=millis()+864000000;//token valid about 10 days
   return klient[w].token;
   break;
   }
  }
}

 String TSSID="Orange_Swiatlowod_9E6A";
 String TPASSWD="C4E5TY7ASKJL";
SSLCert cert = SSLCert(
                 server_crt_DER, server_crt_DER_len,
                 server_key_DER, server_key_DER_len
               );

HTTPSServer secureServer = HTTPSServer(&cert);
Preferences wificonfig;
void handleRoot(HTTPRequest * req, HTTPResponse * res); //Root
void handleLogin(HTTPRequest * req, HTTPResponse * res); //Czy zalogowany
void handleRemote(HTTPRequest * req, HTTPResponse * res); //Pilot
void handleRecive(HTTPRequest * req, HTTPResponse * res); //Odbior
void handleAddCode(HTTPRequest * req, HTTPResponse * res); //Dodaniekodu_IR
void handleSend(HTTPRequest * req, HTTPResponse * res); //Wysylanie_kodu
void handle404(HTTPRequest * req, HTTPResponse * res); //404_not_found
void handleConfig(HTTPRequest * req,HTTPResponse * res);
void ESPConfig(HTTPRequest * req,HTTPResponse * res);
IRsend sender(4);
decode_results results;
uint32_t myTime=0;
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
  //
  wificonfig.begin("config",false);
  pinMode(LED_BUILTIN,OUTPUT);

  kody.push_back("dummy_code");

  users[0] = "admin";
  passwd[0] = "admin";
  Serial.begin(115200);
  sender.begin();
  irrecv.setUnknownThreshold(kMinUnknownSize);
  irrecv.setTolerance(kTolerancePercentage);  // Override the default tolerance.
  irrecv.enableIRIn();
  // Connect to WiFi
  TSSID=wificonfig.getString("SSID");
  TPASSWD=wificonfig.getString("PASSWD");
  Serial.println("Setting up WiFi");
  WiFi.mode(WIFI_STA);
  WiFi.begin(TSSID.c_str(), TPASSWD.c_str());
  uint32_t t1=millis(),t2=millis();
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    if(t2-t1<2000){
    vTaskDelay(100);
    t2=millis();}
    else
    {
      WiFi.mode(WIFI_AP);
      WiFi.begin("ConfigESP","Configuration",1,NULL);
      break;
    }


  }
  Serial.print("Connected. IP=");
  if(WiFi.status()==WL_CONNECTED)
  Serial.println(WiFi.localIP());
  else
  Serial.println(WiFi.softAPIP());
  //Dodanie zakotwiczeń zasobów
  ResourceNode * nodeRoot      = new ResourceNode("/", "GET", &handleRoot);
  ResourceNode * nodeRemote       = new ResourceNode("/Remote", "GET", &handleRemote);
  ResourceNode * nodeRecive = new ResourceNode("/Recive", "GET", &handleRecive);
  ResourceNode * nodeSend = new ResourceNode("/Send", "GET", &handleSend);
  ResourceNode * nodeAdd = new ResourceNode("/Add", "GET", &handleAddCode);
  ResourceNode * nodeLogin = new ResourceNode("/check", "GET", &handleLogin);
  ResourceNode * node404       = new ResourceNode("", "GET", &handle404);
  ResourceNode * nodeConfig = new ResourceNode("/config","GET",&handleConfig);
  ResourceNode * ESPConf = new ResourceNode("/upload","GET",&ESPConfig);
  secureServer.registerNode(nodeRoot);
  secureServer.registerNode(nodeRemote);
  secureServer.registerNode(nodeRecive);
  secureServer.registerNode(nodeSend);
  secureServer.registerNode(nodeAdd);
  secureServer.registerNode(nodeLogin);
  secureServer.registerNode(nodeConfig);
  secureServer.registerNode(ESPConf);
  secureServer.setDefaultNode(node404);

  Serial.println("Starting server...");
  secureServer.start();
  if (secureServer.isRunning()) {
    Serial.println("Server ready.");
  }
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

void loop() {
  secureServer.loop();
  delay(10);
   if (irrecv.decode(&results)&&!is_changed) 
   {
   
    digitalWrite(LED_BUILTIN,1);
    if(millis()-myTime>500){
       myTime=millis();
    proto=marka[results.decode_type + 1];
    adres=String(results.address, HEX);
    komenda=String(results.command, HEX);
    very_long_int=Uint64toPrint(results.value);
    bity=String(results.bits);
    digitalWrite(LED_BUILTIN,0);
    is_changed=true;}
    }
}
/*po wpisaniu adresu IP bądź przypisanego adresu url domyślnie obsłuż to*/
void handleRoot(HTTPRequest * req, HTTPResponse * res) {


  if (!is_auth(req->getHeader("Cookie"))) {
    res->setHeader("Content-Type", "text/html");
    //res->setHeader("Cookie",
    res->print(Login);
  }
  else
    handleRemote(req, res);

}/*Jeżeli klient podał poprawne dane, to wyświetl dane zdalnego pilota*/
void handleRemote(HTTPRequest * req, HTTPResponse * res)
{
  if (is_auth(req->getHeader("Cookie"))) {
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
}/**/
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
  
}/**/
void handleSend(HTTPRequest * req, HTTPResponse * res) {
  if(is_auth(req->getHeader("Cookie"))){
  ResourceParameters * params = req->getParams();
  std::string ss = "", prot = "", war = "", len = "";
  params->getQueryParameter("x", ss);
  byte counter = 0;
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
}/**/
void handleAddCode(HTTPRequest * req, HTTPResponse * res) {
  std::string ww = "";
  ResourceParameters * params = req->getParams();
  params->getQueryParameter("x", ww);
  kody.push_back(ww);
  
}
/**/
void handleLogin(HTTPRequest * req, HTTPResponse * res) {
  ResourceParameters * params = req->getParams();
  std::string xx = "", yy = "";
  params->getQueryParameter("login", xx);
  params->getQueryParameter("passwd", yy);
  int t = params->getQueryParameterCount();
  Serial.printf("Ile jest parametrow %i\n", t);
  Serial.printf("user %s\n", xx.c_str());
  Serial.printf("passwd %s\n" , yy.c_str());
  if (czek(xx.c_str(), yy.c_str()))
    {res->setHeader("Cookie",auth().c_str());
  handleRemote(req, res);}
}
/*Jeżeli nie ma takiego zasobu wyświetl to*/
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
/*Zmiana danych do logowania do sieci*/
void handleConfig(HTTPRequest * req,HTTPResponse * res){
  res->setHeader("Content-type","text/html");
res->print("<!DOCTYPE HTML>");
res->print("<head><meta charset=\"utf-8\">");
res->print("<title>Konfiguracja Wifi</title><script>function config(){let x= new XMLHttpRequest(); var params=\"/upload?ssid=\"document.getElementById('Ssid').value+\"&passwd=\"+document.getElementById('Passwd').value; x.open(\"GET\",params); x.send();document.getElementById('alert').innerHTML=x.responseText();}</script>");
res->print("</head><body>");
res->print("<div id=\"alert\"></div>");
res->print("Nowa nazwa sieci<br>");
res->print("<input type=\"text\" id=\"Ssid\" name=\"ssid\"><br>");
res->print("<input type=\"passwd\" id=\"Passwd\" name=\"passwd\"><br>");
res->print("<button onclick=\"config()\">Konfiguruj</button>");


}/* Zapisanie danych logowania do pamięci urządzenia*/
void ESPConfig(HTTPRequest * req,HTTPResponse * res){
ResourceParameters * x=req->getParams();
std::string ss,ps;
x->getQueryParameter("ssid",ss);
x->getQueryParameter("passwd",ps);
wificonfig.putString("SSID",(String)ss.c_str());
wificonfig.putString("PASSWD",(String)ps.c_str());
res->setHeader("Content-type","text/plain");
res->print("Zaraz nastąpi reset urządzenia");
vTaskDelay(3000);
ESP.restart();
}
