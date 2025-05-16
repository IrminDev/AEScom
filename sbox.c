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

unsigned int degree(unsigned int poly) {
    unsigned int deg = 0;
    for (int i = 31; i >= 0; i--) {
        if ((poly >> i) & 1) {
            deg = i;
            break;
        }
    }
    return deg;
}

unsigned int multiplyx(unsigned int b, unsigned int irr_poly) {
    unsigned int result = b << 1;
    unsigned int n = degree(irr_poly);
    if (b & (1 << (n-1))) {  // Check if the MSB is set
        result ^= irr_poly;
    }
    unsigned int mask = (1 << n) - 1; // Mask of n bits
    result &= mask;
    return result;
}

unsigned int multiply(unsigned int a, unsigned int b, unsigned int irr_poly) {
    unsigned int result = 0;
    unsigned int n = degree(irr_poly);
    unsigned int dp = a;
    
    // Process each bit of b
    for (int i = 0; i < n; i++) {
        if (b & (1 << i)) { // If the i-th bit of b is set, add dp to result
            result ^= dp;
        }
        dp = multiplyx(dp, irr_poly); // Multiply dp by x
    }
    
    return result;
}

void print_polynomial(unsigned int num, unsigned int n) {
    int first = 1;
    
    if (num == 0) {
        printf("0");
        return;
    }
    
    for (int i = n-1; i >= 0; i--) {
        if (num & (1 << i)) {
            if (!first) {
                printf(" + ");
            } else {
                first = 0;
            }
            
            if (i == 0) {
                printf("1");
            } else if (i == 1) {
                printf("x");
            } else {
                printf("x^%d", i);
            }
        }
    }
}

void print_binary(unsigned int num, unsigned int n) {
    for (int i = n-1; i >= 0; i--) {
        printf("%d", (num >> i) & 1);
    }
    printf("\n");
}

void print_hex_dd(unsigned int num) {
    if(num < 16){
        printf("0x0%x", num);
    } else {
        printf("0x%x", num);
    }
}

void print_data(unsigned int num, unsigned int n) {
    printf("Decimal: %u\n", num);
    printf("Hexadecimal: ");
    print_hex_dd(num);
    printf("\n");
    printf("Binary: ");
    print_binary(num, n);
    printf("Polynomial: ");
    print_polynomial(num, n);
    printf("\n");
}

int get_inverse(unsigned int a, unsigned int irr_poly) {
    unsigned int s = irr_poly, r = a, v = 0, u = 1;
    while(degree(r) > 0) {
        int delta = degree(s) - degree(r);
        if (delta < 0) {
            unsigned int temp = s;
            s = r;
            r = temp;
            temp = v;
            v = u;
            u = temp;
            delta = -delta;
        }
        s = s ^ (r << delta);
        v = v ^ (u << delta);
    }

    return u;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <option>\n", argv[0]);
        fprintf(stderr, "Options:\n");
        fprintf(stderr, " -g Generates the s-box\n");
        fprintf(stderr, " -i Generates the inverse s-box\n");
        return 1;
    } else {
        if(strcmp(argv[1], "-g") == 0) {
            unsigned int irr_poly = 0x13;
            unsigned int sbox[16];
            for(unsigned int i = 0; i < 16; i++) {
                unsigned int tmp = i ^ 0x9; // xor with x^3 + x^2 + 1
                tmp = multiply(tmp, 0xD, irr_poly); // multiply by x^3 + x^2 + 1
                sbox[i] = tmp ^ 0x6;
            }

            printf("S-Box:\n");
            for (int i = 0; i < 16; i++) {
                printf("0x%01X, ", sbox[i]);
                if ((i + 1) % 4 == 0) {
                    printf("\n");
                }
            }
            printf("\n");
        } else if (strcmp(argv[1], "-i") == 0) {
            unsigned int irr_poly = 0x13;
            unsigned int sbox[16];
            for(unsigned int i = 0; i < 16; i++) {
                unsigned int tmp = i ^ 0x6; // xor with x^3 + x^2 + 1
                tmp = multiply(tmp, get_inverse(0xD, irr_poly), irr_poly);
                sbox[i] = tmp ^ 0x9;
            }

            printf("Inverse S-Box:\n");
            for (int i = 0; i < 16; i++) {
                printf("0x%01X, ", sbox[i]);
                if ((i + 1) % 4 == 0) {
                    printf("\n");
                }
            }
            printf("\n");
        } else {
            fprintf(stderr, "Invalid option: %s\n", argv[1]);
            return 1;
        }
    }   
    return 1;
}