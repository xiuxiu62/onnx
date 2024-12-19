#pragma once

#include "types.h"

#include <cstdio>

void serialize_string(FILE *file, const char *str);
void deserialize_string(FILE *file, char *&str);
void serialize_string_array(FILE *file, char **data, u32 count);
void deserialize_string_array(FILE *file, char **&data, u32 count);
