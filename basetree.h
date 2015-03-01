#ifndef BASETREE_H
#define BASETREE_H

#include <iostream>
#include <cassert>
#include <exception>
#include <stack>
#include <vector>
#include <cstdlib>

class IteratorException : public std::exception
{
    const char* what() const throw()
    {
        return "There is no such element";
    }
};

template<typename Data>
class TreeIterator;

template<typename Data>
class BaseTree
{
public:
    friend class TreeIterator<Data>;
    typedef TreeIterator<Data> Iterator;
    BaseTree();
    BaseTree(const BaseTree & other);
    BaseTree(BaseTree && other) = default;
    virtual ~BaseTree();

    Iterator find(const Data& value, int size) const;
    Iterator push(const Data& value, int size);
    Iterator remove(const Iterator &position);
    size_t remove(const Data& value, int size);

    Iterator begin() const;
    Iterator end() const;
private:
    Data& asterics(void * pointer) const;
    void next(void *& pointer) const;
    void previous(void *& pointer) const;

    virtual Data & astericsImpl(void *pointer) const = 0;
    virtual void nextImpl(void *& pointer) const = 0;
    virtual void previousImpl(void *& pointer) const = 0;
    virtual void *beginImpl() const = 0;
    virtual void findImpl(const Data& value, void *& pointer) const = 0;
    virtual void pushImpl(const Data& value, void *& pointer) = 0;
    virtual void popImpl(void *& pointer) = 0;
    virtual void clear(){}
    virtual void copy(const BaseTree *){}
};

template<typename Data>
class TreeIterator
{
public:
    friend class BaseTree<Data>;
    TreeIterator();
    TreeIterator(const TreeIterator & other) = default;
    TreeIterator(TreeIterator && other) = default;
    ~TreeIterator();

    TreeIterator & operator = (const TreeIterator & other) = default;
    TreeIterator & operator = (TreeIterator && other) = default;

    Data& operator *() const;
    TreeIterator operator ++();
    TreeIterator operator ++(int);
    TreeIterator operator --();
    TreeIterator operator --(int);
    bool operator == (const TreeIterator & other) const;
    bool operator != (const TreeIterator & other) const;

private:
    const BaseTree<Data> * owner;
    void *current;

    TreeIterator(const BaseTree<Data> * owner, void *current);
};

template<typename Data>
TreeIterator<Data>::TreeIterator():owner(nullptr), current(nullptr){}

template<typename Data>
TreeIterator<Data>::~TreeIterator()
{
    owner = nullptr;
    current = nullptr;
}

template<typename Data>
Data& TreeIterator<Data>::operator *() const
{
    if(owner)
    {
        return *((Data*)owner->asterics(current));
    }
    else
    {
        throw IteratorException();
    }
}

template<typename Data>
TreeIterator<Data> TreeIterator<Data>::operator ++()
{
    if(owner)
    {
        owner->next(current);
    }
    return *this;
}

template<typename Data>
TreeIterator<Data> TreeIterator<Data>::operator ++(int)
{
    TreeIterator<Data> result(*this);
    ++(*this);
    return result;
}

template<typename Data>
TreeIterator<Data> TreeIterator<Data>::operator --()
{
    if(owner)
    {
        owner->previous(current);
    }
    return *this;
}

template<typename Data>
TreeIterator<Data> TreeIterator<Data>::operator --(int)
{
    TreeIterator<Data> result(*this);
    --(*this);
    return result;
}

template<typename Data>
bool TreeIterator<Data>::operator ==(const TreeIterator & other) const
{
    return owner == other.owner && current == other.current;
}

template<typename Data>
bool TreeIterator<Data>::operator !=(const TreeIterator & other) const
{
    return !(*this == other);
}

template<typename Data>
TreeIterator<Data>::TreeIterator(const BaseTree<Data> *owner, void *current):owner(owner), current(current){}

template<typename Data>
BaseTree<Data>::BaseTree(){}

template<typename Data>
BaseTree<Data>::BaseTree(const BaseTree &other)
{
    copy(&other);
}

template<typename Data>
BaseTree<Data>::~BaseTree()
{
    clear();
}

template<typename Data>
typename BaseTree<Data>::Iterator BaseTree<Data>::find(const Data &value, int size) const
{
    void * pointer = nullptr;
    findImpl(value, pointer);
    return Iterator(this, pointer);
}

template<typename Data>
typename BaseTree<Data>::Iterator BaseTree<Data>::push(const Data &value, int size)
{
    void * pointer = nullptr;
    pushImpl(value, pointer);
    return Iterator(this, pointer);
}

template<typename Data>
typename BaseTree<Data>::Iterator BaseTree<Data>::remove(const Iterator &position)
{
    if(this != position.owner)
    {
        throw IteratorException();
    }
    void * pointer = position.current;
    popImpl(pointer);
    return Iterator(this, pointer);
}

template<typename Data>
size_t BaseTree<Data>::remove(const Data &value, int size)
{
    size_t counter = 0;
    void * pointer = nullptr;
    findImpl(value, pointer);
    while(pointer)
    {
        ++counter;
        popImpl(pointer);
        findImpl(value, pointer);
    }
    return counter;
}

template<typename Data>
typename BaseTree<Data>::Iterator BaseTree<Data>::begin() const
{
    return Iterator(this, beginImpl());
}

template<typename Data>
typename BaseTree<Data>::Iterator BaseTree<Data>::end() const
{
    return Iterator(this, nullptr);
}

template<typename Data>
Data & BaseTree<Data>::asterics(void * pointer) const
{
    return astericsImpl(pointer);
}

template<typename Data>
void BaseTree<Data>::next(void *&pointer) const
{
    nextImpl(pointer);
}

template<typename Data>
void BaseTree<Data>::previous(void *&pointer) const
{
    previousImpl(pointer);
}

#endif // BASETREE_H
