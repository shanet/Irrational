// shane tully
// shane@shanetully.com
// shanetully.com
//
// http://en.wikipedia.org/wiki/Machin-like_formula#More_terms

#include "machin.h"

int main(int argc, char **argv) {
    // Thread ID's and term nums
    pthread_t tids[7];
    int terms[] = {1, 2, 3, 4, 5, 6, 7};

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

    // Init the mpfr vars
    mpfr_set_default_prec(precision);
    mpfr_inits(t1, t2, t3, t4, t5, t6, t7, one, pi, NULL);
    mpfr_set_ui(pi, 0, MPFR_RNDN);
    mpfr_set_ui(one, 1, MPFR_RNDN);

    // Get the start time of the calculations
    start_time = time(NULL);

    // Init the thread mutex
    pthread_mutex_init(&sum_mutex, NULL);

    // Start a thread for the calculations of each of the terms
    for(int i=0; i<7; i++) {
        if(pthread_create(&(tids[i]), NULL, calc_term, (void*)&terms[i]) != 0) {
            fprintf(stderr, "Error creating thread: %s. Aborting.\n", strerror(errno));
        }
    }

    // Wait for the threads to finish
    for(int i=0; i<7; i++) {
        pthread_join(tids[i], NULL);
    }

    // pi = 4*pi
    mpfr_mul_ui(pi, pi, 4, MPFR_RNDN);

    // All calculations are done. Store the end time
    end_time = time(NULL);

    // Convert the digits to a (really long) string for accuracy comparison and output
    char *pi_str = malloc(precision + 100);
    if(pi_str == NULL) {
        fprintf(stderr, "Failed to allocated memory for output string.\n");
        return 1;
    }
    mpfr_sprintf(pi_str, "%.*R*f", precision, MPFR_RNDN, pi);

    // Check out accurate we are
    unsigned long accuracy = check_digits(pi_str);

    // Print the results
    if(!hide_pi) printf("%s\n", pi_str);
    // Send the time and accuracy to stderr so pi can be redirected to a file if necessary
    fprintf(stderr, "Time: %d seconds\nAccuracy: %lu digits\n", (int)(end_time - start_time), accuracy);

    mpfr_clears(one, pi, NULL);
    free(pi_str);
    pi_str = NULL;

    return 0;
}


void* calc_term(void* term_num) {
    char x_str[22];
    int op;
    int coeff;
    mpfr_t *term;

    switch(*(int *)term_num) {
        case 1:
            term = &t1;
            coeff = 183;
            op = ADD;
            sprintf(x_str, "239");
            break;
        case 2:
            term = &t2;
            coeff = 32;
            op = ADD;
            sprintf(x_str, "1023");
            break;
        case 3:
            term = &t3;
            coeff = 68;
            op = SUB;
            sprintf(x_str, "5832");
            break;
        case 4:
            term = &t4;
            coeff = 12;
            op = ADD;
            sprintf(x_str, "113021");
            break;
        case 5:
            term = &t5;
            coeff = 100;
            op = SUB;
            sprintf(x_str, "6826318");
            break;
        case 6:
            term = &t6;
            coeff = 12;
            op = SUB;
            sprintf(x_str, "33366019650");
            break;
        case 7:
            term = &t7;
            coeff = 12;
            op = ADD;
            sprintf(x_str, "43599522992503626068");
            break;
        default:
            pthread_exit(NULL);
    }

    // Default precision is local to each thread so it must be specified here again
    mpfr_t x;
    mpfr_set_default_prec(precision);
    mpfr_init_set_str(x, x_str, 10, MPFR_RNDN);

    // t1 = 183 * atan(1/239)
    // t2 = 32 * atan(1/1023)
    // t3 = 68 * atan(1/5832)
    // t4 = 12 * atan(1/113021)
    // t5 = 100 * atan(1/6826318)
    // t6 = 12 * atan(1/33366019650)
    // t7 = 12 * atan(1/43599522992503626068)
    mpfr_atan2(*term, one, x, MPFR_RNDN);
    mpfr_mul_ui(*term, *term, coeff, MPFR_RNDN);

    // Add the term to the sum
    pthread_mutex_lock(&sum_mutex);
    if(op == ADD) {
        mpfr_add(pi, pi, *term, MPFR_RNDN);
    } else {
        mpfr_sub(pi, pi, *term, MPFR_RNDN);
    }
    pthread_mutex_unlock(&sum_mutex);

    // We're done with the current term and x value
    mpfr_clears(*term, x, NULL);

    mpfr_free_cache();
    pthread_exit(NULL);
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