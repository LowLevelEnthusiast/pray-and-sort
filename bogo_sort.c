#include "helper.h"
#include <fcntl.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
void bogosort(uint8_t *arr, int n);
void random_number_generator(uint8_t *arr, int n);
void array_printer(uint8_t *arr, int n);
bool is_sorted(uint8_t *arr, int n);
int main(void) {
  int n;
  printf("Number of random numbers to be generated: ");
  scanf("%d", &n);
  getchar();
  uint8_t arr[n];
  random_number_generator(arr, n);
  printf("Randomly Generated Array: ");
  array_printer(arr, n);
  printf("Press any key to start bogosort on this : ");
  getchar();
  bogosort(arr, n);
  printf("Final sorted array: ");
  array_printer(arr, n);
  return 0;
}

bool is_sorted(uint8_t *arr, int n) {
  for (int i = 0; i < n - 1; i++) {
    if (arr[i] > arr[i + 1]) {
      return false;
    }
  }
  return true;
}

void bogosort(uint8_t *arr, int n) {
  if (is_sorted(arr, n)) {
    return;
  }
  long long unsigned int tries = 0;
  unsigned int index;
  int random = open("/dev/urandom", O_RDONLY);
  if (random == -1) {
    perror("Open: ");
    return;
  }
  cursor_hide();
  while (!is_sorted(arr, n)) {
    tries++;
    printf("Current tries: %llu\n", tries);
    fflush(stdout);
    for (int i = n - 1; i > 0; i--) {
      read(random, &index, sizeof(index));
      index = index % (i + 1);
      int temp = arr[index];
      arr[index] = arr[i];
      arr[i] = temp;
    }
    if (tries % 100 == 0) {
      array_printer(arr, n);
      cursor_move_up(1);
    }
    cursor_move_up(1);
  }
  cursor_move_up(1);
  cursor_move_home();
  line_clear();
  close(random);
  printf("Array sorted in %llu tries\n", tries);
  cursor_show();
}

void array_printer(uint8_t *arr, int n) {
  if (n == 0) {
    printf("{}\n");
    return;
  }
  printf("{");
  for (int i = 0; i < n - 1; i++) {
    printf("%d,", arr[i]);
  }
  printf("%d}\n", arr[n - 1]);
  return;
}
void random_number_generator(uint8_t *arr, int n) {
  int fd = open("/dev/urandom", O_RDONLY);
  if (fd == -1) {
    printf("Error opening file stream\n");
    exit(1);
  }
  if (read(fd, arr, n) != n) {
    printf("Error reading random bytes\n");
    exit(1);
  };
}
