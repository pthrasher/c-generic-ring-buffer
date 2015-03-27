/* Philip Thrasher's Crazy Awesome Ring Buffer Macros!
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
 *   int second;
 *   bufferRead(myBuffer_ptr,second);
 *   assert(second == 72); // true
 *
 *   return 0;
 * }
 *
 */

#ifndef _ringbuffer_h
#define _ringbuffer_h

// (1) = use static memory
// (2) = use calloc heap
#define USE_STATIC_MEMORY (1)

#define ringBuffer_typedef(T, NAME) \
  typedef struct { \
    int size; \
    int start; \
    int end; \
    T* elems; \
  } NAME

#if USE_STATIC_MEMORY == 1
	#define bufferInit(BUF, S, T) \
	  BUF.size = S+1; \
	  BUF.start = 0; \
	  BUF.end = 0; \
	  static char StaticBufMemory[(S+1)*sizeof(T)];\
	  BUF.elems = (T*) (&(StaticBufMemory[0]))
#else
#define bufferInit(BUF, S, T) \
	  BUF.size = S+1; \
	  BUF.start = 0; \
	  BUF.end = 0; \
	  BUF.elems = (T*)calloc(BUF.size, sizeof(T))

      #define bufferDestroy(BUF) free(BUF->elems)
#endif
#define nextStartIndex(BUF) ((BUF->start + 1) % BUF->size)
#define nextEndIndex(BUF) ((BUF->end + 1) % BUF->size)
#define isBufferEmpty(BUF) (BUF->end == BUF->start)
#define isBufferFull(BUF) (nextEndIndex(BUF) == BUF->start)
#define locStartIndex(BUF, LOC) ((BUF->start + LOC) % BUF->size)


//Reset ringbuffer
#define bufferReset(BUF) \
	BUF->start = 0; \
	BUF->end = 0;

//Write element of type <T> into ringbuffer then advance write position +1
#define bufferWrite(BUF, ELEM) \
  BUF->elems[BUF->end] = ELEM; \
  BUF->end = (BUF->end + 1) % BUF->size; \
  if (isBufferEmpty(BUF)) { \
    BUF->start = nextStartIndex(BUF); \
  }

//Get pointer of type <*T> from current write position then advance write position +1
#define getNextBufferWritePointer(BUF, pELEM) \
  pELEM = &(BUF->elems[BUF->end]); \
  BUF->end = (BUF->end + 1) % BUF->size; \
  if (isBufferEmpty(BUF)) { \
    BUF->start = nextStartIndex(BUF); \
  }

//Get pointer of type <*T> from current write position
#define peekNextBufferWritePointer(BUF, pELEM) \
   pELEM = &(BUF->elems[BUF->end]);

//Advance write position +1
#define advanceWritePointer(BUF) \
  BUF->end = (BUF->end + 1) % BUF->size; \
  if (isBufferEmpty(BUF)) { \
    BUF->start = nextStartIndex(BUF); \
  }

//Read element of type <T> from ringbuffer then advance read position +1
#define bufferRead(BUF, ELEM) \
    ELEM = BUF->elems[BUF->start]; \
    BUF->start = nextStartIndex(BUF);

//Get pointer of type <*T> from current read position then advance read position +1
#define getNextBufferReadPointer(BUF, pELEM) \
    pELEM = &(BUF->elems[BUF->start]); \
    BUF->start = nextStartIndex(BUF);

//Get pointer of type <*T> from current read position
#define peekNextBufferReadPointer(BUF, pELEM) \
    pELEM = &(BUF->elems[BUF->start]);


//Get pointer of type <*T> from current read position + LOC
//can be interated in a loop LOC[0...ELEMENT_COUNT] to count elements
#define peekLocBufferReadPointer(BUF, pELEM, LOC) \
		if(locStartIndex(BUF,LOC) == BUF->end) { \
			pELEM = NULL; }\
			else{\
			pELEM = &(BUF->elems[locStartIndex(BUF,LOC)]);\
			}
#endif
