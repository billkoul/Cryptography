#include <stdlib.h>
#include <stdio.h>
#include <gmp.h>
#include <sys/types.h>
#include <unistd.h>

//Variables
mpz_t p, q, n, c, m, nd, pd, qd, x, y, r, s, pp, qq, r1, r2, r3, r4, xr, yr, a, b, ab, temp, x1, x2, y1, y2, _q, _r, seed, check1, check2, check3;
gmp_randstate_t stat;
long sd = 0;

//Functions
int encrypt(mpz_t p, mpz_t q);

/* ***************** Encrypt message ******************* */
int main(){
	mpz_init(p);
	mpz_init(q);
	mpz_init(n);
	mpz_init(c);
	mpz_init(m);
	mpz_init(nd);
	mpz_init(pd);
	mpz_init(qd);
	mpz_init(pp);
	mpz_init(qq);
	mpz_init(r);
	mpz_init(s);
	mpz_init(x);
	mpz_init(y);
	mpz_init(r1);
	mpz_init(r2);
	mpz_init(r3);
	mpz_init(r4);
	mpz_init(xr);
	mpz_init(yr);
	mpz_init(a);
	mpz_init(b);
	mpz_init(ab);
	mpz_init(temp);
	mpz_init(x1);
	mpz_init(x2);
	mpz_init(y1);
	mpz_init(y2);
	mpz_init(_q);
	mpz_init(_r);
	mpz_init(seed);
	mpz_init(check1);
	mpz_init(check2);
	mpz_init(check3);
	
   	gmp_randinit(stat, GMP_RAND_ALG_LC, 120);
	srand( (unsigned) getpid());
   	sd=rand();
	mpz_set_ui(seed, sd);
	gmp_randseed(stat, seed);
	
	//create p such that p = 3 mod 4
	do{
		mpz_urandomb(p, stat, 20);
		mpz_set_ui(check1, 0);
		mpz_set_ui(check2, 0);
		mpz_sub_ui(check1, p, 3);
		mpz_set_ui(check3, 4);
		mpz_mod(check2, check3, check1);
	}while(mpz_probab_prime_p(p, 20) != 0 || mpz_cmp_ui(check2, 0) != 0);
	//[DEBUG]printf("\nP: ");
    	//[DEBUG]mpz_out_str(stdout, 10, p);
	
	//create q such that q = 3 mod 4
	do{
		mpz_urandomb(q, stat, 20);
		mpz_set_ui(check1, 0);
		mpz_set_ui(check2, 0);
		mpz_sub_ui(check1, q, 3);
		mpz_set_ui(check3, 4);
		mpz_mod(check2, check3, check1);
	}while(mpz_probab_prime_p(q, 20) == 0 || mpz_cmp_ui(check2, 0) != 0);
	//[DEBUG]printf("\nQ: ");
    	//[DEBUG]mpz_out_str(stdout, 10, q);
	
	//calculate n = p * q
	mpz_mul(n, p, q);
	//[DEBUG]printf("\nN: ");
    	//[DEBUG]mpz_out_str(stdout, 10, n);

	char plaintext[8] = "vasilis";
	unsigned long ciphertext[7];

	for(int i=0; i<7; i++){
		ciphertext[i] = plaintext[i];
		int last = (ciphertext[i] % 1000) * 1000;
		ciphertext[i] += last;
	}
	for(int i=0; i<7; i++){
		mpz_set_ui(m, ciphertext[i]);	
	
		//encrypt c = m^2 % n
		mpz_powm_ui(c, m, 2, n);
	
		printf("\n[E]Plaintext: ");
    		mpz_out_str(stdout, 10, m);
		printf("\n[E]Encrypted: ");
    		mpz_out_str(stdout, 10, c);
	
		encrypt(p, q);
	}
	return 0;
}


/* ********************* decrypt Message ********************* */
int encrypt(mpz_t p, mpz_t q){
	//Extended Euclidean Algorithm 
	mpz_set(a, p);
	mpz_set(b, q);
 	if (mpz_cmp(b, a) > 0) {
		mpz_set(temp, a);
		mpz_set(a, b);
		mpz_set(b, temp);
    	}
    	mpz_set_ui(x1, 0);
    	mpz_set_ui(y1, 1);
    	mpz_set_ui(x2, 1); 
    	mpz_set_ui(y2, 0); 
    	while (mpz_cmp_ui(b, 0) != 0) {
		mpz_set_ui(temp, 0);
		mpz_div(_q, a, b); 
        mpz_mul(temp, _q, b);
        mpz_sub(_r, a, temp);
        	
		mpz_set_ui(temp, 0);
		mpz_mul(temp, _q, x1);
        mpz_sub(x, x2, temp);
	        	
		mpz_set_ui(temp, 0);
		mpz_mul(temp, _q, y1);
		mpz_sub(y, y2, temp);

		mpz_set(a, b);
		mpz_set(b, _r);
		mpz_set(x2, x1);
		mpz_set(x1, x);
		mpz_set(y2, y1);
		mpz_set(y1, y);
    	}
	mpz_set(y, x2);
	mpz_set(x, y2);

	//[DEBUG]printf("\nx: ");
	//[DEBUG]mpz_out_str(stdout, 10, x);
	//[DEBUG]printf("\ny: ");
	//[DEBUG]mpz_out_str(stdout, 10, y);

	//calculate n = p * q
	mpz_mul(n, p, q);
	
	//calculate r=(pow(c,((p+1)/4),p))
	mpz_add_ui(pd, p, 1);
	mpz_div_ui (pd, pd, 4);
	mpz_powm(r, c, pd, p);
	//[DEBUG]printf("\nr: ");
	//[DEBUG]mpz_out_str(stdout, 10, r);
	
	//calculate s=(pow(c,((q+1)/4),q))
	mpz_add_ui(qd, q, 1);
	mpz_div_ui(qd, qd, 4);
	mpz_powm(s, c, qd, q);
	//[DEBUG]printf("\ns: ");
	//[DEBUG]mpz_out_str(stdout, 10, s);
	
	//calculate r1=((x*p*s)+(y*q*r))%n, r2=((x*p*s)-(y*q*r))%n, r3=(-r1)%n, r4=(-r2)%n ?(Chinese Remainder Theorem)
	mpz_mul(x, x, p);
	mpz_mul(x, x, s);
	mpz_mul(y, y, q);
	mpz_mul(y, y, r);
	mpz_add(xr, x, y);
	mpz_sub(yr, x, y);
	mpz_mod(r1, xr, n);
	mpz_mod(r3, yr, n);
	mpz_sub(r2, n, r1);
	mpz_sub(r4, n, r3);
	
	//[DEBUG]printf("\nr1: ");
    	//[DEBUG]mpz_out_str(stdout, 10, r1);
	//[DEBUG]printf("\nr2: ");
    	//[DEBUG]mpz_out_str(stdout, 10, r2);
	//[DEBUG]printf("\nr3: ");
    	//[DEBUG]mpz_out_str(stdout, 10, r3);
	//[DEBUG]printf("\nr4: ");
    	//[DEBUG]mpz_out_str(stdout, 10, r4);
	//[DEBUG]printf("\n");

	unsigned long root;
	
	root = mpz_get_ui(r1);	
	int part1 = root / 1000;
	int part2 = root % 1000;

	if(part1 == part2){
		char letter = part1;
		printf("\n[D]Letter: %c\n", letter);
	} else {	
		root = mpz_get_ui(r2);	
		int part1 = root / 1000;
		int part2 = root % 1000;
		if(part1 == part2){
			char letter = part1;
			printf("\n[D]Letter: %c\n", letter);
		} else {
			root = mpz_get_ui(r3);	
			part1 = root / 1000;
			part2 = root % 1000;

			if(part1 == part2){
				char letter = part1;
				printf("\n[D]Letter: %c\n", letter);
			} else {
				root = mpz_get_ui(r4);	
				part1 = root / 1000;
				part2 = root % 1000;

				if(part1 == part2){
					char letter = part1;
					printf("\n[D]Letter: %c\n", letter);
				}	 
			} 
		}
	}
	return 0;
}

