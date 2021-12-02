#include <SPI.h> //Import SPI library
#include <RH_RF95.h> // RF95 dari RadioHead Library

#define RFM95_CS 10 // CS jika LoRa terkoneksi ke pin 10
#define RFM95_RST 9 //RST pada LoRa terkoneksi ke pin 9
#define RFM95_INT 2 // INT pada LoRa terkoneksi pada pin 2

// Ubah ke 434.0 atau frekuensi lainnya, harus sesuai dengan frekuensi RX!
#define RF95_FREQ 434.0

// Contoh tunggal dari driver radio
RH_RF95 rf95(RFM95_CS, RFM95_INT);

void setup() 
{
 
//inisialisasi Serial Monitor
  Serial.begin(9600);
  
// Reset LoRa Modul 
  pinMode(RFM95_RST, OUTPUT); 
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

//Inisialisasi LoRa Module
  while (!rf95.init()) {
    Serial.println("LoRa radio init failed");
    while (1);
  }
  

 //Set default frekuensi 434.0MHz
  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }

  rf95.setTxPower(18); //power transimiter pada Lora Module
}

char value = 48;

void loop()
/*{

  Serial.print("Send: ");
  char radiopacket[1] = {char(value)};
  rf95.send((uint8_t *)radiopacket, 1);

    
  delay(1000);
  value++;
  if (value > '9')
  value = 48;
}*/
{
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
