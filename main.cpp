/*
 * Tiny cute serializer - example
 * to test Serializer::processEvents with testStruct, copy and paste
 * data below in program (terminal) and press enter
 *
:
0
2
0
d
6
3
7
5
7
4
6
5
7
3
7
4
7
2
0
0
7
e
e
f
b
e
a
d
d
e

   */

#include <cstdio>
#include <iostream>
#include <stdint.h>
#include "serializer.h"

using namespace std;

#define MYSTRUCT_ID 2
#pragma pack(push, 1)
struct testStruct
{
  char someString[8];
  char character;
  uint32_t number;
};
#pragma pack(pop)


void onReceive(uint8_t type);
void sendChar(char c);
char receiveChar(void);
bool isDataAvailable(void);
testStruct myStruct;


bool dataAvailable_debug = false;

int main()
{
  Serializer serializer;
  serializer.setTriggerChars(':', ':');
 
  serializer.setupEvent(MYSTRUCT_ID, &myStruct, sizeof(testStruct));
  serializer.setOnReceiveFunction(onReceive);
  serializer.setSendCharFunction(sendChar);
  serializer.setReceiveCharFunction(receiveChar);
  serializer.setIsDataAvailableFunction(isDataAvailable);

  myStruct.number = 0xDEADBEEF;
  myStruct.character = '~';
  strcpy(myStruct.someString, "cutestr");
  serializer.sendPacket(MYSTRUCT_ID);
  cout << endl;


  cout << endl << "DATA: " << endl;
  cout << "myStruct.someString: " << myStruct.someString << endl;
  cout << "myStruct.number: " << hex << myStruct.number << endl;
  cout << "myStruct.character: " << myStruct.character << endl;

  
  while(1) 
  {
    dataAvailable_debug = true;
    serializer.processEvents();
    cout << "loopIteration" << endl;
  }

  return 0;
}

//These are four functions which you have to implement by yourself 

void sendChar(char c) //should send character (for example in Arduino Serial.write())
{
  cout << c;
}

void onReceive(uint8_t type) //It's similar to event system
{
  cout << "received packet with ID: " << hex << type << endl;
  switch(type)
  {
    case MYSTRUCT_ID:
      cout << "It's myStruct! Received data below:" << endl;
      cout << "myStruct.someString: " << myStruct.someString << endl;
      cout << "myStruct.number: " << hex << myStruct.number << endl;
      cout << "myStruct.character: " << myStruct.character << endl;
      break;

    default:
      cout << "Unknown packet" << endl;
  }
  return;
}

char receiveChar(void) //should return received character (for example in Arduino Serial.read())
{
  char c[8] = {0};
  cin >> c;

  return c[0]; 
}

bool isDataAvailable(void) //should return true if there's available data to read (for example in Arduino Serial.available())
{
  if(dataAvailable_debug)
  {
    dataAvailable_debug = false;
    return true;
  }
  else
    return false;
}
