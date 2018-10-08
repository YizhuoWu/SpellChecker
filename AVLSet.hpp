// AVLSet.hpp
//
// ICS 46 Spring 2018
// Project #4: Set the Controls for the Heart of the Sun
//
// An AVLSet is an implementation of a Set that is an AVL tree, which uses
// the algorithms we discussed in lecture to maintain balance every time a
// new element is added to the set.  The balancing is actually optional,
// with a bool parameter able to be passed to the constructor to explicitly
// turn the balancing on or off (on is default).  If the balancing is off,
// the AVL tree acts like a binary search tree (e.g., it will become
// degenerate if elements are added in ascending order).
//
// You are not permitted to use the containers in the C++ Standard Library
// (such as std::set, std::map, or std::vector) to store the information
// in your data structure.  Instead, you'll need to implement your AVL tree
// using your own dynamically-allocated nodes, with pointers connecting them,
// and with your own balancing algorithms used.

#ifndef AVLSET_HPP
#define AVLSET_HPP

#include <functional>
#include "Set.hpp"
using namespace std;


template <typename ElementType>
class AVLSet : public Set<ElementType>
{
public:
    // A VisitFunction is a function that takes a reference to a const
    // ElementType and returns no value.
    using VisitFunction = std::function<void(const ElementType&)>;

public:
    // Initializes an AVLSet to be empty, with or without balancing.
    explicit AVLSet(bool shouldBalance = true);

    // Cleans up the AVLSet so that it leaks no memory.
    virtual ~AVLSet() noexcept;

    // Initializes a new AVLSet to be a copy of an existing one.
    AVLSet(const AVLSet& s);

    // Initializes a new AVLSet whose contents are moved from an
    // expiring one.
    AVLSet(AVLSet&& s) noexcept;

    // Assigns an existing AVLSet into another.
    AVLSet& operator=(const AVLSet& s);

    // Assigns an expiring AVLSet into another.
    AVLSet& operator=(AVLSet&& s) noexcept;


    // isImplemented() should be modified to return true if you've
    // decided to implement an AVLSet, false otherwise.
    virtual bool isImplemented() const noexcept override;


    // add() adds an element to the set.  If the element is already in the set,
    // this function has no effect.  This function always runs in O(log n) time
    // when there are n elements in the AVL tree.
    virtual void add(const ElementType& element) override;


    // contains() returns true if the given element is already in the set,
    // false otherwise.  This function always runs in O(log n) time when
    // there are n elements in the AVL tree.
    virtual bool contains(const ElementType& element) const override;


    // size() returns the number of elements in the set.
    virtual unsigned int size() const noexcept override;


    // height() returns the height of the AVL tree.  Note that, by definition,
    // the height of an empty tree is -1.
    int height() const;


    // preorder() calls the given "visit" function for each of the elements
    // in the set, in the order determined by a preorder traversal of the AVL
    // tree.
    void preorder(VisitFunction visit) const;


    // inorder() calls the given "visit" function for each of the elements
    // in the set, in the order determined by an inorder traversal of the AVL
    // tree.
    void inorder(VisitFunction visit) const;


    // postorder() calls the given "visit" function for each of the elements
    // in the set, in the order determined by a postorder traversal of the AVL
    // tree.
    void postorder(VisitFunction visit) const;


private:
    struct Node
    {
        ElementType value;
        Node* left = nullptr;
        Node* right = nullptr;
    };
    Node* root;
    Node* current;
    
    bool balance = true;
    int total_size = 0;
    int the_height(Node*& temp) const;
    void delete_tree(Node* current);
    void copy_tree(Node*& current, const Node* src);
    void insert_node(Node*& current, const ElementType& element);
    void insert_node_not_balance(Node*& current, const ElementType& element);
    void rotate_left(Node*& current);
    void rotate_right(Node*& current);
    void pre_ord(Node* current, VisitFunction visit) const;
    void in_ord(Node* current, VisitFunction visit) const;
    void post_ord(Node* current, VisitFunction visit) const;
    bool if_contain(const ElementType& element, Node* current) const;
};

///-----------------------------Helper Functions------------------------------------------
template <typename ElementType>
int AVLSet<ElementType>::the_height(Node*& temp) const
{
    int left;
    int right;
    if(temp == nullptr)
    {
        return -1;
    }
    left = the_height(temp -> left);
    right = the_height(temp -> right);
    if(left > right)
    {
        return left+1;
    }
    else
    {
        return right+1;
    }
}

template <typename ElementType>
void AVLSet<ElementType>::delete_tree(Node* current)
{
    if(current == nullptr)
    {
        return;
    }
    delete_tree(current -> left);
    delete_tree(current -> right);
    delete current;
}

template <typename ElementType>
void AVLSet<ElementType>::copy_tree(Node*& current, const Node* src)
{
    if(src == nullptr)
    {
        current = nullptr;
    }
    else
    {
        current = new Node{src -> value};
        if(current -> left == nullptr && current -> right == nullptr)
        {
            return;
        }
        copy_tree(current -> left,src -> left);
        copy_tree(current -> right,src -> right);
    }
}

template <typename ElementType>
void AVLSet<ElementType>::insert_node(Node*& current, const ElementType& element)
{
    if(current == nullptr)
    {
        current = new Node{element};
    }
    else if(element > current -> value)
    {
        insert_node(current -> right,element);
        if(the_height(current -> right)-the_height(current->left) == 2)
        {
            if(element > current->right->value)
            {
                rotate_right(current);
            }
            else
            {
                rotate_left(current->right);
                rotate_right(current);
            }
        }
    }
    else if(element < current -> value)
    {
        insert_node(current -> left, element);
        if(the_height(current -> left)-the_height(current -> right) == 2)
        {
            if(element < current -> left -> value)
            {
                rotate_left(current);
            }
            else
            {
                rotate_right(current->left);
                rotate_left(current);
            }
        }
    }
}

template <typename ElementType>
void AVLSet<ElementType>::insert_node_not_balance(Node*& current, const ElementType& element)
{
    if(current == nullptr)
    {
        current = new Node{element};
    }
    else if(element > current-> value)
    {
        insert_node_not_balance(current->right,element);
    }
    else if(element<current->value)
    {
        insert_node_not_balance(current->left,element);
    }
}

template <typename ElementType>
void AVLSet<ElementType>::rotate_left(Node*& current)
{
    Node* result;
    result = current -> left;
    current -> left = result -> right;
    result -> right = current;
    current = result;
}

template <typename ElementType>
void AVLSet<ElementType>::rotate_right(Node*& current)
{
    Node* result;
    result = current -> right;
    current -> right = result -> left;
    result -> left = current;
    current = result;
}

template <typename ElementType>
void AVLSet<ElementType>::pre_ord(Node* current, VisitFunction visit) const
{
    if(current != nullptr)
    {
        visit(current -> value);
        pre_ord(current->left, visit);
        pre_ord(current->right, visit);
    }
}

template <typename ElementType>
void AVLSet<ElementType>::in_ord(Node* current, VisitFunction visit) const
{
    if(current!=nullptr)
    {
        in_ord(current->left, visit);
        visit(current->value);
        in_ord(current->right, visit);
    }
}        

template <typename ElementType>
void AVLSet<ElementType>::post_ord(Node* current, VisitFunction visit) const
{
    if(current!=nullptr)
    {
        post_ord(current->left, visit);
        post_ord(current->right,visit);
        visit(current->value);
    }
}

template <typename ElementType>
bool AVLSet<ElementType>::if_contain(const ElementType& element, Node* current) const
{
    while(current != nullptr)
    {
        if(current->value == element)
        {
            return true;
        }
        else if(current->value < element)
        {
            current = current->right;
        }
        else
        {
            current = current -> left;
        }
    }
    return false;
}
///--------------------------------------------------------------------------------------

template <typename ElementType>
AVLSet<ElementType>::AVLSet(bool shouldBalance)
{
    root = nullptr;
    balance = shouldBalance;
}


template <typename ElementType>
AVLSet<ElementType>::~AVLSet() noexcept
{
    delete_tree(root);
}


template <typename ElementType>
AVLSet<ElementType>::AVLSet(const AVLSet& s)
{
    copy_tree(root, s.root);
}


template <typename ElementType>
AVLSet<ElementType>::AVLSet(AVLSet&& s) noexcept
    :root{nullptr}
{
    swap(root,s.root);
}


template <typename ElementType>
AVLSet<ElementType>& AVLSet<ElementType>::operator=(const AVLSet& s)
{
    if(this != &s)
    {
        delete_tree(this -> root);
        root = nullptr;
        copy_tree(root,s.root);
    }
    return *this;
}


template <typename ElementType>
AVLSet<ElementType>& AVLSet<ElementType>::operator=(AVLSet&& s) noexcept
{
    swap(root,s.root);
    return *this;
}


template <typename ElementType>
bool AVLSet<ElementType>::isImplemented() const noexcept
{
    return true;
}


template <typename ElementType>
void AVLSet<ElementType>::add(const ElementType& element)
{
    try
    {
        if(contains(element) == true)
        {
            return;
        }
        else
        {
            if (balance == true)
            {
                insert_node(root, element);
            }
            else
            {
                insert_node_not_balance(root,element);
            }
            total_size+=1;
        }
    }
    catch(...)
    {
        throw;
    }
}


template <typename ElementType>
bool AVLSet<ElementType>::contains(const ElementType& element) const
{    
    return if_contain(element, root);
}


template <typename ElementType>
unsigned int AVLSet<ElementType>::size() const noexcept
{
    return total_size;
}


template <typename ElementType>
int AVLSet<ElementType>::height() const
{
    Node* result = root;
    return the_height(result);
}


template <typename ElementType>
void AVLSet<ElementType>::preorder(VisitFunction visit) const
{
    try
    {
    pre_ord(root, visit);
    }
    catch(...)
    {
        throw;
    }
}


template <typename ElementType>
void AVLSet<ElementType>::inorder(VisitFunction visit) const
{
    try
    {    
        in_ord(root, visit);
    }
    catch(...)
    {
        throw;
    }
}



template <typename ElementType>
void AVLSet<ElementType>::postorder(VisitFunction visit) const
{
    try
    {
        post_ord(root, visit);
    }
    catch(...)
    {
        throw;
    }
}




#endif // AVLSET_HPP

