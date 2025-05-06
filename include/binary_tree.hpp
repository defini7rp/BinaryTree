#ifndef BINARY_TREE_HPP
#define BINARY_TREE_HPP

#include <type_traits>
#include <ostream>
#include <list>

template <class T>
class BinaryTree;

template <class T>
using BinaryTreeNode = typename BinaryTree<T>::Node;

template <class T>
class BinaryTree
{
    static_assert(std::is_trivially_copyable_v<T>, "T in BinaryTree<T> is not trivially copyable");

public:
    struct Node
    {
        Node(T value, Node* left = nullptr, Node* right = nullptr);

        T value;

        Node* left = nullptr;
        Node* right = nullptr;

        BinaryTreeNode<T>* operator[](size_t index);
    };

public:
    BinaryTree() = default;
    BinaryTree(const BinaryTree&);
    ~BinaryTree();

    BinaryTree& operator=(const BinaryTree&);

    void push(T value);

    void clear();
    void clear(Node* node);

    template <class U>
    void traverse_preorder(U start, U end, Node* node);

    template <class U>
    void traverse_inorder(U start, U end, Node* node);
    
    template <class U>
    void traverse_postorder(U start, U end, Node* node);

    template <class U>
    void traverse_preorder(U start, U end);

    template <class U>
    void traverse_inorder(U start, U end);

    template <class U>
    void traverse_postorder(U start, U end);
    
    template <class U>
    void traverse_levels(U start, U end);

    void remove(T value);
    void remove(Node*& start, T value);

    void copy(const BinaryTree&);

    void remove_duplicates();
    void remove_duplicates(Node* node);

    size_t get_height();
    size_t get_height(Node* node);

public:
    Node* root = nullptr;

private:
    template <class U>
    bool collect_level(Node* node, size_t level, U& start, U end);

    Node* find_leftmost(Node* start);
    bool delete_node(Node** node, T value);

};

#ifdef BINARY_TREE_IMPLEMENTATION
#undef BINARY_TREE_IMPLEMENTATION

template <class T>
BinaryTree<T>::Node::Node(T value, Node* left, Node* right)
    : value(value), left(left), right(right)
{

}

template <class T>
BinaryTreeNode<T>* BinaryTree<T>::Node::operator[](size_t index)
{

}

template <class T>
BinaryTree<T>::BinaryTree(const BinaryTree& other)
{
    copy(other);
}

template <class T>
BinaryTree<T>::~BinaryTree()
{
    clear(root);
}

template <class T>
BinaryTree<T>& BinaryTree<T>::operator=(const BinaryTree& other)
{
    copy(other);
    return *this;
}

template <class T>
void BinaryTree<T>::push(T value)
{
    if (!root)
    {
        root = new Node(value);
        return;
    }

    Node* node = root;

    while (node)
    {
        Node*& test_node = (value <= node->value) ? node->left : node->right;

        if (test_node)
            node = test_node;
        else
        {
            test_node = new Node(value);
            node = nullptr;
        }
    }
}

template <class T>
void BinaryTree<T>::clear()
{
    clear();
}

#define validate_iter(body) if (start == end) return; else body

template <class T>
template <class U>
void BinaryTree<T>::traverse_preorder(U start, U end, Node* node)
{
    if (!node)
        return;

    validate_iter(*start = node);
    std::advance(start, 1);

    validate_iter(traverse_preorder(start, end, node->left));
    std::advance(start, 1);

    validate_iter(traverse_preorder(start, end, node->right));
}

template <class T>
template <class U>
void BinaryTree<T>::traverse_inorder(U start, U end, Node* node)
{
    if (!node)
        return;

    validate_iter(traverse_preorder(start, end, node->left));
    std::advance(start, 1);

    validate_iter(*start = node);
    std::advance(start, 1);

    validate_iter(traverse_preorder(start, end, node->right));
}

template <class T>
template <class U>
void BinaryTree<T>::traverse_postorder(U start, U end, Node* node)
{
    if (!node)
        return;

    validate_iter(traverse_preorder(start, end, node->left));
    std::advance(start, 1);

    validate_iter(traverse_preorder(start, end, node->right));
    std::advance(start, 1);

    validate_iter(*start = node);
}

template<class T>
template<class U>
void BinaryTree<T>::traverse_preorder(U start, U end)
{
    traverse_preorder(start, end, root);
}

template<class T>
template<class U>
void BinaryTree<T>::traverse_inorder(U start, U end)
{
    traverse_inorder(start, end, root);
}

template<class T>
template<class U>
void BinaryTree<T>::traverse_postorder(U start, U end)
{
    traverse_postorder(start, end, root);
}

#undef validate_iter

template <class T>
void BinaryTree<T>::remove(T value)
{
    remove(root, value);
}

template <class T>
template <class U>
void BinaryTree<T>::traverse_levels(U start, U end)
{
    size_t l = 1;
    for (size_t l = 1; collect_level(root, l, start, end); l++);
}

template <class T>
void BinaryTree<T>::copy(const BinaryTree& other)
{
    clear();

    std::memcpy(this, &other, sizeof(BinaryTree));

    auto visit_node = [](Node* my, Node* other)
        {
            my = new Node(other->value);

            visit_node(my->left, other->left);
            visit_node(my->right, other->right);
        };

    visit_node(root);
}

template <class T>
size_t BinaryTree<T>::get_height()
{
    return get_height(root);
}

template<class T>
size_t BinaryTree<T>::get_height(Node* node)
{
    if (!node)
        return 0;

    size_t left = get_height(node->left);
    size_t right = get_height(node->right);

    return std::max(left, right) + 1;
}

template <class T>
void BinaryTree<T>::remove_duplicates()
{
    remove_duplicates(root);
}

template <class T>
template <class U>
bool BinaryTree<T>::collect_level(Node* node, size_t level, U& start, U end)
{
    if (!node || start == end)
        return false;

    if (level == 1)
    {
        *start++ = node;
        return true;
    }

    bool left = collect_level(node->left, level - 1, start, end);
    bool right = collect_level(node->right, level - 1, start, end);

    return left || right;
}

template <class T>
void BinaryTree<T>::remove_duplicates(Node* node)
{
    if (!node)
        return;

    remove_duplicates(node->left);
    remove_duplicates(node->right);

    remove(node->left, node->value);
    remove(node->right, node->value);
}

template<class T>
void BinaryTree<T>::clear(Node* node)
{
    if (node)
    {
        clear(node->left);
        clear(node->right);

        delete node;
    }
}

template <class T>
BinaryTreeNode<T>* BinaryTree<T>::find_leftmost(Node* start)
{
    while (start && start->left)
        start = start->left;

    return start;
}

template <class T>
bool BinaryTree<T>::delete_node(Node** node, T value)
{
    if (!(*node))
        return false;

    bool removed = false;

    if (value == (*node)->value)
    {
        Node* n = *node;

        if (!n->left)
        {
            *node = n->right;
            delete n;
        }
        else if (!n->right)
        {
            *node = n->left;
            delete n;
        }
        else
        {
            Node* leftmost = find_leftmost((*node)->right);
            n->value = leftmost->value;
            delete_node(&n->right, leftmost->value);
        }

        removed = true;
    }
    else
    {
        if (value < (*node)->value)
            removed = delete_node(&(*node)->left, value);
        else
            removed = delete_node(&(*node)->right, value);
    }

    return removed;
}

template <class T>
void BinaryTree<T>::remove(Node*& node, T value)
{
    while (delete_node(&node, value));
}

#endif

#endif
