/****************************************************************************
  FileName     [ myHashSet.h ]
  PackageName  [ util ]
  Synopsis     [ Define HashSet ADT ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2014 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef MY_HASH_SET_H
#define MY_HASH_SET_H

#include <vector>
#include <algorithm>
using namespace std;

//---------------------
// Define HashSet class
//---------------------
// To use HashSet ADT,
// the class "Data" should at least overload the "()" and "==" operators.
//
// "operator ()" is to generate the hash key (size_t)
// that will be % by _numBuckets to get the bucket number.
// ==> See "bucketNum()"
//
// "operator ()" is to check whether there has already been
// an equivalent "Data" object in the HashSet.
// Note that HashSet does not allow equivalent nodes to be inserted
//
template <class Data>
class HashSet
{
public:
   HashSet() : _numBuckets(0), _buckets(0) {}
   HashSet(size_t b) : _numBuckets(0), _buckets(0) { init(b); }
   ~HashSet() { reset(); }

   // TODO: implement the HashSet<Data>::iterator
   // o An iterator should be able to go through all the valid Data
   //   in the Hash
   // o Functions to be implemented:
   //   - constructor(s), destructor
   //   - operator '*': return the HashNode
   //   - ++/--iterator, iterator++/--
   //   - operators '=', '==', !="
   //
   class iterator
   {
      friend class HashSet<Data>;

   public:
      const Data& operator * () const {return (*this);}
      Data& operator * () {return (*subitr);}
      iterator& operator ++ ()
      {
          if(subitr == bucketlist[nBucket-1].end())
          {
              return (*this);
          }
          if(subitr != bucketlist[n].end()-1)
          {
              subitr++;
              return (*this);
          } 
          else
          {
              for(size_t i = n+1; i < nBucket; i++)
              {
                    if(bucketlist[i].size() != 0)
                    {
                        n = i; 
                        subitr = bucketlist[n].begin();
                        return (*this);
                    }
              }
              n = nBucket -1;
              subitr = bucketlist[n].end();
              return (*this);
          }  
      }
      iterator operator ++ (int)
      {
          iterator tmp = (*this);
          this->operator++();
          return tmp;
      }

      iterator & operator -- ()
      {
          if(subitr != bucketlist[n].begin())
          {
              subitr--;
              return (*this);
          }
          else
          {
              for(size_t i = n-1;i >0; i--)
              {
                  if(bucketlist[i].size() != 0)
                  {
                      n=i;
                      subitr = bucketlist[n].end();
                      subitr--;
                      return (*this);
                  }
              }

          }
          return (*this);
      }
      iterator operator -- (int)
      {
          iterator tmp = (*this);
          this->operator--();
          return tmp;
      }

      bool operator == (const iterator& i) const
      {
          return (bucketlist == i.bucketlist) and (subitr == i.subitr); 
      }  
      bool operator != (const iterator& i) const
      {
          return (bucketlist != i.bucketlist) or (subitr != i.subitr);
      }
      iterator& operator = (const iterator& i)
      {
          n = i.n;
          bucketlist = i.bucketlist;
          nBucket = i.nBucket;
          subitr = i.subitr;
      }

   private:
      size_t n;
      vector<Data>* bucketlist;
      size_t nBucket;
      typename vector<Data>::iterator subitr;
   };

   void init(size_t b) {
      reset(); _numBuckets = b; _buckets = new vector<Data>[b]; }
   void reset() {
      _numBuckets = 0;
      if (_buckets) { delete [] _buckets; _buckets = 0; }
   }
   size_t numBuckets() const { return _numBuckets; }

   vector<Data>& operator [] (size_t i) { return _buckets[i]; }
   const vector<Data>& operator [](size_t i) const { return _buckets[i]; }

   // TODO: implement these functions
   //
   // Point to the first valid data
   iterator begin() const 
   { 
       iterator itr;
       itr.bucketlist = _buckets;
       itr.nBucket = _numBuckets;
       for(size_t i =0;i< _numBuckets; i++)
       {
           if(_buckets[i].size() != 0)
           {
               itr.n = i;
               itr.subitr = _buckets[i].begin();
               return itr;
           }
       }
       itr.n = _numBuckets -1;
       itr.subitr = _buckets[_numBuckets-1].end();
       return itr;
   }
   // Pass the end
   iterator end() const 
   {
       iterator itr;
       itr.bucketlist = _buckets;
       itr.nBucket = _numBuckets;
       itr.n = _numBuckets -1;
       itr.subitr = _buckets[_numBuckets-1].end();
       return itr;
   }
   // return true if no valid data
   bool empty() const 
   {
      return size() == 0; 
   }
   // number of valid data
   size_t size() const 
   {
       size_t count = 0;
       for(size_t i = 0;i != _numBuckets;i++)
       {
           count += _buckets[i].size();
       }
       return count;
   }

   // check if d is in the hash...
   // if yes, return true;
   // else return false;
   bool check(const Data& d) const 
   { 
       size_t n = bucketNum(d);
       typename vector<Data>::const_iterator itr = find(_buckets[n].begin(),
                                                        _buckets[n].end(),
                                                        d);
       return itr != _buckets[n].end(); 
   }

   // query if d is in the hash...
   // if yes, replace d with the data in the hash and return true;
   // else return false;
   bool query(Data& d) const 
   { 
       size_t n = bucketNum(d);
       typename vector<Data>::const_iterator itr = find(_buckets[n].begin(),
                                                        _buckets[n].end(),
                                                        d);
       if(itr == _buckets[n].end())
       {
           return false;
       }
       else
       {
           d = (*itr);
           return true;
       }
   }
   // update the entry in hash that is equal to d
   // if found, update that entry with d and return true;
   // else insert d into hash as a new entry and return false;
   bool update(const Data& d) 
   { 
       size_t n = bucketNum(d);
       typename vector<Data>::iterator itr = find(_buckets[n].begin(),
                                                  _buckets[n].end(),
                                                  d);
       if(itr == _buckets[n].end())
       {
           _buckets[n].push_back(d);
           return false;
       }
       else
       {
           (*itr) = d;
           return true;
       }
   }

   // return true if inserted successfully (i.e. d is not in the hash)
   // return false is d is already in the hash ==> will not insert
   bool insert(const Data& d) 
   { 
       size_t n = bucketNum(d);
       typename vector<Data>::const_iterator itr = find(_buckets[n].begin(),
                                                        _buckets[n].end(),
                                                        d);
       if(itr == _buckets[n].end())
       {
           _buckets[n].push_back(d);
           return true;
       }
       else
       {
           return false;
       }
   }
private:
   // Do not add any extra data member
   size_t            _numBuckets;
   vector<Data>*     _buckets;

   size_t bucketNum(const Data& d) const {
      return (d() % _numBuckets); }
};

#endif // MY_HASH_SET_H
