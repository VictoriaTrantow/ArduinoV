/*!
 * \file unitTests.ino
 * 
 * \brief This file implements simple unit tests.
 * 
 * These functions are in a separate file because they probably won't be used once the hardware and software are working.
 */

/*!
 *  Increment display from 0.000 to 100.000
 */
void testdisplayNumber(void) {
  uint8_t i;
  uint8_t d;
  float f;
  
  for (d=0; d<14; d++) {
    f = (float)(1UL<<d); // 2^d power.
    f = f/100.0; // Get two decimal places.
    
    if (f > 100.00) {        
      f = 99.99;
    }
    for (i=0;i<100;i++) {
      displayNumber(f);
    }
  }
}

/*!
 * Tests each digit individually.
 */
void testdisplayDigit(void) {
  uint8_t n; // Number to display.
  uint8_t d; // LED to use.
  uint8_t i; // Number of times to repeat each number. Easier to see.

  for (d=0; d<4; d++) {
    for (n=0; n<10; n++) {
      for (i=0;i<10;i++) {
        digitalWrite(ledEnablePins[d], HIGH); 
        displayDigit(n, false);
        delayMicroseconds(digitOnTime);  
        digitalWrite(ledEnablePins[d], LOW); 
        delay(20);
      }
    }
  }
}

/*
 * Tests each segment individually.
 * Good wiring test.
 */
void testdisplaySegment(void) {
  uint8_t d; // LED to use.

  for (d=0; d<4; d++) {

      digitalWrite(ledEnablePins[d], HIGH); 
      
      digitalWrite(segA, LOW);
      delayMicroseconds(digitOnTime);
      digitalWrite(segA, HIGH);
      delay(400);

      digitalWrite(segB, LOW);
      delayMicroseconds(digitOnTime);
      digitalWrite(segB, HIGH);
      delay(400);

      digitalWrite(segC, LOW);
      delayMicroseconds(digitOnTime);
      digitalWrite(segC, HIGH);
      delay(400);

      digitalWrite(segD, LOW);
      delayMicroseconds(digitOnTime);
      digitalWrite(segD, HIGH);
      delay(400);

      digitalWrite(segE, LOW);
      delayMicroseconds(digitOnTime);
      digitalWrite(segE, HIGH);
      delay(400);

      digitalWrite(segF, LOW);
      delayMicroseconds(digitOnTime);
      digitalWrite(segF, HIGH);
      delay(400);

      digitalWrite(segG, LOW);
      delayMicroseconds(digitOnTime);
      digitalWrite(segG, HIGH);
      delay(400);

      digitalWrite(DECIMAL, LOW);
      delayMicroseconds(digitOnTime);
      digitalWrite(DECIMAL, HIGH);
      delay(400);
      
     digitalWrite(ledEnablePins[d], LOW); 
  }
}

