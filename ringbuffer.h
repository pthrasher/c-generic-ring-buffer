/* Philip Thrasher's Crazy Awesome Ring Buffer Macros!
 *
 * These macros are what has kept pthrash (slightly) sane while learning C. If
 * you don't like macros, I don't care. If you think I'm no good cuz I made
 * these, I don't care. I like them, and they made my life easier, so fuck off.
 * They're very simple, and easy to understand.
 *
 * Example usage:
 *
 * #include <stdio.h>
 *
 * int main() {
 *   struct myBuffer = bufferOfType(int);
 *   bufferInit(myBuffer, 1024, int);
 *
 *   bufferWrite(myBuffer, 37);
 *   bufferWrite(myBuffer, 72);
 *
 *   int first;
 *   bufferRead(myBuffer, first);
 *   printf("First item: %d", first);
 *
 *   int second;
 *   bufferRead(myBuffer, second);
 *   printf("Second item: %d", second);
 *
 *   return 0;
 * }
 *
 */

#ifndef _ringbuffer_h
#define _ringbuffer_h

#define defineBufferOfType(T, NAME) \
  struct { \
    int size; \
    int start; \
    int end; \
    T* elems; \
  } NAME

#define bufferInit(BUF, S, T) \
  BUF.size = S+1; \
  BUF.start = 0; \
  BUF.end = 0; \
  BUF.elems = (T*)calloc(BUF.size, sizeof(T))


#define bufferDestroy(BUF) free(BUF.elems)
#define nextStartIndex(BUF) ((BUF.start + 1) % BUF.size)
#define nextEndIndex(BUF) ((BUF.end + 1) % BUF.size)
#define isBufferEmpty(BUF) (BUF.end == BUF.start)
#define isBufferFull(BUF) (nextEndIndex(BUF) == BUF.start)

#define bufferWrite(BUF, ELEM) \
  BUF.elems[BUF.end] = ELEM; \
  BUF.end = (BUF.end + 1) % BUF.size; \
  if (isBufferEmpty(BUF)) { \
    BUF.start = nextStartIndex(BUF); \
  }

#define bufferRead(BUF, ELEM) \
    ELEM = BUF.elems[BUF.start]; \
    BUF.start = nextStartIndex(BUF);

#endif

