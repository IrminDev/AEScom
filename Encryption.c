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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "operation/OperationMode.h"

typedef unsigned char byte;

// Find the position of a character in the base64_chars table
static int base64_char_value(char c) {
    if (c >= 'A' && c <= 'Z') return c - 'A';
    if (c >= 'a' && c <= 'z') return c - 'a' + 26;
    if (c >= '0' && c <= '9') return c - '0' + 52;
    if (c == '+') return 62;
    if (c == '/') return 63;
    return -1; // Invalid character
}

// Decode Base64 string to binary data
byte* base64_decode(const char *data, size_t *output_length) {
    if (data == NULL) return NULL;
    
    size_t input_length = strlen(data);
    if (input_length == 0) {
        *output_length = 0;
        return NULL;
    }
    
    // Count padding characters ('=')
    size_t padding = 0;
    if (input_length >= 1 && data[input_length - 1] == '=') padding++;
    if (input_length >= 2 && data[input_length - 2] == '=') padding++;
    
    // Calculate output size
    *output_length = (input_length / 4) * 3 - padding;
    
    byte *decoded_data = malloc(*output_length);
    if (decoded_data == NULL) return NULL;
    
    size_t i, j = 0;
    uint32_t sextet_a, sextet_b, sextet_c, sextet_d;
    uint32_t triple;
    
    for (i = 0; i < input_length; i += 4) {
        // Get values for each group of four base 64 characters
        sextet_a = data[i] == '=' ? 0 & i+0 : base64_char_value(data[i+0]);
        sextet_b = data[i+1] == '=' ? 0 & i+1 : base64_char_value(data[i+1]);
        sextet_c = data[i+2] == '=' ? 0 & i+2 : base64_char_value(data[i+2]);
        sextet_d = data[i+3] == '=' ? 0 & i+3 : base64_char_value(data[i+3]);
        
        // Check for invalid characters
        if (sextet_a == -1 || sextet_b == -1 || sextet_c == -1 || sextet_d == -1) {
            free(decoded_data);
            *output_length = 0;
            return NULL;
        }
        
        triple = (sextet_a << 18) + (sextet_b << 12) + (sextet_c << 6) + sextet_d;
        
        if (j < *output_length) decoded_data[j++] = (triple >> 16) & 0xFF;
        if (j < *output_length) decoded_data[j++] = (triple >> 8) & 0xFF;
        if (j < *output_length) decoded_data[j++] = triple & 0xFF;
    }
    
    return decoded_data;
}

// Base64 encoding table
static const char base64_chars[] = 
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

// Encode binary data to Base64 string
char* base64_encode(const unsigned char *data, size_t input_length) {
    size_t output_length = 4 * ((input_length + 2) / 3);
    char *encoded_data = malloc(output_length + 1);
    if (encoded_data == NULL) return NULL;

    size_t i, j;
    for (i = 0, j = 0; i < input_length;) {
        uint32_t octet_a = i < input_length ? data[i++] : 0;
        uint32_t octet_b = i < input_length ? data[i++] : 0;
        uint32_t octet_c = i < input_length ? data[i++] : 0;

        uint32_t triple = (octet_a << 16) + (octet_b << 8) + octet_c;

        encoded_data[j++] = base64_chars[(triple >> 18) & 0x3F];
        encoded_data[j++] = base64_chars[(triple >> 12) & 0x3F];
        encoded_data[j++] = base64_chars[(triple >> 6) & 0x3F];
        encoded_data[j++] = base64_chars[triple & 0x3F];
    }

    // Add padding characters if needed
    for (i = 0; i < (3 - input_length % 3) % 3; i++) {
        encoded_data[output_length - 1 - i] = '=';
    }

    encoded_data[output_length] = '\0';
    return encoded_data;
}

byte* encrypt_data(byte* input, byte key, char* mode) {
    byte* output = (byte*)malloc(strlen((char*)input) + 1);
    if (output == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }
    
    if(strcmp(mode, "ECB") == 0) {
        output = ebc_mode_encrypt(input, key);
    } else if(strcmp(mode, "CBC") == 0) {
       byte iv; 
        output = cbc_mode_encrypt(input, key, &iv);
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
        output = cbc_mode_decrypt(input, key);

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
        fprintf(stderr, "Usage: %s <operation> <input_string> <key> <mode>\n", argv[0]);
        fprintf(stderr, "operation: -e or -d\n");
        return 1;
    }

    char* operation = argv[1];
    int operation_type = 0;
    if (strcmp(operation, "-e") == 0) {
        operation_type = 0;
    } else if (strcmp(operation, "-d") == 0) {
        operation_type = 1;
    } else {
        fprintf(stderr, "Invalid operation: %s\n", operation);
        return 1;
    }

    // Get the input string directly without conversion
    byte* input = (byte*)strdup(argv[2]);  // Make a copy to avoid modifying argv
    if (!input) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    // Convert key from hex string to byte
    byte key = (byte)strtol(argv[3], NULL, 16);

    char* mode = argv[4];

    if(operation_type){
        size_t decoded_length;
        byte* decoded_input = base64_decode((char*)input, &decoded_length);
        if (!decoded_input) {
            fprintf(stderr, "Base64 decoding failed\n");
            free(input);
            return 1;
        }
        
        // Ensure the decoded data is properly null-terminated for CTR mode
        if (strcmp(mode, "CTR") == 0 || strcmp(mode, "CBC") == 0) {
            // We need to add a null terminator after the counter byte
            byte* temp = realloc(decoded_input, decoded_length + 1);
            if (!temp) {
                fprintf(stderr, "Memory reallocation failed\n");
                free(decoded_input);
                free(input);
                return 1;
            }
            decoded_input = temp;
            decoded_input[decoded_length] = '\0';
        }

        printf("Decoded input: ");
        for (size_t i = 0; i < decoded_length; i++) {
            printf("%02x ", decoded_input[i]);
        }
        printf("\n");

        // Decrypt the input data
        byte* decrypted = decrypt_data(decoded_input, key, mode);
        if (!decrypted) {
            fprintf(stderr, "Decryption failed\n");
            free(input);
            free(decoded_input);
            return 1;
        }

        printf("Input: %s\n", input);
        printf("Decrypted: %s\n", decrypted);

        free(decoded_input);
        free(decrypted);

        return 0;
    } else {
        // In the encryption branch
        byte* encrypted = encrypt_data(input, key, mode);
        if (!encrypted) {
            fprintf(stderr, "Encryption failed\n");
            free(input);
            return 1;
        }

        size_t input_len = strlen((char*)input);
        size_t encrypted_len = input_len;

        // For CTR mode, we need to include the counter byte in the Base64 encoding
        if (strcmp(mode, "CTR") == 0 || strcmp(mode, "CBC") == 0) {
            encrypted_len = input_len + 1; // Include counter byte
        }

        printf("Input: %s\n", input);
        printf("Encrypted: ");
        char* b64 = base64_encode(encrypted, encrypted_len);
        printf("%s", b64);
        free(b64);
        printf("\n");

        printf("Hex: ");
        for(int i = 0; i < encrypted_len; i++) {
            printf("%02x ", encrypted[i]);
        }
        printf("\n");
    }
}