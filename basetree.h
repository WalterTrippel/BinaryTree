#ifndef BASETREE_H
#define BASETREE_H

#include <iostream>

class TreeIterator;

class BaseTree
{
public:
    friend class TreeIterator;
    typedef TreeIterator Iterator;
    BaseTree();
    BaseTree(const BaseTree & other);
    BaseTree(BaseTree && other) = default;
    ~BaseTree();

    Iterator find(const void *& value) const;
    Iterator insert(const void *& value);
    Iterator erase(const Iterator &position);
    size_t erase(const void *& value);

    Iterator begin() const;
    Iterator end() const;
private:
    void *& asterics(void * pointer) const;
    void next(void *& pointer) const;
    void previous(void *& pointer) const;

    virtual void *& astericsImpl(void *pointer) const = 0;
    virtual void nextImpl(void *& pointer) const = 0;
    virtual void previousImpl(void *& pointer) const = 0;
    virtual void *beginImpl() const = 0;
    virtual void findImpl(const void *& value, void *& pointer) const = 0;
    virtual void insertImpl(const void *& value, void *& pointer) = 0;
    virtual void eraseImpl(void *& pointer) = 0;
    virtual void clear(){}
    virtual void copy(const BaseTree *){}
};

class TreeIterator
{
public:
    friend class BaseTree;
    TreeIterator();
    TreeIterator(const TreeIterator & other) = default;
    TreeIterator(TreeIterator && other) = default;
    ~TreeIterator();

    TreeIterator & operator = (const TreeIterator & other) = default;
    TreeIterator & operator = (TreeIterator && other) = default;

    void *& operator *() const;
    TreeIterator operator ++();
    TreeIterator operator ++(int);
    TreeIterator operator --();
    TreeIterator operator --(int);
    bool operator == (const TreeIterator & other) const;
    bool operator != (const TreeIterator & other) const;

private:
    const BaseTree * owner;
    void *current;

    TreeIterator(const BaseTree * owner, void *current);
};

class IteratorException : public std::exception
{
    const char* what() const throw()
    {
        return "There is no such element";
    }
};

#endif // BASETREE_H
