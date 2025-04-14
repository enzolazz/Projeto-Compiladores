#pragma once

#include "Token.hpp"
#include <variant>
#include <vector>

class ConcreteTree {
 public:
    using epsilon_type = std::monostate;
    using elem_type = std::variant<epsilon_type, int, Token::Name>;

    class No {
        friend ConcreteTree;
        No *parent;
        std::vector<No> children;
        decltype(children)::size_type next_to_visit;
        No(No *parent, elem_type value);

     public:
        const elem_type value;

       void set_children(auto values) {
           for (const auto &v : values)
              children.push_back({this, v});
       }
    };

    No *current_node;
 private:
    No root_node;

 public:
    explicit ConcreteTree(elem_type root_value);

    No *next_node();
    [[nodiscard]] std::string to_string() const;
};