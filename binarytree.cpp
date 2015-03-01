#include "binarytree.h"

class VoidTree::Implementation
{
public:
    Implementation();
    Implementation(std::vector<const void *> list, int size);
    Implementation(const Implementation & other);
    Implementation(Implementation && other);
    ~Implementation();

    Implementation & operator = (const Implementation & other);
    Implementation & operator = (Implementation && other);

    bool isEmpty() const;
    void clear();

    struct TreeNode
    {
        enum Branching {origin, leftBranch, rightBranch} branching;
        TreeNode() = default;
        TreeNode(const void *& data, int size, TreeNode * parent, Branching branching);
        ~TreeNode();

        TreeNode * left, * right, * parent;
        void * tData;
        int tSize;
    } * root;

    void clear(TreeNode *& root);
    void copy(TreeNode *& copiedNode, TreeNode * otherNode);
    void swap(TreeNode *& rvalue, TreeNode * lvalue);

    void inorder(TreeNode * node, TreeNode * rightMost, std::ostream & out);
    TreeNode * rightMost(TreeNode * node) const;
    TreeNode * leftMost(TreeNode * node) const;
    void delLeftmost(TreeNode* n, TreeNode* parent);
    TreeNode * findMin(TreeNode * node) const;

    /** Iterator compability */
    void *& astericsImpl(void *pointer) const;
    void nextImpl(void *& pointer) const;
    void previousImpl(void *& pointer) const;
    void *beginImpl() const;
    void findImpl(const void *&value, void *&pointer) const;
    void push(const void *&value, int size, void *&pointer);
    void remove(void *&pointer);

    void removeLeaf(TreeNode *& node);
    void removeChain(TreeNode *& node);
    void removeFork(TreeNode *& node);

};


/**
 * TreeNode implementation
*/
VoidTree::Implementation::TreeNode::TreeNode(const void *& data, int size,
                                             TreeNode * parent, Branching branching):branching(branching),
                                                                                     left(nullptr),
                                                                                     right(nullptr),
                                                                                     parent(nullptr),
                                                                                     tData(nullptr),
                                                                                     tSize(size)
{
    if(data && size)
    {
        tData = (void *)(new unsigned char [tSize]);
        for(int i = 0; i < tSize; ++i)
        {
            *((unsigned char *) tData + i) = *((unsigned char *) data + i);
        }
    }
}

VoidTree::Implementation::TreeNode::~TreeNode()
{
    if(tData && tSize)
    {
        for(int i = 0; i < tSize; ++i)
        {
            *((unsigned char *) tData + i) = 0;
        }
        tSize = 0;
        delete [] tData;
        tData = nullptr;
        left = nullptr;
        right = nullptr;
        parent = nullptr;
    }
}

/**
 *
 * BinaryTreeImplementation implementation
*/
VoidTree::Implementation::Implementation():root(nullptr) {}

VoidTree::Implementation::Implementation(const Implementation &other):root(nullptr)
{
    copy(root, other.root);
}

VoidTree::Implementation::Implementation(Implementation &&other):root(nullptr)
{
    swap(root, other.root);
}

VoidTree::Implementation::Implementation(std::vector<const void *> list, int size):root(nullptr)
{
    for(std::vector<const void *>::iterator i = list.begin(); i != list.end(); ++i)
    {
        void * pointer = nullptr;
        push(*i, size, pointer);
    }
}

VoidTree::Implementation::~Implementation()
{
    clear();
}

VoidTree::Implementation & VoidTree::Implementation::operator = (const VoidTree::Implementation & other)
{
    if(this != &other)
    {
        copy(root, other.root);
    }
    return * this;
}

VoidTree::Implementation & VoidTree::Implementation::operator = (VoidTree::Implementation && other)
{
    swap(root, other.root);
    return * this;
}

void VoidTree::Implementation::swap(TreeNode *&rvalue, TreeNode *lvalue)
{
    if(lvalue == nullptr)
    {
        rvalue = lvalue;
    }
    else
    {
        std::swap(rvalue, lvalue);
        swap(rvalue->left, lvalue->left);
        swap(rvalue->right, lvalue->right);
    }
}

void VoidTree::Implementation::clear(TreeNode *&root)
{
    if(root)
    {
        clear(root->left);
        clear(root->right);
        delete root;
        root = nullptr;
    }
}

void VoidTree::Implementation::copy(TreeNode *&copiedNode, TreeNode *otherNode)
{
    if(otherNode == nullptr)
    {
        copiedNode = otherNode;
    }
    else
    {
        copiedNode = new TreeNode((const void *&)otherNode->tData, otherNode->tSize, otherNode->parent, otherNode->branching);
        copy(copiedNode->left, otherNode->left);
        copy(copiedNode->right, otherNode->right);
    }
}

VoidTree::Implementation::TreeNode * VoidTree::Implementation::rightMost(TreeNode *node) const
{
    if(node != nullptr && node->right != nullptr)
    {
        return rightMost(node->right);
    }
    return node;
}

VoidTree::Implementation::TreeNode * VoidTree::Implementation::leftMost(TreeNode *node) const
{
    if(node != nullptr && node->left != nullptr)
    {
        return rightMost(node->left);
    }
    return node;
}

void VoidTree::Implementation::delLeftmost(TreeNode* n, TreeNode* parent)
{
    while(n->left != nullptr)
    {
        parent = n;
        n = n->left;
    }

    if(parent->left == n)
        parent->left = nullptr;
    else
        parent->right = nullptr;

    delete n;
}

VoidTree::Implementation::TreeNode * VoidTree::Implementation::findMin(TreeNode *node) const
{
    if(node == nullptr)
    {
        return nullptr;
    }
    else if(node->left == nullptr)
    {
        return node;
    }
    return findMin(node->left);
}

bool VoidTree::Implementation::isEmpty() const
{
    return root == nullptr;
}

/**
 *
 * BinaryTree itself implementation
*/

void *& VoidTree::Implementation::astericsImpl(void *pointer) const
{
    if(pointer)
    {
        return ((TreeNode *) pointer)->tData;
    }
    else
    {
        throw BSTException();
    }
}

void VoidTree::Implementation::nextImpl(void *&pointer) const
{
    TreeNode * current = (TreeNode *)pointer;
    if(current)
    {
        if(current->right)
        {
            current = current->right;
            while(current->left)
            {
                current = current->left;
            }
        }
        else
        {
            while(current->parent && current->branching == TreeNode::rightBranch)
            {
                current = current->parent;
            }

            if(!current->parent)
            {
                current = nullptr;
            }
            else
            {
                current = current->parent;
            }
        }
        pointer = (void *)current;
    }
}

void VoidTree::Implementation::previousImpl(void *&pointer) const
{
    TreeNode * current = (TreeNode *)pointer;

    if(current)
    {
        if(current->left)
        {
            current = current->left;

            while(current->right)
            {
                current = current->right;
            }
            pointer = (void *)current;
        }
        else
        {
            while(current->parent && current->branching == TreeNode::leftBranch)
            {
                current = current->parent;
            }
            if(current->parent)
            {
                pointer = (void *)current->parent;
            }
            else
            {
                assert(nullptr);
            }
        }
    }
    else
    {
        current = root;

        if(current)
        {
            while(current->right)
            {
                current = current->right;
            }
        }
        pointer = (void *)pointer;
    }
}

void *VoidTree::Implementation::beginImpl() const
{
    TreeNode * current = root;
    if(current)
    {
        while(current->left)
        {
            current = current->left;
        }
    }
    return (void *)current;
}

void VoidTree::Implementation::findImpl(const void *&value, void *&pointer) const
{
    TreeNode * current = root;
    if(!current)
    {
        throw BSTException();
    }
    unsigned char * data = (unsigned char *) value;
    unsigned char * current_data = (unsigned char *) current->tData;
    while(current && *data != *current_data)
    {
        current = *data < *current_data ? current->left : current->right;
        if(current)
            current_data = (unsigned char *) current->tData;
    }
    pointer = (void *)current;
}

void VoidTree::Implementation::push(const void *& data, int size, void *& pointer)
{
    TreeNode * parent = nullptr;
    TreeNode * current = root;
    TreeNode::Branching branching = TreeNode::origin;

    while(current)
    {
        parent = current;

        unsigned char * c_data = (unsigned char *)data;
        unsigned char * curr_c_data = (unsigned char *)current->tData;
        if(*c_data < *curr_c_data)
        {
            branching = TreeNode::leftBranch;
            current = current->left;
        }
        else
        {
            branching = TreeNode::rightBranch;
            current = current->right;
        }
    }

    current = new TreeNode(data, size, parent, branching);
    if(parent)
    {
        (branching == TreeNode::leftBranch ? parent->left : parent->right) = current;
        current->parent = parent;
    }
    else
    {
        root = current;
    }
    pointer = (void *)current;
}

void VoidTree::Implementation::remove(void *&pointer)
{
    if(pointer)
    {
        TreeNode * current = (TreeNode *)pointer;
        if(current->left && current->right)
        {
            removeFork(current);
        }
        else
        {
            nextImpl(pointer);
            bool rootRemoving = (root == current);
            removeChain(current);

            if(rootRemoving)
            {
                root = current;
            }
        }
    }
}

void VoidTree::Implementation::removeLeaf(TreeNode *&node)
{
    assert(node);
    assert(!node->left && !node->right);

    if(node->branching == TreeNode::leftBranch)
    {
        node->parent->left = nullptr;
    }
    else if(node->branching == TreeNode::rightBranch)
    {
        node->parent->right = nullptr;
    }

    delete node;
    node = nullptr;
    assert(!node);
}

void VoidTree::Implementation::removeChain(TreeNode *&node)
{
    assert(node);
    assert(!node->left || !node->right);

    if(!node->left && !node->right)
    {
        removeLeaf(node);
    }
    else
    {
        assert((!node->left && node->right) || (node->left && !node->right));
        TreeNode *& child = node->left ? node->left : node->right;
        TreeNode *& parent = node->parent;
        child->branching = node->branching;
        child->parent = parent;
        if(parent)
        {
            if(node->branching == TreeNode::leftBranch)
            {
                parent->left = child;
            }
            else if(node->branching == TreeNode::rightBranch)
            {
                parent->right = child;
            }
            else
            {
                assert(nullptr);
            }
        }

        assert(child->parent == parent && (parent->left == child || parent->right == child));
        assert((child->branching == TreeNode::leftBranch && parent->left == child) || (child->branching == TreeNode::rightBranch && parent->right == child));
        delete node;
        node = nullptr;
        assert(!node);
        node = child;
    }
}

void VoidTree::Implementation::removeFork(TreeNode *&node)
{
    assert(node);
    assert(node->left && node->right);

    TreeNode * replacement = node;
    replacement = replacement->right;
    while(replacement->left)
    {
        replacement = replacement->left;
    }
    node->tData = nullptr;
    node->tData = (void *)(new unsigned char[node->tSize]);
    for(int i = 0; i < node->tSize; ++i)
    {
        *((unsigned char *)node->tData + i) = *((unsigned char *)replacement->tData + i);
    }
    removeChain(replacement);
}

void VoidTree::Implementation::clear()
{
    clear(root);
}


/** VoidTree implementation */
/*VoidTree::VoidTree(std::vector<const void *> list, int size):pimpl(nullptr)
{
    pimpl = new Implementation(list, size);
}*/

VoidTree::VoidTree():pimpl(nullptr)
{
    pimpl = new Implementation;
}

VoidTree::VoidTree(const VoidTree &other):pimpl(other.pimpl)
{
    pimpl = new Implementation(*other.pimpl);
}

VoidTree::VoidTree(std::vector<const void *> list, int size)
{
    pimpl = new Implementation(list, size);
}

VoidTree::VoidTree(VoidTree &&other):pimpl(other.pimpl)
{
    std::swap(pimpl, other.pimpl);
}

VoidTree & VoidTree::operator = (const VoidTree & other)
{
    if(this != &other)
    {
        *pimpl = *other.pimpl;
    }
    return * this;
}

VoidTree & VoidTree::operator = (VoidTree && other)
{
    std::swap(pimpl, other.pimpl);
    return * this;
}

VoidTree::~VoidTree()
{
    pimpl->clear();
    delete pimpl;
    pimpl = nullptr;
}

void VoidTree::clear()
{
    pimpl->clear();
}

void *& VoidTree::astericsImpl(void *pointer) const
{
    pimpl->astericsImpl(pointer);
}

void VoidTree::nextImpl(void *&pointer) const
{
    pimpl->nextImpl(pointer);
}

void VoidTree::previousImpl(void *&pointer) const
{
    pimpl->previousImpl(pointer);
}

void * VoidTree::beginImpl() const
{
    return pimpl->beginImpl();
}

void VoidTree::findImpl(const void *&value, void *&pointer) const
{
    pimpl->findImpl(value, pointer);
}

void VoidTree::pushImpl(const void *&value, int size, void *&pointer)
{
    pimpl->push(value, size, pointer);
}

void VoidTree::popImpl(void *&pointer)
{
    pimpl->remove(pointer);
}

bool VoidTree::isEmpty() const
{
    return pimpl->isEmpty();
}
