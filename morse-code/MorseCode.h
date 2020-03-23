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
    Signal *signal; // The class used to emit a Morse symbol.
        
    const byte symbolDot  = 0;
    const byte symbolDash = 1;
    
    const byte morseCharacterStartingSymbolMask = B10000000;
    const byte asciiCharacterMask = B01111111;
    
    const byte asciiSpace = 32;

    int delayBetweenSymbolsMs; // Calculated on instantiation
    int delayBetweenLettersMs;
    int delayBetweenWordsMs;  
    int morseSymbolDuration[2];

    boolean sendMorseCharacter(byte morseCharacter);
    void sendMorseSymbols(byte morseSymbolMask, byte morseCharacter);
    void sendMorseSymbol(byte morseSymbol);
    byte skipPaddingBits(byte paddedMorseCharacter);
    byte decodeCurrentSymbol(byte morseSymbolMask, byte morseCharacter);
    char ensureLegalAscii(char asciiCharacter);

    static byte asciiToMorseLookup[];
    
};
 
#endif
