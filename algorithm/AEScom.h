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

#ifndef AESCOM_H
#define AESCOM_H

typedef unsigned char byte;

byte encrypt_block(byte input, byte key);
byte decrypt_block(byte input, byte key);


#endif /* AESCOM_H */