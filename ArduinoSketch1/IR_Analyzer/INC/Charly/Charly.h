/*
  Charly.h
*/

// ensure this library description is only included once
#ifndef Charly_h
#define Charly_h
//#include <avr/pgmspace.h>
#include <Arduino.h>   //Arduino IDE >= V1.0

// Seven-segment definitions
const int SegmentArrayLen = 12;
char  const PROGMEM SegmentArray[]  = {
//char  const SegmentArray[]  = {
  //  ABCDEFG  Segments
  0b1111110, // 0
  0b0110000, // 1
  0b1101101, // 2
  0b1111001, // 3
  0b0110011, // 4
  0b1011011, // 5
  0b1011111, // 6
  0b1110000, // 7
  0b1111111, // 8
  0b1111011, // 9
  0b0000000, // 10  Space
  0b0000001  // 11  Dash
};

const char DigitArrayLen = 3;
const char  PROGMEM DigitArray[] = {
  0b00000001, // 0
  0b00000010, // 1
  0b00000100  // 2
};
const int Dash = 11;
const int Space = 10;



// library interface description
class Charly	
{
  public:
    Charly();
    ~Charly();
    Charly(int Ndigits, int Nsegments );
    int GetSegment(int value, int segment, int digit);
    int GetDigit  (int digit);
  private:
    int _Ndigits;
    int _Nsegments ;
};
#endif





