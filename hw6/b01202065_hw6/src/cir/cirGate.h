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
        CirGate(enum GateType gateType,unsigned int id,unsigned lineNo):gateType(gateType),id(id), lineNo(lineNo) {}
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
        void addFanIn(unsigned int pin)
        {
            fanIn.push_back(pin);
        }
        void addFanOut(unsigned int pin)
        {
            fanOut.push_back(pin);
        }

    private:

    protected:
        enum GateType gateType;
        vector<unsigned int> fanIn;
        vector<unsigned int> fanOut;
        unsigned int id;
        unsigned int lineNo;
};

class CirInputGate: public CirGate
{
    public:
        CirInputGate(enum GateType gateType,unsigned int id, unsigned lineNo):CirGate(gateType, id, lineNo){};
        ~CirInputGate(){};
};

class CirAndGate: public CirGate
{
    public:
        CirAndGate(enum GateType gateType,unsigned int id, unsigned lineNo, unsigned fanIn1, unsigned fanIn2):CirGate(gateType, id, lineNo)
        {
            fanIn.push_back(fanIn1);
            fanIn.push_back(fanIn2);
        }
        ~CirAndGate(){};
        
};
#endif // CIR_GATE_H
