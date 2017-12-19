
/*
อย่าลืมเปลี่ยน ssid, password
*/

#include <MicroGear.h>
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>

const char* ssid     = "Indy";
const char* password = "13245678";

#define APPID   "weight"
#define KEY     "QvwRl0kgVvED6qw"
#define SECRET  "VYaQB67GFhREIAO9Iva5Sev33"

#define ALIAS   "NodeMCU1"
#define TargetWeb "HTML_web"

WiFiClient client;
MicroGear microgear(client);
SoftwareSerial ESPserial(4,0);

int value;

void onMsghandler(char *topic, uint8_t* msg, unsigned int msglen) 
{
    Serial.print("Incoming message --> ");
    msg[msglen] = '\0';
    Serial.println((char *)msg);
}


void onConnected(char *attribute, uint8_t* msg, unsigned int msglen) 
{
    Serial.println("Connected to NETPIE...");
    microgear.setAlias(ALIAS);
}

void setup() 
{
    ESPserial.begin(115200);
     /* Event listener */
    microgear.on(MESSAGE,onMsghandler);
    microgear.on(CONNECTED,onConnected);

    //dht.begin();
    Serial.begin(115200);
    Serial.println("Starting...");

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) 
    {
       delay(250);
       Serial.print(".");
    }

    Serial.println("WiFi connected");  
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    microgear.init(KEY,SECRET,ALIAS);
    microgear.connect(APPID);
}

int buildUp(int index, unsigned char outbox[])
{
  unsigned long d;

  d =  (outbox[index+3] << 24) | (outbox[index+2] << 16)
    | (outbox[index+1] << 8) | (outbox[index]);
  int member = *(int *)&d;
  return member;
}

void loop() 
{
    unsigned char data[4];
    if(ESPserial.available() > 0){
      data[0] = ESPserial.read();
      Serial.print("data0 = ");
      Serial.println(data[0]);
      data[1] = ESPserial.read();
      Serial.print("data1 = ");
      Serial.println(data[1]);
      data[2] = ESPserial.read();
      Serial.print("data2 = ");
      Serial.println(data[2]);
      data[3] = ESPserial.read();
      Serial.print("data3 = ");
      Serial.println(data[3]);
      value = buildUp(0,data);
      if(value < 0) value = 0;
      Serial.print("value = ");
      Serial.println(value);
      
    }
    if (microgear.connected())
    {
       microgear.loop();
       Serial.println("connected");

       microgear.chat(TargetWeb , value); // <- function ส่งขึ้นเน็ตพาย
    }
   else 
   {
    Serial.println("connection lost, reconnect...");
    microgear.connect(APPID);
   }
    delay(1500);
}
