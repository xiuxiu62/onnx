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

usize data_type_size(DataType type) {
    switch (type) {
    case DataType::F32:
        return 4;
    case DataType::U8:
        return 1;
    case DataType::I8:
        return 1;
    case DataType::U16:
        return 2;
    case DataType::U32:
        return 4;
    case DataType::I32:
        return 4;
    case DataType::I64:
        return 8;
    case DataType::U64:
        return 8;
    case DataType::Bool:
        return 1;
    case DataType::F16:
        return 2;
    case DataType::F64:
        return 8;
    case DataType::String:
        return 8;
    case DataType::Complex64:
        return 8;
    case DataType::Complex128:
        return 16;
    case DataType::BFloat16:
        return 2;
    case DataType::F8_E4M3FN:
        return 1;
    case DataType::F8_E4M3FNUZ:
        return 1;
    case DataType::F8_E5M2:
        return 1;
    case DataType::F8_E5M2FNUZ:
        return 1;
    case DataType::U4:
        return 1;
    case DataType::I4:
        return 1;
    case DataType::F4E2M1:
        return 1;
    default:
        return 0;
    }
}
