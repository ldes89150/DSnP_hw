/****************************************************************************
  FileName     [ memCmd.cpp ]
  PackageName  [ mem ]
  Synopsis     [ Define memory test commands ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2007-2014 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/
#include <iostream>
#include <iomanip>
#include "memCmd.h"
#include "memTest.h"
#include "cmdParser.h"
#include "util.h"
#include <algorithm>
#include <vector>
#include <string>
#include <sstream>
using namespace std;

extern MemTest mtest;  // defined in memTest.cpp

bool
initMemCmd()
{
   if (!(cmdMgr->regCmd("MTReset", 3, new MTResetCmd) &&
         cmdMgr->regCmd("MTNew", 3, new MTNewCmd) &&
         cmdMgr->regCmd("MTDelete", 3, new MTDeleteCmd) &&
         cmdMgr->regCmd("MTPrint", 3, new MTPrintCmd)
      )) {
      cerr << "Registering \"mem\" commands fails... exiting" << endl;
      return false;
   }
   return true;
}

string size_t2str(size_t &i)
{
    string s;
    stringstream ss(s);
    ss << i;
    return ss.str();
}
//----------------------------------------------------------------------
//    MTReset [(size_t blockSize)]
//----------------------------------------------------------------------
CmdExecStatus
MTResetCmd::exec(const string& option)
{
   // check option
   string token;
   if (!CmdExec::lexSingleOption(option, token))
      return CMD_EXEC_ERROR;
   if (token.size()) {
      int b;
      if (!myStr2Int(token, b) || b < int(sizeof(MemTestObj))) {
         cerr << "Illegal block size (" << token << ")!!" << endl;
         return CmdExec::errorOption(CMD_OPT_ILLEGAL, token);
      }
      #ifdef MEM_MGR_H
      mtest.reset(toSizeT(b));
      #else
      mtest.reset();
      #endif // MEM_MGR_H
   }
   else
      mtest.reset();
   return CMD_EXEC_DONE;
}

void
MTResetCmd::usage(ostream& os) const
{  
   os << "Usage: MTReset [(size_t blockSize)]" << endl;
}

void
MTResetCmd::help() const
{  
   cout << setw(15) << left << "MTReset: " 
        << "(memory test) reset memory manager" << endl;
}

bool caseInsensitivIn(string strshort,string strlong)
{
    transform(strshort.begin(),strshort.end(),strshort.begin(),::toupper);
    transform(strlong.begin(),strlong.end(),strlong.begin(),::toupper);
    if(strlong.find(strshort)==0)
        return true;
    else
        return false;
}



int machFlag(string opt,vector<string> flags)
{
    if(!(opt[0]=='-' and opt.size()>0))
        return -1;
    string opt_flag(opt.substr(1));
    for(unsigned i =0;i<flags.size();i++)
    {
        if(caseInsensitivIn(opt_flag,flags[i]))
            return (int) i;
    }
    return -2;
}
//----------------------------------------------------------------------
//    MTNew <(size_t numObjects)> [-Array (size_t arraySize)]
//----------------------------------------------------------------------
CmdExecStatus
MTNewCmd::exec(const string& option)
{
    // TODO
    size_t numObjects =0;
    size_t arraySize =0;
    vector<string> tokens;
    if(!lexOptions(option,tokens))
        return CMD_EXEC_ERROR;
    
    if(tokens.size()==0)
        return CmdExec::errorOption(CMD_OPT_MISSING,"");
    if(tokens.size()>3)
        return CmdExec::errorOption(CMD_OPT_EXTRA,tokens[3]);
    bool optFlagFound = false;
    bool numObjFound  = false;
    int tmp;
    vector<string> flags;
    flags.push_back("Array");
    int status =0;
    for(unsigned i=0;i<tokens.size();i++)
    {
        status = machFlag(tokens[i],flags);
        if(status == -2)
            return CmdExec::errorOption(CMD_OPT_ILLEGAL,tokens[i]);
        if(status ==0 and !optFlagFound)
        {
            i++;
            if(i==tokens.size())
                return CmdExec::errorOption(CMD_OPT_MISSING,tokens[i-1]);
            if(!myStr2Int(tokens[i],tmp))
                return CmdExec::errorOption(CMD_OPT_ILLEGAL,tokens[i]);
            if(tmp<=0)
                return CmdExec::errorOption(CMD_OPT_ILLEGAL,tokens[i]);
            arraySize = (size_t) tmp;
            optFlagFound = true;
            continue;
        }
        if(status == -1 and !numObjFound)
        {

            if(!myStr2Int(tokens[i],tmp))
                return CmdExec::errorOption(CMD_OPT_ILLEGAL,tokens[i]);
            if(tmp<=0)
                return CmdExec::errorOption(CMD_OPT_ILLEGAL,tokens[i]);
            numObjects = (size_t) tmp;
            numObjFound = true;
            continue;
        }
        return CmdExec::errorOption(CMD_OPT_ILLEGAL,tokens[i]);

    }
    if(not numObjFound)
    {
        return CmdExec::errorOption(CMD_OPT_MISSING,"");
    }
    //cout<<arraySize<<" "<<numObjects;
    try
    {
        if(arraySize>=1)
        {
            mtest.newArrs(numObjects ,arraySize);
        }
        else
        {
            mtest.newObjs(numObjects);
        }
    }
    catch(std::bad_alloc e)
    {
        return CMD_EXEC_ERROR;
    }
    return CMD_EXEC_DONE;
}

void
MTNewCmd::usage(ostream& os) const
{  
   os << "Usage: MTNew <(size_t numObjects)> [-Array (size_t arraySize)]\n";
}

void
MTNewCmd::help() const
{  
   cout << setw(15) << left << "MTNew: " 
        << "(memory test) new objects" << endl;
}


//----------------------------------------------------------------------
//    MTDelete <-Index (size_t objId) | -Random (size_t numRandId)> [-Array]
//----------------------------------------------------------------------
CmdExecStatus
MTDeleteCmd::exec(const string& option)
{
    // TODO
    size_t objId =0;
    size_t numRandId =0;
    bool arrayFlagFound = false;
    bool mainFlagFound = false;
    bool objIdFound = false;
    bool numRandIdFound = false;
    vector<string> tokens;                                                        
    if(!lexOptions(option,tokens))
        return CMD_EXEC_ERROR;

    if(tokens.size()==0)
        return CmdExec::errorOption(CMD_OPT_MISSING,"");
    if(tokens.size()>3)
        return CmdExec::errorOption(CMD_OPT_EXTRA,tokens[3]);
    vector<string> flags;
    flags.push_back("Index");
    flags.push_back("Random");
    flags.push_back("Array");
    int tmp;
    int status =0;
    for(unsigned i =0;i<tokens.size();i++)
    {
        status = machFlag(tokens[i],flags);
        if(status ==-2 or status == -1)
            return CmdExec::errorOption(CMD_OPT_ILLEGAL,tokens[i]);
        if(!arrayFlagFound and status==2)
        {
            arrayFlagFound = true;
            continue;
        }
        if(!mainFlagFound)
        {
            i++;
            if(i==tokens.size())
                return CmdExec::errorOption(CMD_OPT_MISSING,tokens[i-1]);
            if(!myStr2Int(tokens[i],tmp))
                return CmdExec::errorOption(CMD_OPT_ILLEGAL,tokens[i]);
            if(tmp <0)
                return CmdExec::errorOption(CMD_OPT_ILLEGAL,tokens[i]);

            if(status ==0)
            {
                objId = (size_t) tmp;
                objIdFound=true;
            }
            if(status ==1)
            {
                numRandId =  (size_t) tmp;
                numRandIdFound=true;
            }
            mainFlagFound =true;
            continue;
        }
        return CmdExec::errorOption(CMD_OPT_ILLEGAL,tokens[i]);
    }
    if( not mainFlagFound)
    {
        return CmdExec::errorOption(CMD_OPT_MISSING,"");
    }
    if(objIdFound)
    {
        if(arrayFlagFound)
        {
            if(objId >= mtest.getArrListSize())
            {
                cerr << "Size of array list (" << mtest.getArrListSize() << ") is <= " << objId << "!!" << endl;
                return CmdExec::errorOption(CMD_OPT_ILLEGAL,size_t2str(objId));
            }
            mtest.deleteArr(objId);
        }
        else
        {
            if(objId>=mtest.getObjListSize())
            {
                cerr << "Size of object list (" << mtest.getObjListSize() << ") is <= " << objId << "!!" << endl;
                return CmdExec::errorOption(CMD_OPT_ILLEGAL, size_t2str(objId));
            }
            mtest.deleteObj(objId);
        }
    }
    if(numRandIdFound)
    {
        if(arrayFlagFound && mtest.getArrListSize() == 0)
        {
            cerr << "Size of array list is 0!!" << endl;
            return CmdExec::errorOption(CMD_OPT_ILLEGAL, size_t2str(numRandId));
        }
        if(!arrayFlagFound && mtest.getObjListSize() == 0) // isArray must be false here
        {
            cerr << "Size of object list is 0!!" << endl;
            return CmdExec::errorOption(CMD_OPT_ILLEGAL, size_t2str(numRandId));
        }
        for(size_t i=0;i<numRandId;i++)
        {
            if(arrayFlagFound)
            {
                mtest.deleteArr(rnGen(mtest.getArrListSize()));
            }
             else
             {
                mtest.deleteObj(rnGen(mtest.getObjListSize()));

             }
        }
    }
    return CMD_EXEC_DONE;
}

void
MTDeleteCmd::usage(ostream& os) const
{  
   os << "Usage: MTDelete <-Index (size_t objId) | "
      << "-Random (size_t numRandId)> [-Array]" << endl;
}

void
MTDeleteCmd::help() const
{  
   cout << setw(15) << left << "MTDelete: " 
        << "(memory test) delete objects" << endl;
}


//----------------------------------------------------------------------
//    MTPrint
//----------------------------------------------------------------------
CmdExecStatus
MTPrintCmd::exec(const string& option)
{
   // check option
   if (option.size())
      return CmdExec::errorOption(CMD_OPT_EXTRA, option);
   mtest.print();

   return CMD_EXEC_DONE;
}

void
MTPrintCmd::usage(ostream& os) const
{  
   os << "Usage: MTPrint" << endl;
}

void
MTPrintCmd::help() const
{  
   cout << setw(15) << left << "MTPrint: " 
        << "(memory test) print memory manager info" << endl;
}


