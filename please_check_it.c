#include <stdio.h>
#include <stdint.h>

#define BLOCK_SIZE      16
#define ROUNDS          2
#define ROUND_KEY_SIZE  (ROUNDS + 1) * 16  
#define NUMBER_OF_BLOCKS 13

uint8_t RC[] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36 };


uint8_t SBOX[256] = {
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 };

uint8_t INV_SBOX[256] = {
    0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
    0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
    0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
    0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
    0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
    0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
    0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
    0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
    0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
    0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
    0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
    0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
    0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
    0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
    0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
    0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d };


uint8_t mul2(uint8_t a) {
    return (a & 0x80) ? ((a << 1) ^ 0x1b) : (a << 1);
}


void shift_rows(uint8_t* state) {
    uint8_t temp;
    
    temp = *(state + 1);
    *(state + 1) = *(state + 5);
    *(state + 5) = *(state + 9);
    *(state + 9) = *(state + 13);
    *(state + 13) = temp;
    
    temp = *(state + 2);
    *(state + 2) = *(state + 10);
    *(state + 10) = temp;
    temp = *(state + 6);
    *(state + 6) = *(state + 14);
    *(state + 14) = temp;
   
    temp = *(state + 15);
    *(state + 15) = *(state + 11);
    *(state + 11) = *(state + 7);
    *(state + 7) = *(state + 3);
    *(state + 3) = temp;
}


void inv_shift_rows(uint8_t* state) {
    uint8_t temp;
    
    temp = *(state + 13);
    *(state + 13) = *(state + 9);
    *(state + 9) = *(state + 5);
    *(state + 5) = *(state + 1);
    *(state + 1) = temp;
    
    temp = *(state + 14);
    *(state + 14) = *(state + 6);
    *(state + 6) = temp;
    temp = *(state + 10);
    *(state + 10) = *(state + 2);
    *(state + 2) = temp;
    
    temp = *(state + 3);
    *(state + 3) = *(state + 7);
    *(state + 7) = *(state + 11);
    *(state + 11) = *(state + 15);
    *(state + 15) = temp;
}

void key_expansion(const uint8_t* key, uint8_t* roundkeys) {

    uint8_t temp[4];
    uint8_t* last4bytes; 
    uint8_t* lastround;
    uint8_t i;

    for (i = 0; i < 16; ++i) {
        *roundkeys++ = *key++;
    }

    last4bytes = roundkeys - 4;
    for (i = 0; i < ROUNDS; ++i) {
        
        temp[3] = SBOX[*last4bytes++];
        temp[0] = SBOX[*last4bytes++];
        temp[1] = SBOX[*last4bytes++];
        temp[2] = SBOX[*last4bytes++];
        temp[0] ^= RC[i];
        lastround = roundkeys - 16;
        *roundkeys++ = temp[0] ^ *lastround++;
        *roundkeys++ = temp[1] ^ *lastround++;
        *roundkeys++ = temp[2] ^ *lastround++;
        *roundkeys++ = temp[3] ^ *lastround++;
               
        *roundkeys++ = *last4bytes++ ^ *lastround++;
        *roundkeys++ = *last4bytes++ ^ *lastround++;
        *roundkeys++ = *last4bytes++ ^ *lastround++;
        *roundkeys++ = *last4bytes++ ^ *lastround++;
        
        *roundkeys++ = *last4bytes++ ^ *lastround++;
        *roundkeys++ = *last4bytes++ ^ *lastround++;
        *roundkeys++ = *last4bytes++ ^ *lastround++;
        *roundkeys++ = *last4bytes++ ^ *lastround++;
        
        *roundkeys++ = *last4bytes++ ^ *lastround++;
        *roundkeys++ = *last4bytes++ ^ *lastround++;
        *roundkeys++ = *last4bytes++ ^ *lastround++;
        *roundkeys++ = *last4bytes++ ^ *lastround++;
    }
}

void encrypt(const uint8_t* roundkeys, const uint8_t* plaintext, uint8_t* ciphertext) {

    uint8_t tmp[BLOCK_SIZE * NUMBER_OF_BLOCKS], t;

    for (uint8_t i = 0; i < NUMBER_OF_BLOCKS * BLOCK_SIZE; ++i) 
    {
        ciphertext[i] = plaintext[i] ^ roundkeys[i % BLOCK_SIZE];
    }

    for (uint8_t j = 1; j < ROUNDS; ++j) 
    {
        for (uint8_t n = 0; n < NUMBER_OF_BLOCKS; n++)
        {
            for (uint8_t i = 0; i < BLOCK_SIZE; ++i) 
            {
                tmp[n * BLOCK_SIZE + i] = SBOX[ciphertext[n * BLOCK_SIZE + i]];
            }

            shift_rows(tmp + n * BLOCK_SIZE);

            for (uint8_t i = 0; i < BLOCK_SIZE; i += 4) 
            {
                t = tmp[n * BLOCK_SIZE + i] ^ tmp[n * BLOCK_SIZE + i + 1] ^ tmp[n * BLOCK_SIZE + i + 2] ^ tmp[n * BLOCK_SIZE + i + 3];
                ciphertext[n * BLOCK_SIZE + i] = mul2(tmp[n * BLOCK_SIZE + i] ^ tmp[n * BLOCK_SIZE + i + 1]) ^ tmp[n * BLOCK_SIZE + i] ^ t;
                ciphertext[n * BLOCK_SIZE + i + 1] = mul2(tmp[n * BLOCK_SIZE + i + 1] ^ tmp[n * BLOCK_SIZE + i + 2]) ^ tmp[n * BLOCK_SIZE + i + 1] ^ t;
                ciphertext[n * BLOCK_SIZE + i + 2] = mul2(tmp[n * BLOCK_SIZE + i + 2] ^ tmp[n * BLOCK_SIZE + i + 3]) ^ tmp[n * BLOCK_SIZE + i + 2] ^ t;
                ciphertext[n * BLOCK_SIZE + i + 3] = mul2(tmp[n * BLOCK_SIZE + i + 3] ^ tmp[n * BLOCK_SIZE + i]) ^ tmp[n * BLOCK_SIZE + i + 3] ^ t;
            }

            for (uint8_t i = 0; i < BLOCK_SIZE; ++i) {
                ciphertext[n * BLOCK_SIZE + i] ^= roundkeys[i % BLOCK_SIZE + j * 16];
            }
        }
    }

    for (uint8_t n = 0; n < NUMBER_OF_BLOCKS; n++)
    {
        for (uint8_t i = 0; i < BLOCK_SIZE; ++i) 
        {
            ciphertext[n * BLOCK_SIZE + i] = SBOX[ciphertext[n * BLOCK_SIZE + i]];
        }
        shift_rows(ciphertext + n * BLOCK_SIZE);
        for (uint8_t i = 0; i < BLOCK_SIZE; ++i) 
        {
            ciphertext[n * BLOCK_SIZE + i] ^= roundkeys[i % BLOCK_SIZE + ROUND_KEY_SIZE - 16];
        }
    }
}

void decrypt(const uint8_t* roundkeys, const uint8_t* ciphertext, uint8_t* plaintext) 
{

    uint8_t tmp[BLOCK_SIZE * NUMBER_OF_BLOCKS];
    uint8_t t, u, v;

    roundkeys += ROUND_KEY_SIZE - 16;

    for (uint8_t n = 0; n < NUMBER_OF_BLOCKS; n++)
    {
        for (uint8_t i = 0; i < BLOCK_SIZE; ++i)
        {
            plaintext[n * BLOCK_SIZE + i] = ciphertext[n * BLOCK_SIZE + i] ^ *(roundkeys + i % BLOCK_SIZE);
        }
        
        inv_shift_rows(plaintext + n * BLOCK_SIZE);
        for (uint8_t i = 0; i < BLOCK_SIZE; ++i)
        {
            plaintext[n * BLOCK_SIZE + i] = INV_SBOX[plaintext[n * BLOCK_SIZE + i]];
        }
    }

    roundkeys -= 16;

    for (uint8_t j = 1; j < ROUNDS; ++j) 
    {
        for (uint8_t n = 0; n < NUMBER_OF_BLOCKS; n++)
        {
            for (uint8_t i = 0; i < BLOCK_SIZE; ++i)
            {
                tmp[n * BLOCK_SIZE + i] = plaintext[n * BLOCK_SIZE + i] ^ *(roundkeys + i % BLOCK_SIZE);
            }

            for (uint8_t i = 0; i < BLOCK_SIZE; i += 4)
            {
                t = tmp[n * BLOCK_SIZE + i] ^ tmp[n * BLOCK_SIZE + i + 1] ^ tmp[n * BLOCK_SIZE + i + 2] ^ tmp[n * BLOCK_SIZE + i + 3];
                plaintext[n * BLOCK_SIZE + i] = t ^ tmp[n * BLOCK_SIZE + i] ^ mul2(tmp[n * BLOCK_SIZE + i] ^ tmp[n * BLOCK_SIZE + i + 1]);
                plaintext[n * BLOCK_SIZE + i + 1] = t ^ tmp[n * BLOCK_SIZE + i + 1] ^ mul2(tmp[n * BLOCK_SIZE + i + 1] ^ tmp[n * BLOCK_SIZE + i + 2]);
                plaintext[n * BLOCK_SIZE + i + 2] = t ^ tmp[n * BLOCK_SIZE + i + 2] ^ mul2(tmp[n * BLOCK_SIZE + i + 2] ^ tmp[n * BLOCK_SIZE + i + 3]);
                plaintext[n * BLOCK_SIZE + i + 3] = t ^ tmp[n * BLOCK_SIZE + i + 3] ^ mul2(tmp[n * BLOCK_SIZE + i + 3] ^ tmp[n * BLOCK_SIZE + i]);
                u = mul2(mul2(tmp[n * BLOCK_SIZE + i] ^ tmp[n * BLOCK_SIZE + i + 2]));
                v = mul2(mul2(tmp[n * BLOCK_SIZE + i + 1] ^ tmp[n * BLOCK_SIZE + i + 3]));
                t = mul2(u ^ v);
                plaintext[n * BLOCK_SIZE + i] ^= t ^ u;
                plaintext[n * BLOCK_SIZE + i + 1] ^= t ^ v;
                plaintext[n * BLOCK_SIZE + i + 2] ^= t ^ u;
                plaintext[n * BLOCK_SIZE + i + 3] ^= t ^ v;
            }

            inv_shift_rows(plaintext + n * BLOCK_SIZE);

            for (uint8_t i = 0; i < BLOCK_SIZE; ++i)
            {
                plaintext[n * BLOCK_SIZE + i] = INV_SBOX[plaintext[n * BLOCK_SIZE + i]];
            }
        }
        roundkeys -= 16;
    }

    for (uint8_t i = 0; i < NUMBER_OF_BLOCKS * BLOCK_SIZE; ++i) 
    {
        plaintext[i] ^= *(roundkeys + i % BLOCK_SIZE);
    }

}

int main() {

    uint8_t *key = "Thats my kung fu";
    uint8_t* plaintext = "2 C 6 D 8 S 7 S 5 C 3 C 3 S 7 C 2 D 6 S 6 C 6 H 2 H 9 C K C J H 1 C 4 H 4 S 5 C K H 3 H 5 S Q C 2 S Q H A H A S 1 H A C 7 D 7 H 5 H 3 D Q D Q S 9 D 9 S J C 9 H 8 C J D A D J S 8 D 8 H 4 D 1 D K D 1 S 4 C K S ";
    uint8_t ciphertext[NUMBER_OF_BLOCKS * BLOCK_SIZE];
    uint8_t roundkeys[ROUND_KEY_SIZE];

    key_expansion(key, roundkeys);

    encrypt(roundkeys, plaintext, ciphertext);
  
    printf("Encrypted text:\n");
    for (uint8_t i = 0; i < NUMBER_OF_BLOCKS * BLOCK_SIZE; i++) 
        printf("%0x", ciphertext[i]);
  
    decrypt(roundkeys, ciphertext, ciphertext);

    printf("Plain text:\n");
    for (uint8_t i = 0; i < NUMBER_OF_BLOCKS * BLOCK_SIZE; i++) 
        printf("%0x", ciphertext[i]);

    return 0;
}
