#include "binarytree.h"

int main()
{
    BinaryTree<int> tree = {10, 12, 11, 122, 8, 7, 999};
    //tree.clear();

    tree.remove(5);

    for(BinaryTree<int>::Iterator it = tree.begin(); it != tree.end(); ++it)
    {
        std::cout << *it << " ";
    }

    return 0;
}

