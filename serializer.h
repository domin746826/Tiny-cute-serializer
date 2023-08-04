/*
 * TODO make everything non-blocking, add support for variable length functions
 *
 * up to 254 different packets possible (maybe in the future when I add Varint, bigger number of packets could be possible)
 * packet ID 0 is "bad packet" and is returned when something went wrong
 *
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

#define BUFFER_LENGTH 64 //max packet size, consider decreasing it on small devices like ATTiny
#define MAX_REGISTERED_PACKETS 16 //used space on Arduino, AVR is 4 bytes per one registered packet

struct eventInfo
{
  uint8_t type;
  uint8_t length;
  void* attachedStruct;
};

class Serializer
{
  private:
    eventInfo *badEvent;
    eventInfo *events[MAX_REGISTERED_PACKETS];
    uint8_t receivingBuffer[BUFFER_LENGTH];
    uint8_t sendingBuffer[BUFFER_LENGTH];
    uint8_t currentEventsIndex = 0;
    void (*onReceive)(uint8_t);

    char sendTrigger;
    char receiveTrigger;
    
    void (*sendChar)(char c);
    char (*receiveChar)(void);
    bool (*isDataAvailable)(void);

    bool isReceivingPacket = false;
    uint8_t currentlyReceivedPacketId = 0;
    uint8_t receivedPacketSize = 0;
    uint8_t receivedPacketPos = 0;

    char lastReceivedChar = 0;
    bool receivingHigher = true;


    uint8_t hexToNum(char c);
    char numToHexHigher(uint8_t num);
    char numToHexLower(uint8_t num);

  public:
    Serializer();
    void sendPacket(uint8_t structType);
    uint8_t parsePacket(); 

    void setOnReceiveFunction(void (*func)(uint8_t));
    void setSendCharFunction(void (*func)(char));
    void setReceiveCharFunction(char (*func)(void));
    void setIsDataAvailableFunction(bool (*func)(void));


    void setupEvent(uint8_t structType, void* structToSet, uint8_t structLength); 

    void processEvents(); //for non-blocking sending and receiving
                          
    void setTriggerChars(char sendChar, char receiveChar);
};

#endif

