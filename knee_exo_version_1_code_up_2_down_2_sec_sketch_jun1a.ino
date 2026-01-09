// Pin definitions for L298N
const int IN1 = 2;  // Motor A direction
const int IN2 = 3;  // Motor A direction
const int ENA = 4;  // Motor A enable
const int IN3 = 5;  // Motor B direction
const int IN4 = 6;  // Motor B direction
const int ENB = 7;  // Motor B enable

// Motor control functions
void moveUp() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(ENA, HIGH); // Enable Motor A
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  digitalWrite(ENB, HIGH); // Enable Motor B
}

void moveDown() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(ENA, HIGH); // Enable Motor A
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  digitalWrite(ENB, HIGH); // Enable Motor B
}

void stopMotors() {
  digitalWrite(ENA, LOW); // Disable Motor A
  digitalWrite(ENB, LOW); // Disable Motor B
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

// States for the sequence
enum State { IDLE, UP, DOWN, STOPPED };

// Variables
State currentState = IDLE;
unsigned long startTime = 0;
bool sequenceRunning = false;
int sequenceCount = 0;

void setup() {
  // Initialize pins
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);
  
  Serial.begin(9600);
  stopMotors();
  Serial.println("Ready. Press 's' to start one sequence, 'w' to start four sequences, 'e' to stop.");
}

void loop() {
  // Handle serial input
  if (Serial.available() > 0) {
    char command = Serial.read();
    if (command == 's') {
      if (!sequenceRunning) {
        sequenceCount = 1;
        startSequence();
      } else {
        stopMotors();
        sequenceRunning = false;
        sequenceCount = 0;
        Serial.println("Sequence stopped by user.");
      }
    } else if (command == 'w' && !sequenceRunning) {
      sequenceCount = 4;
      startSequence();
    } else if (command == 'e') {
      stopMotors();
      sequenceRunning = false;
      sequenceCount = 0;
      currentState = STOPPED;
      Serial.println("Motors stopped. Waiting for next command.");
    }
  }
  
  // State machine for sequence
  if (sequenceRunning) {
    switch (currentState) {
      case UP:
        if (millis() - startTime >= 2000) {  // 2 seconds
          currentState = DOWN;
          startTime = millis();
          moveDown();
          Serial.println("Moving down for 1.7 seconds.");
        }
        break;
      case DOWN:
        if (millis() - startTime >= 1700) {  // 1.7 seconds
          currentState = STOPPED;
          stopMotors();
          if (sequenceCount > 1) {
            sequenceCount--;
            currentState = UP;
            startTime = millis();
            moveUp();
            Serial.println("Starting next sequence: Moving up for 2 seconds.");
          } else {
            sequenceRunning = false;
            Serial.println("All sequences completed.");
          }
        }
        break;
      case STOPPED:
        // Do nothing
        break;
    }
  }
}

// Function to start the sequence
void startSequence() {
  sequenceRunning = true;
  currentState = UP;
  startTime = millis();
  moveUp();
  Serial.print("Starting ");
  Serial.print(sequenceCount);
  Serial.println(" sequence(s): Moving up for 2 seconds.");
}