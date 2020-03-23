/*
  MorseCode.cpp - Library for signaling Morse code.
  Created by Maloffstrano, March 2020.
  Released into the public domain.
*/

#include "MorseCode.h"

// Construct a Morse code class that will send Morse code characters for
// ASCII characters based on the timing unit specified, in milliseconds.
MorseCode::MorseCode(int morseTimingUnitMs, Signal *signaller) { 
  // Morse code timing.
  // See: http://www.codebug.org.uk/learn/step/541/morse-code-timing-rules/
  delayBetweenSymbolsMs = morseTimingUnitMs;
  delayBetweenLettersMs = 3 * morseTimingUnitMs;
  delayBetweenWordsMs   = 7 * morseTimingUnitMs;

  // While learning Morse code, uncomment this line to put space between the symbols.
  // delayBetweenSymbolsMs = 2 * morseTimingUnitMs; 

  morseSymbolDuration[symbolDot] = morseTimingUnitMs;
  morseSymbolDuration[symbolDash] = 3 * morseTimingUnitMs;

  signal = signaller;
}

MorseCode::~MorseCode() { /* nothing to destruct */ }

// Send the asciiCharacter to the signaller as morse code.
void MorseCode::send(char asciiCharacter) {
  asciiCharacter = ensureLegalAscii(asciiCharacter);

  if (asciiCharacter == asciiSpace) {
    delay(delayBetweenWordsMs);
  } else {
    byte morseCharacter = asciiToMorseLookup[asciiCharacter];
    sendMorseCharacter(morseCharacter);
  }
}

char MorseCode::ensureLegalAscii(char asciiCharacter) {
  return asciiCharacter & asciiCharacterMask;
}

// Sends the morse code character. Returns true if the Morse character was sent
// or false when there is no equivalent Morse character.
//
// The character is encoded in a single byte with 0 representing a dot and 1
// representing a dash. Morse code is variable length. To properly encode a
// variable length character with only two unique values, 0 & 1, the Morse code 
// is padded with an opposite bit to that of the first symbol bit. 
//
// Two examples:
//
// Morse letter A is dot-dash (.-) represented as bits 01. The first symbol is
// a dot (0) so the remainder of the byte is padded with 1 to indicate
// discard those bits until a 0 (dot) is found. Send the remaining symbols.
//
// Morse letter B is dash-dot-dot-dot (-...) represented as bits 1000. The first
// symbol is dash (1) so the remainder of the byte is padded with 0 to indicate
// discard those bits until a 1 (dash) is found. Send the remaining symbols.
//
// Bit-76543210                            Bit-76543210
//     ------01 Morse letter A .-              ----1000 Morse letter B -...
//     11111101 padded Morse letter A          00001000 padded Morse letter B

boolean MorseCode::sendMorseCharacter(byte morseCharacter) {
  byte morseSymbolMask = skipPaddingBits(morseCharacter);

  if (morseSymbolMask == 0) {
    return false;
  }

  sendMorseSymbols(morseSymbolMask, morseCharacter);

  return true;
}

// Skips over the padding bits of paddedMorseCharacter and returns a 
// mask with a lone set bit positioned at the first symbol of the 
// Morse character to send.
byte MorseCode::skipPaddingBits(byte paddedMorseCharacter) {
  byte morseSymbolMask = morseCharacterStartingSymbolMask;
  byte paddingSymbol = decodeCurrentSymbol(morseSymbolMask, paddedMorseCharacter);

  // Loop ends when the lone mask bit is shifted out and value falls to zero.
  while (morseSymbolMask) {
    morseSymbolMask = morseSymbolMask >> 1;
    byte currentSymbol = decodeCurrentSymbol(morseSymbolMask, paddedMorseCharacter);
    if (paddingSymbol != currentSymbol) {
      break;
    }
  }

  return morseSymbolMask;
}


// Sends all the Morse symbols of morseCharacter starting at the 
// current morseSymbolMask. The mask is a single set bit identifying 
// the next Morse symbol to send.
void MorseCode::sendMorseSymbols(byte morseSymbolMask, byte morseCharacter) {
  // Loop ends when the lone mask bit is shifted out and value falls to zero.
  while (morseSymbolMask) {
    byte currentSymbol = decodeCurrentSymbol(morseSymbolMask, morseCharacter);
    sendMorseSymbol(currentSymbol);
    morseSymbolMask = morseSymbolMask >> 1;
  }
}

// Returns either symbolDash or symbolDot depending on the 
// morseCharacter symbol referenced by the morseSymbolMask.
byte MorseCode::decodeCurrentSymbol(byte morseSymbolMask, byte morseCharacter) {
  return ((morseCharacter & morseSymbolMask) ? symbolDash : symbolDot);
}

// Sends a Morse symbol via the current signaller.
void MorseCode::sendMorseSymbol(byte morseSymbol) {
  signal->on();
  delay(morseSymbolDuration[morseSymbol]);
  signal->off();
  delay(delayBetweenSymbolsMs);  
}

// Array that maps the first 128 ASCII characters to their Morse code
// equivalents. Number 0 indicates no equivalent and will not be sent.
//
// See: https://morsecode0world/international/morse20html
// See: sendMorse() method for an explanation of the bit format

byte MorseCode::asciiToMorseLookup[] {
// 76543210 --- bit position                                        
  B0,        // Null char - NUL 
  B0,        // Start of Heading - SOH 
  B0,        // Start of Text - STX 
  B0,        // End of Text - ETX 
  B11000101, // control_SK_end_transmission - End of Transmission - EOT
  B0,        // Enquiry - ENQ 
  B0,        // Acknowledgment - ACK 
  B0,        // Bell - BEL 
  B0,        // Back Space - BS  
  B0,        // Horizontal Tab - HT  
  B11110101, // control_AA_new_line - Line Feed - LF  
  B00010001, // control_BT_new_paragraph - Vertical Tab - VT  
  B0,        // Form Feed - FF  
  B0,        // Carriage Return - CR  
  B0,        // Shift Out / X-On - SO  
  B0,        // Shift In / X-Off - SI  
  B0,        // Data Line Escape - DLE 
  B0,        // Device Control 1 (oft. XON) - DC1 
  B0,        // Device Control 2 - DC2 
  B0,        // Device Control 3 (oft. XOFF) - DC3 
  B0,        // Device Control 4 - DC4 
  B0,        // Negative Acknowledgement - NAK 
  B0,        // Synchronous Idle - SYN 
  B11101010, // control_AR_end_message - End of Transmit Block - ETB 
  B0,        // Cancel - CAN 
  B0,        // End of Medium - EM  
  B0,        // Substitute - SUB 
  B0,        // Escape - ESC 
  B0,        // File Separator - FS  
  B0,        // Group Separator - GS  
  B0,        // Record Separator - RS  
  B0,        // Unit Separator - US  
  B0,        // punctuation_space
  B00101011, // punctuation_exclamation  
  B11010010, // punctuation_quote        
  B0,        // punctuation_hash         
  B0,        // punctuation_dollar       
  B0,        // punctuation_percent      
  B11101000, // punctuation_ampersand    
  B11011110, // punctuation_apostrophe   
  B00010110, // punctuation_open_paren   
  B00101101, // punctuation_close_paren  
  B0,        // punctuation_asterisk     
  B11101010, // punctuation_plus         
  B00110011, // punctuation_comma        
  B00100001, // punctuation_hyphen       
  B11010101, // punctuation_full_stop    
  B00010010, // punctuation_slash        
  B00011111, // digit_0                  
  B11101111, // digit_1                  
  B11100111, // digit_2                  
  B11100011, // digit_3                  
  B11100001, // digit_4                  
  B11100000, // digit_5                  
  B00010000, // digit_6                  
  B00011000, // digit_7                  
  B00011100, // digit_8                  
  B00011110, // digit_9                  
  B00111000, // punctuation_colon        
  B0,        // punctuation_semicolon    
  B0,        // punctuation_less_than    
  B00010001, // punctuation_equals       
  B0,        // punctuation_greater_than 
  B11001100, // punctuation_question_mark
  B11011010, // punctuation_at_sign      
  B11111101, // letter_A                 
  B00001000, // letter_B                 
  B00001010, // letter_C                 
  B00000100, // letter_D                 
  B11111110, // letter_E                 
  B11110010, // letter_F                 
  B00000110, // letter_G                 
  B11110000, // letter_H                 
  B11111100, // letter_I                 
  B11110111, // letter_J                 
  B00000101, // letter_K                 
  B11110100, // letter_L                 
  B00000011, // letter_M                 
  B00000010, // letter_N                 
  B00000111, // letter_O                 
  B11110110, // letter_P                 
  B00001101, // letter_Q                 
  B11111010, // letter_R                 
  B11111000, // letter_S                 
  B00000001, // letter_T                 
  B11111001, // letter_U                 
  B11110001, // letter_V                 
  B11111011, // letter_W                 
  B00001001, // letter_X                 
  B00001011, // letter_Y                 
  B00001100, // letter_Z                 
  B11110101, // control_AA_new_line - Opening bracket
  B11101010, // control_AR_end_message - Backslash
  B11101000, // control_AS_wait - Closing bracket
  B01000101, // control_BK_break - Caret - circumflex
  B00010001, // control_BT_new_paragraph - Underscore
  B11000101, // control_SK_end_transmission - Grave accent
  B11111101, // letter_A                 
  B00001000, // letter_B                 
  B00001010, // letter_C                 
  B00000100, // letter_D                 
  B11111110, // letter_E                 
  B11110010, // letter_F                 
  B00000110, // letter_G                 
  B11110000, // letter_H                 
  B11111100, // letter_I                 
  B11110111, // letter_J                 
  B00000101, // letter_K                 
  B11110100, // letter_L                 
  B00000011, // letter_M                 
  B00000010, // letter_N                 
  B00000111, // letter_O                 
  B11110110, // letter_P                 
  B00001101, // letter_Q                 
  B11111010, // letter_R                 
  B11111000, // letter_S                 
  B00000001, // letter_T                 
  B11111001, // letter_U                 
  B11110001, // letter_V                 
  B11111011, // letter_W                 
  B00001001, // letter_X                 
  B00001011, // letter_Y                 
  B00001100, // letter_Z  
  B0,        // punctuation_opening_brace
  B0,        // punctuation_vertical_bar
  B0,        // punctuation_closing_brace
  B0,        // punctuation_equivalency_sign - tilde
  B0         // control_DEL_delete  
};  
