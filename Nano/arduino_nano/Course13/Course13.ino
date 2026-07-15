#include "IRSendRev.h"

#define IR_PIN 2

void setup() 
{
  Serial.begin(9600);
  IR.Init(IR_PIN);
  Serial.println("init over");
}

void loop() 
{
  byte dta[10];
  if(IR.IsDta()) 
  {
    byte length= IR.Recv(dta);

    Serial.print("- press -\t");
    switch(dta[8]) {
    case 162: Serial.println("[CH-]");         break;
    case 98:  Serial.println("[CH]");          break;
    case 226: Serial.println("[CH+]");         break;
    case 34:  Serial.println("[PREV]");        break;
    case 2:   Serial.println("[NEXT]");        break;
    case 194: Serial.println("[PLAY/PAUSE]");  break;
    case 224: Serial.println("[VOL-]");        break;
    case 168: Serial.println("[VOL+]");        break;
    case 144: Serial.println("[EQ]");          break;
    case 104: Serial.println("[0]");           break;
    case 152: Serial.println("[100+]");        break;
    case 176: Serial.println("[200+]");        break;
    case 48:  Serial.println("[1]");           break;
    case 24:  Serial.println("[2]");           break;
    case 122: Serial.println("[3]");           break;
    case 16:  Serial.println("[4]");           break;
    case 56:  Serial.println("[5]");           break;
    case 90:  Serial.println("[6]");           break;
    case 66:  Serial.println("[7]");           break;
    case 74:  Serial.println("[8]");           break;
    case 82:  Serial.println("[9]");           break;
    }
  }
}
