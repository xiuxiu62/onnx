#pragma once

#include "types.h"

struct Stream {
    void *ptr; // Backing stream variant

    struct {
        void (*write)(void *ptr, const void *data, usize size);
        void (*read)(void *ptr, void *data, usize size);
        void (*deinit)(void *ptr);
    } vtable;
};

void write_stream(Stream &stream, const void *data, usize size);
void read_stream(Stream &stream, void *data, usize size);
void deinit_stream(Stream &stream);
bool stream_is_valid(Stream &stream);

void write_file(void *ptr, const void *data, usize size);
void read_file(void *ptr, void *data, usize size);
void deinit_file_stream(void *ptr);
Stream create_file_stream(const char *path);
