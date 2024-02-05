/*
  Author:       James Jones
  Date:         01/27/2024
  Description:  This is an Arduino program that works like a laser tag reciever. 
                When it recieves a valid IR code it will beep and increment the number of shots.
                The result is sent to the serial monitor and displayed on an LCD
*/

#include <IRremote.h>
#include <LiquidCrystal.h>

enum mode {SHOT, ACTIVE, LOBBY};

  LiquidCrystal lcd(4, 6, 10, 11, 12, 13);

  int SHOT_COOLDOWN = 5000;

int TRIGGER = 0;
int IR_SENDER = 1;
int buzzer = 2;
int IRPIN = 3;
const COLOR1 = 0x000000;
const COLOR2 = 0X000000;


mode GAMEMODE = ACTIVE;
const GAMEPACK_ID = 0x0102;
const SHOT_PAYLOAD = 0x34;
int shots = 0;
int shotCooldownAcc = 0;
int numSecondsTimer = 600;
int numMS = 1000;
int numShots = 0;
int score = 0;


bool buttonDebounced = true;
int debounceCounter = 0;
String[] *packetString;

void setMode(mode newMode){
  GAMEMODE = newMode;
}

String prefix = "Shots: ";

void setup() {
  lcd.begin(16,2);
  lcd.clear();
  delay(50);
  String lcdBuffer = prefix + shots;
  lcd.setCursor(0, 0);
  lcd.print(lcdBuffer);

pinMode(buzzer, OUTPUT);
pinMode(TRIGGER, INPUT);

Serial.begin(9600);
while(!Serial){}
Serial.println("Enabling IRin");
IrSender.begin(IR_SENDER);
IrReceiver.begin(IRPIN, ENABLE_LED_FEEDBACK);

}

void loop() {
if(recievedPacket(&packetString)){
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
    String[] = new String{"INFO", numSecondsTimer.toString(), score.toString(), numShots.toString()};
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
