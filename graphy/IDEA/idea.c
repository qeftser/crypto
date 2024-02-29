
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ROL25(a,b) a = (a << 25) | (b >> 39); b = (b << 25) | (a >> 39)
#define MLTM216(a,b) (uint16_t)(((uint32_t)a * b) % 0x10001)
#define ADDM216(a,b) (uint16_t)(((uint32_t)a + b) % 0x10000)
#define swap(a,b) temp = a; a = b; b = temp


// C function for extended Euclidean Algorithm
// This is a copy and paste
int gcd_extended(int a, int b, int* x, int* y)
{
	// Base Case
	if (a == 0) {
		*x = 0, *y = 1;
		return b;
	}

	int x1, y1; // To store results of recursive call
	int gcd = gcd_extended(b % a, a, &x1, &y1);

	// Update x and y using results of recursive
	// call
	*x = y1 - (b / a) * x1;
	*y = x1;

	return gcd;
}

// Function to find modulo inverse of a
// This is a copy and paste
int mod_inverse(int A, int M)
{
	int x, y;
	int g = gcd_extended(A, M, &x, &y);
	if (g != 1)
      return 1;
	else {
		// m is added to handle negative x
		int res = (x % M + M) % M;
      return res;
	}
}


void invert_keys(uint16_t * keys) {
   int idx;
   for (int i = 0; i < 9; i++) {
      idx = i*6;
      keys[idx] = (uint16_t)mod_inverse(keys[idx],0x10001);
      keys[idx+1] = (uint16_t)(0x10000 - keys[idx+1]);
      keys[idx+2] = (uint16_t)(0x10000 - keys[idx+2]);
      keys[idx+3] = (uint16_t)mod_inverse(keys[idx+3],0x10001);
   }
}

void encrypt_IDEA(uint64_t *block, uint64_t * key) {
   
   uint16_t Z[54];
   uint16_t X[4];
   uint16_t temp;
   uint16_t S[10];
   uint64_t key_cpy[2];
   int idx;

   memcpy(key_cpy,key,16);

   /* mass generate all the keys at once
    * possibility to optimize here
    */
   for (int i = 0; i < 9; i++) {
      memcpy(&Z[i*6],key_cpy,16);
      ROL25(key_cpy[0],key_cpy[1]);
   }
   /* ensure strong keys */
   for (int i = 0; i < 54; i++) {
      Z[i] = Z[i] ^ 0x0dae;
   }

   /* load blocks */
   memcpy(X,block,8);

   for (int i = 0; i < 8; i++) {
      idx = i*6;

      S[0] = MLTM216(X[0],Z[idx]);
      S[1] = ADDM216(X[1],Z[idx+1]);
      S[2] = ADDM216(X[2],Z[idx+2]);
      S[3] = MLTM216(X[3],Z[idx+3]);
      S[4] = S[0] ^ S[2];
      S[5] = S[1] ^ S[3];
      S[6] = MLTM216(S[4],Z[idx+4]);
      S[7] = ADDM216(S[5],S[6]);
      S[8] = MLTM216(S[7],Z[idx+5]);
      S[9] = ADDM216(S[6],S[8]);
      X[0] = S[0] ^ S[8];
      X[1] = S[2] ^ S[8];
      X[2] = S[1] ^ S[9];
      X[3] = S[3] ^ S[9];

      // couldn't figure out the swaps here
      swap(X[1],X[2]);
   }

   X[0] = MLTM216(X[0],Z[48]);
   X[1] = ADDM216(X[1],Z[49]);
   X[2] = ADDM216(X[2],Z[50]);
   X[3] = MLTM216(X[3],Z[51]);

   /* recopy encryption results */
   memcpy(block,X,8);
}

void decrypt_IDEA(uint64_t *block, uint64_t * key) {
   
   uint16_t Z[54];
   uint16_t X[4];
   uint16_t temp;
   uint16_t S[10];
   uint64_t key_cpy[2];
   int idx;

   memcpy(key_cpy,key,16);

   /* mass generate all the keys at once
    * possibility to optimize here
    */
   for (int i = 0; i < 9; i++) {
      memcpy(&Z[i*6],key_cpy,16);
      ROL25(key_cpy[0],key_cpy[1]);
   }
   /* ensure strong keys */
   for (int i = 0; i < 54; i++) {
      Z[i] = Z[i] ^ 0x0dae;
   }

   invert_keys(Z);

   /* load blocks */
   memcpy(X,block,8);

   for (int i = 8; i > 0; i--) {
      idx = (i*6);

      S[0] = MLTM216(X[0],Z[idx]);
      S[1] = ADDM216(X[1],Z[idx+1]);
      S[2] = ADDM216(X[2],Z[idx+2]);
      S[3] = MLTM216(X[3],Z[idx+3]);
      S[4] = S[0] ^ S[2];
      S[5] = S[1] ^ S[3];
      S[6] = MLTM216(S[4],Z[idx-2]);
      S[7] = ADDM216(S[5],S[6]);
      S[8] = MLTM216(S[7],Z[idx-1]);
      S[9] = ADDM216(S[6],S[8]);
      X[0] = S[0] ^ S[8];
      X[1] = S[2] ^ S[8];
      X[2] = S[1] ^ S[9];
      X[3] = S[3] ^ S[9];

      // Couldn't figure out the spelling here
      swap(X[1],X[2]);
   }

   X[0] = MLTM216(X[0],Z[0]);
   X[1] = ADDM216(X[1],Z[1]);
   X[2] = ADDM216(X[2],Z[2]);
   X[3] = MLTM216(X[3],Z[3]);

   /* recopy encryption results */
   memcpy(block,X,8);
}

int main(void) {

   srand(clock());

   uint64_t num = rand();

   uint64_t key[2] =  { rand(), rand() };

   printf("init: %016lx\n",num);

   encrypt_IDEA(&num,key);

   printf("encr: %016lx\n",num);

   decrypt_IDEA(&num,key);

   printf("decr: %016lx\n",num);


   return 0;
}
