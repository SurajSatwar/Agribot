#include <Servo.h>

// Servo motors
Servo seedServo, npkServos[3], dipServo;

// Pins for sensors and actuators
#define NPK_SENSOR_PIN A0
#define MOISTURE_SENSOR_PIN A1
#define PLOUGH_MOTOR_PIN 6
#define MOTOR_L1 9
#define MOTOR_L2 10
#define MOTOR_R1 11
#define MOTOR_R2 12
#define MAIN_MOTOR_PIN 13  // New pin for main motor control

void setup() {
  Serial.begin(9600); // Bluetooth Communication
  
  // Attach servo motors
  seedServo.attach(3);
  npkServos[0].attach(4);  // N
  npkServos[1].attach(5);  // P
  npkServos[2].attach(7);  // K
  dipServo.attach(8);      // Sensor dipping

  // Setup pins for DC motors
  pinMode(PLOUGH_MOTOR_PIN, OUTPUT);
  pinMode(MOTOR_L1, OUTPUT);
  pinMode(MOTOR_L2, OUTPUT);
  pinMode(MOTOR_R1, OUTPUT);
  pinMode(MOTOR_R2, OUTPUT);
  
  pinMode(MAIN_MOTOR_PIN, OUTPUT); // New motor control pin
}

void loop() {
  // Read sensor data
  int npkValue = analogRead(NPK_SENSOR_PIN);
  int moistureValue = analogRead(MOISTURE_SENSOR_PIN);

  // Send data to mobile app
  Serial.print("NPK: ");
  Serial.println(npkValue);
  Serial.print("Moisture: ");
  Serial.println(moistureValue);

  // Check for commands
  if (Serial.available()) {
    String command = Serial.readString();
    if (command == "SEED") dispenseSeed();
    else if (command == "N") dispenseNPK(0);
    else if (command == "P") dispenseNPK(1);
    else if (command == "K") dispenseNPK(2);
    else if (command == "DIP_DOWN") dipSensors(true);
    else if (command == "DIP_UP") dipSensors(false);
    else if (command == "PLOUGH_ON") digitalWrite(PLOUGH_MOTOR_PIN, HIGH);
    else if (command == "PLOUGH_OFF") digitalWrite(PLOUGH_MOTOR_PIN, LOW);
    else if (command == "FORWARD") moveForward();
    else if (command == "BACKWARD") moveBackward();
    else if (command == "LEFT") turnLeft();
    else if (command == "RIGHT") turnRight();
    else if (command == "STOP") stopMovement();
    else if (command == "MOTOR_ON") digitalWrite(MAIN_MOTOR_PIN, HIGH); // New command
    else if (command == "MOTOR_OFF") digitalWrite(MAIN_MOTOR_PIN, LOW);  // New command
  }

  delay(500);
}

void dispenseSeed() {
  seedServo.write(90); delay(1000); seedServo.write(0);
}

void dispenseNPK(int container) {
  npkServos[container].write(90); delay(1000); npkServos[container].write(0);
}

void dipSensors(bool down) {
  if (down) dipServo.write(90);
  else dipServo.write(0);
}

void moveForward() {
  digitalWrite(MOTOR_L1, HIGH); digitalWrite(MOTOR_L2, LOW);
  digitalWrite(MOTOR_R1, HIGH); digitalWrite(MOTOR_R2, LOW);
}

void moveBackward() {
  digitalWrite(MOTOR_L1, LOW); digitalWrite(MOTOR_L2, HIGH);
  digitalWrite(MOTOR_R1, LOW); digitalWrite(MOTOR_R2, HIGH);
}

void turnLeft() {
  digitalWrite(MOTOR_L1, LOW); digitalWrite(MOTOR_L2, HIGH);
  digitalWrite(MOTOR_R1, HIGH); digitalWrite(MOTOR_R2, LOW);
}

void turnRight() {
  digitalWrite(MOTOR_L1, HIGH); digitalWrite(MOTOR_L2, LOW);
  digitalWrite(MOTOR_R1, LOW); digitalWrite(MOTOR_R2, HIGH);
}

void stopMovement() {
  digitalWrite(MOTOR_L1, LOW); digitalWrite(MOTOR_L2, LOW);
  digitalWrite(MOTOR_R1, LOW); digitalWrite(MOTOR_R2, LOW);
}
