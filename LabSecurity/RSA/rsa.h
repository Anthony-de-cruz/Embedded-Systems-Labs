#ifndef __RSA_H__
#define __RSA_H__
#define LONGLONGSIZE 8
#include "stm32f7xx_hal.h"

// This is the header file for the library librsaencrypt.a


struct public_key_class{
  uint64_t modulus;
  uint64_t exponent;
};

struct private_key_class{
  uint64_t modulus;
  uint64_t exponent;
};

// This function generates public and private keys, then stores them in the structures you
// provide pointers to. The 3rd argument should be the text PRIME_SOURCE_FILE to have it use
// the location specified above in this header.
void rsa_gen_keys(struct public_key_class *pub, struct private_key_class *priv);

// This function will encrypt the data pointed to by message. It returns a pointer to a heap
// array containing the encrypted data, or NULL upon failure. This pointer should be freed when 
// you are finished. The encrypted data will be 8 times as large as the original data.
void rsa_encrypt(const char *message, const uint64_t message_size, const struct public_key_class *pub,uint64_t* encrypted);

// This function will decrypt the data pointed to by message. It returns a pointer to a heap
// array containing the decrypted data, or NULL upon failure. This pointer should be freed when 
// you are finished. The variable message_size is the size in bytes of the encrypted message. 
// The decrypted data will be 1/8th the size of the encrypted data.
void rsa_decrypt(const uint64_t *message, const uint64_t message_size, const struct private_key_class *pub, char* decrypted);

void encode(uint64_t *encrypted,char *buffer,uint64_t message_size);
int dencode(char *buffer,uint64_t *encrypted);
#endif
