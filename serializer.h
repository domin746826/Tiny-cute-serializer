/*
 * TODO make changing send/receive functions possible, make everything non-blocking
 *
 *
 */

#ifndef _SERIALIZER_H
#define _SERIALIZER_H
#include <cstring>
#include <stdint.h>
#include <stdio.h>

#define BUFFER_LENGTH 255 //max packet size, consider decreasing it on small devices like ATTiny
#define MAX_REGISTERED_PACKETS 16

#define PACKET_RECEIVER 1
#define PACKET_GYRO 2

struct eventsInfo
{
  uint8_t type;
  uint8_t length;
};

class Serializer
{
  private:
    eventsInfo *events[MAX_REGISTERED_PACKETS];
    uint8_t receivingBuffer[BUFFER_LENGTH];
    uint8_t sendingBuffer[BUFFER_LENGTH];

  public:
    Serializer();
    sendData();

};

#endif
