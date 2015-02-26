#include "basetree.h"

TreeIterator::TreeIterator():owner(nullptr), current(nullptr){}

TreeIterator::~TreeIterator()
{
    owner = nullptr;
    current = nullptr;
}

void *& TreeIterator::operator *() const
{
    if(owner)
    {
        return owner->asterics(current);
    }
    else
    {
        throw IteratorException();
    }
}

TreeIterator TreeIterator::operator ++()
{
    if(owner)
    {
        owner->next(current);
    }
    return *this;
}

TreeIterator TreeIterator::operator ++(int)
{
    TreeIterator result(*this);
    ++(*this);
    return result;
}

TreeIterator TreeIterator::operator --()
{
    if(owner)
    {
        owner->previous(current);
    }
    return *this;
}

TreeIterator TreeIterator::operator --(int)
{
    TreeIterator result(*this);
    --(*this);
    return result;
}

bool TreeIterator::operator ==(const TreeIterator & other) const
{
    return owner == other.owner && current == other.current;
}

bool TreeIterator::operator !=(const TreeIterator & other) const
{
    return !(*this == other);
}

TreeIterator::TreeIterator(const BaseTree *owner, void *current):owner(owner), current(current){}

BaseTree::BaseTree(){}

BaseTree::BaseTree(const BaseTree &other)
{
    copy(&other);
}

BaseTree::~BaseTree()
{
    clear();
}

BaseTree::Iterator BaseTree::find(const void *&value) const
{
    void * pointer = nullptr;
    findImpl(value, pointer);
    return Iterator(this, pointer);
}

BaseTree::Iterator BaseTree::insert(const void *&value)
{
    void * pointer = nullptr;
    insertImpl(value, pointer);
    return Iterator(this, pointer);
}

BaseTree::Iterator BaseTree::erase(const Iterator &position)
{
    if(this != position.owner)
    {
        throw IteratorException();
    }
    void * pointer = position.current;
    eraseImpl(pointer);
    return Iterator(this, pointer);
}

size_t BaseTree::erase(const void *&value)
{
    size_t counter = 0;
    void * pointer = nullptr;
    findImpl(value, pointer);
    while(pointer)
    {
        ++counter;
        eraseImpl(pointer);
        findImpl(value, pointer);
    }
    return counter;
}

BaseTree::Iterator BaseTree::begin() const
{
    return Iterator(this, beginImpl());
}

BaseTree::Iterator BaseTree::end() const
{
    return Iterator(this, nullptr);
}

void *& BaseTree::asterics(void * pointer) const
{
    return astericsImpl(pointer);
}

void BaseTree::next(void *&pointer) const
{
    nextImpl(pointer);
}

void BaseTree::previous(void *&pointer) const
{
    previousImpl(pointer);
}
