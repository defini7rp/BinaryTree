#define BINARY_TREE_IMPLEMENTATION
#include "binary_tree.hpp"

#include <iostream>
#include <random>
#include <chrono>

void print_sorted(BinaryTree<int>::Node* node)
{
    if (!node)
        return;

    print_sorted(node->left);
    std::cout << node->value << ' ';
    print_sorted(node->right);
}

int main()
{
    for (int i = 0; i < 100; i++)
    {
        std::cout << "Test #" << i + 1 << "\n\n";

        BinaryTree<int> t;

        // Insertion

        std::mt19937 mt(std::chrono::system_clock::now().time_since_epoch().count());
        std::uniform_int_distribution<> dist(0, 10);

        std::cout << "Insertion: ";

        for (int i = 0; i < 10; i++)
        {
            int value = dist(mt);
            t.push(value);
            std::cout << value << ' ';
        }

        std::cout << std::endl;
        print_sorted(t.root);
        std::cout << std::endl << std::endl;

        // Traverse by levels

        std::list<BinaryTreeNode<int>*> nodes(100);

        std::cout << "Traverse by levels:\n";
        t.traverse_levels(nodes.begin(), nodes.end());
        
        for (const auto& n : nodes)
        {
            if (n)
                std::cout << n->value << ' ';
            else
                break;
        }

        std::cout << std::endl << std::endl;
        
        // Height

        std::cout << "Height: " << t.get_height() << std::endl << std::endl;

        // Removing duplicates

        std::cout << "Removing duplicates:\n";
        t.remove_duplicates();

        print_sorted(t.root);
        std::cout << std::endl << std::endl;

        // Deleting an element

        int removed_value = dist(mt);

        std::cout << "Removing " << removed_value << std::endl;
        t.remove(removed_value);

        // Inorder traversal

        nodes.clear();
        nodes.resize(100);

        t.traverse_inorder(nodes.begin(), nodes.end());

        for (const auto& n : nodes)
        {
            if (n)
                std::cout << n->value << ' ';
            else
                break;
        }

        std::cout << std::endl << std::endl;
    }
}
