/*
  charly.cpp
*/

// include this library's description file

#include "Charly.h"
//void* operator new(size_t objsize) {
//    return malloc(objsize);
Charly::Charly()
{
  void*p;
  _Ndigits   = 3;
  _Nsegments = 8;
};

Charly::~Charly()
{
};


Charly::Charly(int Ndigits, int Nsegments ) {
  void*p;
  _Ndigits   = Ndigits;
  _Nsegments = Nsegments;
}


int Charly::GetDigit( int _digit) {
  int Retval = 0;
  if (( _digit >= 0) && (_digit < _Ndigits ))
  {
    Retval = pgm_read_byte(&DigitArray[_digit])  ;
  }
  return Retval;
};


int Charly::GetSegment(int value, int segment, int digit) {
  int Retval = 0;
  if ( value >= 0 && value < SegmentArrayLen &&
       segment >= 0 && segment < _Nsegments &&
       digit >= 0 && digit < _Ndigits )
  {
    Retval =  pgm_read_byte(&(SegmentArray[value])); //& ( 1 << segment)  ;
//    Retval =  SegmentArray[value]; //& ( 1 << segment)  ;
  }
  else Retval = 0;
  Retval &= ( 1 << segment)  ;

  return Retval;
};


