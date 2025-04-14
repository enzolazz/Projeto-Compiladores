#include "ConcreteTree.hpp"
#include <functional>
#include <ranges>

ConcreteTree::No::No(No *parent, const elem_type value) : parent(parent), next_to_visit(0), value(value) {}

ConcreteTree::ConcreteTree(const elem_type root_value) : root_node(nullptr, root_value) {
    current_node = &root_node;
}

ConcreteTree::No *ConcreteTree::next_node() {
    if (current_node == nullptr)
        return nullptr;

    if (current_node == &root_node && root_node.children.empty())
        return current_node;

    No* candidate;
    do {
        if (current_node->next_to_visit < current_node->children.size())
            candidate = &current_node->children[current_node->next_to_visit++];
        else {
            current_node = current_node->parent;
            return next_node();
        }
    } while (std::holds_alternative<Token::Name>(candidate->value) || std::holds_alternative<epsilon_type>(candidate->value));

    current_node = candidate;
    return current_node;
}

std::string ConcreteTree::to_string() const {
    std::function<std::string(const No *, int)> node_to_string = [&](const No *node, const int depth) -> std::string {
        if (node == nullptr)
            return "";

        std::string result(depth * 2, ' ');

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