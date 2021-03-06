#include <LoRa.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include "I2Cdev.h"
#include "MPU6050.h"
#include "Wire.h"
static const int RXPin = 11, TXPin = 12;
static const uint32_t GPSBaud = 9600;

TinyGPSPlus gps;
SoftwareSerial ss(RXPin, TXPin);

// class default I2C address is 0x68
// specific I2C addresses may be passed as a parameter here
// AD0 low = 0x68 (default for InvenSense evaluation board)
// AD0 high = 0x69
MPU6050 accelgyro;
//MPU6050 accelgyro(0x69); // <-- use for AD0 high

int16_t ax, ay, az;
int16_t gx, gy, gz;

// fixed parameters
#define FREQUENCY         915E6   // 915MHz
#define BANDWIDTH         125000  // 125kHz bandwidth

// vary these parameters
#define TX_POWER          6   // valid values are from 6 to 20
#define SPREADING_FACTOR  10    // valid values are 7, 8, 9 or 10
#define SYNC_WORD         115
#define SAMPLE_RATE       100
int count = 0;
double vx, vy, vz = 0;
double real_ax, real_ay = 0;
double dist_travelled = 0;
double calories = 0;
double weight = 20; // in pounds
unsigned long last_send = 0;
unsigned long last_send_2 = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  while (!Serial);
  Wire.begin();

  // The accelerometer
  accelgyro.initialize();

  // The GPS
  ss.begin(GPSBaud);

  // The LoRa Module
  LoRa.setPins(6, 9, 19);
  if (!LoRa.begin(FREQUENCY)){
        Serial.println("Starting LoRa failed!");
  }
  else Serial.println("worked");
  last_send = millis();
        
}


void loop() {
  // put your main code here, to run repeatedly:
  unsigned long curr_time = millis();
  if (curr_time - last_send > 1000 * 5) {
    last_send = curr_time;
    LoRa.beginPacket();
    LoRa.print("Distance Travlleled in One Hour: ");
    LoRa.println(dist_travelled);
    Serial.println("Distance");
    Serial.println(dist_travelled);
    LoRa.print("Calories Burned: ");
    LoRa.println(calories);
    Serial.println("Calories");
    Serial.println(calories);
    LoRa.endPacket();
    dist_travelled = 0;
  }
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  real_ax = (double(ax) / SAMPLE_RATE) / 8192;
  real_ay = (double(ay) / SAMPLE_RATE) / 8192;
  
  if (sqrt(real_ax * real_ax + real_ay * real_ay) > 0.005) {
    count = 0;
  } else {
    count += 1;
  }
  vx += (double(ax) / SAMPLE_RATE) / 8192;
  vy += (double(ay) / SAMPLE_RATE) / 8192;
  if (count > 100) {
    vx = 0;
    vy = 0;
  }


  dist_travelled += sqrt(vx * vx + vy * vy) / SAMPLE_RATE;
  
  calories = weight * dist_travelled * 0.000621371 * 0.75;

  //Serial.println(ax);

  
  float latitude = 37.410403;
  float longitude = -122.059960;
  float speed = 0.0 / 0.0;
  while (ss.available()){
    gps.encode(ss.read());
  }
  if (gps.location.isValid()){
    Serial.println(curr_time);
    latitude = gps.location.lat();
    longitude = gps.location.lng();
  }
  
  if (curr_time - last_send_2 > 1000 * 5){
    last_send_2 = curr_time;
    LoRa.beginPacket();
  
    LoRa.print("Latitude: ");
    LoRa.println(latitude);
    LoRa.print("Longitude: ");
    LoRa.println(longitude);

    LoRa.endPacket();

    Serial.print("Latitude: ");
    Serial.println(latitude);
    Serial.print("Longitude: ");
    Serial.println(longitude);
    
    LoRa.receive();
    delay(150);
    char ans[4] = {0};
    int packetSize = LoRa.parsePacket();
    if (packetSize){
    for (int i = 0; i < 3; i++) {
        ans[i] = (char)LoRa.read();  
    }

    if (strcmp(ans, "ACK") == 0){
      Serial.println("ACK");
    }
      LoRa.idle();
    }
    
  }
  


}
