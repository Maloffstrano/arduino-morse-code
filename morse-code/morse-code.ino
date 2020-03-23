#include "MorseCode.h"

// The message to be sent. Must be an ASCII string. Not all characters in 
// ASCII exist in Morse-code. Those that don't exist won't be sent.
String messageToSend = String("OSO ");

// The length of a timing unit in milliseconds. Determines the speed of 
// transmission of the code.
const int morseTimingUnitMs = 150;


const int digisparkModelA = 1;
const int digisparkModelB = 0;

// Choose the model of DigiSpark you have.
const int ledPin = digisparkModelA;




//------------------------------------------------------------------------------

MorseCode morseCode(morseTimingUnitMs);

// setup() routine runs once when you press reset or when you plug a DigiSpark 
// into the USB port.
void setup() {
  morseCode.signalSetup(); // This is not morseCode's concern, to be fixed.
}

// loop() runs repeated after setup.
void loop() {
  // The first cut of code is really dirty. Written as a single linear function.
  int messageLength = messageToSend.length();
  
  for (int characterIndex = 0; characterIndex < messageLength; characterIndex++) {
    char asciiCharacter = messageToSend.charAt(characterIndex);
    morseCode.send(asciiCharacter);
  }
}
