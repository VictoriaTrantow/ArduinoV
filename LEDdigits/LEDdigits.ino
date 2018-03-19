/*! @file
 * 
 *  \mainpage BME 201
 *  
 *  BME 201
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
 //  AnalogRead to Serial
float aIn = A0;
#include <SD.h>
File myFile; //!< The name of the variable that references the file you wish to save to
int CS = 10; //pin 10 is the CS pin
 
int i = 0; //for the switch, in this lab we're alternating btwn 2 numbers
double display_me = 0; // Initialize value to display in main loop. Eventually this should be the realtime temperature
double temperature = 0;
 /*
 A generic unsigned integer can represent any number between 0 and 2^32-1.
 You can use an 8 bit uint8_t to represent up any value 0-255 which is more
 appropriate for pins and lcd digits.*/
 const uint8_t d1 = 1; //!< Pin used to turn the first LED digit on.
 const uint8_t d2 = 2; //!< Pin used to turn the second LED digit on.
 const uint8_t d3 = 3; //!< Pin used to turn the third LED digit on.
 const uint8_t segA = 4; //!< Pin used to turn segment A of the LED on. 
 const uint8_t segB = 5; 
 const uint8_t segC = 6;
 const uint8_t segD = 7;
 const uint8_t segE = 8;
 const uint8_t segF = 9;
 const uint8_t segG = 10;
 const uint8_t d4 = 11; //!< Pin used to turn the fourth LED digit on.
 const uint8_t DECIMAL = 12;
 const uint8_t heatelement = 13;
 /* Function prototypes: new functions either need to be defined or declared 
  using a prototype before using them. I am putting function prototypes at the
  beginning before loop and main, and they will be called upon during the loop()*/
void return_digits_to_display(double somenumber, uint8_t mydigits[4]);
void displayDigit(uint8_t digit);
void test_function(void);
void displayNumber(double value);
/*! 
 * \brief Returns the four digits to display.
 * 
 * \param [in] somenumber: floating point number to convert into digits.
 * \param [out] digits: array of four digits to represent the double.*/
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
/*
 * \brief Takes a single unsigned integer digit and displays it on the LCD,
  Low corresponds with being on, high with off for this 
  \param [in] digit: the value to display on the LCD. 
 */
void displayDigit(uint8_t digit){
  digitalWrite(DECIMAL, HIGH); //turns off the decimal place
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
  }}
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
  /*Convert the floating point value into four digits.*/
  return_digits_to_display(value, mydigits);
   /*Paint each digit*/
  for(d = 0; d <4; d++) {
    //Turn on a single digit.
    switch(d) {
    case 0:
      digitalWrite(d1, HIGH);
      break;
    case 1:
      digitalWrite(DECIMAL, LOW); //We want decimal to occur after the 2nd digit for the temp
      digitalWrite(d2, HIGH);
      break;
    case 2:
      digitalWrite(d3, HIGH);
      break;
    case 3:
      digitalWrite(d4, HIGH);
      break;
    }
    displayDigit(mydigits[d]); //Turn on the correct segments for this digit.
    delayMicroseconds(10); //Display this digit for 10 microseconds)
    //Turn off all digits.
    digitalWrite(d1, LOW);
    digitalWrite(d2, LOW);
    digitalWrite(d3, LOW);
    digitalWrite(d4, LOW);
  } // for (each LED digit)
}

/*!
 * \brief Arduino setup initialization function.
 */
void setup() {
    Serial.begin(9600);
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
    SD.begin(CS); //initializes CS pin for SD card writing
    pinMode(heatelement, OUTPUT);
}
unsigned long loop_count = 0;
double lowtemp = 35.0;
double hightemp = 39.0;

/*!
 * \brief Arduino standard sketch loop.
*/
void loop() {  
       // read the input on analog pin 0:   
      float sensorValue = analogRead(aIn);  
      // print out the value to serial port:  
      float voltagereading = sensorValue*5/1024; 
      Serial.println(voltagereading);   
      //equation to take voltagereading to temp
      double temp = voltagereading; //change when we get equation       
  
      display_me = temp;
      displayNumber(display_me); //keeps looping through to display the current temp
      
         
      loop_count++;
        if (0 == (loop_count%300))//every 300 ms
        {
          myFile = SD.open("test.txt", FILE_WRITE); //opens up a file on the SD named test.txt for writing
          myFile.println(temp); //prints to the file
          myFile.close(); //closes the file
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
   
