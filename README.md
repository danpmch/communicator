
# Communicator

This project contains the Arduino IDE project for an encrypted
[LoRa](https://en.wikipedia.org/wiki/LoRa) texting device. It's like a walkie
talkie but for texting.

## Hardware

The project runs on an [Adafruit Feather with LoRa 900MHz
chip](https://learn.adafruit.com/adafruit-feather-m0-radio-with-lora-radio-module).
The touchscreen is the [FeatherWing 3.5" TFT
V2](https://www.adafruit.com/product/3651) with resistive touch. So far all
testing has been done using a [simple wire spring
antenna](https://www.adafruit.com/product/4269), which has yielded surprisingly
good results.

### Stylus

The touch screen has proven to be so small and the touch screen sufficiently
imprecise that it requires a stylus to work the software keyboard effectively.
Since the screen is resistive rather than capacitive pretty much any soft, blunt
but pointed object will work. I've been using a wooden hairpin.

## Performance

Testing on the lowest transmit power allowed effective communication across a
small park and corner to corner arcoss a city residential block (standalone
residential houses, not apartment buildings). Under moderate power we were able
to exchange line of sight messages over about 2,000ft, with max power a mile
should be possible.

The signal was effectively blocked by a large warehouse however, so performance
may be quite mixed downtown among office buildings. Further testing will be
required once I have the boards mounted more discretely in a proper case, so
that we don't make such a spectacle of ourselves while testing.

## Battery life

Battery life on a 400mAh battery is 1-2 hrs. The current code is basically as
inefficient with power is possible however, so there should hopefully be a lot
of room for improvement here and low hanging fruit.

## Building the Code

In order for the code to compile and upload to the board you need to have
several additional packages installed in the Arduino IDE.

### Board

The board definition comes from Adafruit, you need to add their board definition
URL in the IDE's Board Manager under "Additional Board Manager URLs". The URL is:

https://adafruit.github.io/arduino-board-index/package_adafruit_index.json

Once that's updated you can search the board manager for SAMD and install
"Adafruit SAMD Boards" by Adafruit.

### Libraries

The project requires additional libraries that can be installed via the Library
Manager:

* RadioHead - For operating the LoRa chip
* Crypto - For encrypting/decrypting messages
* Adafruit HX8357 - For the 3.5" display
* Adafruit GFX - Core graphics for Adafruit displays
* Adafruit TSC2007 - For the touch screen

Once these are installed the project should build and upload to the board successfully.

## TODOs

* Figure out how to 3D print a case
* Test range in urban environment
* Test range with better antenna
* Create script to efficiently set up multiple devices with the same key and
  different participant IDs
* Lower power consumption
* Switch to a hardware keyboard, like a Blackberry
