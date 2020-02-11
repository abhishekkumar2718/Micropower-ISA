#include "memory.h"

char* allocate_memory(int& global_pointer, int& stack_pointer)
{
  // Global pointer points to the beginning of data segment,
  // after text segment
  global_pointer = text_segment_size;

  // Stack pointer points to the end of user space memory
  stack_pointer = text_segment_size + data_segment_size - 1;

  // Text segment points to the beginning of user space memory
  return new char [text_segment_size + data_segment_size];
}

void deallocate_memory(char *&text_segment) { delete[] text_segment; };
