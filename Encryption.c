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
#include "operation/OperationMode.h"

typedef unsigned char byte;

byte* encrypt_data(byte* input, byte key, char* mode) {
    printf("Input 1: %s\n", input);
    byte* output = (byte*)malloc(strlen((char*)input) + 1);
    if (output == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }
    
    if(strcmp(mode, "ECB") == 0) {
        output = ebc_mode_encrypt(input, key);
    } else if(strcmp(mode, "CBC") == 0) {
        // CBC mode logic would go here
        return NULL; // Placeholder for CBC mode
    } else if(strcmp(mode, "CTR") == 0) {
        output = counter_mode_encrypt(input, key);
    } else {
        fprintf(stderr, "Unsupported mode: %s\n", mode);
        free(output);
        return NULL;
    }

    return output;
}

byte* decrypt_data(byte* input, byte key, char* mode) {
    byte* output = (byte*)malloc(strlen((char*)input) + 1);
    if (output == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }
    
    if(strcmp(mode, "ECB") == 0) {
        output = ebc_mode_decrypt(input, key);
    } else if(strcmp(mode, "CBC") == 0) {
        // CBC mode logic would go here
        return NULL; // Placeholder for CBC mode
    } else if(strcmp(mode, "CTR") == 0) {
        output = counter_mode_decrypt(input, key);
    } else {
        fprintf(stderr, "Unsupported mode: %s\n", mode);
        free(output);
        return NULL;
    }

    return output;
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        fprintf(stderr, "Usage: %s <input_string> <key> <mode>\n", argv[0]);
        return 1;
    }

    // Get the input string directly without conversion
    byte* input = (byte*)strdup(argv[1]);  // Make a copy to avoid modifying argv
    if (!input) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
        // Convert key from hex string to byte
    byte key = (byte)strtol(argv[2], NULL, 16);

    char* mode = argv[3];

    // Encrypt the input
    byte* encrypted = encrypt_data(input, key, mode);
    if (!encrypted) {
        fprintf(stderr, "Encryption failed\n");
        free(input);
        return 1;
    }

    // Decrypt the encrypted data
    byte* decrypted = decrypt_data(encrypted, key, mode);
    if (!decrypted) {
        fprintf(stderr, "Decryption failed\n");
        free(input);
        free(encrypted);
        return 1;
    }

    size_t input_len = strlen((char*)input);
    
    printf("Input: %s\n", input);
    printf("Encrypted: ");
    for(size_t i = 0; i < input_len; i++) {
        printf("%02X ", encrypted[i]);
    }
    printf("\n");

    printf("Decrypted: %s\n", decrypted);

    free(input);
    free(encrypted);
    free(decrypted);

    return 0;
}