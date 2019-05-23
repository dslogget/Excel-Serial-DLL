void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available() > 0){
    String incoming = Serial.readString();
    if(incoming == "Time"){
      Serial.print(millis());
    }else{ 
      Serial.print("Echo: ");
      Serial.println(incoming);
    }
  }
  delay(5);

}
