/*
  This is a simple example show the Heltec.LoRa sended data in OLED.

  The onboard OLED display is SSD1306 driver and I2C interface. In order to make the
  OLED correctly operation, you should output a high-low-high(1-0-1) signal by soft-
  ware to OLED's reset pin, the low-level signal at least 5ms.

  OLED pins to ESP32 GPIOs via this connecthin:
  OLED_SDA -- GPIO4
  OLED_SCL -- GPIO15
  OLED_RST -- GPIO16
  
  by Aaron.Lee from HelTec AutoMation, ChengDu, China
  成都惠利特自动化科技有限公司
  https://heltec.org
  
  this project also realess in GitHub:
  https://github.com/Heltec-Aaron-Lee/WiFi_Kit_series
*/

#include "heltec.h"

#define BAND    868E6  //you can set band here directly,e.g. 868E6,915E6

uint32_t counter = 0;
String rssi = "RSSI --";
String packSize = "--";
String packet;
String insultcall = "";

void IRAM_ATTR buttonpush()
{
  counter++;
  loraSend(counter);
}

void setup()
{
   //WIFI Kit series V1 not support Vext control
  Heltec.begin(true /*DisplayEnable Enable*/, true /*Heltec.Heltec.Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, BAND /*long BAND*/);
 
  Heltec.display->init();
  Heltec.display->flipScreenVertically();  
  Heltec.display->setFont(ArialMT_Plain_10);

  Heltec.display->clear();
  
  Heltec.display->drawString(0, 0, "Heltec.LoRa Initial success!");
  Heltec.display->display();
  delay(100);
  pinMode(0, INPUT_PULLUP);
  attachInterrupt(0, buttonpush, FALLING);
}

void loop()
{
  insultcall=insultGen();

  Heltec.display->clear();
  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
  Heltec.display->setFont(ArialMT_Plain_10);
  
  Heltec.display->drawString(0, 0, "Sending packet: ");
  Heltec.display->drawString(90, 0, String(counter));
  Heltec.display->drawStringMaxWidth(0, 20, 128, insultcall);
  Heltec.display->display();
  // send packet
  loraSend(counter);
  counter++;
  digitalWrite(LED, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(100);                       // wait for a second
  digitalWrite(LED, LOW);    // turn the LED off by making the voltage LOW
  delay(10e3);                       // wait for 10 seconds
}

void loraSend(uint32_t counterin)
{
  LoRa.beginPacket();
  LoRa.setTxPower(14,RF_PACONFIG_PASELECT_PABOOST);
/*
 * LoRa.setTxPower(txPower,RFOUT_pin);
 * txPower -- 0 ~ 20
 * RFOUT_pin could be RF_PACONFIG_PASELECT_PABOOST or RF_PACONFIG_PASELECT_RFO
 *   - RF_PACONFIG_PASELECT_PABOOST -- LoRa single output via PABOOST, maximum output 20dBm
 *   - RF_PACONFIG_PASELECT_RFO     -- LoRa single output via RFO_HF / RFO_LF, maximum output 14dBm
*/
  LoRa.print(insultcall);
  LoRa.print(" -");
  LoRa.print(counterin);
  LoRa.endPacket();
}

String insultGen()
{
  String verb[24] = {"lazy","stupid","insecure","idiotic","slimy",
	"slutty","smelly","pompous","communist","dicknose",
	"pie-eating","racist","elitist","white trash","drug-loving",
	"butterface","tone deaf","ugly","creepy","nut busting",
	"son of a","bible thumping","mud dwelling","cum guzzling"};

String adjective[22] = {"douche","ass","turd","rectum","butt","cock","shit",
	"crotch","bitch","turd","prick","slut","taint","fuck",
	"dick","boner","shart","nut","sphincter","foreskin","wank","dildo"};

String noun[26] = {"pilot","canoe","captain","pirate","hammer","knob",
	"box","jockey","nazi","waffle","goblin","blossum","biscuit",
	"clown","socket","monster","hound","dragon","balloon","foreskin",
	"fairy","puddle","pickle","jar","nugget","dragon queen"};

  return "You're nothing but a "+verb[int(random(0,24))]+" "+adjective[int(random(0,22))]+" "+noun[int(random(0,26))]+"!";
}