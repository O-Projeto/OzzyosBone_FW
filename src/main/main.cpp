#include <Servo.h>

Servo panServo;
Servo tiltServo;
Servo rollServo;
Servo mouthServo;



static const int NECK_LEFT_PIN = 25;
static const int NECK_RIGHT_PIN = 26;
static const int NECK_PAN_PIN = 14;
static const int MOUTH_PIN = 27;

static const int LEFT_LED_PIN = 32;
static const int RIGHT_LED_PIN = 33;


void setup() {
    Serial.begin(115200);
    Serial.setTimeout(10);

    panServo.attach(NECK_PAN_PIN);    // Assign the appropriate GPIO pin numbers for servos
    tiltServo.attach(NECK_RIGHT_PIN);   // Adjust these pin numbers according to your hardware setup.
    rollServo.attach(NECK_LEFT_PIN);   
    mouthServo.attach(MOUTH_PIN);  

    pinMode(LEFT_LED_PIN, OUTPUT);
    pinMode(RIGHT_LED_PIN, OUTPUT);

  
}

void loop() {
    if (Serial.available() > 0) {
        String data = Serial.readStringUntil('\n');
        data.trim();

        // Parse the received data
        // 000,000,000,090,1,1,255,255,255

        int panAngle = data.substring(0, 3).toInt();
        int tiltAngle = data.substring(4, 7).toInt();
        int rollAngle = data.substring(8, 11).toInt();
        int mouthAngle = data.substring(12, 15).toInt();
        int leftEye_on = data.substring(16, 17).toInt();
        int rightEyeColor_on = data.substring(18, 19).toInt();


        leftEye_on = (leftEye_on != 0) ? HIGH : LOW;
        rightEyeColor_on = (rightEyeColor_on != 0) ? HIGH : LOW;

        // Ensure angles are within valid range (0 to 180 degrees)
        panAngle = constrain(panAngle, 0, 180);
        tiltAngle = constrain(tiltAngle, 0, 180);
        rollAngle = constrain(rollAngle, 0, 180);
        mouthAngle = constrain(mouthAngle, 0, 180);

      
        // Control servos based on received angles
        panServo.write(panAngle);
        tiltServo.write(tiltAngle);
        rollServo.write(rollAngle);
        mouthServo.write(mouthAngle);

       digitalWrite(RIGHT_LED_PIN,rightEyeColor_on);
       digitalWrite(LEFT_LED_PIN,leftEye_on);

    }
}
