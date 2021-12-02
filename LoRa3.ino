#include <SPI.h>
#include <RH_RF95.h>
#include <MQUnifiedsensor.h>
#include "DHT.h"

#define DHTPIN A0     // what pin we're connected to
#define DHTTYPE DHT22   // DHT type
#define RFM95_CS 10
#define RFM95_RST 9 //RST pada LoRa terkoneksi ke pin 9
#define RFM95_INT 2

//pengenalan pin Sensor MQ
#define Board ("Arduino UNO")
#define Pin (A4)

#define Type ("MQ-4")
#define Voltage_Resolution (5)
#define ADC_Bit_Resolution (10)
#define RatioMQ4CleanAir (4.4)

//deklarasi sensor
MQUnifiedsensor MQ4 (Board, Voltage_Resolution, ADC_Bit_Resolution, Pin, Type);

DHT dht(DHTPIN, DHTTYPE);
int hum;  //Stores humidity value
int temp; //Stores temperature value

RH_RF95 rf95;

void setup() 
{
  Serial.begin(9600);
  pinMode(RFM95_RST, OUTPUT); 
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  dht.begin();
  MQ4.init();
  Serial.print("Mengkalibrasi");
  float calcR0 = 0;
  for(int i = 1; i<=10; i++){
    MQ4.update();
    calcR0 += MQ4.calibrate(RatioMQ4CleanAir);
    Serial.print(".");
  }
  MQ4.setR0(calcR0/10);
  Serial.println("  Done!.");
   
  if(isinf(calcR0)) {Serial.println("Warning: Conection issue founded, R0 is infite (Open circuit detected) please check your wiring and supply"); while(1);}
  if(calcR0 == 0){Serial.println("Warning: Conection issue founded, R0 is zero (Analog pin with short circuit to ground) please check your wiring and supply"); while(1);}
  MQ4.serialDebug(true);
//  if (!rf95.init())
  //  Serial.println("init failed");
  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on


}



void loop()
{
  sukel();
  sigas();
}
void sukel(){
  temp = dht.readTemperature();
  hum = dht.readHumidity();
  String humidity = String(hum); //int to String
  String temperature = String(temp);
  String data = temperature + humidity;
  Serial.print(data);
  char d[5];
  data.toCharArray(d, 5); //String to char array
  Serial.println("Sending to rf95_server");
  rf95.send(d, sizeof(d));
  rf95.waitPacketSent();
  delay(400);
}
void sigas(){
  MQ4.update();
  MQ4.readSensor();
  MQ4.serialDebug();
  delay(500);
}
