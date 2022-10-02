#include "serializer.h"


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

//put at the end of main and it will call function specified by setIsDataAvailableFunction when something was received
void Serializer::processEvents()
{
  while(isDataAvailable())
  {
    if(!isReceivingPacket) //TODO check if ID equals 0 and dump packet in that case
    {
      if(currentlyReceivedPacketId == 0)
        currentlyReceivedPacketId = receiveChar(); 
      else 
      {
        //setup for receiving
        receivedPacketSize = receiveChar();
        isReceivingPacket = true;
        
      }
    }
    else
    {
      receivingBuffer[receivedPacketPos++] = receiveChar();
      
      if(receivedPacketPos == receivedPacketSize)
      {
        for(uint8_t i = 0; i < currentEventsIndex; i++)        
          if(events[i]->type == currentlyReceivedPacketId)
          {
            if(receivedPacketPos > events[i]->length)
              receivedPacketPos = events[i]->length;
            memcpy(events[i]->attachedStruct, receivingBuffer, receivedPacketPos); //write received data to struct
            receivedPacketPos = 0;
            receivedPacketSize = 0;
            currentlyReceivedPacketId = 0;
            isReceivingPacket = false;
            onReceive(events[i]->type);
            return;
          }

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

