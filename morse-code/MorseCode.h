/*
  MorseCode.h - Library for signaling Morse code.
  Created by Maloffstrano, March 2020.
  Released into the public domain.
*/

#ifndef MorseCode_H
#define MorseCode_H

#include "signal.h"
#include <Arduino.h>
 
class MorseCode {
  public:
    MorseCode(int morseTimingUnitMs, Signal *signaller);
    ~MorseCode();
    void send(char asciiCharacter);

  private:
    Signal *signal;
        
    const byte symbolDot  = 0;
    const byte symbolDash = 1;
    
    const byte morseCharacterSymbolMask = B10000000;
    const byte asciiCharacterMask = B01111111;
    
    const byte numberOfBitsPerByte = 8;

    const byte asciiSpace = 32;

    int delayBetweenSymbolsMs;
    int delayBetweenLettersMs;
    int delayBetweenWordsMs;  
    int symbolDurationDotMs;
    int symbolDurationDashMs;

    static byte asciiToMorseLookup[];

    boolean sendMorse(byte morseCharacter);
    void sendMorseSymbol(byte morseSymbol);
    int morseSymbolDuration(byte morseSymbol);
    byte identifyMorseStartingSymbol(byte paddedMorseCharacter);
    byte nextMorseSymbol(byte *morseCharacter);

};
 
#endif
