#include "stream.hpp"

#include "file.h"
#include "types.h"

#include <cstdio>

void write_stream(Stream &stream, const void *data, usize size) {
    stream.vtable.write(stream.ptr, data, size);
}

void read_stream(Stream &stream, void *data, usize size) {
    stream.vtable.write(stream.ptr, data, size);
}

void deinit_stream(Stream &stream) {
    stream.vtable.deinit(stream.ptr);
}

bool stream_is_valid(Stream &stream) {
    return stream.ptr != nullptr && stream.vtable.deinit != nullptr && stream.vtable.read != nullptr &&
           stream.vtable.write != nullptr;
}

void write_file(void *ptr, const void *data, usize size) {
    fwrite(data, 1, size, static_cast<FILE *>(ptr));
}

void read_file(void *ptr, void *data, usize size) {
    fread(data, 1, size, static_cast<FILE *>(ptr));
}

void deinit_file(void *ptr) {
    fclose(static_cast<FILE *>(ptr));
}

Stream create_file_stream(const char *path) {
    Stream stream;
    FILE *file = nullptr;
    file = open_file(path, "r+b");

    if (!file) {
        file = open_file(path, "w+b");
    }

    if (file) {
        stream = {
            .ptr = file,
            .vtable =
                {
                    .write = write_file,
                    .read = read_file,
                    .deinit = deinit_file,
                },
        };
    } else {
        error("Failed to open stream");
    }

    return stream;
}
