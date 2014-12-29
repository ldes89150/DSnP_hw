/****************************************************************************
  FileName     [ myMinHeap.h ]
  PackageName  [ util ]
  Synopsis     [ Define MinHeap ADT ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2014 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef MY_MIN_HEAP_H
#define MY_MIN_HEAP_H

#include <algorithm>
#include <vector>

template <class Data>
class MinHeap
{
public:
   MinHeap() : _data(0) {}
   MinHeap(size_t s) {
      _data.reserve(s);
      for (size_t i = 0; i < s; ++i) _data.push_back(Data());
      sort(_data.begin(), _data.end());
   }
   ~MinHeap() {}

   // For the following member functions,
   // We don't respond for the case vector "_data" is empty!
   const Data& operator [] (size_t i) const { return _data[i]; }   
   Data& operator [] (size_t i) { return _data[i]; }

   size_t size() const { return _data.size(); }

   // TODO
   const Data& min() const 
   { 
       return _data[0];
   }
   void insert(const Data& d) 
   {
      if(_data.empty())
      {
          _data.push_back(d);
          return;
      }
      else
      {
          _data.push_back(d);
          position v = last();
          while(!isRoot(v))
          {
              position u = parent(v);
              if((*u) < (*v)) break;
              std::swap(*u,*v);
              v=u;
          }
      }      
   }
   void delMin() 
   {
       if(_data.empty())
           return;
       if(_data.size() == 1)
       {
           _data.pop_back();
           return;
       }
       position u = root();
       *u = *(last());
       _data.pop_back();
       while(hasLeft(u))
       {
           position v = left(u);
           if(hasRight(u))
               if(*right(u)< *v)
                   v = right(u);
           if(*v <*u)
           {
               std::swap(*u,*v);
               u = v;
           }
           else break;
       }
       return; 
   }
   void delData(size_t i) 
   {
        if(i == 0)
            delMin();
        else
        {
            position u = _data.begin() +i;
            *u = *(last());
            _data.pop_back();
            while(hasLeft(u))
            {
                position v = left(u);
                if(hasRight(u))
                    if(*right(u) < *v)
                        v = right(u);
                if(*v < *u)
                {
                    std::swap(*u,*v);
                    u =v;
                }
                else break;
            }
        }   
        return;
   } // remove _data[i]

private:
    typedef typename vector<Data>::iterator position;
    position root()
    {
        return (position) _data.begin();
    }
    position last()
    {
        return (position) _data.end()-1;
    }
    bool hasLeft(const position& p)
    {
        size_t parentIndex = (size_t)(p-_data.begin());
        return (parentIndex*2+1) < _data.size();
    }
    bool hasRight(const position& p)
    {
        size_t parentIndex = (size_t)(p-_data.begin());
        return (parentIndex*2+2) < _data.size();
    }
    bool isRoot(const position& p)
    {
        return (p-_data.begin())==0;
    }
    
    position parent(const position& p)
    {
        size_t index = (size_t)(p-_data.begin()-1)/2;
        return (position) (_data.begin()+index);
    }

    position left(const position& p)
    {
        size_t index = (size_t)(p-_data.begin())*2+1;
        return (position) (_data.begin()+index);
    }
    position right(const position& p)
    {
        size_t index = (size_t)(p-_data.begin())*2+2;
        return (position) (_data.begin()+index);
    }
    void swap_position(position&a ,position& b)
    {
        std::swap(*a,*b);
    }

   // DO NOT add or change data members
   vector<Data>   _data;
};

#endif // MY_MIN_HEAP_H
