// Author: Garry Cummins - G00335806
// Theory of Algorithms Project 

// Imports for header file input output and bit length integers
#include <stdio.h>
#include <stdint.h>


// Represents message block that accesses e, t, s integers
union msgblock {
	//8 bit integer 
    uint8_t  e[64];
	//32 bit integer
    uint32_t t[16];
	//64 bit integer
    uint64_t  s[8];
};

// A flag used for reading the file 
enum status {READ, PAD0, PAD1, FINISH};

void SHA256(FILE *f);


// Hashing functions
uint32_t sig0(uint32_t x);
uint32_t sig1(uint32_t x);

uint32_t rotr(uint32_t n, uint32_t x);
uint32_t shr(uint32_t n, uint32_t x);

uint32_t SIG0(uint32_t x);
uint32_t SIG1(uint32_t x);

uint32_t Ch(uint32_t x, uint32_t y, uint32_t z);
uint32_t Maj(uint32_t x, uint32_t y, uint32_t z);

// Macro functions for converting from little endian to big endian.
// http://www.mit.edu/afs.new/sipb/project/merakidev/include/bits/byteswap.h
/*define BigEndian32(x)                      \
 ((((x) & 0xff000000) >> 24)                \
 (((x) & 0x00ff0000) >>  8)                \
| (((x) & 0x0000ff00) <<  8)                \
| (((x) & 0x000000ff) << 24))

# define BigEndian64(x)                     \
 ((((x) & 0xff00000000000000) >> 56)        \
| (((x) & 0x00ff000000000000) >> 40)        \
| (((x) & 0x0000ff0000000000) >> 24)        \
| (((x) & 0x000000ff00000000) >> 8)         \
| (((x) & 0x00000000ff000000) << 8)         \
| (((x) & 0x0000000000ff0000) << 24)        \
| (((x) & 0x000000000000ff00) << 40)        \
| (((x) & 0x00000000000000ff) << 56))*/


// Reads in message block 
int nextmsgblock(FILE *msgf, union msgblock *M, enum status *S, uint64_t *nobits);

int main(int argc, char *argv[]){
	// Opens file to use
	FILE* msgf;
    msgf = fopen(argv[1], "r");
	
	// File checking 
	if (msgf == NULL) {
        printf("Incorrect file name used\n");
    }
	
	else{
		SHA256(msgf);
		
		fclose(msgf);
	}

	return 0;
}

//int nextmsgblock();

void SHA256(FILE * msgf){
	//Gets the current message block 
	union msgblock M;
	// Number of bits read from a file 
    uint64_t nobits = 0;
	// Message block 
    uint64_t nobytes;
	// Padding status of the message block 
	enum status S = READ;
	
	// K constants hash values 
	uint32_t K[] = {
		 0x428a2f98 ,0x71374491 ,0xb5c0fbcf ,0xe9b5dba5
		,0x3956c25b ,0x59f111f1 ,0x923f82a4 ,0xab1c5ed5
		,0xd807aa98 ,0x12835b01 ,0x243185be ,0x550c7dc3
		,0x72be5d74 ,0x80deb1fe ,0x9bdc06a7 ,0xc19bf174
		,0xe49b69c1 ,0xefbe4786 ,0x0fc19dc6 ,0x240ca1cc
		,0x2de92c6f ,0x4a7484aa ,0x5cb0a9dc ,0x76f988da
		,0x983e5152 ,0xa831c66d ,0xb00327c8 ,0xbf597fc7
		,0xc6e00bf3 ,0xd5a79147 ,0x06ca6351 ,0x14292967
		,0x27b70a85 ,0x2e1b2138 ,0x4d2c6dfc ,0x53380d13
		,0x650a7354 ,0x766a0abb ,0x81c2c92e ,0x92722c85
		,0xa2bfe8a1 ,0xa81a664b ,0xc24b8b70 ,0xc76c51a3
		,0xd192e819 ,0xd6990624 ,0xf40e3585 ,0x106aa070
		,0x19a4c116 ,0x1e376c08 ,0x2748774c ,0x34b0bcb5 
		,0x391c0cb3 ,0x4ed8aa4a ,0x5b9cca4f ,0x682e6ff3
		,0x748f82ee ,0x78a5636f ,0x84c87814 ,0x8cc70208
		,0x90befffa ,0xa4506ceb ,0xbef9a3f7 ,0xc67178f2
	};
	// Message schedule 
	uint32_t W[64];
	// Working variables 
	uint32_t a, b, c, d, e, f, g, h;
	// Temp variables 
	uint32_t T1, T2;
	
	// Intialising H constants hash values
	uint32_t H[8] = {
		0x6a09e667,
		0xbb67ae85,
		0x3c6ef372,
		0xa54ff53a,
		0x510e527f,
		0x9b05688c,
		0x1f83d9ab,
		0x5be0cd19
	};
	
	// Loop variables 
	int i, j;
	
	// Loops through message blocks
	while (nextmsgblock(msgf, &M, &S, &nobits)){
		
		for(int j = 0; j < 16; j++){
			W[j] = M.t[j];
		}
		
		for (j - 16; j < 64; j++){
			W[j] = sig1(W[j-2]) + W[j-7] + sig0(W[j-15]) + W[j-16];
		}		
		
		// Intialised variables
		a = H[0]; b = H[1]; c = H[2]; d = H[3]; 
		e = H[4]; f = H[5]; g = H[6]; h = H[7];
		
		// Step 3
		for (j = 0; j < 64; j ++){
			T1 = h + SIG1(e) + Ch(e, f, g) + K[j] + W[j];
			T2 = SIG0(a) + Maj(a, b, c);
			h = g;
			g = f;
			f = e;
			e = d + T1;
			d = c;
			c = b;
			b = a;
			a = T1+T2;
		}
		
		// Step 4
		H[0] = a + H[0];
		H[1] = b + H[1];
		H[2] = c + H[2];
		H[3] = d + H[3];
		H[4] = e + H[4];
		H[5] = f + H[5];
		H[6] = g + H[6];
		H[7] = h + H[7];
		}// i for

	printf("%x %x %x %x %x %x %x %x", H[0], H[2], H[3], H[4], H[5], H[6], H[7], H[8]);
}


int nextmsgblock(FILE *msgf, union msgblock *M, enum status *S, uint64_t *nobits){
		
	// Message block with the current number of bytes 
    uint64_t nobytes;
	
	// Loop variable
	int i;
	
	// If message blocks are finished, the S should finish
	if(*S == FINISH){
		return 0;
	}
	// Checks blocks of padding if not finished 
	if(*S == PAD0 || *S == PAD1){
		// Sets first 56 bits to zero bits
		for (i = 0; i < 56; i++){
			M->e[i] = 0X00;
			M->s[7] = *nobits;
			*S = FINISH;
		}
		//Big-endian not working
		// last 64 bit set to the number of bits in the file 
		if (*S == PAD1){
		M->e[0] = 0x80;
		}
		// Gives another iteration to the loop
		return 1;
	}// if
	
	// File isnt read fully
	nobytes = fread(M->e, 1, 64, msgf);

	// Checks number of bytes read in
	*nobits = *nobits + (nobytes * 8);
	
	// If 56 bytes are read, padding can be put in the message block 
	if (nobytes < 56) {
		M->e[nobytes] = 0x80;
		
		while (nobytes < 56) {
			nobytes = nobytes + 1;
			// Setting bites to 0
			M->e[nobytes] = 0x00;
		}
		M->s[7] = *nobits;
		// Finished 
		*S = FINISH;
	}
	// Checks if the message block can be padded 
	else if (nobytes < 64){
		*S = PAD0;
		M->e[nobytes] = 0x80;
		while (nobytes < 64) {
			nobytes + 1;
			M->e[nobytes] = 0x00;
		}
	}// else if
	// Checks if at the end of file
	else if (feof(msgf)){
		// Status changed to PAD1 to add another message block with padding 
		*S = PAD1;
	}	
	// 1 called again 
    return 1;
}

// Functions for hash 
uint32_t rotr(uint32_t n, uint32_t x){
	return(x >> n) | (x << (32 - n));
}
uint32_t shr(uint32_t n, uint32_t x){
	return (x >> n);
	
}

uint32_t sig0(uint32_t x){
	return (rotr(7, x) ^ rotr(18, x) ^ shr(3, x));	
}

uint32_t sig1(uint32_t x){
	return (rotr(17, x) ^ rotr(19, x) ^ shr(10, x));	

}

uint32_t SIG0(uint32_t x){
	(rotr(2, x) ^ rotr(19,x) ^ rotr(22, x));
}
uint32_t SIG1(uint32_t x){
	(rotr(6, x) ^ rotr(11,x) ^ rotr(2, x));

}

uint32_t Ch(uint32_t x, uint32_t y, uint32_t z){
	return ((x & y) ^ ((!x) & z));
	
}
uint32_t Maj(uint32_t x, uint32_t y, uint32_t z){
	return ((x & y) ^ (x & z) ^ (y & z));
}

//int checkEndian() {
	
//}
