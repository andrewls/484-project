#include <openssl/sha.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

// #define SECRET_STRING "HELLO"
// #define SECRET_STRING_LENGTH sizeof(SECRET_STRING)
#define SECRET_STRING_LENGTH 10
#define TOY_HASH_HEX_CHAR_LENGTH 10// corresponds directly to digest length in python
#define TOY_HASH_LENGTH_IN_BYTES (TOY_HASH_HEX_CHAR_LENGTH/2) // conversion to c comparison
#define SHA_FULL_HASH_LENGTH 20

char SECRET_STRING[SECRET_STRING_LENGTH + 1];
unsigned char hash_to_attack[TOY_HASH_LENGTH_IN_BYTES];
const unsigned char characters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ ";

double When() {
    struct timeval tp;
    gettimeofday(&tp, NULL);
    return ((double) tp.tv_sec + (double) tp.tv_usec * 1e-6);
}

void generate_secret_string() {
  int i;
  for (i = 0; i < SECRET_STRING_LENGTH; i++) {
    SECRET_STRING[i] = characters[rand() % strlen(characters)];
  }
  SECRET_STRING[SECRET_STRING_LENGTH] = NULL;
  printf("Generated random string %s\n", SECRET_STRING);
}

void toy_hash(char* data, size_t length, unsigned char * hash) {
  unsigned char long_hash[SHA_FULL_HASH_LENGTH];
  SHA1(data, length, long_hash);
  strncpy(hash, long_hash, TOY_HASH_LENGTH_IN_BYTES);
  hash[TOY_HASH_LENGTH_IN_BYTES] = NULL;
}

int main(int argc, char** argv) {
  int i, index, iterations = 0;
  int j;
  char string[SECRET_STRING_LENGTH + 1];
  unsigned char hash[TOY_HASH_LENGTH_IN_BYTES + 1];
  double start_time = When();
  int character_array_length = strlen(characters);

  srand(time(NULL));

  // start the attack
  printf("Starting a pre-image attack.\n");
  generate_secret_string();

  // first hash the string.
  toy_hash(SECRET_STRING, SECRET_STRING_LENGTH, hash_to_attack);
  printf("Hashing secret string %s to attack: ", SECRET_STRING);
  for (i = 0; i < TOY_HASH_LENGTH_IN_BYTES; i++) {
    printf("%02X", hash_to_attack[i]);
  }
  printf("\n");

  // now initialize our string to be just As
  for (i = 0; i < SECRET_STRING_LENGTH; i++) string[i] = characters[0];
  string[SECRET_STRING_LENGTH] = '\0';

  i = 0;
  while (i < character_array_length) {
    iterations++;
    string[SECRET_STRING_LENGTH - 1] = characters[i];
    // hash the current string
    toy_hash(string, SECRET_STRING_LENGTH, hash);

    if (strcmp(hash, hash_to_attack) == 0) {
      printf("String %s hashes to ", string);
      for (j = 0; j < TOY_HASH_LENGTH_IN_BYTES; j++) printf("%02X", hash[j]);
      printf(". Match found in %d iterations and %f seconds.\n", iterations, When() - start_time);
      return 0;
    }

    if (i == character_array_length - 1) {
      // if it's the last character, we need to bump up the previous characters
      index = SECRET_STRING_LENGTH - 1;
      while (index >= 0 && string[index] == characters[character_array_length - 1]) {
        string[index] = characters[0];
        index--;
      }
      // now we need to bump up the last character by 1
      if (index < 0) {
        // if this is the case, we didn't find a solution
        printf("Pre-image attack unsuccessful.\n");
      }
      else {
        int character_index;
        for (character_index = 0; character_index < character_array_length; character_index++) {
            if (string[index] == characters[character_index]) break;
        }
        string[index] = characters[character_index + 1];
      }
      // reset the character loop
      i = -1;
    }
    i++;
  }
  return 0;
}
