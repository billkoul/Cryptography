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

   printf("Generating 10 random numbers and checking if they are prime.\n");
   for(int i=1; i<=10; i++){
      mpz_urandomb(p1, stat, 10*i); 
      printf("number is = "); 
      mpz_out_str(stdout, 10, p1); 
	  printf(" with %d bits\n", 10*i);
      printf("Checking the primality of %d number:\n", i); 
      primetest = mpz_probab_prime_p(p1, 20);

      if (primetest != 0) { 
         printf("p1 is prime\n");
      } else {
         printf("p1 is composite\n");
      }
   }

return 0; 
}
