#include <Wire.h>
#include <VL53L0X.h>
#include <DFPlayer_Mini_Mp3.h>

int bluePin = 11;
int greenPin = 10;
int redPin = 3;

VL53L0X sensor;

int playerVolume = 1;

int maxDistance = 350; //define here
int minDistance = 50;
int currentDistance = 0;
int map_sound;


int color_value_blue = 255;
int color_value_green = 0;
  
void setup()
{
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT); 
  pinMode (2, INPUT_PULLUP); 
  Serial.begin(9600);
  Wire.begin();
  sensor.init();
  sensor.setTimeout(500);
  sensor.startContinuous();
  mp3_set_serial (Serial);
  mp3_set_volume (playerVolume); 
  mp3_play ();
  delay(1000);
  mp3_set_repeat (1); //the DFPlayer with Red Light and a JC chip are counterfeit
                     //they do not work with loop commands (0X8 or 0X19). So the only way is to have a single file on the card and loop the card.
  delay(1000);
}

void loop()
{
  //Serial.print(sensor.readRangeContinuousMillimeters());
  //Serial.println();
  uint16_t input = sensor.readRangeContinuousMillimeters();
  if (input > minDistance && input < maxDistance){
    input = constrain(input, minDistance, maxDistance); 
    playerVolume = map(input, minDistance, maxDistance, 0, 30);
    // convertto log
    
    color_value_blue = map(input, minDistance, maxDistance, 255, 0);
    color_value_blue = constrain(color_value_blue, 0, 255); 
    color_value_green = map(input, minDistance, maxDistance, 0, 255);
    color_value_green = constrain(color_value_green, 0, 255); 
    mp3_set_volume (playerVolume);  
    setColor(0, color_value_green, color_value_blue);
  }
  else {
    //mp3_set_volume (playerVolume);  
    setColor(0, color_value_green, color_value_blue);     
  }
  button();
  Serial.println("volume: " + String(playerVolume));
  Serial.println("blue: " + String(color_value_blue));
  Serial.println("green: " + String(color_value_green));
}


void setColor(int red, int green, int blue)
{
  red = 255 - red;
  green = 255 - green;
  blue = 255 - blue;
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);  
}

void button(){
  if (digitalRead(2) == LOW){
  mp3_next();
   Serial.println("pressed"); 
  delay (500);
  }
}

