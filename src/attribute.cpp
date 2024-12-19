#include "attribute.hpp"

#include "common.hpp"
#include "io.hpp"
#include "tensor.hpp"

#include <cstdlib>

static void serialize_attribute_list(FILE *file, const AttributeList &list, AttributeType type) {
    write_bytes(file, &list.count, sizeof(usize));

    switch (type) {
    case AttributeType::Float:
        write_bytes(file, static_cast<f32 *>(list.data), list.count * sizeof(f32));
        break;
    case AttributeType::Int:
        write_bytes(file, static_cast<i64 *>(list.data), list.count * sizeof(i64));
        break;
    case AttributeType::String: {
        char **strings = static_cast<char **>(list.data);
        for (usize i = 0; i < list.count; i++) {
            serialize_string(file, strings[i]);
        }
        break;
    }
    case AttributeType::Type:
        write_bytes(file, static_cast<DataType *>(list.data), list.count * sizeof(DataType));
        break;
    default:
        break;
    }
}

static void deserialize_attribute_list(FILE *file, AttributeList &list, AttributeType elementType) {
    read_bytes(file, &list.count, sizeof(usize));

    switch (elementType) {
    case AttributeType::Float: {
        list.data = malloc(list.count * sizeof(f32));
        read_bytes(file, list.data, list.count * sizeof(f32));
        break;
    }
    case AttributeType::Int: {
        list.data = malloc(list.count * sizeof(i64));
        read_bytes(file, list.data, list.count * sizeof(i64));
        break;
    }
    case AttributeType::String: {
        char **strings = (char **)malloc(list.count * sizeof(char *));
        list.data = strings;
        for (usize i = 0; i < list.count; i++) {
            deserialize_string(file, strings[i]);
        }
        break;
    }
    case AttributeType::Type: {
        list.data = malloc(list.count * sizeof(DataType));
        read_bytes(file, list.data, list.count * sizeof(DataType));
        break;
    }
    default:
        break;
    }
}

void serialize_attribute(FILE *file, const Attribute &attribute) {
    serialize_string(file, attribute.name);
    write_bytes(file, &attribute.type, sizeof(AttributeType));

    switch (attribute.type) {
    case AttributeType::Float:
        write_bytes(file, &attribute.value.f, sizeof(f32));
        break;
    case AttributeType::Int:
        write_bytes(file, &attribute.value.i, sizeof(i64));
        break;
    case AttributeType::String:
        serialize_string(file, attribute.value.s);
        break;
    case AttributeType::Tensor:
        serialize_tensor(file, *attribute.value.tensor);
        break;
    case AttributeType::FloatList:
        serialize_attribute_list(file, attribute.value.list, AttributeType::Float);
        break;
    case AttributeType::IntList:
        serialize_attribute_list(file, attribute.value.list, AttributeType::Int);
        break;
    case AttributeType::StringList:
        serialize_attribute_list(file, attribute.value.list, AttributeType::String);
        break;
    case AttributeType::Graph:
        break;
    case AttributeType::GraphList:
        break;
    case AttributeType::Type:
        write_bytes(file, &attribute.value.type, sizeof(DataType));
        break;
    case AttributeType::TypeList:
        serialize_attribute_list(file, attribute.value.list, AttributeType::TypeList);
        break;
    defualt:
        break;
    };
}

void deserialize_attribute(FILE *file, Attribute &attribute) {
    deserialize_string(file, attribute.name);
    read_bytes(file, &attribute.type, sizeof(AttributeType));

    // Read value based on type
    switch (attribute.type) {
    case AttributeType::Float:
        read_bytes(file, &attribute.value.f, sizeof(f32));
        break;

    case AttributeType::Int:
        read_bytes(file, &attribute.value.i, sizeof(i64));
        break;

    case AttributeType::String:
        deserialize_string(file, attribute.value.s);
        break;

    case AttributeType::Tensor:
        attribute.value.tensor = static_cast<Tensor *>(malloc(sizeof(Tensor)));
        deserialize_tensor(file, *attribute.value.tensor);
        break;

    case AttributeType::FloatList:
        deserialize_attribute_list(file, attribute.value.list, AttributeType::Float);
        break;

    case AttributeType::IntList:
        deserialize_attribute_list(file, attribute.value.list, AttributeType::Int);
        break;

    case AttributeType::StringList:
        deserialize_attribute_list(file, attribute.value.list, AttributeType::String);
        break;

    case AttributeType::Type:
        read_bytes(file, &attribute.value.type, sizeof(DataType));
        break;

    case AttributeType::TypeList:
        deserialize_attribute_list(file, attribute.value.list, AttributeType::Type);
        break;
    default:
        break;
    }
}

void free_attribute(Attribute &attribute) {
    free(attribute.name);

    switch (attribute.type) {
    case AttributeType::String:
        free(attribute.value.s);
        break;
    case AttributeType::Tensor:
        free_tensor(*attribute.value.tensor);
        free(attribute.value.tensor);
        break;
    case AttributeType::FloatList:
    case AttributeType::IntList:
    case AttributeType::TypeList:
        free(attribute.value.list.data);
        break;
    case AttributeType::StringList: {
        char **strings = static_cast<char **>(attribute.value.list.data);
        for (usize i = 0; i < attribute.value.list.count; i++) {
            free(strings[i]);
        }
        free(strings);
        break;
    }
    // case AttributeType::Graph:
    //     break;
    // case AttributeType::GraphList:
    //     break;
    default:
        break;
    }
}
