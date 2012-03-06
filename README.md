Philip Thrasher's Crazy Awesome Ring Buffer Macros!
===================================================

In ringbuffer.h are some naughty macros for easily owning and manipulating
generic ring buffers. Yes, they are slightly evil in readability, but they
are really fast, and they work great.

Example usage:

```c
#include <stdio.h>

int main() {
  // This gets defined in the current scope
  defineBufferOfTypeAsName(int, myBuffer);

  bufferInit(myBuffer, 1024, int);

  bufferWrite(myBuffer, 37);
  bufferWrite(myBuffer, 72);

  int first;
  bufferRead(myBuffer, first);
  printf("First item: %d", first);

  int second;
  bufferRead(myBuffer, second);
  printf("Second item: %d", second);

  return 0;
}
```

