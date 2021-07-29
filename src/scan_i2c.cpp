#include <Arduino.h>
#include <Wire.h>
#include "scan_i2c.h"

void scanI2C(){
  byte error, address;

  int nDevices=0;
  // For debugging it is useful to show what's on the i2c bus.
  // But even when seeing to many or no devices, it somehow still works sometimes.
  Serial.println("Scanning I2C bus...");
  for(address = 1; address < 127; address++ )
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address<16)
        Serial.print("0");
      Serial.print(address,HEX);
      Serial.println("  !");
      nDevices++;
    }
    else if (error==4)
    {
      Serial.print("Unknown error at address 0x");
      if (address<16)
        Serial.print("0");
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else{
    Serial.print("I2C Devices:");
    Serial.println(nDevices);
  }
} 
