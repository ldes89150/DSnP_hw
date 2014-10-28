/****************************************************************************
  FileName     [ calcGNum.cpp ]
  PackageName  [ calc ]
  Synopsis     [ Define member functions for class GNum ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2007-2014 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/
#include <iostream>
#include <ctype.h>
#include <cassert>
#include "util.h"
#include "calcGNum.h"
// TODO: Initialize the static data members of class GNum
//       (Note: let default _base = 10)
// TODO: Define the member functions of class GNum

int GNum::_base = 10;
CalcMap GNum::_varMap;



ostream& operator << (ostream& os, const GNum& n)
{
    os<<"#"<<Int2Str(n._num,n._base);
    return os;
}



//operator overloading
GNum GNum::operator + (const GNum &n)const
{
    return GNum(this->_num+n._num);
}

GNum& GNum::operator += (const GNum& n)
{
    this->_num = (this->_num) + n._num;
    return (*this);
}

GNum GNum::operator - (const GNum &n) const
{
    return GNum(this->_num - n._num);
}

GNum& GNum::operator-= (const GNum& n)
{
    this->_num -= n._num;
    return (*this);
}


GNum GNum::operator * (const GNum& n) const
{
    return GNum(this->_num*n._num);
}

GNum& GNum::operator *= (const GNum& n)
{
    this->_num *= n._num;
    return (*this);
}

bool GNum::operator== (const GNum& n) const
{
    return this->_num == n._num;
}

bool GNum::operator!= (const GNum& n) const
{
    return this->_num != n._num;
}

GNum& GNum::operator= (const GNum& n)
{
    this->_num = n._num;
    return (*this);
}

//static methods
void GNum::setVarVal(const string &s, const GNum &n)
{
    _varMap[s] = n;
    return;
}

bool GNum::getVarVal(const string &s, GNum &n)
{
    bool retval = false;
    CalcMap::iterator itr = _varMap.find(s); 
    if(itr == _varMap.end())
    {
        retval = false;       
    }
    else
    {
        n = (*itr).second;
        retval = true;
    }
    return retval;
}

bool GNum::getStrVal(const string& s, GNum& n)
{
     if(isValidVarName(s))
     {
         return getVarVal(s,n);
     }
     else
     {
        int s_int;
        
        if(s[0]=='#' and myStr2Int(s.substr(1),s_int))
        {
            n._num = s_int;
            return true;
        }
        else
        {
            return false;
        }
     }
}        

void GNum::printVars()
{
    for(CalcMap::iterator itr = _varMap.begin();
        itr != _varMap.end();
        itr++)
    {
        cout<<(*itr).first<<" = "<<(*itr).second<<endl;
    }    
}

void GNum::resetVapMap()
{
    _varMap.clear();
}



