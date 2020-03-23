# arduino-morse-code
Send morse code on the built-in LED of an Arduino.

## What this is about
|My experiment in writing modular code C++ for an Arduino-like chip, the Digispark Tiny85. These are available for less than $2 on some sites, and can be programmed through USB. The one pictured can actually slide directly into a USB-A slot, eliminating a cable.|![DigiSpark Tiny85](https://user-images.githubusercontent.com/6452623/77364177-a189e400-6d19-11ea-997f-fc622738a7b1.jpeg)|
|:---|:---:|





## Code modularity
Follow the commits to see the progression of code from a single, massive, `loop()` function into the modular code it is now.

`MorseCode.cpp` encapsulates the entirety of sending Morse code for a given ASCII character. 
My design philosophy is many small methods that do one thing well. (This improves code testability, although in this case,
I did not write specs to test the methods.) When a comment is needed to describe a line of code,
replace the line by a suitably named method that can eliminate the comment.

`Signal.cpp` encapsulates the logic of sending the the Morse code symbol, decoupling that effort
from the class that wants to send a symbol. `Signal.cpp` has two methods `on` and `off` that sends visually over an LED.
File `morse-code.ino` determines the LED to use; currently choosing the onboard LED.

The idea behind `Signal.cpp` is that if you want to send an audible code, you modify this class, and only this class. 
No need to dig into other working code and possibly break it.

My only disappointment is that `MorseCode` is tightly coupled to `Signal` due to the way C++ behaves. 
My original thought process is that it would be nice to have a `VisualSignal` and `AudibleSignal` class and `MorseCode` could use either.
I come from a dynamic language background: Javascript, Ruby. In those languages if two classes have the same methods,
they share the same implicit interface. In C++, to declare an interface you need to declare an abstract base class. 
Since I wasn't about to implement an audible signalling class, I decided against this extra complexity. If you need to, modify `Signal`
to be audible instead of visual.

## macOS Catalina, the Arduino IDE and the Digispark Board Manager.
From macOS Mojave to Catalina, Apple made the decision to go full 64-bit; 
32-bit applications are no longer supported.
Installing the most recent macOS Arduino (1.8.12 at the time of writing) environment gets over the 64-bit problem.

DigiSpark has its own board manager to be able to program its chips. To install
* Choose _Arduino > Preferences_
* In _Additional Board Manager URLs_ add 
  `https://raw.githubusercontent.com/digistump/arduino-boards-index/master/package_digistump_index.json`
  and click `OK`.
* In _Tools > Board > Boards Manager..._ install _Digistump AVR Boards_. My version, at time of writing, is 1.6.7 and exhibited a 32-bit code problem. Perhaps higher versions are fixed.
* In _Tools > Board_ choose _Digispark (Default - 16.5MHz)_. The value of _Tools > Programmer_ does not matter.
* Now comes the fun part: trying to _Verify_ (compile) code on macOS Catalina will end up with an error message in the Arduino window: 
  `avr-g++: bad CPU type in executable`

  Tracking down the problem on the Arduino forums, apparently Digistump is a _special snowflake_ and decided to 
  ship their own 32-bit tool chain rather than use what is provided by the Arduino application. 
  We need to remove their tool chain and trick them into using the 64-bit Arduino tool change.
* Quit the Arduino IDE: _Arduino > Quit Arduino_.
* Start a `Terminal` application and run these commands
  ```sh
  $ cd ~/Library/Arduino15/packages/arduino/tools/avr-gcc
  # remove the Digistump 32-bit tool chain
  $ rm -rfd 4.8.1-arduino5
  # symlink in the Arduino tool chain
  * $ ln -s /Applications/Arduino.app/Contents/Java/hardware/tools/avr 4.8.1-arduino5
  ```
  See: [reference](https://forum.arduino.cc/index.php?topic=620175.msg4374194#msg4374194)
* Start the Arduino IDE and _Verify_ (compile) should now work.

