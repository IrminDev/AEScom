# AEScom: A Lightweight Block Cipher Implementation

A cryptographic library developed at ESCOM-IPN

## Overview

AEScom is a lightweight cryptographic library that implements a simplified block cipher inspired by AES (Advanced Encryption Standard). This project was developed as part of the Cryptography course at ESCOM (Escuela Superior de Cómputo) of the National Polytechnic Institute of Mexico.

The name "AEScom" is a playful combination of "AES" and "ESCOM", reflecting both its inspiration and its place of origin.

## Features 
- *8-bit Block Size*: Operates on single-byte blocks for educational purposes
- *Multiple Operation Modes*:
    - ECB (Electronic Codebook)
    - CTR (Counter)
    - CBC (Cipher Block Chaining) - Placeholder for future implementation
- *SPN Network*: Uses a Substitution-Permutation Network for encryption
- *Modular Architecture*: Clean separation between algorithm core and operation modes

## Project structure
```
project/
│
├── algorithm/
│   ├── AEScom.c      # Core implementation of the block cipher
│   └── AEScom.h      # Header for the core algorithm
│
├── operation/
│   ├── OperationMode.c   # Implementation of ECB, CTR modes
│   └── OperationMode.h   # Header for operation modes
│
├── Encryption.c      # Main application demonstrating usage
├── makefile          # Build configuration
└── README.md         # This documentation file
```

## Building the Project

```
gcc -Wall -Wextra -g Encryption.c operation/OperationMode.c algorithm/AEScom.c -o encryption
```

## Usage 

```
./encryption <input_string> <key> <mode>
```

Where:

- *input_string*: Text to encrypt
- *key*: Encryption key (in hex format)
- *mode*: Operation mode ("ECB", "CTR")

Example:
```
./encryption "Hello World" 2A ECB
```

## Technical Details

Block Cipher

AEScom uses a simplified block cipher with:

- 8-bit block size
- 4-round Substitution-Permutation Network
- Custom S-box for substitution
- Two different permutation tables

Operation Modes

- ECB Mode: Each block is encrypted independently
- CTR Mode: Uses a counter with XOR to create a stream cipher
- CBC Mode: (Placeholder for future implementation)

## Implementation notes

This is a simplified cipher for educational purposes
Not recommended for production use or sensitive data
Demonstrates fundamental cryptographic concepts in a minimal implementation

## License

This project is available under the MIT License.