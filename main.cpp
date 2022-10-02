/*
 * Tiny cute serializer - example
 * to test Serializer::processEvents with testStruct, copy and paste
 * data below in program (terminal) and press enter
02
0d
63
75
74
65
73
74
72
00
7e
ef
be
ad
de
   */

#include <iostream>
#include <stdint.h>
#include "serializer.h"

#define MYSTRUCT_ID 2
using namespace std;

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
bool dataAvailable_debug = false;

testStruct myStruct;
int main()
{
  Serializer serializer;

  /*myStruct.number = 0xDEADBEEF;
  myStruct.character = '~';
  strcpy(myStruct.someString, "cutestr");*/

  serializer.setupEvent(MYSTRUCT_ID, &myStruct, sizeof(testStruct));
  serializer.setOnReceiveFunction(onReceive);
  serializer.setSendCharFunction(sendChar);
  serializer.setReceiveCharFunction(receiveChar);
  serializer.setIsDataAvailableFunction(isDataAvailable);

  //serializer.sendPacket(MYSTRUCT_ID);

  /*serializer.parsePacket();

  cout << endl << "DATA: " << endl;
  cout << "myStruct.someString: " << myStruct.someString << endl;
  cout << "myStruct.number: " << hex << myStruct.number << endl;
  cout << "myStruct.character: " << myStruct.character << endl;*/

  //cout << receiveChar() << endl;
  

  while(1) //for testing only
  {
    dataAvailable_debug = true;
    serializer.processEvents();
    cout << "loopIteration" << endl;
  }

  return 0;
}

void sendChar(char c)
{
  cout << "char: " << ((c >= 32 && c <= 127) ? c : ' ') << "\t/\t" <<  std::hex << (unsigned short) (c&0x00ff) << endl;
}

void onReceive(uint8_t type)
{
  cout << "received: " << hex << type << endl;
  cout << endl << "DATA: " << endl;
  cout << "myStruct.someString: " << myStruct.someString << endl;
  cout << "myStruct.number: " << hex << myStruct.number << endl;
  cout << "myStruct.character: " << myStruct.character << endl;
  cout << "loopIteration" << endl;
  return;
}

char receiveChar(void)
{
  char str[2] = {0}; //get 8 bit hex value (case insensitive, older byte first, for debug purposes)
  cin >> str;
  if(str[0] >= 'a')
    str[0] = str[0] - 'a' + 10;
  else if(str[0] >= 'A')
    str[0] = str[0] - 'A' + 10;
  else
    str[0] -= '0';

  if(str[1] >= 'a')
    str[1] = str[1] - 'a' + 10;
  else if(str[1] >= 'A')
    str[1] = str[1] - 'A' + 10;
  else
    str[1] -= '0';


  return str[0]*16+str[1];
}

bool isDataAvailable(void)
{
  if(dataAvailable_debug)
  {
    dataAvailable_debug = false;
    return true;
  }
  else
    return false;
  //return true;
}
