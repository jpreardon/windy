
#include <AFMotor.h>

AF_Stepper stepper(513, 1);
AF_DCMotor motor(3, MOTOR12_64KHZ); 

const int CALIBRATION_PIN = 14;
const int CLOCKWISE_PIN = 15;
const int ANTICLOCKWISE_PIN = 16;
int currentPosition = 360;
int motorState = 0;

void setup() {
  
  pinMode(CALIBRATION_PIN, INPUT_PULLUP);
  pinMode(CLOCKWISE_PIN, INPUT_PULLUP);
  pinMode(ANTICLOCKWISE_PIN, INPUT_PULLUP);
  
  Serial.begin(9600);           // set up Serial library at 9600 bps
  
  stepper.setSpeed(10);  // 10 rpm   

}

void loop() {
  if (digitalRead(CALIBRATION_PIN) == 0) {
    // Run mode

    if (digitalRead(CLOCKWISE_PIN) == 0) {
     
      int newPosition = random(1, 360);
      int distance = currentPosition - newPosition;
      int shortDistance = 0;
  
      // Calculate the short distance
      if (distance > 180) {
        // If distance > 180, subtract 360 from distance
        shortDistance = distance - 360;
      } else if (distance < -180) {
        // If distance < -180, add 360 to difference
        shortDistance = distance + 360;
      } else {
        // If distance is between -180 and 180, use the distance number
        shortDistance = distance;
      }
  
      // Calculate the number of steps
      int numberSteps = map(abs(shortDistance), 0, 360, 0, 513);
      
      int direction = FORWARD;
      // Reverse direction if distance is greater than zero
      if (shortDistance > 0) {
        direction = BACKWARD; 
      }

      serialLogKeyValue("Current Position", String(currentPosition));
      serialLogKeyValue("New Position", String(newPosition));
      serialLogKeyValue("Distance Degrees", String(distance));
      serialLogKeyValue("Short Distance Degrees", String(shortDistance));
      serialLogKeyValue("Distance Steps", String(numberSteps));
      serialLogKeyValue("Cardinal", cardinalDirection(newPosition));
      Serial.println("---");
  
      // Move it!
      if (numberSteps > 0) {
        stepper.step(numberSteps, direction, SINGLE); 
        stepper.release();
      }
  
      currentPosition = newPosition;

    }

    if (digitalRead(ANTICLOCKWISE_PIN) == 0 && motorState == 0) {

      int speed = random(100, 150);
      serialLogKeyValue("Speed", String(speed));
      Serial.println("---");
      motorState = 1;
      motor.setSpeed(speed);
      motor.run(FORWARD);
      delay(500);
        
        
    } else if (digitalRead(ANTICLOCKWISE_PIN) == 0 && motorState == 1) {

      motor.run(RELEASE);
      delay(500);
      motorState = 0;
      
    }
  }

  if (digitalRead(CALIBRATION_PIN) == 1) {
    // Calibration mode

    if(digitalRead(CLOCKWISE_PIN) == 0) {
      stepper.onestep(FORWARD, SINGLE);
      Serial.println("1 step forward");
      stepper.release();
    }

    if(digitalRead(ANTICLOCKWISE_PIN) == 0) {
      stepper.onestep(BACKWARD, SINGLE);
      Serial.println("1 step back");
      stepper.release();
    }
  }
}

String cardinalDirection(int degrees) {
  if(degrees >= 315 && degrees <= 360) {
    return "northerly";
  }
  if(degrees >= 1 && degrees <= 45) {
    return "northerly";
  }
  if(degrees >= 45 && degrees <= 135) {
    return "easterly";
  }
  if(degrees >= 135 && degrees <= 225) {
    return "southerly";
  }
  if(degrees >= 225 && degrees <= 315) {
    return "westerly";
  }
  return "Out of range";
}

void serialLogKeyValue(String key, String value) {
  Serial.print(key + ": ");
  Serial.println(value);
}
