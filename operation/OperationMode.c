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
#include <time.h>
#include "../algorithm/AEScom.h"
#include "OperationMode.h"

typedef unsigned char byte;

byte counter_mode_encrypt_operation(byte input, byte cnt, byte key){
    byte output = encrypt_block(cnt, key);
    output ^= input;
    return output;
}

byte* counter_mode_encrypt(byte* input, byte key){
    // Read the last byte to get the counter value
    // Generate a new seed for the random number generator
    srand((unsigned int)time(NULL));
    byte cnt = (byte)(rand() % 256); // Random counter value

    byte* output = (byte*)malloc(strlen((char*)input) + 2);
    if (output == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }
    
    for (int i = 0; i < strlen((char*)input); i++) {
        output[i] = counter_mode_encrypt_operation(input[i], cnt, key);
        cnt++;
    }

    output[strlen((char*)input)] = cnt;

    printf("Counter value: %d\n", cnt);
    output[strlen((char*)input) + 1] = '\0';

    return output;
}

byte* counter_mode_decrypt(byte* input, byte key) {
    if (!input) {
        fprintf(stderr, "NULL input in counter_mode_decrypt\n");
        return NULL;
    }
    
    size_t len = strlen((char*)input);
    if (len < 1) {
        fprintf(stderr, "Invalid input length in counter_mode_decrypt\n");
        return NULL;
    }
    
    // Get the counter value from the last byte
    byte start_counter = input[len-1];
    printf("Counter value: %d\n", start_counter);
    
    byte* output = (byte*)malloc(len);
    if (output == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }
    
    // Use the same starting counter as encryption
    byte counter = start_counter - (len - 1);
    for (size_t i = 0; i < len - 1; i++) {
        output[i] = counter_mode_encrypt_operation(input[i], counter, key);
        counter++;
    }
    
    output[len-1] = '\0'; // Null-terminate
    return output;
}

byte* ebc_mode_encrypt(byte* input, byte key){
    byte* output = (byte*)malloc(strlen(input) + 1);
    if (output == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }
    
    for (int i = 0; i < strlen(input); i++) {
        output[i] = encrypt_block(input[i], key);
    }

    output[strlen(input)] = '\0'; // Null-terminate the string

    return output;
}

byte* ebc_mode_decrypt(byte* input, byte key){
    byte* output = (byte*)malloc(strlen(input) + 1);
    if (output == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }
    
    for (int i = 0; i < strlen(input); i++) {
        output[i] = decrypt_block(input[i], key);
    }

    output[strlen(input)] = '\0'; // Null-terminate the string

    return output;
}

byte* cbc_mode_encrypt(byte* input, byte key, byte* iv) {
    size_t len = strlen((char*)input); 
    byte* output = (byte*)malloc(len + 3); 
    if (output == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }

    srand((unsigned int)time(NULL));
    *iv = (byte)(rand() % 256); 
    output[0] = *iv; 

    byte previous_block = *iv; 
    for (size_t i = 0; i < len; i++) {
        byte xor_input = input[i] ^ previous_block; 
        output[i + 1] = encrypt_block(xor_input, key); 
        previous_block = output[i + 1]; 
    }

    output[len + 2] = '\0'; 
    return output;
}

byte* cbc_mode_decrypt(byte* input, byte key) {
    size_t len = strlen((char*)input) - 1; 
    byte* output = (byte*)malloc(len + 1); 
    if (output == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }

    byte iv = input[0]; 
    byte previous_block = iv;
    for (size_t i = 0; i < len; i++) {
        byte decrypted_block = decrypt_block(input[i + 1], key); 
        output[i] = decrypted_block ^ previous_block; 
        previous_block = input[i + 1]; 
    }

    output[len] = '\0'; 
    return output;
}
