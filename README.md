Philip Thrasher's Crazy Awesome Ring Buffer Macros!
===================================================

In ringbuffer.h are some naughty macros for easily owning and manipulating
generic ring buffers. Yes, they are slightly evil in readability, but they
are really fast, and they work great. If you don't like them, don't use them.
I use them because it's the only DRY and sane way to add generic ring buffers
to your C application.

For limited resources systems such as microcontrollers there is
RINGBUFFER_USE_STATIC_MEMORY and RINGBUFFER_AVOID_MODULO defines.


Example usage:

```c
#include <stdio.h>

// So we can use this in any method, this gives us a typedef named 'intBuffer'.
// Really, this is the only *truely* naughty portion of these macros as it
// creates a complete type definition for you. This is something people will
// have to read the header in order to grok.
ringBuffer_typedef(int, intBuffer);

// The above macro translates exactly to:
typedef struct {
  int size;
  int start;
  int end;
  int* elems;
} intBuffer;


int main() {
  // Declare vars.
  intBuffer myBuffer;

  bufferInit(myBuffer,1024,int);

  // We must have the pointer. All of the macros deal with the pointer.
  // (except for init.)
  intBuffer* myBuffer_ptr;
  myBuffer_ptr = &myBuffer;

  // Write two values.
  bufferWrite(myBuffer_ptr,37);
  bufferWrite(myBuffer_ptr,72);

  // Read a value into a local variable.
  int first;
  bufferRead(myBuffer_ptr,first);
  assert(first == 37); // true

  // Get reference of the current element to avoid copies
  int* second_ptr = &bufferReadPeek(myBuffer_ptr);
  assert(*second_ptr == 72); // true
  operate_on_reference(second_prt);
 
  // move to next value
  bufferReadSkip(myBuffer_ptr);

  return 0;
}
```

License
-------

```
The MIT License (MIT)

Copyright (c) 2013 Philip Thrasher

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
```
