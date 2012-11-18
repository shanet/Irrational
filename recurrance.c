// shane tully
// shane@shanetully.com
// shanetully.com
//
// http://en.wikipedia.org/wiki/Borwein%27s_algorithm#Cubic_convergence_.281991.29

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <mpfr.h>
 
#define DEFAULT_PRECISION 1000000UL
#define MAX_ITERS         100UL
#define CHUNK_SIZE        1000

unsigned long check_digits(char *pi);

unsigned long precision;


int main(int argc, char **argv) {
    mpfr_t tmp1;
    mpfr_t tmp2;
    mpfr_t tmp3;
    mpfr_t s1;
    mpfr_t s2;
    mpfr_t r;
    mpfr_t a1;
    mpfr_t a2;

    time_t start_time;
    time_t end_time;

    // Parse command line opts
    int hide_pi = 0;
    if(argc == 2) {
        if(strcmp(argv[1], "--hide-pi") == 0) {
            hide_pi = 1;
        }

        precision = DEFAULT_PRECISION;
    } else if(argc == 3) {
        if(strcmp(argv[1], "--hide-pi") == 0) {
            hide_pi = 1;
        }

        if((precision = atoi(argv[2])) == 0) {
            fprintf(stderr, "Invalid precision specified. Aborting.\n");
            return 1;
        }
    } else {
        precision = DEFAULT_PRECISION;
    }

    // Actual number of correct digits is roughly 3.35 times the requested precision
    precision *= 3.35;

    mpfr_set_default_prec(precision);

    mpfr_inits(tmp1, tmp2, tmp3, s1, s2, r, a1, a2, NULL);

    start_time = time(NULL);

    // a0 = 1/3
    mpfr_set_ui(a1, 1, MPFR_RNDN);
    mpfr_div_ui(a1, a1, 3, MPFR_RNDN);

    // s0 = (3^.5 - 1) / 2
    mpfr_sqrt_ui(s1, 3, MPFR_RNDN);
    mpfr_sub_ui(s1, s1, 1, MPFR_RNDN);
    mpfr_div_ui(s1, s1, 2, MPFR_RNDN);

    unsigned long i = 0;
    while(i < MAX_ITERS) {
        // r = 3 / (1 + 2(1-s^3)^(1/3))
        mpfr_pow_ui(tmp1, s1, 3, MPFR_RNDN);
        mpfr_ui_sub(r, 1, tmp1, MPFR_RNDN);
        mpfr_root(r, r, 3, MPFR_RNDN);
        mpfr_mul_ui(r, r, 2, MPFR_RNDN);
        mpfr_add_ui(r, r, 1, MPFR_RNDN);
        mpfr_ui_div(r, 3, r, MPFR_RNDN);

        // s = (r - 1) / 2
        mpfr_sub_ui(s2, r, 1, MPFR_RNDN);
        mpfr_div_ui(s2, s2, 2, MPFR_RNDN);

        // a = r^2 * a - 3^i(r^2-1)
        mpfr_pow_ui(tmp1, r, 2, MPFR_RNDN);
        mpfr_mul(a2, tmp1, a1, MPFR_RNDN);
        mpfr_sub_ui(tmp1, tmp1, 1, MPFR_RNDN);
        mpfr_ui_pow_ui(tmp2, 3UL, i, MPFR_RNDN);
        mpfr_mul(tmp1, tmp1, tmp2, MPFR_RNDN);        
        mpfr_sub(a2, a2, tmp1, MPFR_RNDN);
        
        // s1 = s2
        mpfr_set(s1, s2, MPFR_RNDN);
        // a1 = a2
        mpfr_set(a1, a2, MPFR_RNDN);

        i++;
    }

    // pi = 1/a
    mpfr_ui_div(a2, 1, a2, MPFR_RNDN);

    end_time = time(NULL);

    // Write the digits to a string for accuracy comparison
    char *pi = malloc(precision + 100);
    if(pi == NULL) {
        fprintf(stderr, "Failed to allocated memory for output string.\n");
        return 1;
    }

    mpfr_sprintf(pi, "%.*R*f", precision, MPFR_RNDN, a2);

    // Check out accurate we are
    unsigned long accuracy = check_digits(pi);

    // Print the results
    if(!hide_pi) printf("%s\n", pi);
    // Send the time and accuracy to stderr so pi can be redirected to a file if necessary
    fprintf(stderr, "Time: %d seconds\nAccuracy: %lu digits\n", (int)(end_time - start_time), accuracy);

    mpfr_clears(tmp1, tmp2, tmp3, s1, s2, r, a1, a2, NULL);

    return 0;
}


unsigned long check_digits(char *pi) {
    // If pi.txt exists, compare the digits
    FILE *digits = fopen("pi.txt", "r");
    char *buffer = malloc(CHUNK_SIZE);
    size_t chunk_size;
    unsigned long bytes_read = 0;
    unsigned long accuracy = 0;

    if(digits == NULL || buffer == NULL) {
        return 0;
    }

    while(!feof(digits)) {
        chunk_size = fread(buffer, 1, 100, digits);

        if(ferror(digits)) {
            fprintf(stderr, "Error while reading pi.txt.\n");
            break;
        }

        // Compare the digits
        for(unsigned int i=bytes_read, j=0; i<bytes_read+chunk_size; i++, j++) {
            if(i > precision || buffer[j] != pi[i]) {
                goto outer;
            }
            accuracy++;
        }
        bytes_read += chunk_size;
    }

    outer:
    fclose(digits);
    free(buffer);
    buffer = NULL;

    // Don't count the "3." as accurate digits
    return (accuracy > 2) ? accuracy-2 : accuracy;
}