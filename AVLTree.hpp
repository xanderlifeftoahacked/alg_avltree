#pragma once
#include <functional>
#include <iostream>

namespace ds {
template <typename T> class AVLTree {
  class Node {
  private:
    Node(T _data) : left{nullptr}, right{nullptr}, height{1} { data = _data; }
    Node *left;
    T data;
    int height;
    Node *right;
  };

  int height(Node *_node) { return _node == nullptr ? 0 : _node->height; }

  int is_balanced(Node *_node) {
    return _node == nullptr ? 0 : height(_node->left) - height(_node->right);
  }

  Node *rotate_left(Node *_node) {
    Node *new_root = _node->right;
    _node->right = new_root->left;
    new_root->left = _node;
    _node->height = std::max(height(_node->left), height(_node->right)) + 1;
    new_root->height =
        std::max(height(new_root->left), height(new_root->right)) + 1;
    return new_root;
  }

  Node *rotate_right(Node *_node) {
    Node *new_root = _node->left;
    _node->left = new_root->right;
    new_root->right = _node;
    _node->height = std::max(height(_node->left), height(_node->right)) + 1;
    new_root->height =
        std::max(height(new_root->left), height(new_root->right)) + 1;
    return new_root;
  }

public:
  Node *root;
  AVLTree() : root{nullptr} {};

  void show(Node *_node, int _level) {
    if (_node == nullptr) {
      return;
    }
    show(_node->right, _level + 1);
    std::string spaces(_level, ' ');
    std::cout << spaces << _node->data << '\n';
    show(_node->left, _level + 1);
  }

  void insert(T _data) {
    std::function<Node *(Node *, T)> _insert = [&](Node *_node,
                                                   T _data) -> Node * {
      if (_node == nullptr) {
        return new Node(_data);
      } else if (_data < _node->data) {
        _node->left = _insert(_node->left, _data);
      } else if (_data > _node->data) {
        _node->right = _insert(_node->right, _data);
      }

      _node->height = std::max(height(_node->left), height(_node->right)) + 1;
      int balance = is_balanced(_node);

      if (balance > 1 && _data < _node->left->data) {
        return rotate_right(_node);
      } else if (balance > 1 && _data > _node->left->data) {
        _node->left = rotate_left(_node->left);
        return rotate_right(_node);
      } else if (balance < -1 && _data > _node->right->data) {
        return rotate_left(_node);
      } else if (balance < -1 && _data < _node->right->data) {
        _node->right = rotate_right(_node->right);
        return rotate_left(_node);
      }

      return _node;
    };

    root = _insert(root, _data);
  }

  Node *find(T _data, Node *_node) {
    if (_node == nullptr) {
      return nullptr;
    }
    if (_node->data == _data) {
      return _node;
    } else if (_data < _node->data) {
      return find(_data, _node->left);
    } else {
      return find(_data, _node->right);
    }
  }

  Node *find(T _data) { return find(_data, root); }
  // TODO void delete()
  ~AVLTree() {
    std::function<void(Node *)> del = [&del](Node *_node) {
      std::pair<Node *, Node *> _return(_node->left, _node->right);
      delete _node;
      if (_return.first != nullptr)
        del(_return.first);
      if (_return.second != nullptr)
        del(_return.second);
      if (_return.first == nullptr && _return.second == nullptr)
        return;
    };
    del(root);
  }
};
} // namespace ds
