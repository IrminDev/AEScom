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
    
    for (size_t i = 0; i < strlen((char*)input); i++) {
        output[i] = counter_mode_encrypt_operation(input[i], cnt, key);
        cnt++;
    }

    output[strlen((char*)input)] = cnt;

    output[strlen((char*)input) + 1] = '\0';

    return output;
}

byte* counter_mode_decrypt(byte* input, byte key, size_t input_len) {
    if (!input) {
        fprintf(stderr, "NULL input in counter_mode_decrypt\n");
        return NULL;
    }
    byte start_counter = input[input_len-1];
    byte* output = (byte*)malloc(input_len);
    if (output == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }
    
    byte counter = start_counter - (input_len - 1);
    for (size_t i = 0; i < input_len - 1; i++) {
        output[i] = counter_mode_encrypt_operation(input[i], counter, key);
        counter++;
    }
    
    output[input_len-1] = '\0'; // Null-terminate
    return output;
}

byte* ebc_mode_encrypt(byte* input, byte key){
    size_t input_len = strlen((char*)input);
    byte* output = (byte*)malloc(input_len + 1);
    if (output == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }
    
    for (size_t i = 0; i < input_len; i++) {
        output[i] = encrypt_block(input[i], key);
    }

    output[input_len] = '\0'; // Null-terminate the string

    return output;
}

byte* ebc_mode_decrypt(byte* input, byte key, size_t input_len) {
    byte* output = (byte*)malloc(input_len + 1);
    if (output == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }
    
    for (size_t i = 0; i < input_len; i++) {
        output[i] = decrypt_block(input[i], key);
    }

    output[input_len] = '\0'; // Null-terminate the string
    
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

byte* cbc_mode_decrypt(byte* input, byte key, size_t input_len) {
    if (input_len < 1) {
        fprintf(stderr, "Invalid input length for CBC decrypt\n");
        return NULL;
    }
    
    // The actual data length is input_len - 1 (excluding IV)
    size_t data_len = input_len - 1;
    byte* output = (byte*)malloc(data_len + 1); 
    if (output == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }

    byte iv = input[0]; 
    byte previous_block = iv;
    
    // Process only the encrypted data bytes (excluding IV)
    for (size_t i = 0; i < data_len; i++) {
        byte decrypted_block = decrypt_block(input[i + 1], key); 
        output[i] = decrypted_block ^ previous_block; 
        previous_block = input[i + 1]; 
    }

    output[data_len] = '\0';  // Null-terminate at the correct position
    return output;
}