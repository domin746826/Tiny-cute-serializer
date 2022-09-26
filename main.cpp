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
char receiveChar(void);
bool isDataAvailable(void);

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
  serializer.setReceiveCharFunction(receiveChar);
  serializer.setIsDataAvailableFunction(isDataAvailable);

   serializer.sendPacket(MYSTRUCT_ID);
  // serializer.parsePacket();

  // cout << myStruct.someString << endl;
  // cout << myStruct.floatNumber << endl;

  //cout << receiveChar() << endl;

  return 0;
}

void sendChar(char c)
{
  cout << "char: " << ((c >= 32 && c <= 127) ? c : ' ') << "\t/\t" <<  std::hex << (unsigned short) (c&0x00ff) << endl;
}

void onReceive(uint8_t type)
{
  cout << "received: " << type << endl;
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
  return true;
}
