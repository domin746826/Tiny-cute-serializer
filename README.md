# Tiny-cute-serializer 
Simple and reliable data serializer dedicated for 8 bit microcontrollers to use with UART/SPI etc.


## Features
- auto detection of packet types
- driven by events
- 

## Usage
Example usage is shown in main.cpp file. \
Build using:
```bash
make
```

#### Example data packet
For struct
```cpp
#pragma pack(push, 1)
struct testStruct
{
  char someString[8];
  char character;
  uint32_t number;
};
#pragma pack(pop)


testStruct myStruct;
strcpy(myStruct.someString, "cutestr");
myStruct.number =  0xdeadbeef;
myStruct.character = '~';
```
packet should be:
``:020d63757465737472007eefbeadde`` \
I'm using ``:`` as start of packet and data is sent in hexadecimal format but by ASCII (which makes it 2 times slower than raw format) but it's still faster and simpler than sending data by JSON.
It's important to put these ``#pragma``s because it forces struct to generate same results on diifferent hardware which is important when using my library to communicate PC and embdedded stuff.

### TODO 
- [ ] move ASCII style packet functions to core of the library (almost done except parsePacket)
- [ ] add examples
- [ ] add headers and modify makefile to generate library file
- [ ] check for bugs
- [ ] optimize for memory usage

