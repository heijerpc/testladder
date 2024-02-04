#include "MCP23017.h"
#define MCP23017_I2C_ADDRESS 0x24 


 
int analogIn = A1;  // Analoge input van de potmeter   WEG
int outputvoltage = 0;   // waarde te meten van A0 noemen we potmeter WEG

#include "Wire.h"
void setup()
{
  Wire.begin();
  MCP23017init();
  Serial.begin(9600);              /// WEG
  setRelayChannel(1);
}

void loop()
{
testVolumerelays();
//binaryCount();
//testChannelrelays();
//testje();
}

void testVolumerelays()
{ 
  setRelayChannel(1);
  Serial.println();
  float voltage;
  uint8_t bitwaarde;
  for (byte relay=1; relay<7; relay++)
  {
    delay(1000);
    uint8_t tussenstap;
    tussenstap = relay;
    bitwaarde = (1 << (tussenstap - 1)); 
    //bitwaarde = 0xFF ^ (1 << (tussenstap-1));  
    Serial.print("bitwaarde ");
    Serial.print(bitwaarde, BIN);
    Serial.print(" testing relay : ");
    Serial.print(relay);
    for (byte a=0; a<3; a++)
    {
      setRelayVolume(bitwaarde);
      delay(2000);
      outputvoltage = analogRead(analogIn);    // lees de analoge waarde:
      voltage= outputvoltage * (5.0 / 1023.0);
      Serial.print("  V = ");  // print "potmeterwaarde = "
      Serial.print(voltage);           // print de potmeterwaarde
      setRelayVolume(0);
      delay(2000);
      outputvoltage = analogRead(analogIn);    // lees de analoge waarde:
      voltage = outputvoltage * (5.0 / 1023.0);
      Serial.print("  V = ");  // print "potmeterwaarde = "
      Serial.print(voltage);           // print de potmeterwaarde
    }
    Serial.println();
  }
}

void testChannelrelays()
{ 
  uint8_t inverseWord;
  for (uint8_t relay=0; relay<4; relay++) {
    Serial.print("  channel = ");
    Serial.print(relay);
    if (relay==0){
      inverseWord=0xff;
    }
    else{
       inverseWord= 0xFF ^ (0x01 << (relay-1)); 
    }
    Serial.print("  inverse word ");
    Serial.println(inverseWord, BIN);
    for (byte a=0; a<10; a++){
      setRelayChannel(relay);
      delay(1000);
      setRelayChannel(0);
    }
  }
}
void setRelayVolume(uint8_t Word)             // send data to the display
{
  Wire.beginTransmission(MCP23017_I2C_ADDRESS);
  Wire.write(0x13);
  Wire.write(Word);
  Wire.endTransmission();
}

void setRelayChannel(uint8_t relay)             // send data to the display
{
  uint8_t inverseWord;
  if (relay==0){
    inverseWord=0xff;
  }
  else{
    inverseWord= 0xFF ^ (0x01 << (relay-1)); 
  }
  Wire.beginTransmission(MCP23017_I2C_ADDRESS);
  Wire.write(0x12);
  Wire.write(inverseWord);
  Wire.endTransmission();
}

void MCP23017init()
//initialize the MCP23017 controling the relays
// set I/O pins to outputs
{
  Wire.beginTransmission(MCP23017_I2C_ADDRESS);
  Wire.write(0x00);                            // IODIRA register
  Wire.write(0x00);                            // set all of port A to outputs
  Wire.write(0x01);                            // IODIRB register
  Wire.write(0x00);                            // set all of port B to outputs
  Wire.write(0x12);                            // gpioA
  Wire.write(0xFF);                            // set all ports high, low is active
  Wire.write(0x13);                            // gpioB
  Wire.write(0x00);                            // set all ports low, high is active
  Wire.endTransmission();
}

void binaryCount()
// testing voltage
{
 for (byte a=0x00; a<64; a++)
  {
  setRelayVolume(a);
  delay(2000);
  outputvoltage = analogRead(analogIn);    // lees de analoge waarde:
  float voltage= outputvoltage * (5.0 / 1023.0);
  Serial.print(a, BIN);
  Serial.print(" stand relay = ");
  Serial.print(a);
  Serial.print("  Spanning output = ");   "
  Serial.println(voltage);           // 
  }
}
void testje()
// testing voltage
{
 for (byte a=0; a<64; a++)
  {
  setRelayVolume(0x01);
  delay(2000);
  setRelayVolume(0x00);
  delay(2000);
  }

}