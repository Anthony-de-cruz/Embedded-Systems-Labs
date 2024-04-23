#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include "primes.c"
#include "rsa.h"
#define MAX_BUFFERSIZE 1024
int i,j = 0;


// This should totally be in the math library.
uint64_t gcd(uint64_t a, uint64_t b)
{
  uint64_t c;
  while ( a != 0 ) {
    c = a; a = b%a;  b = c;
  }
  return b;
}


uint64_t ExtEuclid(uint64_t a, uint64_t b)
{
 uint64_t x = 0, y = 1, u = 1, v = 0, gcd = b, m, n, q, r;
 while (a!=0) {
   q = gcd/a; r = gcd % a;
   m = x-u*q; n = y-v*q;
   gcd = a; a = r; x = u; y = v; u = m; v = n;
   }
   return y;
}
static inline uint64_t modmult(uint64_t a,uint64_t b,uint64_t mod)
{
   // this is necessary since we will be dividing by a
   if (a == 0 ){
         return 0;
   }
   register uint64_t product = a * b;
    //if multiplication does not overflow, we can use it
   if (product / a == b){
          return product % mod;
   }
   // if a % 2 == 1 i. e. a >> 1 is not a / 2
   if ( a & 1 ) {
         product = modmult((a>>1), b, mod);
         if ((product << 1) > product ){
         return ((( product << 1 ) % mod ) + b) % mod;
      }
   }
   //implicit else
   product = modmult((a >> 1), b, mod);
   if ((product << 1) > product){
         return (product << 1) % mod ;
         }
   //implicit else: this is about 10x slower than the code above, but it will not overflow
    uint64_t sum;
    sum = 0;
    while(b>0)
    {
        if(b&1)
            sum = (sum + a) % mod;
        a = (2*a) % mod;
        b>>=1;
    }
    return sum;
}
uint64_t rsa_modExp(uint64_t b, uint64_t e, uint64_t m)
{
      uint64_t product;
      product = 1;
      if (b < 0 || e < 0 || m <= 0){
            return -1;
      }
      b = b % m;
      while ( e > 0){
            if (e & 1){
                  product = modmult(product, b, m);
            }
            b = modmult(b, b, m);
            e >>= 1;
      }
      return product;
}
// Calling this function will generate a public and private key and store them in the pointers
// it is given.
void rsa_gen_keys(struct public_key_class *pub, struct private_key_class *priv)
{
  
  // choose random primes from the list, store them as p,q

  uint64_t p = 0;
  uint64_t q = 0;

//values of e should be sufficiently large to protect against naive attacks
  uint64_t e = (2 << 16) +1;
  uint64_t d = 0;
  uint64_t max = 0;
  uint64_t phi_max = 0;

  srand(time(NULL));
  printf("select primer \n");
  
  do{
    // a and b are the positions of p and q in the list
    unsigned int a =  rand() % (PRIMES_SIZE+1);
    unsigned int b =  rand() % (PRIMES_SIZE+1);
    p = PRIMES[a];
    q = PRIMES[b];
    
    max = p*q;
    phi_max = (p-1)*(q-1);
    
  }
  while(!(p && q) || (p == q) || (gcd(phi_max, e) != 1));

  // Next, we need to choose a,b, so that a*max+b*e = gcd(max,e). We actually only need b
  // here, and in keeping with the usual notation of RSA we'll call it d. We'd also like
  // to make sure we get a representation of d as positive, hence the while loop.
  d = ExtEuclid(phi_max,e);
  while(d < 0){
    d = d+phi_max;
  }

  //printf("primes are %lld and %lld\n",(uint64_t)p, (uint64_t )q);
  // We now store the public / private keys in the appropriate structs
  pub->modulus = max;
  pub->exponent = e;

  priv->modulus = max;
  priv->exponent = d;
}


void rsa_encrypt(const char *message, const uint64_t message_size,
                     const struct public_key_class *pub, uint64_t* encrypted)
{
  uint64_t i=0;
 
  for(i=0; i < message_size; i++){
    if ((encrypted[i] = rsa_modExp(message[i], pub->exponent, pub->modulus)) == -1) break; //error
  }
}


void rsa_decrypt(const uint64_t *message,
                  const uint64_t message_size,
                  const struct private_key_class *priv,
									char* decrypted)
{
	uint64_t i=0;
	
  char temp[MAX_BUFFERSIZE];
  // Now we go through each 8-byte chunk and decrypt it.
  for(i=0; i < message_size/LONGLONGSIZE; i++){
    if ((temp[i] = rsa_modExp(message[i], priv->exponent, priv->modulus)) == -1){
          //return NULL;
      }
  }
  // The result should be a number in the char range, which gives back the original byte.
  // We put that into decrypted, then return.
  for(i=0; i < message_size/8; i++){
    decrypted[i] = temp[i];
  }
}


void encode(uint64_t *encrypted,char *buffer,uint64_t message_size){
    int index = 0;
    int i;
    for(i=0;i<message_size;i++){
        index += snprintf(&buffer[index],sizeof(uint64_t)*100, "%8llx", encrypted[i]);// encode to 8 bytes string
    }
    
}


int dencode(char *buffer,uint64_t *encrypted){
    int i,j;
    int index=0;
    if(strlen(buffer)%8!=0){
        //fprintf(stderr, "Error in dencode!\n");
        return 0;
    }
    for(i=0;i<strlen(buffer);){
        char tmp[10]={'\0'};
        for(j=0;j<8;j++){
           tmp[j]=buffer[i+j];
        }
				encrypted[index++]=strtoul(tmp,NULL,16);
        i+=8;
    }
		
		return 1;
}