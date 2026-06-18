#ifndef SCARENA
#define SCARENA

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define SCARENA_SIZE 4096

typedef struct {
  size_t _arena_len;
  size_t _arena_cap;
  size_t len;
  char*  values[SCARENA_SIZE];
  char   _arena[SCARENA_SIZE];
} scarena;

int scarena_push(scarena* arena, char* s) {
  size_t len = strlen(s);
  if (arena->_arena_len + len + 1 < arena-> _arena_cap) {
    memcpy(arena->_arena + arena->_arena_len, s, len + 1);
    arena->values[arena->len] = arena->_arena + arena->_arena_len;
    arena->_arena_len = arena->_arena_len + len + 1;
    arena->len += 1;
    return 1;
  }
  return 0;
}

#define SCARENA_INIT() {._arena_len = 0, ._arena_cap = SCARENA_SIZE, .len = 0 }

#endif

int main() {
  scarena arena = SCARENA_INIT();
  printf("sizeof(arena) = %zu\n", sizeof(arena));

  scarena_push(&arena, "Hello!");
  scarena_push(&arena, "How are you?");
  scarena_push(&arena, "Hope you're doing well.");
  scarena_push(&arena, "YO!");

  for (size_t i = 0; i < arena.len; i++) {
    printf("%s\n", arena.values[i]);
  }

  for (size_t i = 0; i < arena._arena_len; i++) {
    if (arena._arena[i] == '\0') {
      printf("\\0");
    } else {
      printf("%c", arena._arena[i]);      
    }
  }
  
  return 0;
}
