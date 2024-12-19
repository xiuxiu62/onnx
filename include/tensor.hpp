#pragma once

#include "types.h"

#include <cstdio>

enum class DataType {
    F32 = 1,
    U8 = 2,
    I8 = 3,
    U16 = 4,
    U32 = 5,
    I32 = 6,
    I64 = 7,
    U64 = 8,
    Bool = 9,
    F16 = 10,
    F64 = 11,
    String = 12,
    Complex64 = 14,
    Complex128 = 15,
    BFloat16 = 16,
    F8_E4M3FN = 17,   // 4-bit exponent 3-bit mantissa + finite-only
    F8_E4M3FNUZ = 18, // 4-bit exponent 3-bit mantissa + finite-only + unsigned + allows zero
    F8_E5M2 = 19,     // 5-bit exponent 2-bit mantissa
    F8_E5M2FNUZ = 20, // 5-bit exponent 2-bit mantissa + finite-only + unsigned + allows zero
    U4 = 21,
    I4 = 22,
    F4E2M1 = 23,
};

struct Tensor {
    char *name, *doc_string;
    DataType data_type;
    u32 dim_count;
    u64 *dims;
    void *data;
    usize data_size;
    u64 *strides; // For non-contiguous data
    bool owns_data;
};

void serialize_tensor(FILE *file, const Tensor &tensor);
void deserialize_tensor(FILE *file, Tensor &tensor);
void free_tensor(Tensor &tensor);
usize data_type_size(DataType type);
