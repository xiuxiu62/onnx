#pragma once

#include "types.h"

#include <cstdio>

void serialize_string(FILE *file, const char *str);
void deserialize_string(FILE *file, char *&str);
void serialize_string_array(FILE *file, char **data, u32 count);
void deserialize_string_array(FILE *file, char **&data, u32 count);

char *duplicate_string(const char *str);
void *resize_array(void *arr, usize old_count, usize new_count, usize stride);
