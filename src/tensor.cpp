#include "tensor.hpp"

#include "io.hpp"
#include "types.h"

#include <cstdio>
#include <cstdlib>

void serialize_tensor(FILE *file, const Tensor &tensor) {
    write_bytes(file, &tensor.data_type, sizeof(DataType));

    write_bytes(file, &tensor.dim_count, sizeof(i32));
    for (usize i = 0; i < tensor.dim_count; i++) {
        write_bytes(file, &tensor.dims[i], sizeof(i64));
    }

    write_bytes(file, &tensor.data_size, sizeof(usize));
    write_bytes(file, tensor.data, tensor.data_size);
}

void deserialize_tensor(FILE *file, Tensor &tensor) {
    read_bytes(file, &tensor.data_type, sizeof(DataType));

    read_bytes(file, &tensor.dim_count, sizeof(i32));
    tensor.dims = (u64 *)malloc(tensor.dim_count * sizeof(i64));
    for (int i = 0; i < tensor.dim_count; i++) {
        read_bytes(file, &tensor.dims[i], sizeof(i64));
    }

    read_bytes(file, &tensor.data_size, sizeof(usize));
    tensor.data = malloc(tensor.data_size);
    read_bytes(file, tensor.data, tensor.data_size);
}

void free_tensor(Tensor &tensor) {
    free(tensor.dims);
    free(tensor.data);
}
