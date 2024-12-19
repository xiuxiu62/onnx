#include "node.hpp"

#include "attribute.hpp"
#include "common.hpp"
#include "io.hpp"

#include <cstdlib>

void serialize_node(FILE *file, const Node &node) {
    serialize_string(file, node.name);
    serialize_string(file, node.domain);
    serialize_string(file, node.doc_string);

    write_bytes(file, &node.op_type, sizeof(OpType));

    serialize_string_array(file, node.input_names, node.input_count);
    serialize_string_array(file, node.output_names, node.output_count);

    write_bytes(file, &node.attribute_count, sizeof(u32));
    for (usize i = 0; i < node.attribute_count; i++) {
        serialize_attribute(file, node.attributes[i]);
    }
}

void deserialize_node(FILE *file, Node &node) {
    deserialize_string(file, node.name);
    deserialize_string(file, node.domain);
    deserialize_string(file, node.doc_string);

    read_bytes(file, &node.op_type, sizeof(OpType));

    deserialize_string_array(file, node.input_names, node.input_count);
    deserialize_string_array(file, node.output_names, node.output_count);

    read_bytes(file, &node.attribute_count, sizeof(u32));
    if (node.attribute_count > 0) {
        node.attributes = static_cast<Attribute *>(malloc(node.attribute_count * sizeof(Attribute)));
        for (usize i = 0; i < node.attribute_count; i++) {
            deserialize_attribute(file, node.attributes[i]);
        }
    } else {
        node.attributes = nullptr;
    }
}

static void try_free(void *data) {
    if (data) {
        free(data);
    }
}

void free_node(Node &node) {
    try_free(node.name);
    try_free(node.domain);
    try_free(node.doc_string);

    if (node.input_names) {
        for (usize i = 0; i < node.input_count; i++) {
            try_free(node.input_names[i]);
        }
        free(node.input_names);
    }

    if (node.output_names) {
        for (usize i = 0; i < node.output_count; i++) {
            try_free(node.output_names[i]);
        }
        free(node.output_names);
    }

    if (node.attributes) {
        for (usize i = 0; i < node.attribute_count; i++) {
            free_attribute(node.attributes[i]);
        }
        free(node.attributes);
    }
}
