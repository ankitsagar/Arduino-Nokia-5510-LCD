
/*
Displaying Temperature, Humidity and Stopwatch on NOKIA 5510 LCD

*/

// Including the required header files
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include<dht11.h>

// Software SPI (slower updates, more flexible pin options):
// pin 7 - Serial clock out (SCLK)
// pin 6 - Serial data out (DIN)
// pin 5 - Data/Command select (D/C)
// pin 4 - LCD chip select (CS)
// pin 3 - LCD reset (RST)
Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);

// Global Varible to access the library function
dht11 DHT11;


// Defining pins for temperature, humiduty and power button
// Temperature sensor should be connected to Pin Analog5 in Arduino
// Humidity sensor should be connected to Pin Analog4 in Arduino
// Power Button should be connected to Pin Digital 2 in Arduino
int temperaturePin = A5;
int humidityPin = A4;
int resetPin = 2;

// Global varible to store Temprature and Humidity
float temperature;
float humidity;

// Global variable to store power button Status
// This program uses Slide Switch as a Power Button
boolean resetPinStatus;

// Initializing Stopwatch values globally
int seconds = 0;
int minutes = 0;
int hours = 0;
int abc = 0;
int flag = 0;


//Genrated binary font for temperature icon through The Dot Factory

static const unsigned char PROGMEM logo16_glcd_bmp[] =
{ 0b00001000, 0b00000000, //     #    
  0b00100010, 0b00000000, //   #   #  
  0b01000001, 0b00000000, //  #     # 
  0b01000001, 0b00000000, //  #     # 
  0b01000001, 0b00000000, //  #     # 
  0b01000001, 0b00000000, //  #     # 
  0b01000001, 0b00000000, //  #     # 
  0b01001001, 0b00000000, //  #  #  # 
  0b01001001, 0b00000000, //  #  #  # 
  0b01001001, 0b00000000, //  #  #  # 
  0b10001000, 0b10000000, // #   #   #
  0b10011100, 0b10000000, // #  ###  #
  0b10011100, 0b10000000, // #  ###  #
  0b10001000, 0b10000000, // #   #   #
  0b01000001, 0b00000000, //  #     # 
  0b00011100, 0b00000000, //    ###   
};

// Genrated binary font for humidity icon through The Dot Factory
static const unsigned char PROGMEM  humidityBitmaps[] =
{  0b00000110, 0b00000000, //      ##     
    0b00001001, 0b00000000, //     #  #    
    0b00000000, 0b00000000, //             
    0b00010000, 0b10000000, //    #    #   
    0b00100000, 0b01000000, //   #      #  
    0b00000000, 0b00000000, //             
    0b01000000, 0b00100000, //  #        # 
    0b00010000, 0b00000000, //    #        
    0b10000000, 0b00010000, // #          #
    0b10100000, 0b00010000, // # #        #
    0b10100000, 0b00010000, // # #        #
    0b00000000, 0b00000000, //             
    0b00000000, 0b00000000, //             
    0b01000000, 0b00100000, //  #        # 
    0b00100000, 0b01000000, //   #      #  
    0b00000110, 0b00000000, //      ##     
};


void setup() {
  // Setting serial monitor connection Speed to 9600 bits per second
      Serial.begin(9600);
      
  // Specifying Pins 
      pinMode(INPUT, A5);
      pinMode(INPUT, A4);
      pinMode(INPUT, 2);     

      display.begin();

      // you can change the contrast around to adapt the display
      // for the best viewing!
      display.setContrast(90);

      display.display(); // show splashscreen
      
      display.clearDisplay();   // clears the screen and buffer
}

void loop() {
      display.drawBitmap(0,0,logo16_glcd_bmp,16,16,BLACK);      //Displaying temprature icon on LCD
      display.drawBitmap(0,16,humidityBitmaps,16,16,BLACK);     //Displaying humidity icon on LCD
     
      // calling function to get Temprature, Humadity and Reset Pin Status
      // and storing it in variables 
      temperature = getTemperature();
      humidity = getHumidity();
      resetPinStatus = getResetStatus();

      //Calling Lines function to divide the LCD screen into 3 parts
      Lines();

      //Displaying Tempetrature and Humidity on LCD
      display.setCursor(20,5);
      display.setTextColor(BLACK);
      display.print(temperature);
      display.setCursor(20,20);
      display.setTextColor(BLACK);
      display.print(humidity);

      // If power button is on then it will fall in this condition
      if (resetPinStatus == 1){
        
          if (flag == 1){     // Resets the Watch and flag
            hours = 0;
            minutes = 0;
            seconds = 0;  
            flag = 0;
            startWatch();
          }
          else{
            startWatch();  // Starts the watch
          }
        
        }
        
      // When power button is off
      else{
          stopWatch();  // Stops the watch at that time 
          flag = 1;
        }
        
      display.display();  // Displays all elements on LCD Screen
      delay(1000);        // delay of 1000 micro seconds = 1 second
      display.clearDisplay(); // clears the display buffer

}

// Draw Lines at given point
void Lines(){
  display.drawLine(0,16,83,16,BLACK);
  display.drawLine(0,34,83,34,BLACK);
  display.display();
}

// gets the temperature from sensor and returns it
float getTemperature(){
  int sensorValue = analogRead(temperaturePin);
  float miliVolt = sensorValue*(5.0/1023.0)*1000;
  return miliVolt/10.0;
}

// gets the Humidity from sensor and returns it
float getHumidity(){
  DHT11.read(humidityPin);  
  return (float)DHT11.humidity;
}

// Prints time in hours:minutes:seconds Format
void printTime(){
  display.setCursor(20,40);
  display.setTextColor(BLACK);
  printDigit(hours);
  display.print(":");
  printDigit(minutes);
  display.print(":");
  printDigit(seconds);
  display.display();
}

// if any digit in Stopwatch is between 0 to 9 then
// it will add "0" before it
void printDigit(int digit){
      if(digit < 10){
        display.print("0");
        display.print(digit);
       }
       else{
        display.print(digit); 
       }  
}

// Gets the slide switch Status
boolean getResetStatus(){
  return (boolean)digitalRead(resetPin);
}

// Starts the Watch
void startWatch(){
  printTime();
  if (seconds == 59){
    seconds = 0;
  }
  else{
    seconds++;
  }

  if(minutes == 59){  
    minutes = 0;
    abc = 60;   // this Varible is used to change the hour after minute goes 59 to 0
  }
  else{
    abc = 0;
    if (seconds == 0){
      minutes++;
 
    }
  }

  hours = abc/60 + hours;

}

// Stops the Watch
void stopWatch(){
  printTime();
}
