#include <assert.h>
#include "bit_map.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>


// returns the number of bytes to store bits booleans
int BitMap_getBytes(int bits){
  return bits/8 + ((bits%8)!=0);
}

// initializes a bitmap on an external array
void BitMap_init(BitMap* bit_map, int num_bits, uint8_t* buffer){
  bit_map->buffer=buffer;
  bit_map->num_bits=num_bits;
  bit_map->buffer_size=BitMap_getBytes(num_bits);
}

// sets a the bit bit_num in the bitmap
// status= 0 or 1
void BitMap_setBit(BitMap* bit_map, int bit_num, int status){
  // get byte
  int byte_num=bit_num>>3;
  assert(byte_num<bit_map->buffer_size);
  int bit_in_byte=bit_num&0x07;
  if (status) {
    bit_map->buffer[byte_num] |= (1<<bit_in_byte);
  } else {
    bit_map->buffer[byte_num] &= ~(1<<bit_in_byte);
  }
}

// inspects the status of the bit bit_num
int BitMap_bit(const BitMap* bit_map, int bit_num){
  //printf("we're now in BitMap_bit funct...\n\n");
  if(bit_num >= bit_map->buffer_size*8){
    printf("max bit number exceeded  %d\n", bit_map->buffer_size*8);
    perror("error bit_map_get");
  }
  int byte_num=bit_num/8; 
  int bit_in_byte=bit_num%8;
  return (bit_map->buffer[byte_num] >> bit_in_byte) & 1;
}
