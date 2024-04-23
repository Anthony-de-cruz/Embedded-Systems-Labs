// Online C compiler to run C program online
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// A list of primes, you can extend the number of primes it by yourself.
static unsigned int PRIMES[]={10007,10009,10037,10039,10061,10067,10069,10079,10091,10093};
static unsigned int PRIMES_SIZE=sizeof(PRIMES) / sizeof(PRIMES[0]);

struct public_key_class{
  long long modulus;
  long long exponent;
};

struct private_key_class{
  long long modulus;
  long long exponent;
};


// This should totally be in the math library.
long long gcd(long long a, long long b)
{
  long long c;
  while ( a != 0 ) {
    c = a; a = b%a;  b = c;
  }
  return b;
}


long long ExtEuclid(long long a, long long b)
{
 long long x = 0, y = 1, u = 1, v = 0, gcd = b, m, n, q, r;
 while (a!=0) {
   q = gcd/a; r = gcd % a;
   m = x-u*q; n = y-v*q;
   gcd = a; a = r; x = u; y = v; u = m; v = n;
   }
   return y;
}
static inline long long modmult(long long a,long long b,long long mod)
{
   // this is necessary since we will be dividing by a
   if (a == 0 ){
         return 0;
   }
   register long long product = a * b;
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
    long long sum;
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
long long rsa_modExp(long long b, long long e, long long m)
{
      long long product;
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

  long long p = 0;
  long long q = 0;

//values of e should be sufficiently large to protect against naive attacks
  long long e = (2 << 16) +1;
  long long d = 0;
  long long max = 0;
  long long phi_max = 0;

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

  //printf("primes are %lld and %lld\n",(long long)p, (long long )q);
  // We now store the public / private keys in the appropriate structs
  pub->modulus = max;
  pub->exponent = e;

  priv->modulus = max;
  priv->exponent = d;
}

int main(int argc, char **argv)
{
  struct public_key_class pub[1];
  struct private_key_class priv[1];
  
  rsa_gen_keys(pub, priv);
  printf("Private Key:\n Modulus: %lld\n Exponent: %lld\n", (long long)priv->modulus, (long long) priv->exponent);
  printf("Public Key:\n Modulus: %lld\n Exponent: %lld\n", (long long)pub->modulus, (long long) pub->exponent);}