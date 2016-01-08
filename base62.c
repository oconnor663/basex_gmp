#include<stdio.h>
#include<string.h>
#include<gmp.h>
#include<math.h>

// int main() {
//   char s[100];
//   mpz_t x;
//   mpz_init_set_ui(x, 3843);
//   mpz_get_str(s, 62, x);
//   printf("%s\n", s);
//   mpz_add_ui(x, x, 1);
//   mpz_get_str(s, 62, x);
//   printf("%s\n", s);
//   size_t count;
//   memset(s, 0, 100);
//   mpz_export(s, &count, 1 /*big*/, 1 /*1 byte words*/, 1 /*big*/, 0 /*nails*/, x);
//   for (int i=0; i<count; i++) {
//     printf("%d ", s[i]);
//   }
//   printf("\n");
//   mpz_clear(x);
// }

float LOG2_62;

size_t min_chars_for_bytes(size_t num_bytes) {
  // if (num_bytes == 32) {
  //   return 43;
  // }
  return ceilf(num_bytes * 8 / LOG2_62);
}

int main() {
  // initialize the log_2(A)
  LOG2_62 = log2f(62);

  char buffer[32];
  char output[44];
  size_t count;
  mpz_t bignum;
  mpz_init(bignum);
  while (1) {
    count = fread(buffer, 1, 32, stdin);
    if (count == 0) {
      break;
    }
    mpz_import(bignum, count, 1, 1, 1, 0, buffer);
    // convert to digits
    mpz_get_str(output, 62, bignum);
    size_t got_digits = strlen(output);
    size_t required_digits = min_chars_for_bytes(count);
    // print any extra padding zeroes
    for (int i=0; i < required_digits - got_digits; i++) {
      printf("0");
    }
    // print the converted digits
    printf("%s ", output);
  }
  printf("\n");
  mpz_clear(bignum);
}
