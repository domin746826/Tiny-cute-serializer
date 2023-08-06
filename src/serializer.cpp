#include "serializer.h"

#ifdef __AVR__
#include <string.h>
#else
#include <cstring>
#endif


Serializer::Serializer()
{
  badEvent = new eventInfo;
  badEvent->type = 0;

  for(uint8_t i = 0; i < MAX_REGISTERED_PACKETS; i++)
    events[i] = badEvent;
}


void Serializer::sendPacket(uint8_t structType) //can be blocking, depends on implementation of sendChar function
{
  for(int i = 0; i < currentEventsIndex; i++)
  {
    if(events[i]->type == structType)
    {
      memcpy(sendingBuffer, events[i]->attachedStruct, events[i]->length);
      sendChar(sendTrigger);
      sendChar(numToHexHigher(structType));
      sendChar(numToHexLower(structType));
      sendChar(numToHexHigher(events[i]->length));
      sendChar(numToHexLower(events[i]->length));
      for(int i2 = 0; i2 < events[i]->length; i2++)
      {
        sendChar(numToHexHigher(sendingBuffer[i2]));
        sendChar(numToHexLower(sendingBuffer[i2]));
      }
      return;
    }
  }
  return;
}

uint8_t Serializer::parsePacket() //TODO rewrite, do not use until rewrite is complete
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

//put at the end of main loop of your program and it will call functions specified by setIsDataAvailableFunction when something was received
void Serializer::processEvents()
{
  while(isDataAvailable())
  {
    char c = receiveChar();
    if(c == receiveTrigger) //clear everything
    {
      currentlyReceivedPacketId = 0;
      receivingHigher = true;
      lastReceivedChar = c;
      receivedPacketPos = 0;
      isReceivingPacket = false;
      receivedPacketSize = 0;
      return;
    }
    if(!isReceivingPacket) //TODO check if ID equals 0 and dump packet in that case
    { 
      if(currentlyReceivedPacketId == 0)
      {
        if(receivingHigher)
          receivingHigher = false;
        else
        {
          currentlyReceivedPacketId = hexToNum(lastReceivedChar) << 4 | hexToNum(c);
          receivingHigher = true;
        }
      }
      else 
      {
        if(receivingHigher)
          receivingHigher = false;
        else
        {
          //setup for receiving
          receivingHigher = true;
          receivedPacketSize = hexToNum(lastReceivedChar) << 4 | hexToNum(c);
          isReceivingPacket = true;
          
          for(uint8_t i = 0; i < currentEventsIndex; i++) //find matching packet
            if(events[i]->type == currentlyReceivedPacketId)
              if(receivedPacketSize > events[i]->length)
                receivedPacketSize = events[i]->length;
        } 
      }
      lastReceivedChar = c;
    }
    else
    {
      if(receivingHigher)
        receivingHigher = false;
      else
      {
        receivingHigher = true;
        receivingBuffer[receivedPacketPos++] = hexToNum(lastReceivedChar) << 4 | hexToNum(c);
      }
      
      lastReceivedChar = c; 
      if(receivedPacketPos == receivedPacketSize && receivingHigher) 
      {
        for(uint8_t i = 0; i < currentEventsIndex; i++) //find matching packet
          if(events[i]->type == currentlyReceivedPacketId)
          {
            if(receivedPacketPos > events[i]->length)
              receivedPacketPos = events[i]->length;
            memcpy(events[i]->attachedStruct, receivingBuffer, receivedPacketPos); //write received data to struct 
            onReceive(events[i]->type);
            break;
          } 

        receivedPacketPos = 0;
        receivedPacketSize = 0;
        currentlyReceivedPacketId = 0;
        isReceivingPacket = false;
      }  
    }
  }
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

void Serializer::setSendCharFunction(void (*func)(char))
{
  sendChar = func;
}


uint8_t Serializer::hexToNum(char c)
{
  if(c >= '0' && c <= '9')
    return c - '0';
  else if(c >= 'a' && c <= 'f')
    return c - 'W'; //because 0xa is 10 and 'W' is 10 places before the 'a' letter
  else
    return 0;
}


char Serializer::numToHexHigher(uint8_t num)
{
  num = (num & 0xF0) >> 4;
  if(num < 10)
    return num + '0';
  else
    return num + 'W';
}

char Serializer::numToHexLower(uint8_t num)
{
  num &= 0x0F;
  if(num < 10)
    return num + '0';
  else
    return num + 'W';
}


void Serializer::setTriggerChars(char sendChar, char receiveChar)
{
  sendTrigger = sendChar;
  receiveTrigger = receiveChar;
}
