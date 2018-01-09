/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2013 Philip Thrasher
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 *
 * TODO:
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

  assert(isBufferFull(myBuffer_ptr));

  int first;
  bufferRead(myBuffer_ptr,first);
  assert(first == 38);

  int second;
  bufferRead(myBuffer_ptr,second);
  assert(second == 73);

  bufferWrite(myBuffer_ptr,42);
  
  bufferWritePeek(myBuffer_ptr) = 17;
  bufferWritePeek(myBuffer_ptr) = 43;
  bufferWriteInc(myBuffer_ptr);
  
  bufferWrite(myBuffer_ptr,44); // test wrap around -- this should overwrite 42

  int third;
  third = bufferReadPeek(myBuffer_ptr);
  assert(third == 43);
  third = bufferReadPeek(myBuffer_ptr);
  assert(third == 43);
  
  bufferReadInc(myBuffer_ptr);

  int fourth;
  bufferRead(myBuffer_ptr,fourth);
  assert(fourth == 44);

}

int main() {
  // Declare vars.
  intBuffer myBuffer;
  intBuffer* myBuffer_ptr;

  bufferInit(myBuffer,2,int);

  // We must have the pointer. All of the macros deal with the pointer. (except
  // for init.
  myBuffer_ptr = &myBuffer;

  assert(isBufferEmpty(myBuffer_ptr));

  bufferWrite(myBuffer_ptr,37);
  bufferWrite(myBuffer_ptr,72);

  assert(!isBufferEmpty(myBuffer_ptr));

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
