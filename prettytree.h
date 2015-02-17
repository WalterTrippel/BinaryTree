#ifndef PRETTYTREE_H
#define PRETTYTREE_H

#include "consoletree.h"

template<typename Data>
class PrettyTree : public ConsoleTree<Data>
{
public:
    PrettyTree();
    PrettyTree(std::initializer_list<Data> list);
    PrettyTree(const PrettyTree & other);
    PrettyTree(PrettyTree && other);
    ~PrettyTree();

    PrettyTree & operator = (const PrettyTree & other) = default;
    PrettyTree & operator = (PrettyTree && other) = default;

private:
    void printElements(std::vector<void *> array, std::ostream & out) const override;
};

template<typename Data>
PrettyTree<Data>::PrettyTree():ConsoleTree<Data>() {}

template<typename Data>
PrettyTree<Data>::PrettyTree(std::initializer_list<Data> list):ConsoleTree<Data>(list) {}

template<typename Data>
PrettyTree<Data>::PrettyTree(const PrettyTree &other):ConsoleTree<Data>(other) {}

template<typename Data>
PrettyTree<Data>::PrettyTree(PrettyTree &&other):ConsoleTree<Data>(other) {}

template<typename Data>
PrettyTree<Data>::~PrettyTree() {}

template<typename Data>
void PrettyTree<Data>::printElements(std::vector<void *> array, std::ostream &out) const
{
    for(std::vector<void *>::iterator i = array.begin(); i != array.end() ; ++i)
    {
        Data a = *(Data *)*i;
        out << "(" << *(Data *)*i << "->" << ")" << "\n";
    }
}


#endif // PRETTYTREE_H
