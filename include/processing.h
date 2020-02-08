/*
 Helper functions for processing source text into useful structures -
 labels, directives and instructions.
 */
#ifndef PROCESSING_H
#define PROCESSING_H

#include "label.h"

// Process source text into labels.
void process_lines(const std::vector<std::string>, std::vector<Label>&);
#endif
