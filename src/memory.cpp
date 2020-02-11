#include "memory.h"

void allocate_memory(
    char *&text_segment,
    char *&global_pointer,
    char *&stack_pointer)
{
  // Text segment points to the beginning of user space memory
  text_segment = new char [text_segment_size + data_segment_size];

  // Global pointer points to the beginning of data segment,
  // after text segment
  global_pointer = text_segment + text_segment_size;

  // Stack pointer points to the end of user space memory
  stack_pointer = text_segment + text_segment_size + data_segment_size - 1;
}

void deallocate_memory(char *&text_segment) { delete[] text_segment; };
