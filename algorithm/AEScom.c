/*
 _______________________________________
/ The best way to success is failing in \
\ the attempt                           /
 ---------------------------------------
   \
    \  .——————.
      .———.    \
     (     )   +——\
      `———´    |  |
      |        |  |
      |   __   +——/
      \__/  \__/
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AEScom.h"

typedef unsigned char byte;

byte sbox[16] = {
    0x9, 0x4, 0x0, 0xD, 
    0x8, 0x5, 0x1, 0xC, 
    0xB, 0x6, 0x2, 0xF, 
    0xA, 0x7, 0x3, 0xE
};

byte inverse_sbox[16] = {
    0x2, 0x6, 0xA, 0xE, 
    0x1, 0x5, 0x9, 0xD, 
    0x4, 0x0, 0xC, 0x8, 
    0x7, 0x3, 0xF, 0xB
};

int permutation1[4] = {2, 0, 3, 1};
int inverse_permutation1[4] = {1, 3, 0, 2};

int permutation2[4] = {3, 2, 0, 1};
int inverse_permutation2[4] = {2, 3, 1, 0};

byte permutation(int* permutation, byte input) {
    byte output = 0;
    for (int i = 0; i < 4; i++) {
        output |= ((input >> (3 - permutation[i])) & 1) << (3 - i);
    }
    return output;
}

byte roundFunction(byte input, byte key) {
    byte tmp = permutation(permutation1, input);
    tmp = sbox[tmp];
    tmp = tmp ^ key;
    tmp = permutation(permutation2, tmp);
    return tmp;
}

byte inverse_roundFunction(byte input, byte key) {
    byte tmp = permutation(inverse_permutation2, input);
    tmp = tmp ^ key;
    tmp = inverse_sbox[tmp];
    tmp = permutation(inverse_permutation1, tmp);
    return tmp;
}

byte spn_network(byte input, byte key) {
    byte right = input & 0x0F;
    byte left = (input >> 4) & 0x0F;
    byte tmp = left ^ right;
    tmp = roundFunction(tmp, key & 0x0F); 
    tmp = left ^ tmp;
    right = left;
    left = tmp;
    tmp = (left << 4) | right;
    return tmp;
}

byte inverse_spn_network(byte input, byte key) {
    byte left = (input >> 4) & 0xF;
    byte right = input & 0xF;
    byte tmp = left ^ right;
    tmp = inverse_roundFunction(tmp, key & 0x0F);
    tmp = right ^ tmp;
    left = right;
    right = tmp;
    return (left << 4) | right;
}

byte key_schedule(byte key, int round) { // Input (n bits key) Output (4 bits key)
 byte rotated_key = ((key << 1) | (key >> 3)) & 0x0F;

    byte round_constant = (round * 0x03) & 0x0F; 
    return rotated_key ^ round_constant;
}

byte encrypt_block(byte input, byte key) {
    byte output = input;
    for (int i = 0; i < 4; i++) {
        output = spn_network(output, key_schedule(key, i));
    }
    return output;
}

byte decrypt_block(byte input, byte key) {
    byte output = input;
    for (int i = 3; i >= 0; i--) {
        output = inverse_spn_network(output, key_schedule(key, i));
    }
    return output;
}
