  /*
  AS608-Optical-Fingerprint-Sensor-enroll
    Modified on 06 Jan 2021
  by Amir Mohammad Shojaee @ Electropeak
  Home
    based on Adafruit Library

*/

#include <Adafruit_Fingerprint.h>
#include <Servo.h>
#define COMMON_ANODE

Servo servo1;
int servoPin = 8;
int bluePin = 9;
int greenPin = 10;
int redPin = 11;
volatile int finger_status = -1;

SoftwareSerial mySerial(2, 3); // TX/RX on fingerprint sensor

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

void setup()  
{
  servo1.attach(servoPin); 
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  Serial.begin(9600);
  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(100);
  Serial.println("\n\nAdafruit finger detect test");

  // set the data rate for the sensor serial port
  finger.begin(57600);
  
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) { delay(1); }
  }

  finger.getTemplateCount();
  Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");
  Serial.println("Waiting for valid finger...");
}

void setRGB(int red, int green, int blue)
 {
    analogWrite(redPin, red);
  	analogWrite(greenPin, green);
  	analogWrite(bluePin, blue);
}

void rest_state(){
  servo1.write(0);
  setRGB(0,255,0);//blue
}
void wait_state(){
  setRGB(255,0,0);//green
  delay(5000);
  setRGB(0,255,0);//blue
  servo1.write(0);
}
void open_state(){
  setRGB(255,0,0);//green
  servo1.write(0);
  delay(1000);
  servo1.write(90);
}

void loop()                     // run over and over again
{
  rest_state();
  finger_status = getFingerprintIDez();
  if (finger_status!=-1 and finger_status!=-2){
    Serial.print("Match");
    open_state();
    wait_state();
  } else{
    if (finger_status==-2){
      for (int ii=0;ii<5;ii++){
        Serial.print("Not Match");
      }
      setRGB(0,0,255);//red
      delay(1000);
    }
  }
  delay(50);            //don't ned to run this at full speed.
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      
  uint8_t p = finger.getImage();
  if (p!=2){
    Serial.println(p);
  }
  if (p != FINGERPRINT_OK)  return -1;
  
  p = finger.image2Tz();
  if (p!=2){
    Serial.println(p);
  }
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -2;
  
  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  return finger.fingerID; 
}
