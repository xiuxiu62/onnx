#pragma once

#include "tensor.hpp"
#include "types.h"

enum class AttributeType {
    Float,
    Int,
    String,
    Tensor,
    FloatList,
    IntList,
    StringList,
    Graph,
    GraphList,
    Type,
    TypeList,
};

struct AttributeList {
    void *data;
    usize count;
};

union AttributeValue {
    f32 f;
    i64 i;
    char *s;
    Tensor *tensor;
    AttributeList list;
    void *graph;
    DataType type;
};

struct Attribute {
    char *name;
    AttributeType type;
    AttributeValue value;
};

void serialize_attribute(FILE *file, const Attribute &attribute);
void deserialize_attribute(FILE *file, Attribute &attribute);
void free_attribute(Attribute &attribute);
