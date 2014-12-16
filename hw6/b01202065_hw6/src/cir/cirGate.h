/****************************************************************************
  FileName     [ cirGate.h ]
  PackageName  [ cir ]
  Synopsis     [ Define basic gate data structures ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-2014 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef CIR_GATE_H
#define CIR_GATE_H

#include <string>
#include <vector>
#include <iostream>
#include "cirDef.h"
#include <utility>
#include <set>

using namespace std;

class CirGate;
class GateV;
//------------------------------------------------------------------------
//   Define classes
//------
//------------------------------------------------------------------
// TODO: Define your own data members and member functions, or classes
class CirGate
{
    
    friend class CirMgr;
    public:
        typedef pair<unsigned,bool> net;
        net make_net(unsigned &pin)
        {
            return net(pin/2,(pin%2) == 1);
        }
        static unsigned get_pin(net l)
        {
            if(l.second)
                return l.first*2+1;
            else
                return l.first*2;
        }



        CirGate(enum GateType gateType,unsigned int id,unsigned lineNo):gateType(gateType),id(id), lineNo(lineNo), reachability(false) {}
        virtual ~CirGate() {}
        
        // Basic access methods
        string getTypeStr() const { return ""; }
        unsigned int getLineNo() const { return lineNo; }
        unsigned int getID() const {return id;}

        // Printing functions
        //virtual void printGate() const = 0;
        void reportGate() const;
        void reportFanin(int level) const;
        void reportFanout(int level) const;
        /*
        void addFanIn(unsigned int pin)
        {

            fanIn.push_back(pin);
        }
        */
        void addFanOut(unsigned ID,bool inverse)
        {
            fanOut.push_back(net(ID,inverse));
        }
        static string gateTypeStr(enum GateType gt)
        {
        switch(gt)
        {
            case PI_GATE:
                return "PI";
            case PO_GATE:
                return "PO";
            case AIG_GATE:
                return "AIG";
            case CONST_GATE:
                return "CONST";
            case UNDEF_GATE:
                return "UNDEF";
            default:
                return "";
        }
        }
    private:
        void printUndef(unsigned inden, bool inverse, unsigned undefID) const;
        void printFanIn(unsigned inden, int level, bool inverse, set<unsigned>* &reported) const;
        void printFanOut(unsigned inden, int level, bool inverse, set<unsigned>* &reported) const;


    protected:
        enum GateType gateType;
        vector<net> fanIn;
        vector<net> fanOut;
        unsigned int id;
        unsigned int lineNo;
        bool reachability;
};

class CirConstGate: public CirGate
{
    public:
        CirConstGate():CirGate(CONST_GATE,0,0){}
        ~CirConstGate(){}
};



class CirInputGate: public CirGate
{
    public:
        CirInputGate(enum GateType gateType,unsigned int id, unsigned lineNo):CirGate(gateType, id, lineNo){}
        ~CirInputGate(){return;}
};

class CirOutputGate: public CirGate
{
    public:
        CirOutputGate(enum GateType gateType, unsigned id, unsigned lineNo,unsigned pin):CirGate(gateType,id,lineNo)
    {
        fanIn.push_back(make_net(pin));
    }
        ~CirOutputGate(){return;}
};

class CirAndGate: public CirGate
{
    public:
        CirAndGate(enum GateType gateType,unsigned int id, unsigned lineNo, unsigned fanIn1, unsigned fanIn2):CirGate(gateType, id, lineNo)
        {
            fanIn.push_back(make_net(fanIn1));
            fanIn.push_back(make_net(fanIn2));
        }
        ~CirAndGate(){};
};
#endif // CIR_GATE_H
