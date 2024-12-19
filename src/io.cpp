#include "io.hpp"

#include "types.h"

#include <cstdio>

void write_bytes(FILE *file, const void *data, usize size) {
    const u8 *bytes = static_cast<const u8 *>(data);
    for (usize i = 0; i < size; i++) {
        fputc(bytes[i], file);
    }
}

void read_bytes(FILE *file, void *data, usize size) {
    u8 *bytes = static_cast<u8 *>(data);
    for (usize i = 0; i < size; i++) {
        bytes[i] = fgetc(file);
    }
}
