#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* C Slab
 * --------------------------------
 * Create a slab of memory.
 *
 * Pros: 
 * - Reduces number of allocations,
 *   with large potential for perf-
 *   ormance improvements.
 *
 * Cons:
 * - Prone to fragmentation
 * - Casting required through macros
 *   or manually.
 * - Use-after-free if not careful
 */
 
typedef struct {
  size_t cap;
  size_t len;
  void  *bytes;
} Slab;

Slab newSlab(size_t capacity) {
  void *bytes = calloc(capacity, 1);
  if (bytes == NULL) {
    printf("[FATAL ERROR] Failed allocating slab: %zu bytes\n", capacity);
    exit(1);
  }
  return (Slab) {
    .cap = capacity,
    .len = 0,
    .bytes = bytes
  };
}

void *pushSlab(Slab *slab, void *data, size_t value_size, size_t n_values) {
  size_t data_size = value_size * n_values;
  size_t required_memory = (slab->len + (data_size));

  if (required_memory > slab->cap) {
    size_t new_cap = slab->cap;

    while (new_cap < required_memory) {
      new_cap *= 2;
    }

    void *bytes = realloc(slab->bytes, new_cap);

    if (bytes == NULL) {
      printf("[FATAL ERROR] Failed reallocating slab: %zu bytes\n", new_cap);
      exit(1);     
    }

    unsigned char* writer = (unsigned char*)bytes;
    for (size_t i = required_memory; i < new_cap; i++) {
      
      writer[i] = '\0';
    }

    slab->cap = new_cap;
    slab->bytes = bytes;    
  }
  memcpy(slab->bytes + slab->len, data, data_size);    
  slab->len = required_memory;
  return slab->bytes + slab->len - data_size; 
}

void freeSlab(Slab *slab) {
  free(slab->bytes);
}

#define pushSlabStr(slab, str) (char*)(pushSlab(slab, str, 1, strlen(str)+1))

// Set to a value reasonable for the workload (512, 4096, 65536, etc.)
// Using 1 in this example to verify reallocation works as expected
#define DEFAULT_SLAB_SIZE 1

int main() {
  
  Slab slab = newSlab(DEFAULT_SLAB_SIZE); 
  char *name = "Bob";
  char *age  = "45";
  char *sex  = "M";
  char *_name = pushSlabStr(&slab, name);
  char *_age = pushSlabStr(&slab, age);
  char *_sex = pushSlabStr(&slab, sex);

  printf("_name   : %s\n", _name);
  printf("_age    : %s\n", _age);
  printf("_sex    : %s\n", _sex);
  printf("slab len: %zu\n", slab.len);
  printf("slab cap: %zu\n", slab.cap);

  return 0;
}
