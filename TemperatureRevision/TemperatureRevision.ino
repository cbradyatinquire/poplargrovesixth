#include <OneWire.h>

int DS18S20_Pin = 2; //DS18S20 Signal pin on digital 2

//CHANGE THESE VALUES TO SET THE RANGE THAT YOU WANT THE TEMP TO BE IN
float TOOHIGHTEMP = 28.0;
float TOOLOWTEMP = 20.0;

//Temperature chip i/o
OneWire ds(DS18S20_Pin);  // on digital pin 2

void setup(void) {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(10, OUTPUT);
  Serial.begin(9600);
}

void loop(void) {
  float temperature = getTemp();
  Serial.println(temperature);

   if (temperature > TOOHIGHTEMP ) { 
      digitalWrite(10, HIGH);
      Serial.println("too hot ");
    } else {
      if (temperature > TOOLOWTEMP) {
        analogWrite(10, 100 * (temperature - TOOLOWTEMP) / (TOOHIGHTEMP - TOOLOWTEMP) );  //brighter and brighter as voltage gets lower and lower
        for (int i = 1; i < (temperature - TOOLOWTEMP + .01); i++ ) {
          digitalWrite(LED_BUILTIN, HIGH);
          delay(150);
          digitalWrite(LED_BUILTIN, LOW);
          delay(150);
        }
      } else {
        digitalWrite(10, LOW);
        digitalWrite(LED_BUILTIN, HIGH);
        Serial.println("too cool ");
      }
    }
    delay(500);
}


float getTemp(){
  //returns the temperature from one DS18S20 in DEG Celsius

  byte data[12];
  byte addr[8];

  if ( !ds.search(addr)) {
      //no more sensors on chain, reset search
      ds.reset_search();
      return -1000;
  }

  if ( OneWire::crc8( addr, 7) != addr[7]) {
      Serial.println("CRC is not valid!");
      return -1000;
  }

  if ( addr[0] != 0x10 && addr[0] != 0x28) {
      Serial.print("Device is not recognized");
      return -1000;
  }

  ds.reset();
  ds.select(addr);
  ds.write(0x44,1); // start conversion, with parasite power on at the end

  byte present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE); // Read Scratchpad

  
  for (int i = 0; i < 9; i++) { // we need 9 bytes
    data[i] = ds.read();
  }
  
  ds.reset_search();
  
  byte MSB = data[1];
  byte LSB = data[0];

  float tempRead = ((MSB << 8) | LSB); //using two's compliment
  float TemperatureSum = tempRead / 16;
  
  return TemperatureSum;
  
}
