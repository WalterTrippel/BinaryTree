#ifndef CONSOLETREE_H
#define CONSOLETREE_H

#include "binarytree.h"

template<typename Data>
class ConsoleTree : public BinaryTree<Data>
{
public:
    ConsoleTree();
    ConsoleTree(std::initializer_list<Data> list);
    ConsoleTree(const ConsoleTree & other);
    ConsoleTree(ConsoleTree && other);
    virtual ~ConsoleTree();

    ConsoleTree & operator = (const ConsoleTree & other) = default;
    ConsoleTree & operator = (ConsoleTree && other) = default;

    template<class T>
    friend std::ostream & operator << (std::ostream & out, ConsoleTree & other);

    template<typename T>
    friend std::istream & operator >> (std::istream & in, ConsoleTree & other);

    virtual void printElements(std::vector<void *> array, std::ostream & out) const;
};

template<typename Data>
ConsoleTree<Data>::ConsoleTree():BinaryTree<Data>() {}

template<typename Data>
ConsoleTree<Data>::ConsoleTree(std::initializer_list<Data> list):BinaryTree<Data>(list) {}

template<typename Data>
ConsoleTree<Data>::ConsoleTree(const ConsoleTree &other):BinaryTree<Data>(other) {}

template<typename Data>
ConsoleTree<Data>::ConsoleTree(ConsoleTree &&other):BinaryTree<Data>(other) {}

template<typename Data>
ConsoleTree<Data>::~ConsoleTree() {}

template<typename Data>
void ConsoleTree<Data>::printElements(std::vector<void *> array, std::ostream & out) const
{
    std::vector<void *>::iterator i = array.begin();
    for(;i != array.end() - 1; ++i)
    {
        out << *(Data *)(*i) << ", ";
    }
    out << *(Data *)(*i) << "\n";
}

template<typename T>
std::ostream & operator << (std::ostream & out, ConsoleTree<T> & other)
{
    std::vector<void *> array;
    int size = 0;
    other.getElements(array, size);
    other.printElements(array, out);

    return out;
}

template<typename T>
std::istream & operator >> (std::istream & in, ConsoleTree<T> & other)
{
    T data;
    in >> data;
    other.push(data);
    return in;
}

#endif // CONSOLETREE_H
