/*
  Author:       James Jones
  Date:         02/07/2024
  Description:  This is an Arduino program that works like a laser tag reciever. 
                When it recieves a valid IR code it will beep and increment the number of shots.
                The result is sent to the serial monitor and displayed on an LCD
*/

#include <IRremote.h>
#include <LiquidCrystal.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

enum mode {SHOT, ACTIVE, LOBBY};

  LiquidCrystal lcd(4, 6, 10, 11, 12, 13);

  int SHOT_COOLDOWN = 5000;

int TRIGGER = 0;
int IR_SENDER = 1;
int buzzer = 2;
int IRPIN = 3;
const short COLOR1 = 0x00000000;
const short COLOR2 = 0x00000000;


mode GAMEMODE = ACTIVE;
const short GAMEPACK_ID = 0x0102;
const short SHOT_PAYLOAD = 0x34;
int shots = 0;
int shotCooldownAcc = 0;
int numSecondsTimer = 600;
int numMS = 1000;
int numShots = 0;
int score = 0;

const char* ssid = "SSID";
const char* password = "PASSWORD";

WiFiUDP Udp;
unsigned int localUdpPort = 1337;  // local port of board
unsigned int remoteUdpPort = 3301; //port of base station
char incomingPacket[255];  // buffer for incoming packets
bool initalized = false;
String hostName = "VEST1";


bool buttonDebounced = true;
int debounceCounter = 0;
String packetString[4];

void setMode(mode newMode){
  GAMEMODE = newMode;
}

String prefix = "Shots: ";

void setup() {
   Serial.begin(9600);
   while(!Serial){}
  Serial.println();

  Serial.printf("Connecting to %s ", ssid);
    WiFi.hostname(hostName.c_str());
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" connected");

  Udp.begin(localUdpPort);
  Serial.printf("Now listening at IP %s, UDP port %d, remote IP: %s\n", WiFi.localIP().toString().c_str(), localUdpPort, Udp.remoteIP().toString().c_str());
  lcd.begin(16,2);
  lcd.clear();
  delay(50);
  String lcdBuffer = prefix + shots;
  lcd.setCursor(0, 0);
  lcd.print(lcdBuffer);

pinMode(buzzer, OUTPUT);
pinMode(TRIGGER, INPUT);

Serial.println("Enabling IRin");
IrSender.begin(IR_SENDER);
IrReceiver.begin(IRPIN, ENABLE_LED_FEEDBACK);

}

bool recievedPacket(){
  int packetSize = Udp.parsePacket();
  if (packetSize)
  {
    // receive incoming UDP packets
    Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
    int len = Udp.read(incomingPacket, 255);
    if (len > 0)
    {
      incomingPacket[len] = 0;
    }
    
    Serial.printf("UDP packet contents: %s\n", incomingPacket);
    int j = 0;
    for(int i = 0; i <4; i++){
      packetString[i] = "";
      }
    for(int i = 0; i < len; i++){
      if(incomingPacket[i] == '|'){
        j++;
        }
        else{
          packetString[j] = packetString[j] + incomingPacket[i];
          }
     }
    return true;
  }
  return false;
}
void sendPacket(char packetData[]){
    Udp.beginPacket(Udp.remoteIP(), remoteUdpPort);
    Udp.write(packetData);
    Udp.endPacket();
    Serial.printf("Sent to %s, port %d\n", Udp.remoteIP().toString().c_str(), Udp.remotePort());
}
void broadcastPacket(char packetData[]){
  IPAddress broadcastIP(255, 255, 255, 255);
    Udp.beginPacket(broadcastIP, remoteUdpPort);
    Udp.write(packetData);
    Udp.endPacket();
    Serial.printf("Sent to %s, port %d\n", Udp.remoteIP().toString().c_str(), Udp.remotePort());
}
bool submitAndverifyHit(short shotData){
  char b[2];
  b[0] = shotData & 0xff;
b[1] = (shotData >> 8) & 0xff;
  char pDat[9] = {'S', 'H','O','T','B','Y','|', b[0], b[1]};
  sendPacket(pDat);
  for(int i = 0; i < 1000; i++){
    if(recievedPacket()){
      if(packetString[0] == "SHOTCONFIRMATION"){
        if(packetString[1] == "TRUE"){
            return true;
          }
          else{
            return false;
            }
        }
      }
    }
    return false;
}
void loop() {
if(recievedPacket()){
  if(packetString[0] == "SETGAMEMODE"){
    if(packetString[1] == "SHOT") setMode(SHOT);
    if(packetString[1] == "LOBBY") setMode(LOBBY);
    if(packetString[1] == "ACTIVE") setMode(ACTIVE);
  }
  else if(packetString[0] == "SETTIMER"){
    numSecondsTimer = packetString[1].toInt();
    numMS = packetString[2].toInt();
  }
  else if(packetString[0] == "SETSCORE"){
    score = packetString[1].toInt();
  }
  else if(packetString[0] == "SYNC"){
    //String[] = new String{"INFO", numSecondsTimer.toString(), score.toString(), numShots.toString()};
  }
}

if (IrReceiver.decode())

{
Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX);
if(IrReceiver.decodedIRData.decodedRawData != 0x00 && GAMEMODE ==  ACTIVE && submitAndverifyHit(IrReceiver.decodedIRData.decodedRawData)){
  lcd.clear();
  //lcd.clear();
  lcd.print("");
  delay(50);
  shots++;
  String lcdBuffer = prefix + shots;
Serial.println(lcdBuffer);
lcd.print(lcdBuffer);
  lcd.clear();
  delay(50);
lcd.print(lcdBuffer);

for(int i = 0; i <3; i++){
  tone(buzzer, 1000);
  delay(250);
  noTone(buzzer);
  delay(250);
  setMode(SHOT);

}
}
IrReceiver.resume();

}
  else if(digitalRead(TRIGGER) && buttonDebounced && GAMEMODE == ACTIVE){
    IrSender.sendNEC(GAMEPACK_ID, SHOT_PAYLOAD, 0);
    buttonDebounced = false;
    debounceCounter = 0;
  }
  else{
    if(!buttonDebounced){
      if(debounceCounter > 49){
        buttonDebounced = true;
      }
      else{
        debounceCounter++;
      }
    }
    if(GAMEMODE == SHOT){
      if(shotCooldownAcc >= SHOT_COOLDOWN){
        GAMEMODE = ACTIVE;
      }
      else{
        shotCooldownAcc++;
      }
    }
  }

delay(1);

}
