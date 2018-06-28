#include <Arduino.h>
#include <elapsedMillis.h>
#include <EEPROM.h>

#include "FastLED.h"
#include "NewPing.h"

#define DATA_PIN 6       // Data pin for FastLED LED control
#define TRIGGER 3        // Pin used to trigger HC-SR04
#define ECHO 2           // Pin that receives unltrasound ping
#define LED_TYPE NEOPIXEL // Type of LED pixels for FastLED
#define NUM_LEDS 24       // Number of LEDs being used
#define BRIGHTNESS 120    // How brignt will LESs appear.
#define PARKING_DISTANCE 31  // Default parking distance
#define MAX_DISTANCE 75      // Sensor is less precise at longer distances
#define RESET_DISTANCE 5      // Distance that will trigger parking distance setting
#define SAMPLE_READINGS 5     // Number of readings to increase precision
#define TIME_TO_OFF 20000     // How long before LEDs go off

CRGB leds[NUM_LEDS];
elapsedMillis timeElapsed;

int olddistance = 0;
bool parked = false;
NewPing sensor(TRIGGER, ECHO, MAX_DISTANCE);

uint16_t parking_distance = PARKING_DISTANCE;
float readings[SAMPLE_READINGS];

void setup() {
  // Small delay to let eveerything get started
  delay(1000);

  FastLED.addLeds<LED_TYPE,DATA_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  Serial.begin(115200);
  Serial.println("Started");
  led_off();

  parking_distance = 31;

}


void loop() {
  uint16_t distance = 0;

  // No mode sampling approach
  distance = round_to_base(sensor.convert_cm(sensor.ping_median()));
  
  Serial.println(timeElapsed);
  Serial.println(distance);
  
  if (distance <= 1)  // Nothing going on. Show nothing
  {
    led_off();
    timeElapsed = 0;
    parked = false;
  }

  else if (distance <= parking_distance)
  {
      if ((!parked) || (timeElapsed < TIME_TO_OFF)){
        led_stop();
        parked = true;
      }
      else if(parked && (timeElapsed >= TIME_TO_OFF) && leds[0].getLuma())
      {
        led_fade_off(); //If lEDs are on after being parked for a few secs turn off
      }
  }
  else if (distance > parking_distance && distance <= MAX_DISTANCE){
      //identify car not moving (take into consideration inacurracy of sensor)
      if((olddistance > distance-2) && (olddistance < distance+2)){
        parked = true;
      }
      else{   //car is still in motion
        parked = false;
        timeElapsed = 0;
        olddistance = distance;
        led_show_value(distance);
      }
      if(parked=true && (timeElapsed >= TIME_TO_OFF)){  //If not motion for set time turn off leds
        led_fade_off();
      }
        
  }
  delay(5);
}

uint16_t reset_parking_distance(){

  led_countdown();
  uint16_t new_distance = round_to_base(sensor.convert_cm(sensor.ping_median(25)));
  new_distance += 5;  // a little buffer to ensure we're "Red"
  timeElapsed = 0;

  return (new_distance);
}

uint16_t round_to_base(uint16_t value)
{
  uint16_t base = 5;
  return uint16_t(base * round(float(value)/base));
}

