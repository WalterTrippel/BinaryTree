#ifndef BINARYTREE_H
#define BINARYTREE_H

#include <iostream>
#include <exception>
#include <stack>
#include <vector>
#include <cstdlib>

class BSTException : public std::exception
{
    const char * what() const throw()
    {
        return "The element was not found!";
    }
};

class VoidTree;

template<typename Data>
class BinaryTree
{
public:
    BinaryTree();
    BinaryTree(const BinaryTree & other);
    BinaryTree(std::initializer_list<Data> list);
    BinaryTree(BinaryTree && other);
    ~BinaryTree();

    BinaryTree & operator = (const BinaryTree & other);
    BinaryTree & operator = (BinaryTree && other);

    void clear();
    void push(Data data);
    void remove(Data data);
    bool isEmpty() const;

    void getElements(std::vector<void *>&array, int &size);
private:
    VoidTree * pimpl;
};

class VoidTree
{
public:
    VoidTree();
    VoidTree(std::vector<const void *> list, int size);
    VoidTree(const VoidTree & other);
    VoidTree(VoidTree && other);
    ~VoidTree();

    VoidTree & operator = (const VoidTree & other);
    VoidTree & operator = (VoidTree && other);

    void clear();
    void push(const void * data, int size);
    void remove(void * data);
    bool isEmpty() const;
    void getElements(std::vector<void *>&array, int &size, int data_size);

private:
    class Implementation;
    Implementation  * pimpl;
};

template<typename Data>
BinaryTree<Data>::BinaryTree():pimpl(nullptr)
{
    pimpl = new VoidTree;
}

template<typename Data>
BinaryTree<Data>::BinaryTree(const BinaryTree &other):pimpl(other.pimpl)
{
    pimpl = new VoidTree(*other.pimpl);
}

template<typename Data>
BinaryTree<Data>::BinaryTree(std::initializer_list<Data> list):pimpl(nullptr)
{
    std::vector<const void *> void_list;
    for(typename std::initializer_list<Data>::iterator i = list.begin(); i != list.end(); ++i)
    {
        void_list.push_back(&(*i));
    }
    pimpl = new VoidTree(void_list, sizeof(Data));
}

template<typename Data>
BinaryTree<Data>::BinaryTree(BinaryTree &&other):pimpl(other.pimpl)
{
    std::swap(pimpl, other.pimpl);
}

template<typename Data>
BinaryTree<Data> & BinaryTree<Data>::operator =(const BinaryTree & other)
{
    if(this != & other)
    {
        *pimpl = *other.pimpl;
    }
    return * this;
}

template<typename Data>
BinaryTree<Data> & BinaryTree<Data>::operator =(BinaryTree && other)
{
    std::swap(pimpl, other.pimpl);
    return * this;
}

template<typename Data>
BinaryTree<Data>::~BinaryTree()
{
    pimpl->clear();
    delete pimpl;
    pimpl = nullptr;
}

template<typename Data>
void BinaryTree<Data>::clear()
{
    pimpl->clear();
}

template<typename Data>
void BinaryTree<Data>::push(Data data)
{
    pimpl->push((void *) &data, sizeof(Data));
}

template<typename Data>
void BinaryTree<Data>::remove(Data data)
{
    pimpl->remove((void *) &data);
}

template<typename Data>
bool BinaryTree<Data>::isEmpty() const
{
    return pimpl->isEmpty();
}

template<typename Data>
void BinaryTree<Data>::getElements(std::vector<void *>&array, int &size)
{
    pimpl->getElements(array, size, sizeof(Data));
}

#endif // BINARYTREE_H
