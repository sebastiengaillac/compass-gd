#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

// pin 7 - Serial clock out (SCLK)
// pin 6 - Serial data out (DIN)
// pin 5 - Data/Command select (D/C)
// pin 4 - LCD chip select (CS)
// pin 3 - LCD reset (RST)
Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);

// Reference the HMC5883L Compass Library
#include <HMC5883L.h>

// Store our compass as a variable.
HMC5883L compass;
// Record any errors that may occur in the compass.
int error = 0;

#define Addr 0x1E               // 7-bit address of HMC5883 compass

static unsigned char Leters[] = { 'N' , 'E' , 'S' , 'W' };

static unsigned char PROGMEM arrow_bmp[] =
{ B00100000,
  B00100000,
  B01110000,
  B01110000,
  B11111000,
  B11111000,
  B11111000,
  B11111000,
}; 


void setup() {
  Serial.begin(9600);
  delay(100);                   // Power up delay
  Wire.begin();
  
  compass = HMC5883L(); // Construct a new HMC5883 compass.

  display.begin();
  display.setContrast(58);

  display.clearDisplay();   // clears the screen and buffer
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(0,0);
  display.print(" Galin Dimitrov");
  display.display(); // show splashscreen
  delay(1000);
  display.clearDisplay();   // clears the screen and buffer

}

void loop() {

 // Retrive the raw values from the compass (not scaled).
  MagnetometerRaw raw = compass.ReadRawAxis();
  // Retrived the scaled values from the compass (scaled to the configured scale).
  MagnetometerScaled scaled = compass.ReadScaledAxis();
  
  // Values are accessed like so:
  int MilliGauss_OnThe_XAxis = scaled.XAxis;// (or YAxis, or ZAxis)

  // Calculate heading when the magnetometer is level, then correct for signs of axis.
  float heading = atan2(scaled.YAxis, scaled.XAxis);
  
  // Once you have your heading, you must then add your 'Declination Angle', which is the 'Error' of the magnetic field in your location.
  // Find yours here: http://www.magnetic-declination.com/
  // Mine is: 2� 37' W, which is 2.617 Degrees, or (which we need) 0.0456752665 radians, I will use 0.0457
  // If you cannot find your Declination, comment out these two lines, your compass will be slightly off.
  float declinationAngle = 0.07737;
  heading += declinationAngle;
  
  // Correct for when signs are reversed.
  if(heading < 0)
    heading += 2*PI;
    
  // Check for wrap due to addition of declination.
  if(heading > 2*PI)
    heading -= 2*PI;
   
  // Convert radians to degrees for readability.
  float headingDegrees = heading * 180/M_PI; 


  int CompassAngle=(int)headingDegrees;
  
  display.clearDisplay();
/*  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(0,0);
  display.print(" X =");
  display.println(x);
  display.setTextColor(WHITE, BLACK); // 'inverted' text
  display.setTextSize(1);
  display.print(" Y =");
  display.println(y);
  display.setTextColor(BLACK);
  display.setTextSize(1);
  display.print(" Z =");
  display.println(z);
  */
  //display.drawBitmapInvert(30, 30,  N_bmp, 12, 8, 1);
  display.drawBitmapInvert(39, 24,  arrow_bmp, 12, 8, 1);
  display.drawLine(42, 0, 42, 24, BLACK);
  testDrawRow(CompassAngle);
  display.setCursor(32,34);  
  display.print(CompassAngle);
  display.print("~");
  display.display();
  delay(700);

}

void testDrawRow(int angle) {


  display.drawLine(0, 0, 84, 0, BLACK);
  display.drawLine(0, 1, 84, 1, BLACK);

  display.drawLine(0, 22, 91, 22, BLACK);
  display.drawLine(0, 23, 95, 23, BLACK);
  
  display.setTextSize(2);
  display.setTextColor(BLACK);  
  
  int start = 42 + angle / 3 ;
  if (start > 120) start += -120 ;
  int x = 0 ;
  int y = 18 ;
  for (int i=0; i<4; i++) {
      x = start + (i*30) -1;
      if (x>119) x += -120;

      display.drawPixel(x+1, y-2, 1);

      display.drawPixel(x, y, 1);
      display.drawPixel(x+1, y, 1);
      display.drawPixel(x+2, y, 1);
      display.drawPixel(x, y-1, 1);
      display.drawPixel(x+1, y-1, 1);
      display.drawPixel(x+2, y-1, 1);
      display.setCursor((x-4),(y-16));  
      display.write(Leters[i]);
   }
  for (int i=0; i<24; i++) {
      x = start + (i*5) -1;
      if (x>119) x += -120;
      display.drawPixel(x+1, y+1, 1);
      display.drawPixel(x, y+2, 1);
      display.drawPixel(x+1, y+2, 1);
      display.drawPixel(x+2, y+2, 1);
      display.drawPixel(x, y+3, 1);
      display.drawPixel(x+1, y+3, 1);
      display.drawPixel(x+2, y+3, 1);
   };
   
  for (int i=0; i<8; i++) {
      x = start + (i*15)-1;
      if (x>119) x += -120;
      display.drawPixel(x+1, y-1, 1);

      display.drawPixel(x, y, 1);
      display.drawPixel(x+1, y, 1);
      display.drawPixel(x+2, y, 1);
      display.drawPixel(x, y+1, 1);
      display.drawPixel(x+1, y+1, 1);
      display.drawPixel(x+2, y+1, 1);

   };


  //display.drawBitmap(0, 18,  row_bmp, 84, 4, 1);
}



