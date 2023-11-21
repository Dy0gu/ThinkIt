int guess = 0, pushStart = 0, pushEnd = 0;
int lastButtonStateYes = HIGH, lastButtonStateNo = HIGH;

void setup() {
  Serial.begin(9600);

  // Initialize button pins and LED pins
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  for (int i = 6; i <= 12; i++)
    pinMode(i, OUTPUT);
}

void waitButtonPush() {
  while (true) {
    if (digitalRead(3) == LOW) {
      // Button "yes" pressed
      lastButtonStateYes = LOW;
      pushStart = millis();
      while (lastButtonStateYes == digitalRead(3)) {}
      pushEnd = millis();
      return;
    } else if (digitalRead(4) == LOW) {
      // Button "no" pressed
      lastButtonStateNo = LOW;
      pushStart = millis();
      while (lastButtonStateNo == digitalRead(4)) {}
      pushEnd = millis();
      return;
    }
  }
}

void loop() {
  Serial.print("Think of a number from 0 to 127!");

  for (int power = 0; power <= 6; power++) {
    if ((pushEnd - pushStart) >= 2000) {
      // Skip iterations if reset occurred
    } else {
      Serial.println("\nIs your number in the following sequence?");
      for (int seqNum = 0; seqNum <= 127; seqNum++) {
        if ((seqNum >> power) & 1) {
          // Print relevant numbers in the sequence
          Serial.print(seqNum);
          Serial.print(" ");
        }
      }
      waitButtonPush();

      if ((pushEnd - pushStart) >= 2000) {
        // Indicate reset occurrence
        Serial.println("\nRESET!");
        delay(500);
      } else if (lastButtonStateYes == LOW) {
        // Light up LEDs for the guessed bits
        digitalWrite(6 + power, HIGH);
        lastButtonStateYes = HIGH;
        guess += (1 << power);  // Build the guessed number
      } else if (lastButtonStateNo == LOW) {
        // Reset the variable for button press detection
        lastButtonStateNo = HIGH;
      }
    }
  }

  if ((pushEnd - pushStart) >= 2000) {
    pushEnd = 0;
    pushStart = 0;
  } else {
    Serial.print("\nThe number you thought of was: ");
    Serial.print(guess);
    delay(5000);
  }

  // Reset variables and perform LED sweeping effect
  guess = 0;
  for (int i = 6; i <= 12; i++) {
    digitalWrite(i, LOW);
  }
  for (int i = 6; i <= 12; i++) {
    digitalWrite(i, HIGH);
    delay(100);
    digitalWrite(i, LOW);
  }
}
