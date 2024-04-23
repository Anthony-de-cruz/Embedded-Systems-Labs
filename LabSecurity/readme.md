This is a lab to to practice the RSA Encryption/Decryption and UART communicaiton on a stm32f746g board.
The files are allocated as follows:
-main.h: every example share the same header file 
-RSA
    --rsa.h: header file of rsa library
    --rsa.c: code file of rsa library
    --simpleRSA.c: example code of RSA
    --primes.c: a set of prime number
    --gen_key.c: key generate extract from the library. You can run it on PC, or using an online compiler
-UART
    --rx.c: receiver example
    --tx.c: transmitter example