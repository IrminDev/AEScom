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

#ifndef OPERATIONMODE_H
#define OPERATIONMODE_H


typedef unsigned char byte;

byte* counter_mode_encrypt(byte* input, byte key);
byte* counter_mode_decrypt(byte* input, byte key);
byte* ebc_mode_encrypt(byte* input, byte key);
byte* ebc_mode_decrypt(byte* input, byte key);


#endif /* OPERATIONMODE_H */