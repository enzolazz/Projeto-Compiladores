#include "ArvoreConcreta.hpp"

#include <functional>
#include <ranges>

ArvoreConcreta::No::No(No *parent, elem_type value) : parent(parent), next_to_visit(0), value(value) {}

ArvoreConcreta::ArvoreConcreta(const elem_type root_value) : root_node(nullptr, root_value) {
    current_node = &root_node;
}

ArvoreConcreta::No *ArvoreConcreta::next_node() {
    if (std::holds_alternative<Token::Name>(current_node->value) && !current_node->children.empty())
        throw "fdsklfjd";

    if (current_node == nullptr)
        return nullptr;

    // NÃO TERMINAL
    if (std::holds_alternative<int>(current_node->value)) {
        current_node = current_node->children[current_node->next_to_visit];
    }
}

ArvoreConcreta::No ArvoreConcreta::peek_next() {}

std::string ArvoreConcreta::to_string() const {
    std::function<std::string(const No *, int)> node_to_string = [&](const No *node, int depth) -> std::string {
        if (node == nullptr)
            return "";

        // Indentação para representar a hierarquia
        std::string result(depth * 2, ' ');

        // Adiciona o valor do nó
        if (std::holds_alternative<epsilon_type>(node->value)) {
            result += "epsilon";
        } else if (const auto v = std::get_if<int>(&node->value)) {
            result += "NT(" + std::to_string(*v + 2) + ")";
        } else if (const auto name = std::get_if<Token::Name>(&node->value)) {
            result += "Token(" + Token::to_string(*name) + ")";
        }
        result += "\n";

        for (const auto &child : node->children)
            result += node_to_string(&child, depth + 1);

        return result;
    };

    return node_to_string(&root_node, 0);
}