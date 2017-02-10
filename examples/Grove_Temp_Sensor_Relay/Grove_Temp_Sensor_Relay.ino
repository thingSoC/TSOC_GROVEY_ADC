// Demo code for Grove - Temperature Sensor V1.1/1.2

#include <math.h>
#include <Wire.h>
#include <Adafruit_ADS1015.h>
#include <SeeedGrayOLED.h>
#include <stdlib.h>
#include <string.h>

// Adafruit_ADS1115 ads;  /* Use this for the 16-bit version */
Adafruit_ADS1015 ads;     /* Use this for the 12-bit version */

const int B = 4255;              // B value of the thermistor
const int R0 = 100000;           // R0 = 100k
const int pinTempSensor = 2;     // Grove - Temperature Sensor connect to ADC AIN1
const int pinRelay = 7;          // Grove - Temperature Sensor connect to ADC AIN1

char buf[16];
int status;

void setup()
{
//  Serial.begin(9600);
//  while (!Serial) {
//    ; // wait for serial port to connect. Needed for native USB
//  }

  pinMode(pinRelay, OUTPUT);
  digitalWrite(pinRelay, LOW);
  Wire.begin();  
  SeeedGrayOled.init();  //initialize SEEED Gray OLED display

  SeeedGrayOled.clearDisplay();           //clear the screen and set start position to top left corner
  SeeedGrayOled.setNormalDisplay();       //Set display to Normal mode
  SeeedGrayOled.setVerticalMode();        // Set to vertical mode for displaying text
  SeeedGrayOled.setGrayLevel(15); //Set Grayscale level. Any number between 0 - 15.
 
  // The ADC input range (or gain) can be changed via the following
  // functions, but be careful never to exceed VDD +0.3V max, or to
  // exceed the upper and lower limits if you adjust the input range!
  // Setting these values incorrectly may destroy your ADC!
  //                                                                ADS1015  ADS1115
  //                                                                -------  -------
  ads.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit = 3mV      0.1875mV (default)
  // ads.setGain(GAIN_ONE);        // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
  // ads.setGain(GAIN_TWO);        // 2x gain   +/- 2.048V  1 bit = 1mV      0.0625mV
  // ads.setGain(GAIN_FOUR);       // 4x gain   +/- 1.024V  1 bit = 0.5mV    0.03125mV
  // ads.setGain(GAIN_EIGHT);      // 8x gain   +/- 0.512V  1 bit = 0.25mV   0.015625mV
  // ads.setGain(GAIN_SIXTEEN);    // 16x gain  +/- 0.256V  1 bit = 0.125mV  0.0078125mV
  
  ads.begin();    
}

void loop()
{
    // int a = analogRead(pinTempSensor);
    int a = ads.readADC_SingleEnded(pinTempSensor);

    //float R = 1023.0/((float)a)-1.0;
    float R = 1689/((float)a)-1.0;
    R = 100000.0*R;

    float temperature=1.0/(log(R/100000.0)/B+1/298.15)-273.15;//convert to temperature via datasheet ;

    SeeedGrayOled.setTextXY(0,0);           //Set the cursor to 0th line, 0th Column
    SeeedGrayOled.putString("Temperature");
    SeeedGrayOled.setTextXY(1,0);
    SeeedGrayOled.putString("in Celsius:");
    SeeedGrayOled.setTextXY(2,0); 
    dtostrf(temperature, 4, 2, buf);  
    SeeedGrayOled.putString(buf);
       
 //   Serial.print("Temperature in Celsius = ");
 //   Serial.println(temperature);
    if (temperature > 20){
      digitalWrite(pinRelay, HIGH);
    } else {
      digitalWrite(pinRelay, LOW);
    }
    delay(100);
}
