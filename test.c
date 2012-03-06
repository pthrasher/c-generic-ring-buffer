/* TODO:
 *   1. Add more tests.
 *   2. Add comments.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "ringbuffer.h"

int main() {
  defineBufferOfTypeAsName(int,myBuffer);

  bufferInit(myBuffer,1024,int);

  bufferWrite(myBuffer,37);
  bufferWrite(myBuffer,72);

  int first;
  bufferRead(myBuffer,first);
  assert(first == 37);

  int second;
  bufferRead(myBuffer,second);
  assert(second == 72);

  printf("All tests passed.\n");
  return 0;
}

