/*
 * The following constant valus are used to address the different pins.
 */
const uint8_t d1 = 1;
const uint8_t d2 = 2;
const uint8_t d3 = 3;
const uint8_t d4 = 11; 

const uint8_t segA = 4; 
const uint8_t segB = 5; 
const uint8_t segC = 6;
const uint8_t segD = 7;
const uint8_t segE = 8;
const uint8_t segF = 9;
const uint8_t segG = 10;
const uint8_t DECIMAL = 12;

/*
 * Function prototypes.
 */
void return_digits_to_display(double somenumber, uint8_t mydigits[4]);
void displayDigit(uint8_t digit);
void test_function(void);
void displayNumber(double value);

 
/*
 * \brief This function returns the four digints to display.
 * 
 * \param [in] somenumber The floating point number to convert into digits.
 * \param [out] digits Four digits to represent the double.
 */
void return_digits_to_display(double somenumber, uint8_t mydigits[4]){
   //or just cast to int and multiply ex: 84.65

  Serial.println("return_digits_to_display() invalid input.");
  Serial.println(somenumber);
  
  if ((10.0 > somenumber) && (0.0 <= somenumber)) {
    mydigits[0] = somenumber/10; //8.465 --> 8
    mydigits[1] = (somenumber - mydigits[0]*10); // 84.65 - 80, 4
    mydigits[2] = (somenumber - (mydigits[0]*10 + mydigits[1]))*10; //84.65-84, *10, 6
    mydigits[3] = (somenumber - (mydigits[0]*10 + mydigits[1] + mydigits[2]*.1))*100; //84.65 - 84.6, *100, 5  
    Serial.println(mydigits[0]);
    Serial.println(mydigits[1]);
    Serial.println(mydigits[2]);
    Serial.println(mydigits[3]);    
  }else{
    Serial.println("return_digits_to_display() invalid input.");
    mydigits[0] = 0;
    mydigits[1] = 0;
    mydigits[2] = 0;
    mydigits[3] = 0;
  }  
}

/*
 * \brief Test function to verify.
 */
void test_function(void) {

  double value=1.234;
  uint8_t mydigits[4]; // Memory for each digit.
  unsigned long delay_msec = 1000; // Number of milliseconds to delay between each digit.

  Serial.println("test_function().");
  
  return_digits_to_display(value, mydigits);
  displayDigit(mydigits[0]);
  delay(delay_msec);
  
  displayDigit(mydigits[1]);
  delay(delay_msec);
  
  displayDigit(mydigits[2]);
  delay(delay_msec);

  displayDigit(mydigits[3]);
  delay(delay_msec);

  Serial.println("test_function(done).");  
}

/*
 * \brief Takes an single unsigned integer digit and displays it on the LCD,
 * 
 * This only turns on the segments for a single digit of the LCD. 
 * 
 * \param [in] digit The value to display on the LCD.
 */
void displayDigit(uint8_t digit){
  
  //digitalWrite(DECIMAL, HIGH);
  
  switch(digit){
   case 0:   //To display 0, we turn on all segments except for segment G
      digitalWrite(segA, LOW);
      digitalWrite(segB, LOW);
      digitalWrite(segC, LOW);
      digitalWrite(segD, LOW);
      digitalWrite(segE, LOW);
      digitalWrite(segF, LOW);
      digitalWrite(segG, HIGH);
      break;
  case 1:
      digitalWrite(segA, HIGH);
      digitalWrite(segB, HIGH);
      digitalWrite(segC, HIGH);
      digitalWrite(segD, HIGH);
      digitalWrite(segE, LOW);
      digitalWrite(segF, LOW);
      digitalWrite(segG, HIGH);
      break;
  case 2:
      digitalWrite(segA, LOW);
      digitalWrite(segB, LOW);
      digitalWrite(segC, HIGH);
      digitalWrite(segD, LOW);
      digitalWrite(segE, LOW);
      digitalWrite(segF, HIGH);
      digitalWrite(segG, LOW);
      break;
  case 3:
      digitalWrite(segA, LOW);
      digitalWrite(segB, LOW);
      digitalWrite(segC, LOW);
      digitalWrite(segD, LOW);
      digitalWrite(segE, HIGH);
      digitalWrite(segF, HIGH);
      digitalWrite(segG, LOW);
      break;
  case 4:
      digitalWrite(segA, HIGH);
      digitalWrite(segB, LOW);
      digitalWrite(segC, LOW);
      digitalWrite(segD, HIGH);
      digitalWrite(segE, HIGH);
      digitalWrite(segF, LOW);
      digitalWrite(segG, LOW);
      break;
  case 5:
      digitalWrite(segA, LOW);
      digitalWrite(segB, HIGH);
      digitalWrite(segC, LOW);
      digitalWrite(segD, LOW);
      digitalWrite(segE, HIGH);
      digitalWrite(segF, LOW);
      digitalWrite(segG, LOW);
      break;
  case 6:
      digitalWrite(segA, LOW);
      digitalWrite(segB, HIGH);
      digitalWrite(segC, LOW);
      digitalWrite(segD, LOW);
      digitalWrite(segE, LOW);
      digitalWrite(segF, LOW);
      digitalWrite(segG, LOW);
      break;
   case 7:
      digitalWrite(segA, LOW);
      digitalWrite(segB, LOW);
      digitalWrite(segC, LOW);
      digitalWrite(segD, HIGH);
      digitalWrite(segE, HIGH);
      digitalWrite(segF, HIGH);
      digitalWrite(segG, HIGH);
      break;
  case 8:
      digitalWrite(segA, LOW);
      digitalWrite(segB, LOW);
      digitalWrite(segC, LOW);
      digitalWrite(segD, LOW);
      digitalWrite(segE, LOW);
      digitalWrite(segF, LOW);
      digitalWrite(segG, LOW);
      break;
  case 9:
      digitalWrite(segA, LOW);
      digitalWrite(segB, LOW);
      digitalWrite(segC, LOW);
      digitalWrite(segD, LOW);
      digitalWrite(segE, HIGH);
      digitalWrite(segF, LOW);
      digitalWrite(segG, LOW);
      break;
  default:
    Serial.println("displayDigit() invalid digit.");
    Serial.println(digit);
    break;
  }
}

#define DISPLAY_BRIGHTNESS  500

void displayNumber(double value) 
{
  uint8_t d;
  uint8_t mydigits[4];
  /*
   * Convert the floating point value into four digits.
   */
  return_digits_to_display(value, mydigits);
   /*
   *  "Paint each digit"
   */
  for(d = 0; d <4; d++) {
    //Turn on a single digit.
    switch(d) {
    case 1:
      digitalWrite(d1, HIGH);
      break;
    case 2:
      digitalWrite(d2, HIGH);
      break;
    case 3:
      digitalWrite(d3, HIGH);
      break;
    case 4:
      digitalWrite(d4, HIGH);
      break;
    }
    
    displayDigit(mydigits[d]); //Turn on the correct segments for this digit.
      
    delayMicroseconds(DISPLAY_BRIGHTNESS); //Display this digit for a fraction of a second (between 1us and 5000us, 500 is pretty good)

    //Turn off all digits. Don't bother figuring out which one to turn off, just do all of them.
    digitalWrite(d1, LOW);
    digitalWrite(d2, LOW);
    digitalWrite(d3, LOW);
    digitalWrite(d4, LOW);
  } // for (each LED digit)
} // displayNumber()


//set up initializes all the pins to be used 
void setup() {  
  pinMode(d1, OUTPUT);
  pinMode(d2, OUTPUT);
  pinMode(d3, OUTPUT);
  pinMode(d4, OUTPUT);
  pinMode(segA, OUTPUT);
  pinMode(segB, OUTPUT);
  pinMode(segC, OUTPUT);
  pinMode(segD, OUTPUT);
  pinMode(segE, OUTPUT);
  pinMode(segF, OUTPUT);
  pinMode(segG, OUTPUT);
  pinMode(DECIMAL, OUTPUT);

  /*
   *  Test function goes here and will only execute once.
   *  Remove this function once everything is tested out.
   */
   test_function();
}


double display_me = 1.234; // Value to display in main loop.
unsigned long loop_count = 0;  // Number of times loop has been called.

// main code here, to run repeatedly:
// arrray of the 4 digits, for loop around the function?
void loop()
{
  displayNumber(display_me);

   loop_count++;  // Increment the counter.
   if (0 == (loop_count%100)) // Every 100th loop.
   {
      display_me = display_me + 0.001; // Change the value.
   }

  delay(10); // Wait msec between refreshing the LEDs.
}

