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
 *
 * Philip Thrasher's Crazy Awesome Ring Buffer Macros!
 *
 * Below you will find some naughty macros for easy owning and manipulating
 * generic ring buffers. Yes, they are slightly evil in readability, but they
 * are really fast, and they work great.
 *
 * Example usage:
 *
 * #include <stdio.h>
 *
 * // So we can use this in any method, this gives us a typedef
 * // named 'intBuffer'.
 * ringBuffer_typedef(int, intBuffer);
 *
 * int main() {
 *   // Declare vars.
 *   intBuffer myBuffer;
 *
 *   bufferInit(myBuffer,1024,int);
 *
 *   // We must have the pointer. All of the macros deal with the pointer.
 *   // (except for init.)
 *   intBuffer* myBuffer_ptr;
 *   myBuffer_ptr = &myBuffer;
 *
 *   // Write two values.
 *   bufferWrite(myBuffer_ptr,37);
 *   bufferWrite(myBuffer_ptr,72);
 *
 *   // Read a value into a local variable.
 *   int first;
 *   bufferRead(myBuffer_ptr,first);
 *   assert(first == 37); // true
 *
 *   // Get reference of the current element to avoid copies
 *   int* second_ptr = &bufferReadPeek(myBuffer_ptr);
 *   assert(*second_ptr == 72); // true
 *   operate_on_reference(second_prt);
 * 
 *   // move to next value
 *   bufferReadSkip(myBuffer_ptr);
 *
 *   return 0;
 * }
 *
 */

#ifndef _ringbuffer_h
#define _ringbuffer_h

/* Setting RINGBUFFER_USE_STATIC_MEMORY to 1 will use a static memory area
 * allocated for the ringbuffer instead of a dynamic one, useful if there is no
 * dynamic allocation or the buffer will always be around anyways. There is no
 * bufferDestroy() in this case */
#ifndef RINGBUFFER_USE_STATIC_MEMORY
#define RINGBUFFER_USE_STATIC_MEMORY 0
#endif

/* Setting RINGBUFFER_AVOID_MODULO to 1 will replace the modulo operations with
 * comparisons, useful if there is no hardware divide operations */
#ifndef RINGBUFFER_AVOID_MODULO
#define RINGBUFFER_AVOID_MODULO 0
#endif

#define ringBuffer_typedef(T, NAME) \
  typedef struct { \
    int size; \
    int start; \
    int end; \
    T* elems; \
  } NAME
  

/* To allow for S elements to be stored we allocate space for (S + 1) elements,
 * otherwise a full buffer would be indistinguishable from an empty buffer.
 * 
 * Also as the elements will be accessed like an array it makes sense to pad
 * the type of the elements to the platform wordsize, otherwise you will end up
 * with unaligned accesses. */
#if RINGBUFFER_USE_STATIC_MEMORY == 1
#define bufferInit(BUF, S, T) \
  { \
    static T StaticBufMemory[S + 1];\
    BUF.elems = StaticBufMemory; \
  } \
    BUF.size = S; \
    BUF.start = 0; \
    BUF.end = 0;
#else
  
#define bufferInit(BUF, S, T) \
  BUF.size = S; \
  BUF.start = 0; \
  BUF.end = 0; \
  BUF.elems = (T*)calloc(BUF.size + 1, sizeof(T))

#define bufferDestroy(BUF) free((BUF)->elems)

#endif

  
#if RINGBUFFER_AVOID_MODULO == 1
  
#define nextStartIndex(BUF) (((BUF)->start != (BUF)->size) ? ((BUF)->start + 1) : 0)
#define nextEndIndex(BUF) (((BUF)->end != (BUF)->size) ? ((BUF)->end + 1) : 0)
  
#else
  
#define nextStartIndex(BUF) (((BUF)->start + 1) % ((BUF)->size + 1))
#define nextEndIndex(BUF) (((BUF)->end + 1) % ((BUF)->size + 1))
  
#endif

#define isBufferEmpty(BUF) ((BUF)->end == (BUF)->start)
#define isBufferFull(BUF) (nextEndIndex(BUF) == (BUF)->start)

#define bufferWritePeek(BUF) (BUF)->elems[(BUF)->end]
#define bufferWriteSkip(BUF) \
  (BUF)->end = nextEndIndex(BUF); \
  if (isBufferEmpty(BUF)) { \
    (BUF)->start = nextStartIndex(BUF); \
  }

#define bufferReadPeek(BUF) (BUF)->elems[(BUF)->start]
#define bufferReadSkip(BUF) \
  (BUF)->start = nextStartIndex(BUF);

#define bufferWrite(BUF, ELEM) \
  bufferWritePeek(BUF) = ELEM; \
  bufferWriteSkip(BUF)

#define bufferRead(BUF, ELEM) \
  ELEM = bufferReadPeek(BUF); \
  bufferReadSkip(BUF)
  
#endif
