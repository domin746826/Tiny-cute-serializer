#include "serializer.h"


Serializer::Serializer()
{
}

void Serializer::sendData(uint8_t structType)
{
}

uint8_t Serializer::parsePacket()
{
}

void Serializer::parseNonblocking()
{
}

void setupEvent(uint8_t structType, void* structToSet, uint8_t structLength)
{
}

void setFunctionToEvent(uint8_t structType, void (*onReceive)(uint8_t))
{
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
