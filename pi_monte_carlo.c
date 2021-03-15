/* File:      pi_monte_carlo.c
 * Purpose:   Estimate pi using OpenMP and a monte carlo method
 * 
 * Compile:   gcc -fopenmp -o pi_monte_carlo 
 *                  pi_monte_carlo.c my_rand.c
 *            *needs my_rand.c, my_rand.h
 *
 * Run:       ./pi_monte_carlo <number of threads>
 *                  <number of tosses>
 *
 * Input:     None
 * Output:    Estimate of pi
 *
 * Note:      The estimated value of pi depends on both the number of 
 *            threads and the number of "tosses".  
 */

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "my_rand.h"

/* Serial function */
void Get_args(char* argv[], int* thread_count_p, 
      long long int* number_of_tosses_p);
void Usage(char* prog_name);

/* Parallel function */
long long int Count_hits(long long int number_of_tosses, int thread_count);

double get_random_zo();

/*---------------------------------------------------------------------*/
int main(int argc, char* argv[]) {
   double pi_estimate;
   int thread_count;
   long long int number_in_circle;
   long long int number_of_tosses;
   
   if (argc != 3) Usage(argv[0]);
   Get_args(argv, &thread_count, &number_of_tosses);
   
   number_in_circle = Count_hits(number_of_tosses, thread_count);

   pi_estimate = 4*number_in_circle/((double) number_of_tosses);
   printf("Estimated pi: %e\n", pi_estimate);

}

/*---------------------------------------------------------------------
 * Function:      Count_hits
 * Purpose:       Calculate number of hits in the unit circle
 * In arg:        number_of_tosses, thread_count
 * Return val:    number_in_circle
 */

long long int Count_hits(long long int number_of_tosses, int thread_count) {
   int i; 
   unsigned seed = 1, xx;
   double y, x;
   long long int hits = 0;
   #pragma omp parallel num_threads(thread_count) shared(seed, xx) private(i) reduction(+: hits)
    {
      xx = my_rand(&seed);
      #pragma omp for
      for (i = 0; i < number_of_tosses; i++) {
         y = my_drand(&xx);
         x = my_drand(&xx);
         double dist = (x*x) + (y*y);
         if(dist <= 1) {
            hits++;
         }
      }
   }

   
   return hits;
  
}  /* Count_hits */

/*---------------------------------------------------------------------
 * Function:  Usage 
 * Purpose:   Print a message showing how to run program and quit
 * In arg:    prog_name:  the name of the program from the command line
 */

void Usage(char prog_name[] /* in */) {
   fprintf(stderr, "usage: %s ", prog_name); 
   fprintf(stderr, "<number of threads> <total number of tosses>\n");
   exit(0);
}  /* Usage */

/*------------------------------------------------------------------
 * Function:    Get_args
 * Purpose:     Get the command line args
 * In args:     argv
 * Out args:    thread_count_p, number_of_tosses_p
 */

void Get_args(
           char*           argv[]              /* in  */,
           int*            thread_count_p      /* out */,
           long long int*  number_of_tosses_p  /* out */) {
   
   *thread_count_p = strtol(argv[1], NULL, 10);  
   *number_of_tosses_p = strtoll(argv[2], NULL, 10);
}  /* Get_args */

