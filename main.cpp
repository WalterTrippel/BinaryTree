#include "binarytree.h"

int main()
{
    IterableTree<int> tree = {10, 12, 11, 122, 8, 7, 9};

    tree.remove(122, sizeof(int));

    for(IterableTree<int>::Iterator it = tree.begin(); it != tree.end(); ++it)
    {
        std::cout << *it << " ";
    }

    return 0;
}

