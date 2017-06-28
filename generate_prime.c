#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <sys/types.h>
#include <unistd.h>

gmp_randstate_t stat;

int main(void)
{
	int primetest;
	long sd = 0;
	mpz_t p1, m;
	mpz_t seed;
	gmp_randinit(stat, GMP_RAND_ALG_LC, 120);
	mpz_init(p1);
	mpz_init(m);
	mpz_init(seed);
	srand( (unsigned) getpid());
	sd=rand();
	mpz_set_ui(seed, sd);
	gmp_randseed(stat, seed);

	int bits, t;

	printf("Give Bits: ");
	scanf("%d", &bits); 
	printf("Give T: ");
	scanf("%d", &t); 
   
	int found = 0;
	printf("Generating prime number...\n"); 
	while(found == 0){ 
		mpz_urandomb(p1, stat, bits); 
		primetest = mpz_probab_prime_p(p1, t);
		if (primetest != 0) {
			printf("Prime number is : "); 
			mpz_out_str(stdout, 10, p1); 
			printf(" with %d bits.\n", bits);
			found = 1;
		}
	}
	return 0; 
}
