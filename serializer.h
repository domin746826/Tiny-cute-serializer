/*
 * TODO make everything non-blocking, add support for variable length functions
 *
 * up to 254 different packets possible (maybe in the future when I add Varint, bigger number of packets could be possible)
 *
 * the format is: 
 * byte 0 - packet ID
 * byte 1 - packet length (because some packets can have variable size)
 * ...
 */

#ifndef _SERIALIZER_H
#define _SERIALIZER_H
#include <cstring>
#include <stdint.h>
#include <stdio.h>

#define BUFFER_LENGTH 255 //max packet size, consider decreasing it on small devices like ATTiny
#define MAX_REGISTERED_PACKETS 16

struct eventsInfo
{
  uint8_t type;
  uint8_t length;
  void* attachedStruct;
};

class Serializer
{
  private:
    eventsInfo *events[MAX_REGISTERED_PACKETS];
    uint8_t receivingBuffer[BUFFER_LENGTH];
    uint8_t sendingBuffer[BUFFER_LENGTH];
    uint8_t currentEventsIndex = 0;
    void (*onReceive)(uint8_t);
    void (*sendChar)(char c);


  public:
    Serializer();
    void sendData(uint8_t structType);
    uint8_t parsePacket(); //
    void parseNonblocking(); //put at the end of main program loop and it will call right function

    void setOnReceiveFunction(void (*func)(uint8_t));
    void setSendCharFunction(void (*func)(char));


    void setupEvent(uint8_t structType, void* structToSet, uint8_t structLength); 

    void processEvents(); //for non-blocking sending and receiving
};

#endif

