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

    void clear();
    void push(const void * data, int size);
    void remove(void * data);
    bool isEmpty() const;

    struct TreeNode
    {
        TreeNode() = default;
        TreeNode(const void * data, int size);
        ~TreeNode();

        void push(const void * data, int size);
        TreeNode * left;
        TreeNode * right;
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
    void getElements(std::vector<void *>& array, int &size, int data_size) const;
};


/**
 * TreeNode implementation
*/
VoidTree::Implementation::TreeNode::TreeNode(const void * data, int size):left(nullptr), right(nullptr), tData(nullptr), tSize(size)
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

    }
}

void VoidTree::Implementation::TreeNode::push(const void * data, int size)
{
    unsigned long * c_data = (unsigned long *)data;
    unsigned long * curr_c_data = (unsigned long *)this->tData;
    if(*c_data < *curr_c_data)
    {
        if(left == nullptr)
        {
            left = new TreeNode(data, size);
        }
        else
        {
            left->push(data, size);
        }
    }
    else if(*c_data >= *curr_c_data)
    {
        if(right == nullptr)
        {
            right = new TreeNode(data, size);
        }
        else
        {
            right->push(data, size);
        }
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
        push(*i, size);
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

void VoidTree::Implementation::clear()
{
    clear(root);
}

void VoidTree::Implementation::clear(TreeNode *&root)
{
    if(root != nullptr)
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
        copiedNode = new TreeNode(otherNode->tData, otherNode->tSize);
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

void VoidTree::Implementation::push(const void * data, int size)
{
    if(root == nullptr)
    {
        root = new TreeNode(data, size);
    }
    else
    {
        root->push(data, size);
    }
}

bool VoidTree::Implementation::isEmpty() const
{
    return root == nullptr;
}

void VoidTree::Implementation::inorder(TreeNode *node, TreeNode *rightMost, std::ostream &out)
{
    if(node == nullptr)
    {
        return;
    }
    else if(node->left)
    {
        inorder(node->left, rightMost, out);
    }
    unsigned char * c_data = (unsigned char *)node->tData;
    out << *c_data;
    if(node != rightMost)
    {
        out << ", ";
    }
    if(node->right)
    {
        inorder(node->right, rightMost, out);
    }
}

void VoidTree::Implementation::getElements(std::vector<void *>&array, int &size, int data_size) const
{
    size = 0;
    array.clear();
    Implementation::TreeNode * node = root;
    std::stack< Implementation::TreeNode*> stack;
    do
    {
        while(node != nullptr)
        {
            ++size;
            stack.push(node);
            node = node->left;
        }
        do
        {
            Implementation::TreeNode * top = stack.top();
            stack.pop();
            if(top != nullptr && top->right != nullptr)
            {
                stack.push(node);
                node = top->right;
                break;
            }
        }
        while(!stack.empty());
    }
    while(!stack.empty());


    node = root;
    int index = 0;
    for(int i = 0; i < size; ++i)
    {
        void * current = (void *)(new unsigned char[data_size]);
        array.push_back(current);
    }

    do
    {
        while(node != nullptr)
        {
            stack.push(node);
            node = node->left;
        }
        do
        {
            Implementation::TreeNode * top = stack.top();
            stack.pop();
            if(top != nullptr)
            {
                for(int j = 0; j < data_size; ++j)
                {
                    *((unsigned char *) array.at(index) + j) = *((unsigned char *) top->tData + j);
                }
                ++index;
            }
            if(top != nullptr && top->right != nullptr)
            {
                stack.push(node);
                node = top->right;
                break;
            }
        }
        while(!stack.empty());
    }
    while(!stack.empty());
}

/**
 *
 * BinaryTree itself implementation
*/

void VoidTree::Implementation::remove(void *data)
{
    TreeNode *temp = root, *parent = root;
    unsigned char * key = (unsigned char *) data;
    unsigned char * local_data = (unsigned char *)temp->tData;
    if(isEmpty())
    {
        throw BSTException();
    }
    else
    {
        while(temp)
        {
            local_data = (unsigned char *)temp->tData;
            if(*local_data == *key)
            {
                break;
            }
            else
            {
                parent = temp;
                temp = *key > *local_data ? temp->right : temp->left;
            }
        }
    }
    if(temp == nullptr)
    {
        return;
    }
    else if(temp == root)
    {
        if(temp->right == nullptr && temp->left == nullptr)
        {
            root = nullptr;
        }
        else if(temp->left == nullptr)
        {
            root = temp->right;
        }
        else if(temp->right == nullptr)
        {
            root = temp->left;
        }
        else
        {
            TreeNode *current_temp;
            current_temp = temp->right;
            while(current_temp->left != nullptr)
            {
                temp = current_temp;
                current_temp = current_temp->left;
            }
            if(current_temp != temp->right)
            {
                temp->left = current_temp->right;
                current_temp->right = root->right;
            }
            current_temp->left = root->left;
            root = current_temp;
        }
    }
    else
    {
        if(temp->right == nullptr && temp->left == nullptr){
            if(parent->right == temp)
            {
                parent->right = nullptr;
            }
            else
            {
                parent->left = nullptr;
            }
        }
        else if(temp->left == nullptr){
            if(parent->right == temp)
            {
                parent->right = temp->right;
            }
            else
            {
                parent->left = temp->right;
            }
        }
        else if(temp->right == nullptr)
        {
            if(parent->right == temp)
            {
                parent->right = temp->left;
            }
            else
            {
                parent->left = temp->left;
            }
        }
        else if (temp->left != nullptr && temp->right != nullptr)
        {
            TreeNode *curr_right = temp->right;
            if(curr_right->left == nullptr && curr_right->right == nullptr)
            {
                temp->tData = nullptr;
                temp->tData = (void *)(new unsigned char[temp->tSize]);
                for(int i = 0; i < temp->tSize; ++i)
                {
                    *((unsigned char *)temp->tData + i) = *((unsigned char *)curr_right->tData + i);
                }
                delete curr_right;
                temp->right = nullptr;
            }
            else
            {
                if((temp->right)->left != nullptr)
                {
                    TreeNode* lcurr;
                    TreeNode* lcurr_parent;
                    lcurr_parent = temp->right;
                    lcurr = (temp->right)->left;
                    while(lcurr->left != nullptr)
                    {
                        lcurr_parent = lcurr;
                        lcurr = lcurr->left;
                    }
                    temp->tData = nullptr;
                    temp->tData = (void *)(new unsigned char[temp->tSize]);
                    for(int i = 0; i < temp->tSize; ++i)
                    {
                        *((unsigned char *)temp->tData + i) = *((unsigned char *)lcurr->tData + i);
                    }
                    delete lcurr;
                    lcurr_parent->left = nullptr;
                }
                else
                {
                    TreeNode *tmp = temp->right;
                    temp->tData = nullptr;
                    temp->tData = (void *)(new unsigned char[temp->tSize]);
                    for(int i = 0; i < temp->tSize; ++i)
                    {
                        *((unsigned char *)temp->tData + i) = *((unsigned char *)tmp->tData + i);
                    }
                    temp->right = tmp->right;
                    delete tmp;
                }
            }
        }
    }
}

VoidTree::VoidTree():pimpl(nullptr)
{
    pimpl = new Implementation;
}

VoidTree::VoidTree(const VoidTree &other):pimpl(other.pimpl)
{
    pimpl = new Implementation(*other.pimpl);
}

VoidTree::VoidTree(std::vector<const void *> list, int size):pimpl(nullptr)
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

void VoidTree::push(const void *data, int size)
{
    pimpl->push(data, size);
}

void VoidTree::remove(void *data)
{
    pimpl->remove(data);
}

bool VoidTree::isEmpty() const
{
    return pimpl->isEmpty();
}

void VoidTree::getElements(std::vector<void *>&array, int &size, int data_size)
{
    pimpl->getElements(array, size, data_size);
}
