/****************************************************************************
  FileName     [ bst.h ]
  PackageName  [ util ]
  Synopsis     [ Define binary search tree package ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2005-2014 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#define CHECK_NODE_HEALTH 0


#ifndef BST_H
#define BST_H

#include <cassert>
#include <string>
#include<iostream>
using namespace std;

template <class T> class BSTree;

// BSTreeNode is supposed to be a private class. User don't need to see it.
// Only BSTree and BSTree::iterator can access it.
//
// DO NOT add any public data member or function to this class!!
//
template <class T>
class BSTreeNode
{
    // TODO: design your own class!!
    friend class BSTree<T>;
    friend class BSTree<T>::iterator;
    
    BSTreeNode(const T& data, BSTreeNode* parent, BSTreeNode* left = 0, BSTreeNode* right = 0):
        _data(data), _parent(parent), _left(left), _right(right)
    {
    }
    ~BSTreeNode(){}

    T _data;
    BSTreeNode<T> *_parent;
    BSTreeNode<T> *_left;
    BSTreeNode<T> *_right;
    bool hasRight(){return _right != 0;}
    bool hasLeft(){return _left != 0;}
    bool hasChild(){return hasLeft() or hasRight();}
    bool isRoot(){return _parent == 0;}
    bool isRightChild(){return this == (_parent->_right);}
    bool isLeftChild(){return this == (_parent->_left);}
    void setRight(BSTreeNode<T>* child)
    {
        _right = child;
        child->_parent = this;
    }
    void setLeft(BSTreeNode<T>* child)
    {
        _left = child;
        child->_parent = this;
    }
    void insertRight(const T& i)
    {
        _right = new BSTreeNode<T>(i, this);
    }
    void insertLeft(const T& i)
    {
        _left = new BSTreeNode<T>(i, this);
    }
    size_t childNum()
    {
        if(hasLeft() and hasRight())
            return 2;
        if(hasLeft() or hasRight())
            return 1;
        return 0;
    }
    bool isHealthy()
    {
        if(not isRoot())
        {
            if(not(isLeftChild() or isRightChild()))
                return false;
        }
        if(hasLeft())
        {
            if(_left->_parent != this)
                return false;
        }
        if(hasRight())
        {
            if(_right->_parent != this)
                return false;
        }
        return true;
    }
};


template <class T>
class BSTree
{
    // TODO: design your own class!!
    private:
        size_t _size;
        BSTreeNode<T>* _root;
        BSTreeNode<T>* _end;

    public:
        BSTree():_size(0)
        {
            _root = _end =new BSTreeNode<T>(T(),0);
        }
        ~BSTree(){clear(); delete _end;}

        class iterator 
        {
            private:
                BSTreeNode<T>* _node;
            friend class BSTree;
            public:
                iterator(BSTreeNode<T>* n =0):_node(n){}
                iterator(const iterator &i):_node(i._node){}
                ~iterator(){}

                const T& operator*()const{return this->_node->_data;}
                T& operator*() {return this->_node->_data;}
                
                iterator& operator ++()
                {
                    _node = BSTree::inOrderNext(_node);
                    #if CHECK_NODE_HEALTH
                    assert(_node->isHealthy());
                    #endif
                    return *(this);
                }
                iterator operator ++(int)
                {
                    iterator tmp = *(this);
                    _node = BSTree::inOrderNext(_node);
                    #if CHECK_NODE_HEALTH
                    assert(_node->isHealthy());
                    #endif
                    return tmp;
                }
                
                iterator& operator --()
                {
                    _node = BSTree::inOrderPrev(_node);
                    return *(this);
                }
                iterator operator --(int)
                {
                    iterator tmp = *(this);
                    _node = BSTree::inOrderPrev(_node);
                    return tmp;
                }
                iterator& operator = (const iterator& i)
                {
                    _node = i._node;
                    return *(this);
                }
                
                bool operator != (const iterator& i) const 
                { 
                    return _node != i._node;
                }
                bool operator == (const iterator& i) const 
                { 
                    return _node == i._node;
                } 
    
        };
        iterator begin() const
        {
            return iterator(min(_root));
        }
        iterator end() const
        {   
            return iterator(max(_root));
        }
    
        size_t size(){return _size;}
        bool empty(){return _size == 0;}
        bool erase(iterator itr)
        {
            if(empty())
            {
                return false;
            }
            else
            {
                eraseNode(itr._node);//the _size has been
                return true;
            }
        }
        bool erase(const T& i)
        {
            if(empty())
                return false;
            if(i<min(_root)->_data or max(_root)->_parent->_data<i)
                return false;
            BSTreeNode<T>* ptr = _root;
            while(true)
            {
                if(ptr->_data == i)
                {
                    eraseNode(ptr);
                    return true;
                }
                if(ptr->hasLeft() and i<ptr->_data)
                {
                    ptr = ptr->_left;
                    continue;
                }
                if(ptr->hasRight() and ptr->_data<i)
                {
                    ptr = ptr->_right;
                    continue;
                }
                return false;
            }
        }

        void clear()
        {
            clearNode(_root);
            _size = 0;
            _root = _end = new BSTreeNode<T>(T(),0); 
        }
        void print()
        {
            printNode(_root,0);
        }
        bool insert(const T& i)
        {
            if(empty())
            {
                _root = new BSTreeNode<T>(i,0);
                _size++;
                _root->setRight(_end);
                return true;
            }
            BSTreeNode<T>* ptr =_root;
            while(true)
            {
                if(i == ptr->_data)
                {
                    BSTreeNode<T>* nodeNew = new BSTreeNode<T>(i,0);
                    _size++;
                    if(ptr->hasRight())
                        nodeNew->setRight(ptr->_right);
                    ptr->setRight(nodeNew);
                    return true;
                }
                if(i < (ptr->_data))
                {
                    if(not ptr->hasLeft())
                    {
                        ptr->insertLeft(i);
                        _size++;
                        return true;
                    }
                    ptr = ptr->_left;
                    continue;
                }
                else //_data < i
                {
                    if(not ptr->hasRight())
                    {
                        ptr->insertRight(i);
                        _size++;
                        return true;
                    }
                    if(ptr->_right == _end)
                    {
                        ptr->insertRight(i);
                        _size++;
                        ptr->_right->setRight(_end);
                        return true;
                    }
                    ptr = ptr->_right;
                    continue;
                }
            }
            #if CHECK_NODE_HEALTH
            checkHealth(_root);
            #endif
        }
        void sort(){}//dummy function
        void pop_back()
        {
            if(empty())
                return;
            #if CHECK_NODE_HEALTH
            cout<<endl<<"pop_back:"<<_end->_parent->_data<<endl;
            #endif
            eraseNode(_end->_parent);
        }
        void pop_front()
        {
            if(empty())
                return;

            eraseNode(min(_root));
        } 
    private:
        void printSpace(size_t n)
        {
            for(size_t i=0;i!=n;i++)
            {
                cout<<" ";
            }
        }
        void printNode(BSTreeNode<T>* node, size_t preSpace)
        {
            printSpace(preSpace);
            #if CHECK_NODE_HEALTH
            if(node ==_end)
            {
                cout<<"[END]"<<endl;
                return;
            }
            #endif
            if(node == 0 or node == _end)
            {
                cout<<"[0]"<<endl;
                return;
            }
            cout<<(node->_data)<<endl;
            printNode(node->_left,preSpace+2);
            printNode(node->_right,preSpace+2);

        }
        void checkHealth(BSTreeNode<T>* root )
        {
            assert(root->isHealthy());
            if(root->hasRight())
                checkHealth(root->_right);
            if(root->hasLeft())
                checkHealth(root->_left);
            
        }

        void clearNode(BSTreeNode<T>* node)
        {
            if(node == 0)
                return;
            clearNode(node->_left);
            clearNode(node->_right);
            delete node;
        }
        void eraseNode(BSTreeNode<T>* node)
        {
            #if CHECK_NODE_HEALTH
            assert(not empty());
            #endif
            switch(node->childNum())
            {
                case 0:
                    #if CHECK_NODE_HEALTH
                    assert(not node->isRoot());
                    #endif
                    if(node->isLeftChild())
                        node->_parent->_left = 0;
                    else
                        node->_parent->_right = 0;
                    break;
                case 1:
                    if(node->hasLeft())
                    {
                        replaceRelationWithParent(node,node->_left);
                    }
                    else
                    {
                        replaceRelationWithParent(node,node->_right);
                    }
                    break; 
                case 2:
                    if(node->_right->_data == node->_data and node->_right != _end)
                    {
                        node->_right->setLeft(node->_left);
                        replaceRelationWithParent(node,node->_right);
                    }
                    else
                    {
                        BSTreeNode<T>* prev = max(node->_left);
                        if(prev->isLeftChild())//in this case prev->_parent == node
                        {
                            prev->setRight(node->_right);
                        }
                        else
                        {
                            prev->_parent->_right = 0;
                            assert(prev->_right == 0);
                            prev->setRight(node->_right);
                            min(prev)->setLeft(node->_left);
                        }
                        replaceRelationWithParent(node,prev);
                    }
                    break;
            }

            delete node;
            _size--;
            #if CHECK_NODE_HEALTH
            checkHealth(_root);
            #endif
            return;
        } 
        void replaceRelationWithParent(BSTreeNode<T>* &origin, BSTreeNode<T>* &nodeNew)
        {
            if(origin->isRoot())
            {
                _root = nodeNew;
                nodeNew->_parent = 0;
                return;
            }
            if(origin->isLeftChild())
            {
                origin->_parent->setLeft(nodeNew);
                return;
            }
            else
            {
                origin->_parent->setRight(nodeNew);
                return;
            }
        }
        static BSTreeNode<T>* inOrderNext(BSTreeNode<T>* const& node)
        {
            if(node->hasRight())
            {
                return min(node->_right);
            }
            else
            {
                BSTreeNode<T>* ptr = 0;
                if(node->isRoot())
                {
                    return 0;
                }
                for(ptr = node;ptr->isRightChild();ptr=ptr->_parent)
                {
                    if(ptr->_parent->isRoot())
                        return 0;
                }
                return ptr->_parent;
            }
        }

        static BSTreeNode<T>* inOrderPrev(BSTreeNode<T>* const& node) 
        {
            if(node->hasLeft())
            {
                return max(node->_left);
            }
            else
            {
                BSTreeNode<T>* ptr = 0;
                if(node->isRoot())
                {
                    return 0;
                } 
                for(ptr = node;ptr->isLeftChild();ptr=ptr->_parent)
                {
                    if(ptr->_parent->isRoot())
                        return 0;
                }
                return ptr->_parent;
            }
        }

        static BSTreeNode<T>* min(BSTreeNode<T>* const& node)
        {
            BSTreeNode<T>* ptr = node;
            while(ptr->hasLeft())
            {
                ptr = ptr->_left;
            }    
            return ptr;
        }
        static BSTreeNode<T>* max(BSTreeNode<T>* const& node)
        {
            BSTreeNode<T>* ptr = node;
            while(ptr->hasRight())
            {
                ptr = ptr->_right;
            }    
            return ptr;
        }
};
#endif // BST_H
