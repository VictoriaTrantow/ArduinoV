/*! @file
 * 
 *  \mainpage BME 201
 *  
 *  \author Victoria Trantow
 *  \date 3/10/18
 *
 * \section intro_sec Introduction
 *
 * This is the introduction.
 *
 * \section install_sec Installation
 *
 * \subsection step1 Step 1: Opening the box
 *
 * \section operation_sec Theory of operation
 * 
 * read in an ADC value, convert that ADC value to voltage and then temperature*,
 * display that temperature on the LCD display, record the temperature (approximately
 * every 1/3 sec) onto an SD card, and either turn the heating element on or off
 * based on the temperature.
 */
#include <SD.h>

File myFile; //!< The name of the variable that references the file you wish to save to
uint32_t last_sd_time; //!< Keeps track of millis() for saving to file.
const uint16_t digitOnTime = 25; //!< Time in microseconds to turn the LED digit on.
/*
 * Pins for SD. The SD library assigns and initializes DI, DO, SCK.
 */
const uint8_t CS = 10; //!< Output pin for SD Chip Select(CS).
//const uint8_t DI = 11; //!< Pin connected to the SD Data In(DI).
//const uint8_t DO = 12; //!< Pin connected to the SD Data Out(DO).
//const uint8_t SCK = 13;//!< Pin connected to the SD System Clock(SCK).
 /*  
  *   Pins for 7 segment display. 
  */
/*
 * Leave pins 0, 1 for Rx,Tx serial communications.
 */
const uint8_t segA = 2;  //!< Output pin connected to segment A of the 7 seg display.
const uint8_t segB = 3; 
const uint8_t segC = 4;
const uint8_t segD = 5;
const uint8_t segE = 6;
const uint8_t segF = 7;
const uint8_t segG = 8;
const uint8_t DECIMAL = 9;
const uint8_t ledEnablePins[4] = {A2, A3, A4, A5}; //!< Output pins to power each LED digit.

const uint8_t heatelement = A1; //!< Output pin connected to the heater.

const uint8_t aIn = A0; //!< Analog input pin connected to the temperature sensor.

/* 
*  Function prototypes: new functions either need to be defined or declared using a prototype before using them. 
*  I am putting function prototypes at the beginning before loop and main, and they will be called upon during the loop()
*/
void return_digits_to_display(double somenumber, uint8_t mydigits[4]);
void displayDigit(uint8_t digit);
void displayNumber(double value);

void testdisplayDigit(void);

/*! 
 * \brief Returns the four digits to display.
 * 
 * \param [in] somenumber: floating point number to convert into digits.
 * \param [out] mydigits: array of four digits to represent the double. */
void return_digits_to_display(double somenumber, uint8_t mydigits[4]){
     /* example for proof of concept: 84.65
     since the array is of integers, reassigning double values to int will just
     eliminate the digits after the decimal place. I'm using this and some basic
     algebra to my advantage to separate the double into 4 separate digits.*/
  if ((100.00 > somenumber) && (0.00 <= somenumber)) {
    mydigits[0] = somenumber/10; //8.465 --> 8
    mydigits[1] = (somenumber - mydigits[0]*10); // 84.65 - 80, 4
    mydigits[2] = (somenumber - (mydigits[0]*10 + mydigits[1]))*10; //84.65-84, *10, 6
    mydigits[3] = (somenumber + 0.005 - (mydigits[0]*10 + mydigits[1] + mydigits[2]*.1))*100; //84.65 - 84.6, *100, 5  
    //to verify:
    Serial.print(mydigits[0]); 
    Serial.print(mydigits[1]); 
    Serial.print(".");
    Serial.print(mydigits[2]); 
    Serial.println(mydigits[3]);    
  }else{
    Serial.println("return_digits_to_display() invalid input.");
    mydigits[0] = 0;
    mydigits[1] = 0;
    mydigits[2] = 0;
    mydigits[3] = 0;
  }  
}

/*!
 * \brief Takes a single unsigned integer digit and displays it on the LCD.
 * 
 * Low corresponds with being on, high with off for this.
 * \param [in] digit The value to display on the LCD. 
 * \param [in] decimalPoint true if the decimal point should be displayed to the right of this digit.
 */
void displayDigit(uint8_t digit, bool decimalPoint){

  if (decimalPoint) {
    digitalWrite(DECIMAL, LOW); //turns on the decimal place
  } 
  
  //switch works so that if digit is equal to the case, that case will proceed.
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

/*! 
 * This takes a double, and refers to return_digits_to_display() function, 
 *  in order to change this double into an integer array. Then, it steps 
 *  through a for loop, at values 0,1,2,3. It's set up so that each [0 1 2 3]
 *  will correspond to a digit of the physical display, and it also corresponds 
 *  to that index in the int array of the double. So, the digit will be displayed
 *  in the correct place, and in between each loop all the places get turned off.
 *  This function gets called in the main loop, rapidly, so the number will 
 *  appear to constantly be displayed.
 *  \param [in] value Number to convert to four digits and display.
*/
void displayNumber(double value) {
  uint8_t d;
  uint8_t mydigits[4];
  /* Convert the floating point value into four digits.*/
  return_digits_to_display(value, mydigits);
   /* Paint each digit*/
  for(d = 0; d <4; d++) {
    //Turn on a single digit.
    if (1 == d) {
      displayDigit(mydigits[d], true ); //Turn on the correct segments for this digit.
    }else {
      displayDigit(mydigits[d], false ); // No decimal point after this digit.
    }
    digitalWrite(ledEnablePins[d], HIGH); 
    delayMicroseconds(digitOnTime); //Display this digit.
    digitalWrite(ledEnablePins[d], LOW); //Turn off the digit.
  } // for (each LED digit)
}

/*!
 * \brief Arduino setup initialization function.
 */
void setup() {
    uint8_t d;
    
    Serial.begin(9600);
    
    for (d=0; d<4; d++) {
      pinMode(ledEnablePins[d], OUTPUT);
      digitalWrite(ledEnablePins[d], LOW); // Don't power any of the LEDs.
    }
    pinMode(segA, OUTPUT);
    pinMode(segB, OUTPUT);
    pinMode(segC, OUTPUT);
    pinMode(segD, OUTPUT);
    pinMode(segE, OUTPUT);
    pinMode(segF, OUTPUT);
    pinMode(segG, OUTPUT);
    pinMode(DECIMAL, OUTPUT);
    /*
     * Initialize all the seqgments to off.
     */
    digitalWrite(segA, HIGH);
    digitalWrite(segB, HIGH);
    digitalWrite(segC, HIGH);
    digitalWrite(segD, HIGH);
    digitalWrite(segE, HIGH);
    digitalWrite(segF, HIGH);
    digitalWrite(segG, HIGH);
    digitalWrite(DECIMAL, HIGH);

    SD.begin(CS); // Initializes CS pin for SD card writing. Library intializes (DI, SO, SCK).
    
    pinMode(heatelement, OUTPUT);

    last_sd_time = millis();

    testdisplaySegment();
    testdisplayDigit();
    testdisplayNumber();

    for (;;);
}

/*!
 * \brief Arduino standard sketch loop.
*/
void loop() {  
  double display_me; // Initialize value to display in main loop. Eventually this should be the realtime temperature
  float sensorValue = analogRead(aIn);  // read the input on analog pin 0:     
  float voltagereading = sensorValue*5/1024; 
  
  Serial.println(voltagereading);   // print out the value to serial port:  
  //equation to take voltagereading to temp
  double temp = voltagereading; //change when we get equation       
  
  display_me = temp;
  displayNumber(display_me); //keeps looping through to display the current temperature.
  
  const double lowtemp = 35.0;            // Low and hightemp limits are not changed so declare them as const. 
  const double hightemp = 39.0;
  
  if (300 <= (millis() - last_sd_time))
  {
    myFile = SD.open("test.txt", FILE_WRITE); //opens up a file on the SD named test.txt for writing
    myFile.println(temp); //prints to the file
    myFile.close(); //closes the file
    last_sd_time = millis();
  }  
  
  if(temp<lowtemp)
  {
    digitalWrite(heatelement,HIGH);
  }
  else if(temp>hightemp)
  {
    digitalWrite(heatelement,LOW);
  }              
}
   
