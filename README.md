# Arduino-Garage
Parking distance sensor using ESP8266, HC-SR04, and Addressable LED strips/rings

Credits for the code goes to rvrickv @ https://github.com/rvrickv/Arduino-Garage
Excellent and simple solution for parking sensor!

This fork is updated to work on Arduino Nano and other devices without the EEPROM.

Con: Had to remove ability to change parking distance with re-flashing the arduino
Pro:  Added flashing red light when too close (instead of solid)
      Added extra color to led ring to allow for a parking range not just an exact spot.
      Tweaked to shut off light if in range not just when fully parked. (Keep leds from being on constantly)


That's it! :)

Instructions:
Step 1: First a little measurement and math....Measure the closest distance the vehicle can be to the sensor in cm - this is where it will blink red (in my case 31cm). Then measure the furthest distance where you could park safely (in my case 53cm). Subtract the further from the closest distance (so in my case I will take 53-31=22).  Add that value (22) to furthest value (53 which equals 75). This gives me the distance where the display will show.

Set these variables:
#define PARKING_DISTANCE 31 // Minimum safe distance
#define MAX_DISTANCE 75 // Maximum safe distance

Why? When you hit 75 cm the led light up, notifying the drive that the parking assistant is working. It is showing the leds in yellow to caution that it is not yet safe to park. When you get to the max safe distance (53 in the example) 1/2 the leds will be lit, afterwhich the leds light up green indicating it is safe to park.  The led circle continues growing green until you hit the danger area (31), where it will blink red.

Step 2: Adjust for your number of leds.  I'm assuming a 24 led ring.  Does not need to be NeoPixel brand, I purchased this from Aliexpress for $3  
https://www.aliexpress.com/item/RGB-LED-Ring-24-Bits-LEDs-WS2812-5050-RGB-LED-Ring-Lamp-Light-with-Integrated-Drivers/32835427711.html?spm=2114.search0104.3.186.515c2715uXJ8qU&ws_ab_test=searchweb0_0,searchweb201602_3_10152_10151_10065_10344_10068_5722918_10130_10342_5722818_10547_10343_10340_10548_10341_5722618_10696_10084_10083_10618_10139_10307_5722718_10059_306_100031_10103_5722518_10624_10623_10622_10621_10620,searchweb201603_25,ppcSwitch_4&algo_expid=b3ca5ac4-1904-4a7e-b2d1-7c55427cb2a3-26&algo_pvid=b3ca5ac4-1904-4a7e-b2d1-7c55427cb2a3&priceBeautifyAB=0

If using smaller than 24 leds, adjust #define NUM_LEDS 24  and fill_solid(&leds[0], 24,CRGB::Red); with the appropriate number.

Step 3: Flash
Step 4: Enjoy
