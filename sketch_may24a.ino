#include <RTClib.h>
#include <Servo.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo servo1,servo2,servo3;  
RTC_DS3231 rtc;
int g;
char t[32];
int h[10],m[10];
int counter = 0;
int currentStateCLK;
int lastStateCLK;
unsigned long lastButtonPress = 0;
//function displaying date----------------------------------------------------------------------------------------------------------------------
void dat()
{
DateTime now = rtc.now();
sprintf(t, "%02d:%02d:%02d %02d/%02d/%02d <<<<<<<<", now.hour(), now.minute(), now.second(), now.day(), now.month(), now.year());  
Serial.println(t);
}
//function for inputing through rotary----------------------------------------------------------------------------------------------------------
int input(){
counter=0;
while (true)
{
// Read the current state of CLK
currentStateCLK = digitalRead(2);
// If last and current state of CLK are different, then pulse occurred
// React to only 1 state change to avoid double count
if (currentStateCLK != lastStateCLK  && currentStateCLK == 1){

// If the DT state is different than the CLK state then
// the encoder is rotating CCW so decrement
if (digitalRead(3) != currentStateCLK) {
counter ++;
} else {
counter --;
}
Serial.print(" ");
Serial.print(counter);
if(counter>=0&&counter<10){
lcd.setCursor(10,1);
lcd.print(counter);
lcd.print(" ");
}
else{
lcd.setCursor(10,1);
lcd.print(counter);}
}
// Remember last CLK state
lastStateCLK = currentStateCLK;
// Read the button state
int btnState = digitalRead(4);
//If we detect LOW signal, button is pressed
if(btnState ==LOW)
{
 return counter; 
}}}
//function for monitoring switch(rotary)--------------------------------------------------------------------------------------------------------
int ex(){
int btnState = digitalRead(4);
if(btnState ==LOW)
{
 return(1); 
}
return 0;
}
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
void setup() {
// Setup Serial Monitor
Serial.begin(9600);

lcd.begin();


  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("MEDICINE VENDER");

 pinMode(0, OUTPUT);
  pinMode(1, OUTPUT);
pinMode(10,OUTPUT);//led
pinMode(11,OUTPUT);//buzzer

servo1.attach(9);
servo2.attach(8);
servo3.attach(7);

pinMode (5, INPUT); // irsensor pin

Wire.begin();

rtc.begin();
//rtc.adjust(DateTime(F(__DATE__),F(__TIME__)));


 pinMode(2,INPUT);//clk
 pinMode(3,INPUT);//dt
 pinMode(4, INPUT_PULLUP);//sw
 

// Read the initial state of CLK
lastStateCLK = digitalRead(2);

}
// the loop function runs over and over again forever *******************************************************************************************
void loop() {
  DateTime now = rtc.now();
  Serial.print(F("\nDate/Time: "));
  dat();
  delay(1000);
  
  lcd.setCursor(0,0);
  lcd.print("Enter the number");
  lcd.setCursor(0,1);
  lcd.print("of doses:");
  
  Serial.print("Enter the number of doses:");
  int a=input();delay(1000);
  for(int i=0;i<a;i++){
    int v=i+1;

    lcd.setCursor(0,0);
  lcd.print("Enter the time");
  lcd.setCursor(0,1);
  lcd.print("for dose ");
  lcd.setCursor(10,1);
  lcd.print(v);
  lcd.setCursor(11,1);
  lcd.print(">");
  
  Serial.print("\nEnter the time for dose ");
 Serial.print(v);
 Serial.print("->");
  delay(1000);
lcd.setCursor(0,0);
lcd.print("Time            ");
lcd.setCursor(0,1);
lcd.print("hour:");
  
  Serial.print("\nhour:");
  h[i]=input();
  delay(1000);

lcd.setCursor(0,1);
lcd.print("minute:");
  
  Serial.print("\nminute:");
  m[i]=input();
  delay(1000);
  }
  Serial.print("\n*******************************");
 // DateTime now = rtc.now();
 while(true){
  if(ex()==1){
  break;
  }
  int i=0;
 //Serial.print(".\n-\n");
  for(i=0;i<a;i++){
  if(h[i]==now.hour()&& m[i]==now.minute()){
    
 lcd.setCursor(0,0);
lcd.print(">MEDCINES ARE    ");
 lcd.setCursor(0,1);
lcd.print("DELIVERD : ");
 digitalWrite(0, LOW);
 digitalWrite(1, HIGH);

  Serial.print("\n>>>>>>>>MEDCINES ARE DELIVERD : ");
  dat();
  tone(11,200);
 delay(3000);
  noTone(11);
  Serial.println("servo1");
  servo1.write(90);
  delay(3000);
  servo1.write(0);
  Serial.println("servo2");
  servo2.write(90);
  delay(3000);
  servo2.write(0);
  delay(5000);
   digitalWrite(1, LOW);
 digitalWrite(0,LOW);
  g=now.minute() ;
  while(g!=(m[i]+2) || (g!=((m[i]+2)%60))){
  now = rtc.now();
  g=now.minute();
   // Serial.println("\n");
   //Serial.println(g);
  int statusSensor = digitalRead(5);
 if (statusSensor == 0) {

  
 lcd.setCursor(0,0);
 lcd.print(">MEDCINES ARE    ");
 lcd.setCursor(0,1);
lcd.print("TAKEN ");
  
 Serial.print("\n>>>>>>>>MEDCINES ARE TAKEN :");
 dat();
 delay(60000);
 delay(60000);
 delay(60000);
 break;
 }}
 }}
now = rtc.now();
 for(i=0;i<a;i++){
 if (now.minute()==(m[i]+2) || (now.minute()==((m[i]+2)%60))) {
  
lcd.setCursor(0,0);
 lcd.print(">MEDCINES ARE    ");
 lcd.setCursor(0,1);
lcd.print("CLEARED ");
digitalWrite(1, LOW);
 digitalWrite(0, HIGH); 
 Serial.print("\n>>>>>>>>MEDCINES ARE NOT TAKEN\nMEDCINES ARE CLEARED :");
 dat();
  Serial.print("\nservo3");
 servo3.write(90);
 delay(3000);
 servo3.write(0);
delay(60000);
digitalWrite(1, LOW);
 digitalWrite(1, LOW);
break;
}
}}}
