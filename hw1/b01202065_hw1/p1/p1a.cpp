#include "p1a.h"
#include <iostream>
#include <string>


using namespace std;

void P1a::assign(const string& s)
{
    this->_str = s;
}

void P1a::print()  const
{
    cout<<this->_str<<endl;
}

string P1a::getstr() const
{
    return (string) this->_str;
}

P1a& P1a::append(const P1a& p)
{
    //bugs here...
   string s;
   s = p.getstr();
    this->_str += s;
}
