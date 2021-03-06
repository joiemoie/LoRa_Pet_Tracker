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
int16_t vx, vy, vz = 0;
int16_t dist_travelled = 0;
double calories = 0;
double weight = 20; // in pounds
unsigned long last_send = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  while (!Serial);
  Wire.begin();

  // The accelerometer
  accelgyro.initialize();

  // The GPS
  //ss.begin(GPSBaud);

  // The LoRa Module
  LoRa.setPins(6, 9, 19);
  if (!LoRa.begin(FREQUENCY)){
        Serial.println("Starting LoRa failed!");
  }
  else Serial.println("worked");
  //LoRa.setSyncWord(115);
  last_send = millis();
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned long curr_time = millis();
  
//  if (curr_time - last_send > 1000 * 10) {
//    last_send = curr_time;
//    LoRa.beginPacket();
//
//    LoRa.print("Distance Travlleled in One Hour: ");
//    LoRa.println(dist_travelled);
//    Serial.println("Distance");
//    Serial.println(dist_travelled);
//    LoRa.print("Calories Burned: ");
//    LoRa.println(calories);
//    Serial.println("Calories");
//    Serial.println(calories);
//    LoRa.endPacket();
//    dist_travelled = 0;
//  }

  if (calib == true)
  stDevX = calibrate();
  float Xmax = ax + stDevX;
  float Xmin = ax - stDevX;
  float Ymax = ay + stDevY;
  float Ymin = ay - stDevY;
  float Zmax = ay + stDevZ;
  float Zmin = ay - stDevZ;
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  //whatever function you want to do with the accelerometer
  vx += ax / SAMPLE_RATE;
  vy += ay / SAMPLE_RATE;
  vz += az / SAMPLE_RATE;
  Serial.println("ax, ay, gz, dist_travelled");
  Serial.println(ax);
  Serial.println(ay);
  Serial.println(az);
  dist_travelled += sqrt(vx * vx + vy * vy + vz * vz) / SAMPLE_RATE;
  Serial.println(dist_travelled);
  calories = weight * dist_travelled * 0.000621371 * 0.75;

  //Serial.println(ax);
  /*
  float latitude = 0.0 / 0.0;
  float longitude = 0.0 / 0.0;
  float speed = 0.0 / 0.0;
  while (ss.available())
    gps.encode(ss.read());
  if (gps.location.isValid()){
    latitude = gps.location.lat();
    longitude = gps.location.lng();
  }
  if (gps.speed.isValid()){
    speed = gps.speed.kmph();
  }
  */
//  LoRa.beginPacket();
//
//  LoRa.print("Pet Tracker");
//
//  LoRa.endPacket();
//
//  if (curr_time - last_send > 10000){
//    Serial.println("Pet Tracker ");
//  }
//  last_send = curr_time;



}

float calibrate()
{
  calib = true
  int sampX[numSamples],int sampY[numSamples],int sampZ[numSamples];
  int sampXsum = int sampYsum = int sampZsum = 0;
  float sampXdevSum = float sampYdevSum = float sampZdevSum = 0
  numSamples = 50;
  for ( int i = 0; i<numSamples; i++)
    {
      accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
//      vx += ax / SAMPLE_RATE;
//      vy += ay / SAMPLE_RATE;
//      vz += az / SAMPLE_RATE;
      sampX[i] = ax;
      sampXsum += ax;
      sampY[i] = ay;
      sampYsum += ay;
      sampZ[i] = az;
      sampZsum += az;
    }
     float meanSampX = sampXsum/float(numSamples);
     float meanSampY = sampYsum/float(numSamples);
     float meanSampZ = sampZsum/float(numSamples);

   for(int i=0; i<numSamples; i++)
   {
    sampXdevSum += pow((meanSampX - float(sampX[i])), 2);
    sampYdevSum += pow((meanSampY - float(sampY[i])), 2);
    sampZdevSum += pow((meanSampZ - float(sampZ[i])), 2);
    }

   float stDevX = sqrt(sampXdevSum/float(numSamples));
   float stDevY = sqrt(sampYdevSum/float(numSamples));
   float stDevZ = sqrt(sampZdevSum/float(numSamples));
  
   return stDevX, stDevY, stDevZ;

}

