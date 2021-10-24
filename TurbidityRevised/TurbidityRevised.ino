

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(10, OUTPUT);
  Serial.begin(9600); 
}

void loop() {
  int sensorValue = analogRead(A1);  // read the input on analog pin 1:
  float voltage = sensorValue * (5.0 / 1024.0); // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  Serial.println(voltage);  // print out the value you read:

  if (voltage > 4.5) { 
    digitalWrite(LED_BUILTIN, HIGH);
    digitalWrite(10, LOW);
    
    Serial.println("low turbidity");
  } 
  if (voltage < 4.5) { 
    if (voltage > 1) {
      digitalWrite(LED_BUILTIN, LOW);
      analogWrite(10, 225 * (1 - (voltage - 1) / 3.5) );  //brighter and brighter as voltage gets lower and lower
    } else
    {
      digitalWrite(LED_BUILTIN, LOW);
      digitalWrite(10, HIGH);
      Serial.println("very high turbidity");
    }
   }
  
  
  delay(100);
}
