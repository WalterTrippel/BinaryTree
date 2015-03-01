#ifndef BINARYTREE_H
#define BINARYTREE_H

#include "basetree.h"

class BSTException : public std::exception
{
    const char * what() const throw()
    {
        return "The element was not found!";
    }
};

template<typename Data>
class IterableTree;

template<typename Data>
class BinaryTree
{
public:
    typedef TreeIterator<Data> Iterator;
    BinaryTree();
    BinaryTree(const BinaryTree & other);
    BinaryTree(std::initializer_list<Data> list);
    BinaryTree(BinaryTree && other);
    ~BinaryTree();

    BinaryTree & operator = (const BinaryTree & other);
    BinaryTree & operator = (BinaryTree && other);

    void push(const Data &data);
    void remove(const Data &data);
    void clear();
    bool isEmpty() const;
    Iterator begin() const;
    Iterator end() const;

private:
    IterableTree<Data> * pimpl;
};

class VoidTree;

template<typename Data>
class IterableTree : public BaseTree<Data>
{
public:
    typedef TreeIterator<Data> Iterator;
    IterableTree();
    IterableTree(const IterableTree & other);
    IterableTree(std::initializer_list<Data> list);
    IterableTree(IterableTree && other);
    ~IterableTree();

    IterableTree & operator = (const IterableTree & other);
    IterableTree & operator = (IterableTree && other);

    bool isEmpty() const;
    void clear();

private:
    Data & astericsImpl(void *pointer) const;
    void nextImpl(void *& pointer) const;
    void previousImpl(void *& pointer) const;
    void *beginImpl() const;
    void findImpl(const Data& value, void *& pointer) const;
    void pushImpl(const Data& value, void *& pointer);
    void popImpl(void *& pointer);
    VoidTree * pimpl;
};

class VoidTree
{
public:
    VoidTree();
    VoidTree(const VoidTree & other);
    VoidTree(std::vector<const void *> list, int size);
    VoidTree(VoidTree && other);
    ~VoidTree();

    VoidTree & operator = (const VoidTree & other);
    VoidTree & operator = (VoidTree && other);

    void *& astericsImpl(void *pointer) const;
    void nextImpl(void *& pointer) const;
    void previousImpl(void *& pointer) const;
    void *beginImpl() const;
    void findImpl(const void *&value, void *&pointer) const;
    void pushImpl(const void *&value, int size, void *&pointer);
    void popImpl(void *&pointer);
    void clear();
    bool isEmpty() const;

private:
    class Implementation;
    Implementation * pimpl;
};

/** BinaryTree implementation */
template<typename Data>
BinaryTree<Data>::BinaryTree():pimpl(nullptr)
{
    pimpl = new IterableTree<Data>;
}

template<typename Data>
BinaryTree<Data>::BinaryTree(const BinaryTree &other):pimpl(other.pimpl)
{
    pimpl = new IterableTree<Data>(*other.pimpl);
}

template<typename Data>
BinaryTree<Data>::BinaryTree(std::initializer_list<Data> list):pimpl(nullptr)
{
    pimpl = new IterableTree<Data>(list);
}

template<typename Data>
BinaryTree<Data>::BinaryTree(BinaryTree &&other):pimpl(other.pimpl)
{
    std::swap(pimpl, other.pimpl);
}

template<typename Data>
BinaryTree<Data>::~BinaryTree()
{
    pimpl->clear();
    delete pimpl;
    pimpl = nullptr;
}

template<typename Data>
BinaryTree<Data> & BinaryTree<Data>::operator =(const BinaryTree & other)
{
    if(this != &other)
    {
        *this = *other.pimpl;
    }
    return *this;
}

template<typename Data>
BinaryTree<Data> & BinaryTree<Data>::operator =(BinaryTree && other)
{
    std::swap(pimpl, other.pimpl);
    return *this;
}

template<typename Data>
void BinaryTree<Data>::push(const Data &data)
{
    pimpl->push(data, sizeof(Data));
}

template<typename Data>
void BinaryTree<Data>::remove(const Data &data)
{
    pimpl->remove(data, sizeof(Data));
}

template<typename Data>
void BinaryTree<Data>::clear()
{
    pimpl->clear();
}

template<typename Data>
bool BinaryTree<Data>::isEmpty() const
{
    return pimpl->isEmpty();
}

template<typename Data>
TreeIterator<Data> BinaryTree<Data>::begin() const
{
    return pimpl->begin();
}

template<typename Data>
TreeIterator<Data> BinaryTree<Data>::end() const
{
    return pimpl->end();
}

/** IterableTree pimpl implementation */
template<typename Data>
IterableTree<Data>::IterableTree():pimpl(nullptr)
{
    pimpl = new VoidTree;
}

template<typename Data>
IterableTree<Data>::IterableTree(const IterableTree &other):pimpl(other.pimpl)
{
    pimpl = new VoidTree(*other.pimpl);
}

template<typename Data>
IterableTree<Data>::IterableTree(std::initializer_list<Data> list):pimpl(nullptr)
{
    std::vector<const void *> vect;
    for(typename std::initializer_list<Data>::iterator it = list.begin(); it != list.end(); ++it)
    {
        vect.push_back(&(*it));
    }
    pimpl = new VoidTree(vect, sizeof(Data));
}

template<typename Data>
IterableTree<Data>::IterableTree(IterableTree &&other):pimpl(other.pimpl)
{
    std::swap(pimpl, other.pimpl);
}

template<typename Data>
IterableTree<Data> & IterableTree<Data>::operator =(const IterableTree & other)
{
    if(this != &other)
    {
        *pimpl = *other.pimpl;
    }
    return * this;
}

template<typename Data>
IterableTree<Data> & IterableTree<Data>::operator =(IterableTree && other)
{
    std::swap(pimpl, other.pimpl);
    return * this;
}

template<typename Data>
IterableTree<Data>::~IterableTree()
{
    pimpl->clear();
    delete pimpl;
    pimpl = nullptr;
}

template<typename Data>
void IterableTree<Data>::clear()
{
    pimpl->clear();
}

template<typename Data>
Data & IterableTree<Data>::astericsImpl(void *pointer) const
{
    pimpl->astericsImpl(pointer);
}

template<typename Data>
void IterableTree<Data>::nextImpl(void *&pointer) const
{
    pimpl->nextImpl(pointer);
}

template<typename Data>
void * IterableTree<Data>::beginImpl() const
{
    return pimpl->beginImpl();
}

template<typename Data>
void IterableTree<Data>::findImpl(const Data &value, void *&pointer) const
{
    const void * _data = (void *) &value;
    pimpl->findImpl(_data, pointer);
}

template<typename Data>
void IterableTree<Data>::previousImpl(void *&pointer) const
{
    pimpl->previousImpl(pointer);
}

template<typename Data>
void IterableTree<Data>::pushImpl(const Data &data, void *& pointer)
{
    const void * _data = (void *) &data;
    pimpl->pushImpl(_data, sizeof(Data), pointer);
}

template<typename Data>
void IterableTree<Data>::popImpl(void *& pointer)
{
    pimpl->popImpl(pointer);
}

template<typename Data>
bool IterableTree<Data>::isEmpty() const
{
    return pimpl->isEmpty();
}

#endif // BINARYTREE_H
