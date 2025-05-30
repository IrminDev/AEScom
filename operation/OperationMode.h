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
byte* counter_mode_decrypt(byte* input, byte key, size_t input_len);
byte* ebc_mode_encrypt(byte* input, byte key);
byte* ebc_mode_decrypt(byte* input, byte key, size_t input_len);
byte* cbc_mode_encrypt(byte* input, byte key, byte* iv);
byte* cbc_mode_decrypt(byte* input, byte key, size_t input_len);


#endif /* OPERATIONMODE_H */