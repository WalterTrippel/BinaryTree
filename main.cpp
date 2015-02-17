#include "prettytree.h"

int main()
{
    PrettyTree<int> tree = {1, 4, 2, 11, 45, 12, 47};
    tree.remove(1);
    std::cout << tree;
    return 0;
}

