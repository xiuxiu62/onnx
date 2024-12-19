#pragma once

#include "types.h"

#include <cstdio>

void write_bytes(FILE *file, const void *data, usize size);
void read_bytes(FILE *file, void *data, usize size);
