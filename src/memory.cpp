#include "memory.h"

char* allocate_memory(long long& global_pointer)
{
  // Global pointer points to the beginning of data segment,
  // after text segment
  global_pointer = text_segment_size;

  // Text segment points to the beginning of user space memory
  return new char [text_segment_size + data_segment_size];
}

void deallocate_memory(char *&text_segment) { delete[] text_segment; };
