/****************************************************************************
  FileName     [ cirGate.cpp ]
  PackageName  [ cir ]
  Synopsis     [ Define class CirAigGate member functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-2014 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdarg.h>
#include <cassert>
#include "cirGate.h"
#include "cirMgr.h"
#include "util.h"
#include <set>
using namespace std;

extern CirMgr *cirMgr;

// TODO: Implement memeber functions for class(es) in cirGate.h

/**************************************/
/*   class CirGate member functions   */
/**************************************/

void
CirGate::reportGate() const
{
    stringstream ss;
    string gname = cirMgr->getGateName(id);
    ss<<"= "<<CirGate::gateTypeStr(gateType)<<'('<<id<<')';
    if(not gname.empty())
    {
        ss<<"\""<<gname<<"\"";
    }
    ss<<", line "<<lineNo;
    ss<<string(49-ss.tellp(),' ')<<'=';
    cout<<"=================================================="<<endl
        <<ss.str()<<endl
        <<"=================================================="<<endl;
}

void
CirGate::reportFanin(int level) const
{
   assert (level >= 0);
   set<unsigned>* reported = new set<unsigned>();
   this->printFanIn(0,level,false,reported);
   delete reported;
}

void
CirGate::reportFanout(int level) const
{
   assert (level >= 0);
   set<unsigned>* reported = new set<unsigned>();
   this->printFanOut(0,level,false,reported);
   delete reported;
}

void CirGate::printFanIn(unsigned inden, int level, bool inverse, set<unsigned>* &reported) const
{
    cout<<string(inden, ' ');
    if(inverse)
    {
        cout<<'!';
    }
   
    cout<<gateTypeStr(gateType)<<' '<<id;
    if(reported->count(id)==1 and !fanIn.empty())
    {
        cout<<" (*)"<<endl;
        return;
    }
    else
    {
        cout<<endl;
        if(fanIn.empty() or level==0)
        {
            return;
        }
        reported->insert(id);
        for(vector<CirGate::net>::const_iterator itr = fanIn.begin();
            itr != fanIn.end();itr++)
        {
            CirGate* gate = cirMgr->getGate(itr->first);
            if(gate == 0)
            {
                printUndef(inden+2,itr->second,itr->first);
            }
            else
            {
                gate->printFanIn(inden+2,level-1,(itr->second),reported);
            }
        }

    }

}


void CirGate::printFanOut(unsigned inden, int level, bool inverse, set<unsigned>* &reported) const
{
    cout<<string(inden, ' ');
    if(inverse)
    {
        cout<<'!';
    }
   
    cout<<gateTypeStr(gateType)<<' '<<id;
    if(reported->count(id)==1 and !fanOut.empty())
    {
        cout<<" (*)"<<endl;
        return;
    }
    else
    {
        cout<<endl;
        if(fanOut.empty() or level==0)
        {
            return;
        }
        reported->insert(id);
        for(vector<CirGate::net>::const_iterator itr = fanOut.begin();
            itr != fanOut.end();itr++)
        {
            CirGate* gate = cirMgr->getGate(itr->first);
            if(gate == 0)
            {
                printUndef(inden+2,itr->second,itr->first);
            }
            else
            {
                gate->printFanOut(inden+2,level-1,(itr->second),reported);
            }
        }

    }

}
void CirGate::printUndef(unsigned inden, bool inverse, unsigned undefID) const
{
    cout<<string(inden, ' ');
    if(inverse)
    {
        cout<<'!';
    }
    cout<<gateTypeStr(UNDEF_GATE)<<' '<<undefID<<endl;
}
