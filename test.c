/* TODO:
 *   1. Add more tests.
 *   2. Add comments.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "ringbuffer.h"

ringBuffer_typedef(int, intBuffer);

void passByReference(intBuffer* myBuffer_ptr) {

  assert(isBufferEmpty(myBuffer_ptr));

  bufferWrite(myBuffer_ptr,38);
  bufferWrite(myBuffer_ptr,73);

  int first;
  bufferRead(myBuffer_ptr,first);
  assert(first == 38);

  int second;
  bufferRead(myBuffer_ptr,second);
  assert(second == 73);
}

int main() {
  // Declare vars.
  intBuffer myBuffer;
  intBuffer* myBuffer_ptr;

  bufferInit(myBuffer,1024,int);

  // We must have the pointer. All of the macros deal with the pointer. (except
  // for init.
  myBuffer_ptr = &myBuffer;

  bufferWrite(myBuffer_ptr,37);
  bufferWrite(myBuffer_ptr,72);

  int first;
  bufferRead(myBuffer_ptr,first);
  assert(first == 37);

  int second;
  bufferRead(myBuffer_ptr,second);
  assert(second == 72);

  passByReference(myBuffer_ptr);

  printf("All tests passed.\n");
  return 0;
}

