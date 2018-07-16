#include <Wire.h>
#include <VL53L0X.h>
#include <DFPlayer_Mini_Mp3.h>

int bluePin = 11;
int greenPin = 10;
int redPin = 3;

VL53L0X sensor;

int playerVolume = 0;
int track = 1;

int maxDistance = 250; //define here
int minDistance = 60;

unsigned long currentMillis;
unsigned long previousMillis = 0; 

int color_value = 255;
int color_direction = -1;
int fadeperiod = 2000 + (random (1000) - 500);
float fadestep = 255.0 / fadeperiod;
  
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
  if (input > 1 && input < maxDistance){
    input = constrain(input, minDistance, maxDistance); 
    int map_sound = map(input, minDistance, maxDistance, 30, 0);
    //map_sound = 30 - map_sound; //invert
    int map_colour = map(input, minDistance, maxDistance, 255, 0);
    map_colour = constrain(map_colour, 0, 255); 
    mp3_set_volume (map_sound);  
    setColor(map_colour, 0, 0);
  }
  else if (input > maxDistance){
    mp3_set_volume (0);
    currentMillis = millis();
    int color_step = int( (currentMillis - previousMillis) * fadestep);
    color_value = color_value+color_step*color_direction;
    if (color_value > 255) {
      color_value = 255;
      color_direction = color_direction * -1;
    }
    else if (color_value < 0){
      color_value = 0;
      color_direction = color_direction * -1;
    }
    setColor(0, 0, color_value);
    //Serial.println(fadeperiod);
    //Serial.println(color_value);
    previousMillis = currentMillis;
  }

  button();

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
  track++;
  if (track == 5){
  track = 1;
  }
   Serial.println("pressed"); 
  delay (500);
  }
}  
