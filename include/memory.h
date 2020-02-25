/*
 * Simplified memory layout for MIPS. Initializes only user space memory
 * c.f http://www.it.uu.se/education/course/homepage/os/vt18/module-0/mips-and-mars/mips-memory-layout/
 */

#ifndef MEMORY_H
#define MEMORY_H

#include <cstddef>

// Allocate 2^20 bytes i.e 1 Mb for text segment
const size_t text_segment_size = (1 << 20);

// Allocate 2^22 bytes i.e 4 Mb for data segment
const size_t data_segment_size = (1 << 22);

const size_t text_segment_base = 0x04000000;

const size_t data_segment_base = 0x10000000;

char* allocate_memory(long long&);

void deallocate_memory(char*&);
#endif
