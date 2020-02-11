/*
 Helper functions for processing source text into useful structures -
 labels, directives and instructions.
 */
#ifndef PROCESSING_H
#define PROCESSING_H

#include<string>

void preprocess_lines(std::vector<std::string>&);

// Check if given line is a data label
bool is_data_label(const std::string&);

// Check if given line is a text label
bool is_text_label(const std::string&);

// Check if given line is an assembly directive
bool is_assembly_directive(const std::string&);

// Check if given line is an assembly directive
bool is_comment(const std::string&);
#endif
