/****************************************************************************
  FileName     [ array.h ]
  PackageName  [ util ]
  Synopsis     [ Define dynamic array package ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2005-2014 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef ARRAY_H
#define ARRAY_H

#include <cassert>
#include <algorithm>

using namespace std;

// NO need to implement class ArrayNode
//
template <class T>
class Array
{
public:
   Array() : _data(0), _size(0), _capacity(0) {}
   ~Array() { delete []_data; }

   // DO NOT add any more data member or function for class iterator
   class iterator
   {
      friend class Array;

   public:
      iterator(T* n= 0): _node(n) {}
      iterator(const iterator& i): _node(i._node) {}
      ~iterator() {} // Should NOT delete _node

      // TODO: implement these overloaded operators
      const T& operator * () const { return (*_node); }
      T& operator * () { return (*_node); }
      iterator& operator ++ () 
      { 
          _node++;
          return (*this); 
      }
      iterator operator ++ (int) 
      { 
          iterator tmp = (*this);
          _node++;
          return tmp; 
      }
      iterator& operator -- () 
      { 
          _node--;
          return (*this); 
      }
      iterator operator -- (int) 
      { 
          iterator tmp = (*this);
          _node--;
          return tmp;
      }

      iterator operator + (int i) const 
      { 
          return iterator(_node+i); 
      }

      iterator& operator += (int i) 
      { 
          _node+=i;
          return (*this); 
      }

      iterator& operator = (const iterator& i) 
      { 
          _node = i._node;
          return (*this); 
      }

      bool operator != (const iterator& i) const { return _node != i._node; }
      bool operator == (const iterator& i) const { return _node == i._node; }

   private:
      T*    _node;
   };

   // TODO: implement these functions
   iterator begin() const { return iterator(_data); }
   iterator end() const { return iterator(_data+_size); }
   bool empty() const { return _size == 0; }
   size_t size() const { return _size; }

   T& operator [] (size_t i) { return _data[i]; }
   const T& operator [] (size_t i) const { return _data[i]; }

   void push_back(const T& x) 
   {
       if(_capacity == _size)
       {
           if(_capacity<10)
               _capacity+=5;
           else
               _capacity*=2;
           T* dataArrayNew = new T[_capacity];
           for(size_t i = 0; i != _size;i++)
           {
              dataArrayNew[i] = _data[i];
           }
           _data = dataArrayNew;
       }
       _data[_size++] = x;
   }
   void pop_front() 
   {
      if(not empty())
      {
          for(T* ptr = begin()._node;ptr!=end()._node-1;++ptr)
          {
              *ptr = *(ptr+1);
          }
          _size--;
      } 
   }
   void pop_back() 
   {
       if(not empty())
       {
           _size--;
       }
   }

   bool erase(iterator pos) 
   {
       if(empty())
       {
           return false;
       }
       else
       {
           for(T* ptr = pos._node;ptr != end()._node-1;++ptr)
           {
               *ptr = *(ptr+1);
           }
           _size--;
           return true;
       }
   }
   bool erase(const T& x) 
   { 
       if(empty())
       {
           return false;
       }
       else
       {
           bool delx = false;
           for(iterator itr = begin();itr != end();++itr)
           {
               if((*itr)==x)
               {
                   erase(itr);
                   delx = true;
               }
           }
           return delx;
       }
   }

   void clear() 
   {
       _size = 0;
   }

   // This is done. DO NOT change this one.
   void sort() const { if (!empty()) ::sort(_data, _data+_size); }

   // Nice to have, but not required in this homework...
   // void reserve(size_t n) { ... }
   // void resize(size_t n) { ... }

private:
   T*           _data;
   size_t       _size;       // number of valid elements
   size_t       _capacity;   // max number of elements

   // [OPTIONAL TODO] Helper functions; called by public member functions
};

#endif // ARRAY_H
