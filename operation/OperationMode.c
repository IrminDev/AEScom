#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    byte cnt = 0;
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
    
    byte cnt = input[len-1];
    
    byte* output = (byte*)malloc(len); // This includes null terminator space
    if (output == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }
    
    byte counter = 0;
    for (size_t i = 0; i < len - 1; i++) { // -1 to exclude counter byte
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
