#include <Arduino.h>
#include <WiFiNINA.h>
#include <PubSubClient.h>
#include <time.h>
#include <Wire.h>
#include <DHT.h>
#include <rgb_lcd.h>
#define DHTPIN 4
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);


#define max_data 50
#define ACK_timeout 1000
#define retry 3
bool ackReceived = false;
rgb_lcd lcd;



 const char* ssid ="Asus-wifi";
 const char*password = "00000000";
 String clientId;
 String payload;

 const char* mqtt_server = "broker.emqx.io";
 const int mqtt_port = 1883;
 const char* topic_pub = "mkrtohost1";///change this 
 const char* topic_sub = "host2mkr";/// change this 

 WiFiClient wifiClient;
 PubSubClient client(wifiClient);
 
 const int trig = A1;// change all the parameter 
 const int eco = 3;
 int data;
 float filteredValue = 0;
 float alpha = 0.5;

struct Message {
  int id;
  float filteredValue;
  float Temperature;
  float relative_Humidity;
  float dew_point;
  bool waitForACK;

};

Message Buffer[max_data]; 
int Bufferstart =0;
int bufferend = 0;
int id = 1;

void Message_sent(float filteredValue, float Temperature, float relative_Humidity, float dew_point){
    if (Buffer[bufferend].id == 0){
        Buffer[bufferend].id = id++;

    }
  
  

  Buffer[bufferend].filteredValue = filteredValue;
  Buffer[bufferend].Temperature = Temperature;
  Buffer[bufferend].relative_Humidity = relative_Humidity;
  Buffer[bufferend].dew_point = dew_point;

  Buffer[bufferend].waitForACK = true;
  bufferend = (bufferend +1) % max_data;

}

void datafrompietomqtt(char*topic, byte*payload, unsigned int length ){
    String msg= " ";
    for (int i=0; i<length; i++)
    msg+= (char)payload[i];

    msg.trim();
    Serial.println(msg);
    if(String(topic)=="host2mkr"){
        if (msg == "ACK"){
            ackReceived = true;

        }



    }
    }
bool waitforACK(){
        ackReceived =false;

        unsigned long starttime = millis();
        while (millis()-starttime < ACK_timeout){
            client.loop();

            if (ackReceived){
                return true;


            }




        }
        return false;


    }
void Bufferedmsgtobesend() {
  int try_again= 0;
  
  while(Bufferstart != bufferend){
    payload = "{";
    payload += "\"Sensor_ID\" : \"Proximity_Sensor\",";
    payload += " \"distance\":" + String(Buffer[Bufferstart].filteredValue)+ ",";
    payload += "\"Sensor_ID\" : \"DHT11\",";
    payload += " \"Temperature\":" + String(Buffer[Bufferstart].Temperature)+",";
    payload += " \"Humidity\":" + String(Buffer[Bufferstart].relative_Humidity)+",";
    payload += " \"dew_point\":" + String(Buffer[Bufferstart].dew_point);
    
    payload  += "}";
    client.publish("mkrtohost1", payload.c_str());

    if (waitforACK()){
      Serial.println("The ACK has been received");
      Bufferstart = (Bufferstart +1)% max_data;
      try_again =0;}
    else {
      try_again++;
      if (try_again >= retry){
      Serial.println("The ACK has not been received" + String(retry));
      Serial.println("The id of the message tried to reach gatewas is:" + String(Buffer[Bufferstart].id));

      Bufferstart = (Bufferstart +1)% max_data;
      try_again = 0;}
    
     }
   }
 }

void connectWiFi(){  /*  This function is used to connect wifi with MKR1010*/
 
 Serial.println("Connecting to wifi..");
 WiFi.begin(ssid,password);

 while (WiFi.status() != WL_CONNECTED){
  delay(500);
 Serial.print(".");}
 
  Serial.println("Wifi is connected");
  Serial.println("IP address is :");
  Serial.println(WiFi.localIP());

 }

 void connectMQTT(){

  client.setServer(mqtt_server, mqtt_port); /* This is used to connect with mqtt*/
  Serial.print("Connecting to mqtt.......");

  while(!client.connected()){
  String clientId = "MKR_PRXI_1010";
  if (client.connect(clientId.c_str())){
    Serial.print("Connected");}else{
      Serial.print(client.state());
      delay(2000);



    }



  }

  
  
  }

void setup() {
 
 Serial.begin(9600);
 pinMode(eco, INPUT); 
 pinMode(trig, OUTPUT);
 WiFi.begin(ssid,password);
 connectWiFi();
 connectMQTT();
 dht.begin();
 lcd.begin(16,2);
 lcd.setRGB(255,255,0);
 delay(1500);
 lcd.clear();

}

void loop() {
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  delayMicroseconds(10);
  data = digitalRead(eco);
  
  Serial.print("\n");

  long duration = pulseIn(eco,HIGH);

  
  float distance = (duration * 0.034/2);



 filteredValue = alpha * distance + (1-alpha)*filteredValue;

 Serial.print("Raw distance:  ");
 Serial.print(distance);    
  
  Serial.print(",");    
  Serial.print("Filtered Value :  ");        
  Serial.println(filteredValue);

  float Temperature = dht.readTemperature();
  float relative_Humidity = dht.readHumidity();

  float dew_point = Temperature -((100-relative_Humidity)/5);

  Serial.print(Temperature);
  Serial.print("");
  Serial.print ("|");
  Serial.print(relative_Humidity);
  Serial.print("");
  Serial.print ("|");
  Serial.print (dew_point);
  Serial.println("");
  delay(2000);

lcd.clear();
lcd.setCursor(0, 0);
lcd.print("Dist:");
lcd.print(filteredValue);

lcd.setCursor(0, 1);
lcd.print("T:");
lcd.print(Temperature);
lcd.print(" H:");
lcd.print(relative_Humidity);


// -------------------------


  delay(1000);
 
  Message_sent(filteredValue,Temperature,relative_Humidity, dew_point);
  Bufferedmsgtobesend();
  delay(1000);
      

}  
