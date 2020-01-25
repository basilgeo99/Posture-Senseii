//Posture senseii by Basil, Nikhil, Karthik

#include <Wire.h>  // Wire library - used for I2C communication


#define CUSTOM_SETTINGS
#define INCLUDE_TERMINAL_MODULE
#define INCLUDE_NOTIFICATION_MODULE
#define INCLUDE_MUSIC_MODULE
#include <Dabble.h>


String Serialdata = "";
bool dataflag = 0;


int ADXL345 = 0x53; // The ADXL345 sensor I2C address
float X_out, Y_out, Z_out;  // Outputs
int alert = 13;
#include <Wire.h>  // Wire library - used for I2C communication




void setup() {
  Serial.begin(9600); // Initiate serial communication for printing the results on the Serial monitor
  Dabble.begin(9600);



  Wire.begin();                    // Initiate the Wire library
                                   // Set ADXL345 in measuring mode
  Wire.beginTransmission(ADXL345); // Start communicating with the device
  Wire.write(0x2D);                // Access talk to POWER_CTL Register - 0x2D
                                   // Enable measurement
  Wire.write(8);                   // (8dec -> 0000 1000 binary) Bit D3 High for measuring enable
  Wire.endTransmission();



  //Dabble.waitForAppConnection();               //waiting for App to connect
  Notification.clear();
  Notification.setTitle("Posture Alert");


  pinMode(alert, OUTPUT);
}




void loop() {
  Dabble.processInput();
  // === Read acceleromter data === //
  Wire.beginTransmission(ADXL345);
  Wire.write(0x32); // Start with register 0x32 (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(ADXL345, 6, true); // Read 6 registers total, each axis value is stored in 2 registers
  X_out = ( Wire.read() | Wire.read() << 8); // X-axis value
  X_out = X_out / 256; //For a range of +-2g, we need to divide the raw values by 256, according to the datasheet
  Y_out = ( Wire.read() | Wire.read() << 8); // Y-axis value
  Y_out = Y_out / 256;
  Z_out = ( Wire.read() | Wire.read() << 8); // Z-axis value
  Z_out = Z_out / 256;

  Serial.print("Xa= ");
  Serial.print(X_out);
  Serial.print("   Ya= ");
  Serial.print(Y_out);
  Serial.print("   Za= ");
  Serial.println(Z_out);

  Serialdata = ("X = " + String(X_out) + "\nY = " + String(Y_out) + "\nZ = " + String(Z_out));

  Terminal.print(Serialdata);

  if (X_out <= 0.85) {
    Notification.notifyPhone(String("Maintain Posture Please "));
    digitalWrite(alert, HIGH);
    //Music.play("Po");
  } else {
    Notification.clear();
    digitalWrite(alert, LOW);
  }
  delay(1000);
}
