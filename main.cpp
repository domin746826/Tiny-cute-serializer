#include <iostream>
#include <stdint.h>
#include "serializer.h"

#define MYSTRUCT_ID 2
using namespace std;

#pragma pack(push, 1)
struct testStruct
{
  char someString[16];
  char character;
  char character2;
  uint32_t number;
  float floatNumber; 
};
#pragma pack(pop)

void onReceive(uint8_t type);
void sendChar(char c);

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
  serializer.setOnReceiveFunction(onReceive);
  serializer.setSendCharFunction(sendChar);
  //serializer.setReceiveCharFunction(receiveChar);
  //serializer.setIsDataAvailableFunction(isDataAvailable);

  serializer.sendData(MYSTRUCT_ID);

  return 0;
}

void sendChar(char c)
{
  cout << "char: " << ((c >= 32 && c <= 127) ? c : ' ') << "\t/\t" <<  std::hex << (unsigned short) (c&0x00ff) << endl;
}

void onReceive(uint8_t type)
{
  return;
}
