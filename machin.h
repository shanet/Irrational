// shane tully
// shane@shanetully.com
// shanetully.com

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <time.h>
#include <mpfr.h>
 
#define DEFAULT_PRECISION 1000000UL
#define CHUNK_SIZE        1000

#define ADD 0
#define SUB 1

mpfr_t t1;
mpfr_t t2;
mpfr_t t3;
mpfr_t t4;
mpfr_t t5;
mpfr_t t6;
mpfr_t t7;
mpfr_t one;
mpfr_t pi;

unsigned long precision;

pthread_mutex_t sum_mutex;

unsigned long check_digits(char *pi);
void* calc_term(void* term_num);