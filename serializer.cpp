#include "serializer.h"
#include <iostream>
using namespace std;


Serializer::Serializer()
{
  badEvent = new eventInfo;
  badEvent->type = 0;

  for(uint8_t i = 0; i < MAX_REGISTERED_PACKETS; i++)
    events[i] = badEvent;
}


void Serializer::sendPacket(uint8_t structType)
{
  for(int i = 0; i < currentEventsIndex; i++)
  {
    if(events[i]->type == structType)
    {
      memcpy(sendingBuffer, events[i]->attachedStruct, events[i]->length);
      sendChar(structType);
      sendChar(events[i]->length);
      for(int i2 = 0; i2 < events[i]->length; i2++)
        sendChar(sendingBuffer[i2]);
      return;
    }
  }
  return;
}

uint8_t Serializer::parsePacket() //blocking way
{
  while(!isDataAvailable()) {}
  char packetId = receiveChar();
  while(!isDataAvailable()) {}
  char packetLength = receiveChar();

  for(int i = 0; i < packetLength; i++)
  {
    while(!isDataAvailable()) {}
    receivingBuffer[i] = receiveChar();
  }
  
  for(uint8_t i = 0; i < currentEventsIndex; i++)
    if(events[i]->type == packetId)
    {
      if(packetLength > events[i]->length)
        packetLength = events[i]->length;
      memcpy(events[i]->attachedStruct, receivingBuffer, packetLength); //write received data to struct
      return packetId;
    }


  return 0;
}

void Serializer::setSendCharFunction(void (*func)(char))
{
  sendChar = func;
}

void Serializer::parseNonblocking()
{
}

void Serializer::setupEvent(uint8_t structType, void* structToSet, uint8_t structLength)
{
  if(currentEventsIndex >= 16)
    return;

  events[currentEventsIndex] = new eventInfo;
  events[currentEventsIndex]->type = structType;
  events[currentEventsIndex]->length = structLength;
  events[currentEventsIndex]->attachedStruct = structToSet;  
  currentEventsIndex++;
}



void Serializer::setOnReceiveFunction(void (*func)(uint8_t))
{
  onReceive = func;
}

void Serializer::setReceiveCharFunction(char (*func)(void))
{
  receiveChar = func;
}

void Serializer::setIsDataAvailableFunction(bool (*func)(void))
{
  isDataAvailable = func;
}

/*struct receiver //these structs are from my drone code and I am turning this into library
{
  uint8_t power;
  int8_t rotate ;
  int8_t fb; //forward backward
  int8_t lr; //left right
  float alignment;
  float alignment2;
};

struct gyro
{
  float x = 0;
  float y = 0;
  float z = 0;
};

receiver received;


uint8_t packetType = 0;
uint8_t buffer[64] = {0};
uint8_t bufferPosition = 0;
void parsePacket()
{
  if(!Serial.available())
    return;
  char c = Serial.read();
  char c;

  switch(packetType)
  {
    case 0:
      packetType = c;
      break;    
      
    case PACKET_RECEIVER:
    {
      if(bufferPosition != sizeof(received))
        buffer[bufferPosition++] = c;
      else
      {
        memcpy(&received, &buffer, bufferPosition);
        bufferPosition = 0;
        packetType = 0;
      }        
    }
    break;          
  }
}

*/
