#include <iostream>
#include <stdint.h>
#include "serializer.h"

#define MYSTRUCT_ID 2
using namespace std;

struct testStruct
{
  char someString[16];
  char character;
  char character2;
  uint32_t number;
  float floatNumber; 
};

int main()
{
  Serializer serializer;

  testStruct myStruct;
  myStruct.floatNumber = 2.3;
  myStruct.number = 0xDEADBEEF;
  myStruct.character = '~';
  myStruct.character2 = '\\';
  strcpy(myStruct.someString, "cutestring");

  serializer.setupEvent(MYSTRUCT_ID, &myStruct, sizeof(testStruct));
  serializer.sendData(MYSTRUCT_ID);
  return 0;
}


