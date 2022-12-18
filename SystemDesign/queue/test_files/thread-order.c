#include "queue.h"

#include <unistd.h>
#include <stdio.h>

int main(int argc, char **argv) {
  (void)argc;
  (void)argv;
  queue_t *q = queue_new(0);
  if (q == NULL) {
    return 1;
  }

  // push a 1
  queue_push(q, (void *)'h');
  queue_push(q, (void *)'i');
  queue_push(q, (void *)'l');

  // expect to pop a 1
  void *r1;
  queue_pop(q, &r1);
  if ((long)r1 != 'h') {
    // if not, then we failed
    return 1;
  }
  void *r2;
  queue_pop(q, &r2);
  // printf("r2: %ld\n", (long)r2);

  queue_delete(&q);
  return 0;
}
