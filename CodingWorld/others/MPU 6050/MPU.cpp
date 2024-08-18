#include <Wire.h>

float RateRoll, RatePitch, RateYaw;
float AccX, AccY, AccZ;
float AngleRoll, AnglePitch, AngleYaw;
float previousTime = 0;
float currentTime = 0;
float deltaTime = 0;
float yawAngle = 0; // Initialize yawAngle

void gyro_signals(void) {
    Wire.beginTransmission(0x68);
    Wire.write(0x1A);
    Wire.write(0x05);
    Wire.endTransmission();
    
    Wire.beginTransmission(0x68);
    Wire.write(0x1C);
    Wire.write(0x10);
    Wire.endTransmission();
    
    Wire.beginTransmission(0x68);
    Wire.write(0x3B);
    Wire.endTransmission();
    
    Wire.requestFrom(0x68, 6);
    
    int16_t AccXLSB = Wire.read() << 8 | Wire.read();
    int16_t AccYLSB = Wire.read() << 8 | Wire.read();
    int16_t AccZLSB = Wire.read() << 8 | Wire.read();
    
    Wire.beginTransmission(0x68);
    Wire.write(0x1B);
    Wire.write(0x08);
    Wire.endTransmission();
    
    Wire.beginTransmission(0x68);
    Wire.write(0x43);
    Wire.endTransmission();
    
    Wire.requestFrom(0x68, 6);
    
    int16_t GyroX = Wire.read() << 8 | Wire.read();
    int16_t GyroY = Wire.read() << 8 | Wire.read();
    int16_t GyroZ = Wire.read() << 8 | Wire.read();
    
    RateRoll = (float)GyroX / 65.5;
    RatePitch = (float)GyroY / 65.5;
    RateYaw = (float)GyroZ / 65.5;
    
    
    AccX = (float)AccXLSB / 4096;
    AccY = (float)AccYLSB / 4096;
    AccZ = (float)AccZLSB / 4096;
    



    AngleRoll = atan(AccY / sqrt(AccX * AccX + AccZ * AccZ)) * (180 / 3.142);
    AnglePitch = -atan(AccX / sqrt(AccY * AccY + AccZ * AccZ)) * (180 / 3.142);
    
    // Update yaw angle using integration of RateYaw
    currentTime = millis() / 1000.0; // Convert milliseconds to seconds
    deltaTime = currentTime - previousTime;
    previousTime = currentTime;
    
    yawAngle += RateYaw * deltaTime;
    AngleYaw = yawAngle; // Set the yaw angle to AngleYaw
}

void setup() {
    Serial.begin(57600);
    pinMode(13, OUTPUT);
    digitalWrite(13, HIGH);
    Wire.setClock(400000);
    Wire.begin();
    delay(250);
    
    Wire.beginTransmission(0x68);
    Wire.write(0x6B);
    Wire.write(0x00);
    Wire.endTransmission();
}

void loop() {
    gyro_signals();
    
 
    Serial.print(" AngleRoll [degrees]= ");
    Serial.print(RateRoll);
    Serial.print(" AnglePitch [degrees]= ");
    Serial.print(RatePitch);
    Serial.print(" AngleYaw [degrees]= ");
    Serial.println(RateYaw);
    
    delay(500);
}
