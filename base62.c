#include <stdio.h>
#include <string.h>
#include <gmp.h>
#include <math.h>
#include <stdbool.h>

float LOG2_62;

size_t min_chars_for_bytes(size_t num_bytes) {
  return ceilf(num_bytes * 8 / LOG2_62);
}

void print_chars(mpz_t* bignum, char* bytes_buffer, size_t bytes_count) {
  // import our bytes into the bignum
  mpz_import(*bignum, bytes_count, 1, 1, 1, 0, bytes_buffer);
  char output[44];  // space for terminating null
  mpz_get_str(output, 62, *bignum);
  size_t got_digits = strlen(output);
  size_t required_digits = min_chars_for_bytes(bytes_count);
  // print any leading zeroes
  for (int i=0; i < required_digits - got_digits; i++) {
    putchar('0');
  }
  // print the converted digits
  printf("%s", output);
}

void encode() {
  char buffer[32];
  size_t count;
  mpz_t bignum;
  mpz_init(bignum);
  bool first = true;
  while (true) {
    count = fread(buffer, 1, 32, stdin);
    if (count == 0) {
      break;
    }
    // print the separating space
    if (!first) {
      putchar(' ');
    }
    first = false;
    // print the bignum in base 62
    print_chars(&bignum, buffer, count);
  }
  putchar('\n');
  mpz_clear(bignum);
}

bool is_base62_char(char c) {
  return (('0' <= c && c <= '9') ||
          ('a' <= c && c <= 'z') ||
          ('A' <= c && c <= 'Z'));
}

size_t max_bytes_for_chars(size_t num_chars) {
  return floorf(num_chars * LOG2_62 / 8);
}

void print_bytes(mpz_t* bignum, char* chars) {
  size_t num_chars = strlen(chars);
  mpz_set_str(*bignum, chars, 62);
  char output_block[32];
  size_t got_bytes;
  mpz_export(output_block, &got_bytes, 1, 1, 1, 0, *bignum);
  // print any leading zero bytes
  size_t required_bytes = max_bytes_for_chars(num_chars);
  for (int i=0; i < required_bytes - got_bytes; i++) {
    putchar('\0');
  }
  fwrite(output_block, 1, got_bytes, stdout);
}

void decode() {
  char buffer[1024];
  char input_block[44]; // space for terminating null
  size_t block_index = 0;
  input_block[43] = '\0';  // make sure the input_block is null-terminated
  mpz_t bignum;
  mpz_init(bignum);
  while (true) {
    size_t buffer_count = fread(buffer, 1, 1024, stdin);
    if (buffer_count == 0) {
      break;
    }
    for (int i=0; i<buffer_count; i++) {
      char c = buffer[i];
      if (is_base62_char(c)) {
        input_block[block_index] = c;
        block_index = (block_index + 1) % 43;
        // If we just filled the block, serialize it.
        if (block_index == 0) {
          print_bytes(&bignum, input_block);
        }
      }
    }
  }
  // Decode any remaining characters.
  if (block_index > 0) {
    input_block[block_index] = '\0';  // make sure it's null-terminated again
    print_bytes(&bignum, input_block);
  }
}

int main(int argc, char** argv) {
  LOG2_62 = log2f(62);  // initialize the log_2(A)
  if (argc >= 2 && strcmp(argv[1], "-d") == 0) {
    decode();
  } else {
    encode();
  }
}
