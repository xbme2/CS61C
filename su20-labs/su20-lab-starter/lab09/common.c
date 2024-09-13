#include "common.h"
#include <emmintrin.h>
#include <stdio.h>
#include <time.h>
#include <x86intrin.h>

long long int sum(unsigned int vals[NUM_ELEMS]) {
  clock_t start = clock();

  long long int sum = 0;
  for (unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
    for (unsigned int i = 0; i < NUM_ELEMS; i++) {
      if (vals[i] >= 128) {
        sum += vals[i];
      }
    }
  }
  clock_t end = clock();
  printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
  return sum;
}

long long int sum_unrolled(unsigned int vals[NUM_ELEMS]) {
  clock_t start = clock();
  long long int sum = 0;

  for (unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
    for (unsigned int i = 0; i < NUM_ELEMS / 4 * 4; i += 4) {
      if (vals[i] >= 128)
        sum += vals[i];
      if (vals[i + 1] >= 128)
        sum += vals[i + 1];
      if (vals[i + 2] >= 128)
        sum += vals[i + 2];
      if (vals[i + 3] >= 128)
        sum += vals[i + 3];
    }

    // This is what we call the TAIL CASE
    // For when NUM_ELEMS isn't a multiple of 4
    // NONTRIVIAL FACT: NUM_ELEMS / 4 * 4 is the largest multiple of 4 less than
    // NUM_ELEMS
    for (unsigned int i = NUM_ELEMS / 4 * 4; i < NUM_ELEMS; i++) {
      if (vals[i] >= 128) {
        sum += vals[i];
      }
    }
  }
  clock_t end = clock();
  printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
  return sum;
}

long long int sum_simd(unsigned int vals[NUM_ELEMS]) {
  clock_t start = clock();
  __m128i _127 = _mm_set1_epi32(
      127); // This is a vector with 127s in it... Why might you need this?
  long long int result = 0; // This is where you should put your final result!
  /* DO NOT DO NOT DO NOT DO NOT WRITE ANYTHING ABOVE THIS LINE. */

  for (unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
  	unsigned int results[4];
    __m128i tempResult = _mm_setzero_si128();
    unsigned int i = 0;
    for (; i + 4 <= NUM_ELEMS; i += 4) {
      __m128i val = _mm_loadu_si128((__m128i *)&vals[i]);
      __m128i compare = _mm_cmpgt_epi32(val, _127);
      __m128i newval = _mm_and_si128(val, compare);
      tempResult = _mm_add_epi32(tempResult, newval);
    }
	_mm_storeu_si128((__m128i *)results, tempResult);
    /* You'll need a tail case. */
    for (; i < NUM_ELEMS; i++) {
      if (vals[i] >= 128)
	  	 results[0]+= vals[i];
    }
	for (unsigned int j = 0; j < 4; j++) {
		result += results[j];
	}
  }

  clock_t end = clock();
  printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
  return result;
}

long long int sum_simd_unrolled(unsigned int vals[NUM_ELEMS]) {
  clock_t start = clock();
  __m128i _127 = _mm_set1_epi32(127);
  long long int result = 0;
  for (unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
  	unsigned int results[4];
    __m128i tempResult = _mm_setzero_si128();
    unsigned int i = 0;
    for (; i < NUM_ELEMS / 16 * 16; i += 16) {
      __m128i val1 = _mm_loadu_si128((__m128i *)&vals[i]);
      __m128i val2 = _mm_loadu_si128((__m128i *)&vals[i+4]);
      __m128i val3 = _mm_loadu_si128((__m128i *)&vals[i+8]);
      __m128i val4 = _mm_loadu_si128((__m128i *)&vals[i+12]);

      __m128i compare1 = _mm_cmpgt_epi32(val1, _127);
      __m128i compare2 = _mm_cmpgt_epi32(val2, _127);
      __m128i compare3 = _mm_cmpgt_epi32(val3, _127);
      __m128i compare4 = _mm_cmpgt_epi32(val4, _127);

      __m128i newval1 = _mm_and_si128(val1, compare1);
      __m128i newval2 = _mm_and_si128(val2, compare2);
      __m128i newval3 = _mm_and_si128(val3, compare3);
      __m128i newval4 = _mm_and_si128(val4, compare4);

      tempResult = _mm_add_epi32(tempResult, newval1);
      tempResult = _mm_add_epi32(tempResult, newval2);
      tempResult = _mm_add_epi32(tempResult, newval3);
      tempResult = _mm_add_epi32(tempResult, newval4);

    }
	_mm_storeu_si128((__m128i *)results, tempResult);
    /* You'll need 1 or maybe 2 tail cases here. */
    for (; i < NUM_ELEMS; i++) {
      if (vals[i] >= 128)
	  	 results[0]+= vals[i];
    }
	for (unsigned int j = 0; j < 4; j++) {
		result += results[j];
	}
	
  }
  clock_t end = clock();
  printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
  return result;
}