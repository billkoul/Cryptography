//with a litte help from wikipedia

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gmp.h>
#include <sys/types.h>
#include <unistd.h>
 
int main(void)
{
	mpz_t p, q, n, d, e, phi, pt, ct, tmp1, tmp2, seed, f, pf, qf;
	gmp_randstate_t stat;

	mpz_init(pt);
	mpz_init(ct);
	mpz_init(p);
    mpz_init(q);
    mpz_init(e);
    mpz_init(n);
	mpz_init(d);
	mpz_init(f);
	mpz_init(pf);
	mpz_init(qf);
	mpz_init(tmp1);
	mpz_init(tmp2);
	mpz_init(phi);
	mpz_init(seed);
	long sd = 0;

	gmp_randinit(stat, GMP_RAND_ALG_LC, 120);
	srand( (unsigned) getpid());
	sd=rand();
	mpz_set_ui(seed, sd);
	gmp_randseed(stat, seed);

	int found = 0;
	printf("Generating prime number p...\n"); 
	while(found == 0){ 
		mpz_urandomb(p, stat, 512); //512bits
		int primetest = mpz_probab_prime_p(p, 10);
		if (primetest != 0) { found = 1; }
	}

	found = 0;
	printf("Generating prime number q...\n"); 
	while(found == 0){
		mpz_urandomb(q, stat, 512); //512bits
		int primetest = mpz_probab_prime_p(q, 10);
		if (primetest != 0) { found = 1; }
	}

	mpz_mul(n, p, q);
	mpz_set_ui(e, 3);
	/*
	found = 1;
	while(found == 0){
		mpz_urandomb(e, stat, 512); //512bits
		
		mpz_sub_ui(pf, p, 1);
		mpz_sub_ui(qf, q, 1);
		mpz_mul(f, pf, qf);
		
		mpz_gcd(tmp1, e, f);
		if (mpz_cmp_ui(tmp1, 1) == 0) { found = 1; }
	}
	*/
	mpz_sub_ui(tmp1, p, 1);
    mpz_sub_ui(tmp2, q, 1);
    mpz_mul(phi, tmp1, tmp2);

    /* Calculate d (multiplicative inverse of e mod phi) 
	Compute the inverse of op1 modulo op2 and put the result in rop*/
    mpz_invert(d, e, phi);

    const char *plaintext = "Plaintext";
    mpz_import(pt, strlen(plaintext), 1, 1, 0, 0, plaintext);
 
    if (mpz_cmp(pt, n) > 0)
        abort();
 
    mpz_powm(ct, pt, e, n); //ciphertext = plaintext^e mod n
    gmp_printf("Encoded:   %Zd\n", ct);
 
    mpz_powm(pt, ct, d, n); //plaintext = ciphertext^d mod n
    gmp_printf("Decoded:   %Zd\n", pt);
 
    char buffer[64];
    mpz_export(buffer, NULL, 1, 1, 0, 0, pt);
    printf("Plaintext: %s\n", buffer);

   	return 0;
}
