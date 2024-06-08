#include <Servo.h>

static const int NECK_LEFT_PIN = 25;
static const int NECK_RIGHT_PIN = 26;
static const int NECK_PAN_PIN= 14;
static const int MOUTH_PIN = 27;


Servo neck_left;
Servo neck_right;
Servo neck_pan;
Servo mouth;

void setup() {
    Serial.begin(115200);
    neck_left.attach(NECK_LEFT_PIN);
    neck_right.attach(NECK_RIGHT_PIN);
    neck_pan.attach(NECK_PAN_PIN);
    mouth.attach(MOUTH_PIN);

    neck_left.write(90);
    neck_right.write(90);
    neck_pan.write(90);
    mouth.write(90);
    
}

void loop() {
    
    for(int posDegrees = 30; posDegrees <= 150; posDegrees++) {
        neck_pan.write(posDegrees);
        neck_left.write(posDegrees);
        neck_right.write(posDegrees);
        // neck_pan.write(90);
        // mouth.write(90);
        Serial.println(posDegrees);
        delay(20);
    }

    for(int posDegrees = 150; posDegrees >= 30; posDegrees--) {
        neck_pan.write(posDegrees);
        neck_left.write(posDegrees);
        neck_right.write(posDegrees);
        // neck_pan.write(90);
        // mouth.write(90);
        delay(20);
    }

    for(int posDegrees = 70; posDegrees <= 180; posDegrees++) {
        
        mouth.write(posDegrees);
        // mouth.write(90);
        Serial.println(posDegrees);
        delay(20);
    }

    for(int posDegrees = 180; posDegrees >= 70; posDegrees--) {
        
        mouth.write(posDegrees);
        // mouth.write(90);
        delay(20);
    }
}
