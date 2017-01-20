/*
 * File        : divA.h
 * Author      : frc
 * Date        : 2003/11/24 15:53:08
 * Description : Utilities for the course "Introduction to the C programming language
 */

#ifndef __divA_h__
#define __divA_h__

#include <time.h>
#include <stdlib.h>


/* 
 * set de seed of the random generator 
 */

void set_random_generator_seed (unsigned int seed)
{
  srand(seed);
}

/* 
 * initialize the random generator with the value of the current time
 */

void init_random_generator() 
{
    time_t currentTime;

    (srand(time (&currentTime)));
}

/*
 * Generates a random integer whose value is between 0 and max_number-1
 */

int random_int (int max_number)
{
    return (int)(((float)rand()/(RAND_MAX)) * max_number);
}

/* -------------------------------------------------------------------------*/
void empty_stdin()
{
    while (getc(stdin) != '\n') {}
}


int min (int a, int b)
{
   return (a<b?a:b);
}

int max (int a, int b)
{
   return (a>b?a:b);
}



#endif
