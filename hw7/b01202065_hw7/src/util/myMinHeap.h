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
          if(_data[0]<d)
          {
              _data.push_back(d);
          }
          else
          {
              Data tmp = _data[0];
              _data[0] = d;
              _data.push_back(d);
          }
          return;
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
       typename vector<Data>::iterator itr = std::min_element(_data.begin()+1,_data.end());
       std::swap(_data[0],*itr);
       std::swap(*(_data.end()-1),*itr);
       _data.pop_back();
       return; 
   }
   void delData(size_t i) 
   {
      if(i == 0)
         delMin();
     else
        _data.erase(_data.begin()+i); 
     return;
   } // remove _data[i]

private:
   // DO NOT add or change data members
   vector<Data>   _data;
};

#endif // MY_MIN_HEAP_H
