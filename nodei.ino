

#ifdef ESP32
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>
#endif
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>   
#include <ArduinoJson.h>


const char* ssid = "Ddde";
const char* password = "alwin123456";


#define BOTtoken "5563434390:AAGOnNWn-lOl4tueCwS8UusruZ-17TBT5_4"  


#define CHAT_ID "-1001735110088"

#ifdef ESP8266
  X509List cert(TELEGRAM_CERTIFICATE_ROOT);
#endif

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

// Checks for new messages every 1 second.
int botRequestDelay = 1000;
unsigned long lastTimeBotRan;

const int ledPin = 2;
bool ledState = HIGH;

// Handle what happens when you receive new messages
void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  for (int i=0; i<numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID){
      bot.sendMessage(chat_id, "Unauthorized user", "");
      continue;
    }
    
    String text = bot.messages[i].text;
    Serial.println(text);

    String from_name = bot.messages[i].from_name;
int  b= digitalRead(D7);
int  c= digitalRead(D6);
    if (b==1&&c==0 ) {
      bot.sendMessage(chat_id, "MEDICINE IS DELIVERED", "");
      ledState = HIGH;
      digitalWrite(D0, ledState);
       delay(20000);
    }
    
 if (b==0&&c==1 ) {
      bot.sendMessage(chat_id, " ALERT:MEDICINES ARE CLEARED", "");
      ledState = LOW;
      digitalWrite(D0, ledState);
      delay(20000);
    }
    
    if (text == "s") {
      if (digitalRead(ledPin)){
        bot.sendMessage(chat_id," ", "");
      }
      else{
        bot.sendMessage(chat_id, " ", "");
      }
    }
  }
}

void setup() {
  Serial.begin(115200);

  #ifdef ESP8266
    configTime(0, 0, "pool.ntp.org");      
    client.setTrustAnchors(&cert); 
  #endif
  pinMode(D0, OUTPUT);
  digitalWrite(D0,HIGH);
  pinMode(D6,INPUT);
  pinMode(D7, INPUT);
  // Connect to Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  #ifdef ESP32
    client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
  #endif
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
 
  Serial.println(WiFi.localIP());

  bot.sendMessage(CHAT_ID, "Bot Started", "");
}

void loop() {
  if (millis() > lastTimeBotRan + botRequestDelay) {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while(numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }
}
