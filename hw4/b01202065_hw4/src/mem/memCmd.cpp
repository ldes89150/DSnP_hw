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
    cout<<"short: "<<strshort<<" long:"<<strlong<<endl;
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
        if(caseInsensitivIn(opt,flags[i]))
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
    bool optFlagFound = false;
    bool numObjFound  = false;
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
            if(!i<tokens.size())
                return CmdExec::errorOption(CMD_OPT_MISSING,tokens[i]);
            int i;
            if(!myStr2Int(tokens[i],i))
                return CmdExec::errorOption(CMD_OPT_ILLEGAL,tokens[i]);
            if(!i>0)
                return CmdExec::errorOption(CMD_OPT_ILLEGAL,tokens[i]);
            arraySize = (size_t) i;
            optFlagFound = true;
            continue;
        }
        if(status == -1 and !numObjFound)
        {

            if(!i<tokens.size())
                return CmdExec::errorOption(CMD_OPT_MISSING,tokens[i]);
            int i;
            if(!myStr2Int(tokens[i],i))
                return CmdExec::errorOption(CMD_OPT_ILLEGAL,tokens[i]);
            if(!i>0)
                return CmdExec::errorOption(CMD_OPT_ILLEGAL,tokens[i]);
            numObjects = (size_t) i;
            numObjFound = true;
            continue;
        }
        return CmdExec::errorOption(CMD_OPT_ILLEGAL,tokens[i]);

    }
    cout<<arraySize<<" "<<numObjects;

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


