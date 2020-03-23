#ifndef MORSE_CODE_H
#define MORSE_CODE_H
 
#include <Arduino.h>
 
class MorseCode {
  public:
    MorseCode(int morseTimingUnitMs);
    ~MorseCode();
    void send(char asciiCharacter);

    // THESE TO BE MOVED TO ANOTHER CLASS
    void signalOn();
    void signalOff();
    void signalSetup();

  private:
    const int ledPin = 1;
    
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
