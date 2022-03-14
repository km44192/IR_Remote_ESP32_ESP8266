const static char *Login={
"<!DOCTYPE html>\r\n"
"<html lang=\"pl\">\r\n"
"<head>\r\n"
"<meta charset=\"UTF-8\">\r\n"
"<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\r\n"
"<title>ESP32 Login</title>\r\n"
"<script>\r\n"
"    function login(){\r\n"
"const xhttp=new XMLHttpRequest();\r\n"
"if(document.getElementById('_1').value!=\"\"&&document.getElementById('_2').value!=\"\")\r\n"
"{let params=document.getElementById('_1').name+\"=\"+document.getElementById('_1').value+\"&\"+document.getElementById('_2').name+\"=\"+document.getElementById('_2').value\r\n"
"document.getElementById('alert').hidden=false;\r\n"
"xhttp.open(\"GET\",\"/check?\"+params)\r\n"
"xhttp.send(); \r\n"
"}\r\n"
"else{\r\n"
"    document.getElementById('alert').style.backgroundColor=\"red\";\r\n"
"    document.getElementById('alert').style.width=\"150px\";\r\n"
"    document.getElementById('alert').style.height=\"25px\";\r\n"
"    document.getElementById('alert').hidden=false;\r\n"
"    document.getElementById('alert').innerHTML=\"nie wpisano danych\"\r\n"
"}\r\n"
"if(xhttp.status==200){\r\n"
"    alert(\"zalogowano\")\r\n"
"    delay(200)\r\n"
"}\r\n"
"    }\r\n"
"</script>\r\n"
"</head>\r\n"
"<body>\r\n"
"<div id=\"alert\" hidden></div>\r\n"
"<p>Login</p>\r\n"
"<input type=\"text\" id=\"_1\" name=\"login\" ><br>\r\n"
"<p>Password</p>\r\n"
"<input type=\"password\" id=\"_2\" name=\"passwd\">\r\n"
"<button onclick=\"login()\">Login</button>   \r\n"
"</form>\r\n"
"</body>\r\n"
"</html>\r\n"
};
static String users[5];
static String passwd[5];
static bool czek(String us,String pass){
  for(byte i=0;i<5;i++)
  if((us==users[i])&&(pass==passwd[i]))
  {return true;
  break;}
  return false;
}
//tutaj bedą przchowywane kody do IR
std::vector<std::string> kody;
const static String marka[] = {
  "UNKNOWN" ,
  "UNUSED",
  "RC5",
  "RC6",
  "NEC",
  "SONY",
  "PANASONIC",  // (5)
  "JVC",
  "SAMSUNG",
  "WHYNTER",
  "AIWA_RC_T501",
  "LG",  // (10)
  "SANYO",
  "MITSUBISHI",
  "DISH",
  "SHARP",
  "COOLIX",  // (15)
  "DAIKIN",
  "DENON",
  "KELVINATOR",
  "SHERWOOD",
  "MITSUBISHI_AC",  // (20)
  "RCMM",
  "SANYO_LC7461",
  "RC5X",
  "GREE",
  "PRONTO",  // Technically not a protocol, but an encoding. (25)
  "NEC_LIKE",
  "ARGO",
  "TROTEC",
  "NIKAI",
  "RAW",  // Technically not a protocol, but an encoding. (30)
  "GLOBALCACHE",  // Technically not a protocol, but an encoding.
  "TOSHIBA_AC",
  "FUJITSU_AC",
  "MIDEA",
  "MAGIQUEST",  // (35)
  "LASERTAG",
  "CARRIER_AC",
  "HAIER_AC",
  "MITSUBISHI2",
  "HITACHI_AC",  // (40)
  "HITACHI_AC1",
  "HITACHI_AC2",
  "GICABLE",
  "HAIER_AC_YRW02",
  "WHIRLPOOL_AC",  // (45)
  "SAMSUNG_AC",
  "LUTRON",
  "ELECTRA_AC",
  "PANASONIC_AC",
  "PIONEER",  // (50)
  "LG2",
  "MWM",
  "DAIKIN2",
  "VESTEL_AC",
  "TECO",  // (55)
  "SAMSUNG36",
  "TCL112AC",
  "LEGOPF",
  "MITSUBISHI_HEAVY_88",
  "MITSUBISHI_HEAVY_152",  // 60
  "DAIKIN216",
  "SHARP_AC",
  "GOODWEATHER",
  "INAX",
  "DAIKIN160",  // 65
  "NEOCLIMA",
  "DAIKIN176",
  "DAIKIN128",
  "AMCOR",
  "DAIKIN152",  // 70
  "MITSUBISHI136",
  "MITSUBISHI112",
  "HITACHI_AC424",
  "SONY_38K",
  "EPSON",  // 75
  "SYMPHONY",
  "HITACHI_AC3",
  "DAIKIN64",
  "AIRWELL",
  "DELONGHI_AC",  // 80
  "DOSHISHA",
  "MULTIBRACKETS",
  "CARRIER_AC40",
  "CARRIER_AC64",
  "HITACHI_AC344",  // 85
  "CORONA_AC",
  "MIDEA24",
  "ZEPEAL",
  "SANYO_AC",
  "VOLTAS",  // 90
  "METZ",
  "TRANSCOLD",
  "TECHNIBEL_AC",
  "MIRAGE",
  "ELITESCREENS",  // 95
  "PANASONIC_AC32",
  "MILESTAG2",
  "ECOCLIM",
  "XMP",
  "TRUMA",  // 100
  "HAIER_AC176",
  "TEKNOPOINT",
  "KELON",
  "TROTEC_3550",
  "SANYO_AC88",  // 105
  "BOSE",
  "ARRIS",
  "RHOSS"
};

static String Uint64toPrint(uint64_t s){
String tmp="";
while(s>0)
{
  byte w=s%16;
  char t=(w < 10 ? w + '0' : w + 'A' - 10);
  tmp=t+tmp;
  s=s/16;
  

}
return "0x"+tmp;
}
static struct{
  std::string token="";
  uint32_t expire=0;
  }klient[4];
 
