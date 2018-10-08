// HashSet.hpp
//
// ICS 46 Spring 2018
// Project #4: Set the Controls for the Heart of the Sun
//
// A HashSet is an implementation of a Set that is a separately-chained
// hash table, implemented as a dynamically-allocated array of linked
// lists.  At any given time, the HashSet has a "size" indicating
// how many elements are stored within it, along with a "capacity"
// indicating the size of the array.
//
// As elements are added to the HashSet and the proportion of the HashSet's
// size to its capacity exceeds 0.8 (i.e., there are more than 80% as many
// elements as there are array cells), the HashSet should be resized so
// that it is twice as large as it was before.
//
// You are not permitted to use the containers in the C++ Standard Library
// (such as std::set, std::map, or std::vector) to store the information
// in your data structure.  Instead, you'll need to use a dynamically-
// allocated array and your own linked list implemenation; the linked list
// doesn't have to be its own class, though you can do that, if you'd like.

#ifndef HASHSET_HPP
#define HASHSET_HPP

#include <functional>
#include "Set.hpp"
using namespace std;


template <typename ElementType>
class HashSet : public Set<ElementType>
{
public:
    // The default capacity of the HashSet before anything has been
    // added to it.
    static constexpr unsigned int DEFAULT_CAPACITY = 10;

    // A HashFunction is a function that takes a reference to a const
    // ElementType and returns an unsigned int.
    using HashFunction = std::function<unsigned int(const ElementType&)>;

public:
    // Initializes a HashSet to be empty, so that it will use the given
    // hash function whenever it needs to hash an element.
    explicit HashSet(HashFunction hashFunction);

    // Cleans up the HashSet so that it leaks no memory.
    virtual ~HashSet() noexcept;

    // Initializes a new HashSet to be a copy of an existing one.
    HashSet(const HashSet& s);

    // Initializes a new HashSet whose contents are moved from an
    // expiring one.
    HashSet(HashSet&& s) noexcept;

    // Assigns an existing HashSet into another.
    HashSet& operator=(const HashSet& s);

    // Assigns an expiring HashSet into another.
    HashSet& operator=(HashSet&& s) noexcept;


    // isImplemented() should be modified to return true if you've
    // decided to implement a HashSet, false otherwise.
    virtual bool isImplemented() const noexcept override;


    // add() adds an element to the set.  If the element is already in the set,
    // this function has no effect.  This function triggers a resizing of the
    // array when the ratio of size to capacity would exceed 0.8.  In the case
    // where the array is resized, this function runs in linear time (with
    // respect to the number of elements, assuming a good hash function);
    // otherwise, it runs in constant time (again, assuming a good hash
    // function).
    virtual void add(const ElementType& element) override;


    // contains() returns true if the given element is already in the set,
    // false otherwise.  This function runs in constant time (with respect
    // to the number of elements, assuming a good hash function).
    virtual bool contains(const ElementType& element) const override;


    // size() returns the number of elements in the set.
    virtual unsigned int size() const noexcept override;


    // elementsAtIndex() returns the number of elements that hashed to a
    // particular index in the array.  If the index is out of the boundaries
    // of the array, this function returns 0.
    unsigned int elementsAtIndex(unsigned int index) const;


    // isElementAtIndex() returns true if the given element hashed to a
    // particular index in the array, false otherwise.  If the index is
    // out of the boundaries of the array, this functions returns 0.
    bool isElementAtIndex(const ElementType& element, unsigned int index) const;


private:
    HashFunction hashFunction;
    struct Nodes
    {
        ElementType key;
        Nodes* next = nullptr;

        
        Nodes(const ElementType& value, Nodes* next =nullptr):key(value),next(next)
        {}
        

};
    Nodes** hash;
    unsigned int total_capacity;
    float total_size = 0.0;
    void add_node(const ElementType& element,Nodes** hash);
    Nodes* copy_hash(Nodes* n);
    void resize_hash(Nodes** n,const ElementType& element);
    void delete_node(Nodes** n);

};



namespace impl_
{
    template <typename ElementType>
    unsigned int HashSet__undefinedHashFunction(const ElementType& element)
    {
        return 0;
    }
}
///--------------------------------------Helper Function---------------------------------------
template <typename ElementType>
void HashSet<ElementType>::add_node(const ElementType& element,Nodes** hash)
{
    
    unsigned int index = static_cast<unsigned int>(hashFunction(element)) % total_capacity;
    
    if(hash[index] == nullptr)
    {
        hash[index] = new Nodes{element,nullptr};
    }
    else
    {           
        
        hash[index] = new Nodes{element,hash[index]};
       
    }
    total_size+=1;
}

template<typename ElementType>
typename HashSet<ElementType>::Nodes* HashSet<ElementType>::copy_hash(Nodes* n)
{
    try
    {
        if(n == nullptr)
        {
            return nullptr;
        }
        else 
        {
            return new Nodes(n->key,copy_hash(n->next));
        }
    }
    catch(...)
    {
        throw;
    }
}

template <typename ElementType>
void HashSet<ElementType>::resize_hash(Nodes** n,const ElementType& element)
{
    Nodes** old_hash = hash;
    total_capacity = total_capacity * 2;

    hash = new Nodes*[total_capacity];

    for(int i=0;i<total_capacity;i++)
    {
        hash[i]=nullptr;
    }

    for(int m=0;m<total_capacity/2;m++)
    {
        if(old_hash[m]!=nullptr)
        {
            Nodes* temp = old_hash[m];
            while(temp!=nullptr)
            {
                add_node(temp->key,hash);
                temp = temp -> next;
            }
        }
    }

    
}


template <typename ElementType>
void HashSet<ElementType>::delete_node(Nodes** n)
{
    for(int i = 0;i<total_capacity;i++)
    {
        
        if(n[i]!=nullptr)
        {
            
            while(n[i]!=nullptr)
            {
       
                Nodes* temp = n[i];
                
                n[i]=n[i]->next;
             
                delete temp;
               
            }
        }
    }
    delete[] hash;
    hash = nullptr;
}
///--------------------------------------------------------------------------------------------

template <typename ElementType>
HashSet<ElementType>::HashSet(HashFunction hashFunction)
    : hashFunction{hashFunction}
{

    total_capacity = DEFAULT_CAPACITY;
    hash = new Nodes*[DEFAULT_CAPACITY];
    for(int i=0;i<total_capacity;i++)
    {
        hash[i] = nullptr;
    }
}


template <typename ElementType>
HashSet<ElementType>::~HashSet() noexcept
{
    delete_node(hash);
}


template <typename ElementType>
HashSet<ElementType>::HashSet(const HashSet& s)
    : hashFunction{impl_::HashSet__undefinedHashFunction<ElementType>},hash(s.hash),total_capacity(s.total_capacity),total_size(s.total_size)
{
    total_capacity = s.total_capacity;
    hash = new Nodes*[total_capacity];
    for(int i=0;i<total_capacity;i++)
    {
        hash[i]=nullptr;
    }
    total_size = s.total_size;
    for(int i = 0;i<total_capacity;i++)
    {
        if(s.hash[i]!=nullptr)
        {
            hash[i]=copy_hash(s.hash[i]);
        }
    }
}


template <typename ElementType>
HashSet<ElementType>::HashSet(HashSet&& s) noexcept
    : hashFunction{impl_::HashSet__undefinedHashFunction<ElementType>},hash(nullptr),total_capacity(0),total_size(0)
{
    swap(hash,s.hash);
    swap(total_capacity,s.total_capacity);
    swap(total_size,s.total_size);
}


template <typename ElementType>
HashSet<ElementType>& HashSet<ElementType>::operator=(const HashSet& s)
{
    
    if (this != &s)
    {
        
        delete_node(hash); 
        hashFunction = s.hashFunction;
       
        total_capacity = s.total_capacity;
        hash = new Nodes*[total_capacity];
        for(int i=0;i<total_capacity;i++)
        {
            hash[i]=nullptr;
        }
        total_size = s.total_size;
        for(int i = 0;i<total_capacity;i++)
        {
            if(s.hash[i]!=nullptr)
            {
                hash[i]=copy_hash(s.hash[i]);
            }
        }

    }
    return *this;
}


template <typename ElementType>
HashSet<ElementType>& HashSet<ElementType>::operator=(HashSet&& s) noexcept
{
    swap(hash,s.hash);
    swap(total_capacity,s.total_capacity);
    swap(total_size,s.total_size);
    return *this;
}


template <typename ElementType>
bool HashSet<ElementType>::isImplemented() const noexcept
{
    return true;
}


template <typename ElementType>
void HashSet<ElementType>::add(const ElementType& element)
{
    
    add_node(element,hash);
    
    if(total_size/total_capacity >= 0.8)
    {
        resize_hash(hash,element);
    }
    
   
}


template <typename ElementType>
bool HashSet<ElementType>::contains(const ElementType& element) const
{
    unsigned int index = static_cast<unsigned int>(hashFunction(element)) % total_capacity;
    
    if(hash[index]!=nullptr)
    {
        Nodes* n = hash[index];
        while(n!=nullptr)
        {
            if(n->key == element)
            {
                return true;
            }
            n = n -> next;
        }
    }

    return false;
}


template <typename ElementType>
unsigned int HashSet<ElementType>::size() const noexcept
{
    return total_size;
}


template <typename ElementType>
unsigned int HashSet<ElementType>::elementsAtIndex(unsigned int index) const
{
    int result = 0;
    if(index < total_capacity && hash[index]!=nullptr)
    {
        Nodes* temp = hash[index];
        while(temp!=nullptr)
        {
            result += 1;
            temp = temp -> next;
        }
    }
    return result;
}


template <typename ElementType>
bool HashSet<ElementType>::isElementAtIndex(const ElementType& element, unsigned int index) const
{
    Nodes* temp = hash[index];
    
    while(temp!=nullptr)
    {
        try
        {
            if (temp->key == element)
            {
                return true;
            }
            else
            {
                temp = temp -> next;
            }
        }
        catch(...)
        {
            throw;
        }
    }
    return false;
}



#endif // HASHSET_HPP

