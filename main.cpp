#include "prettytree.h"

int main()
{
    PrettyTree<int> tree = {8, 7, 17, 88, 77, 777777777};
    std::cout << tree;
    return 0;
}

