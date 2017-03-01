/*Miler-Rabin 4.24
MILLER-RABIN(n,t)
INPUT: an odd integer n ≥ 3 and security parameter t ≥ 1.
OUTPUT: an answer “prime” or “composite” to the question: “Is n prime?”
1. Write n − 1=2sr such that r is odd.
2. For i from 1 to t do the following:
2.1 Choose a random integer a, 2 ≤ a ≤ n − 2.
2.2 Compute y = ar mod n using Algorithm 2.143.
2.3 If y 	= 1 and y 	= n − 1 then do the following:
j←1.
While j ≤ s − 1 and y 	= n − 1 do the following:
Compute y←y2 mod n.
If y = 1 then return(“composite”).
j←j + 1.
If y 	= n − 1 then return (“composite”).
3. Return(“prime”).
*/
#include <gmp.h>
#include <time.h>
#include <stdio.h>

int isProbPrime(mpz_t n, mpz_t a)
{
	//Init: Set r ← n − 1. Set s ← 0.
    mpz_t r, n1, y;
    mpz_inits(r, n1, y, NULL);
    mpz_sub_ui(n1, n, 1);
    mpz_set(r, n1);
    int s = 0;

	//Step 1: if r is even, set r ← r / 2, set s ← s + 1, and go to Step 2.
    while (mpz_even_p(r))
    {
        mpz_divexact_ui(r, r, 2);
        s += 1;
    }

	for(int i=0; i<t; t++){
		//Step 2.2: Set y ← a^r (mod n)
		mpz_powm(y, a, r, n);
	
		//Step 2.3 If y = 1 or y = n − 1, return PROBABLY PRIME and stop.
		if (mpz_cmp_ui(y, 1) == 0 || mpz_cmp(y, n1) == 0)
		{
			mpz_clears(r, n1, y, NULL);
			return 1;
		}
	}

	//Set s ← s − 1. If s = 0, output COMPOSITE and stop.
    while (--s > 0)
    {
		//Set y = y^2 (mod n). If y = n − 1, output PROBABLY PRIME and stop. else go to Step 4.
        mpz_mul(y, y, y);
        mpz_mod(y, y, n);
        if (mpz_cmp(y, n1) == 0)
        {
            mpz_clears(r, n1, y, NULL);
            return 1;
        }
    }

    mpz_clears(r, n1, y, NULL);
    return 0;
}

int main(int argc, char *argv[])
{
	mpz_t a, n;
    mpz_init(a);
	mpz_init(n);
    mpz_set_ui(n, 413413417);
	
    clock_t begin = clock();
	
	if(isProbPrime(n, a))
		printf("Is probably prime!\n");
	else
		printf("Is composite\n");
	
	clock_t end = clock();
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("Algorithm executed in %f seconds\n", time_spent);
	
	return 0;
}
