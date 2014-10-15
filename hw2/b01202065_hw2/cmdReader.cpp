/****************************************************************************
  FileName     [ cmdReader.cpp ]
  PackageName  [ cmd ]
  Synopsis     [ Define command line reader member functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2007-2014 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/
#include <cassert>
#include <cstring>
#include "cmdParser.h"

using namespace std;

//----------------------------------------------------------------------
//    Extrenal funcitons
//----------------------------------------------------------------------
void mybeep();
char mygetc(istream&);
ParseChar checkChar(char, istream&);


//----------------------------------------------------------------------
//    Member Function for class Parser
//----------------------------------------------------------------------


string strToAdd(char* buffbegin, char* buffend, bool &strnull, bool complete = false);



char prompt[] = "cmd> ";
void
CmdParser::readCmd()
{
   if (_dofile.is_open()) {
      readCmdInt(_dofile);
      _dofile.close();
   }
   else
      readCmdInt(cin);
}

void
CmdParser::readCmdInt(istream& istr)
{
   resetBufAndPrintPrompt();

   while (1) {
      char ch = mygetc(istr);
      ParseChar pch = checkChar(ch, istr);
      if (pch == INPUT_END_KEY) break;
      switch (pch) {
         case LINE_BEGIN_KEY :
         case HOME_KEY       : moveBufPtr(_readBuf); break;
         case LINE_END_KEY   :
         case END_KEY        : moveBufPtr(_readBufEnd); break;
         case BACK_SPACE_KEY : if(_readBufPtr == _readBuf){break;}
                               moveBufPtr(_readBufPtr-1);
                               deleteChar(); break;
         case DELETE_KEY     : deleteChar(); break;
         case NEWLINE_KEY    : addHistory();
                               cout << char(NEWLINE_KEY);
                               resetBufAndPrintPrompt(); break;
         case ARROW_UP_KEY   : moveToHistory(_historyIdx - 1); break;
         case ARROW_DOWN_KEY : moveToHistory(_historyIdx + 1); break;
         case ARROW_RIGHT_KEY: moveBufPtr(_readBufPtr+1); break;
         case ARROW_LEFT_KEY : moveBufPtr(_readBufPtr-1); break;
         case PG_UP_KEY      : moveToHistory(_historyIdx - PG_OFFSET); break;
         case PG_DOWN_KEY    : moveToHistory(_historyIdx + PG_OFFSET); break;
         case TAB_KEY        : insertChar(' ',TAB_POSITION - (int)(_readBufEnd-_readBuf)%TAB_POSITION); break;
         case INSERT_KEY     :
               //debug code
        //cout<<_history.size()<<" "<<_historyIdx<<" "<<_readBufEnd-_readBuf;

         break;
         // not yet supported; fall through to UNDEFINE
         case UNDEFINED_KEY:   mybeep(); break;
         default:  // printable character
            insertChar(char(pch)); break;
      }
      #ifdef TA_KB_SETTING
      taTestOnly();
      #endif
   }
}




// This function moves _readBufPtr to the "ptr" pointer
// It is used by left/right arrowkeys, home/end, etc.
//
// Suggested steps:
// 1. Make sure ptr is within [_readBuf, _readBufEnd].
//    If not, make a beep sound and return false. (DON'T MOVE)
// 2. Move the cursor to the left or right, depending on ptr
// 3. Update _readBufPtr accordingly. The content of the _readBuf[] will
//    not be changed
//
// [Note] This function can also be called by other member functions below
//        to move the _readBufPtr to proper position.
void moveCursor(int n)
{
    //[Note] This function moves cursor. (doesn;t maintain _readBufPtr)
    if(n==0)
        return;
    if(n>0)
        cout<<"\033["<<n<<"C";
    else
        cout<<"\033["<<-n<<"D";
    return;
}
void coutSlicechar(char* beginptr, char* endptr, unsigned whitespaceintheend = 0)
{
    for(char* ptr = beginptr; ptr != endptr; ptr++)
        cout<<*ptr;
    if(whitespaceintheend != 0)
    {
        for(unsigned i = 0;i!= whitespaceintheend;i++)
            cout<<" ";
    }
    return;
}

string strToAdd(char* buffbegin, char* buffend, bool &strnull, bool complete)
{
    char* beginptr = buffbegin;
    char* endptr = buffend;
    strnull = true;
    for(char* ptr = buffbegin;ptr != buffend;ptr++)
    {
        if(((char)(*ptr)) != ' ')
        {
            if(strnull)
            {
                beginptr = ptr;
                strnull = false;
            }
            endptr = ptr+1;
        }
    }
    if(complete)
        return string(buffbegin,(size_t)(buffend-buffbegin));


    if(strnull)
    {
        return string();
    }
    else
    {
        return string(beginptr,(size_t)(endptr-beginptr));
    }
}


bool
CmdParser::moveBufPtr(char* const ptr)
{
   // TODO..
   //ref: http://www.tldp.org/HOWTO/Bash-Prompt-HOWTO/x361.html

   if(ptr < _readBuf ||  ptr >_readBufEnd)
   {
       mybeep();
       return false;
   }
    int ptrmove = ptr -_readBufPtr;
    moveCursor(ptrmove);
    _readBufPtr = ptr;
    return true;
}


// [Notes]
// 1. Delete the char at _readBufPtr
// 2. mybeep() and return false if at _readBufEnd
// 3. Move the remaining string left for one character
// 4. The cursor should stay at the same position
// 5. Remember to update _readBufEnd accordingly.
// 6. Don't leave the tailing character.
// 7. Call "moveBufPtr(...)" if needed.
//
// For example,
//
// cmd> This is the command
//              ^                (^ is the cursor position)
//
// After calling deleteChar()---
//
// cmd> This is he command
//              ^
//
bool
CmdParser::deleteChar()
{
   // TODO...
   if(_readBufPtr == _readBufEnd)
   {
       mybeep();
       return false;
   }
   for(char* ptr = _readBufPtr;ptr!=_readBufEnd;ptr++)
   {
       *ptr = *(ptr+1);
   }
   coutSlicechar(_readBufPtr,_readBufEnd,1);
   moveCursor(_readBufPtr-_readBufEnd);
   _readBufEnd--;
   return true;
}

// 1. Insert character 'ch' at _readBufPtr
// 2. Move the remaining string right for one character
// 3. The cursor should move right for one position afterwards
//
// For example,
//
// cmd> This is the command
//              ^                (^ is the cursor position)
//
// After calling insertChar('k') ---
//
// cmd> This is kthe command
//               ^
//
void
CmdParser::insertChar(char ch, int repeat)
{
    assert(repeat>=1);
    for(int i = 0;i<repeat;i++)
    {
        for(char* ptr =_readBufEnd+1;ptr!=_readBufPtr;ptr--)
        {
            *ptr = *(ptr-1);
        }
        *_readBufPtr = ch;
        _readBufEnd ++;
        coutSlicechar(_readBufPtr,_readBufEnd);
        moveCursor(_readBufPtr-_readBufEnd);
        _readBufPtr += 1;
        moveCursor(1);
    }
    return;
}

// 1. Delete the line that is currently shown on the screen
// 2. Reset _readBufPtr and _readBufEnd to _readBuf
// 3. Make sure *_readBufEnd = 0
//
// For example,
//
// cmd> This is the command
//              ^                (^ is the cursor position)
//
// After calling deleteLine() ---
//
// cmd>
//      ^
//
void
CmdParser::deleteLine()
{
    moveBufPtr(_readBuf);
    int strlength = (int)(_readBufEnd-_readBuf);
    for(int i = 0; i < strlength;i++)
        cout<<' ';
    moveCursor(-strlength);
    _readBufEnd = _readBufEnd;
    return;
   // TODO...
}


// This functions moves _historyIdx to index and display _history[index]
// on the screen.
//
// Need to consider:
// If moving up... (i.e. index < _historyIdx)
// 1. If already at top (i.e. _historyIdx == 0), beep and do nothing.
// 2. If at bottom, temporarily record _readBuf to history.
//    (Do not remove spaces, and set _tempCmdStored to "true")
// 3. If index < 0, let index = 0.
//
// If moving down... (i.e. index > _historyIdx)
// 1. If already at bottom, beep and do nothing
// 2. If index >= _history.size(), let index = _history.size() - 1.
//
// Assign _historyIdx to index at the end.
//
// [Note] index should not = _historyIdx
//
void
CmdParser::moveToHistory(int index)
{
   // TODO...
   assert(index!=_historyIdx);
   if(_historyIdx == (int)_history.size())
   {
       assert(_tempCmdStored == false);
       bool strnull;
       string str2add = strToAdd(_readBuf,_readBufEnd, strnull,true);
       _history.push_back(str2add);
       _tempCmdStored = true;
   }


    int _historyEnd = (int) _history.size()-1;

    if(index < 0)
    {
        if(_historyIdx==0)
        {
            mybeep();
            return;
        }
        else
        {
            index = 0;
        }
    }
    if(index > _historyEnd)
    {
        if(_historyIdx==_historyEnd)
        {
            mybeep();
            return;
        }
        else
        {
            index = _historyEnd;
        }
    }


    _historyIdx = index;
    retrieveHistory();
    if(_historyIdx == _historyEnd and _tempCmdStored == true)
    {
        _history.pop_back();
        _tempCmdStored = false;
    }
    return;
}


// This function adds the string in _readBuf to the _history.
// The size of _history may or may not change. Depending on whether
// there is a temp history string.
//
// 1. Remove ' ' at the beginning and end of _readBuf
// 2. If not a null string, add string to _history.
//    Be sure you are adding to the right entry of _history.
// 3. If it is a null string, don't add anything to _history.
// 4. Make sure to clean up "temp recorded string" (added earlier by up/pgUp,
//    and reset _tempCmdStored to false
// 5. Reset _historyIdx to _history.size() // for future insertion
//

void
CmdParser::addHistory()
{
    if(_tempCmdStored)
    {
        _history.pop_back();
        _tempCmdStored = false;
    }
    bool strnull;
    string str2add = strToAdd(_readBuf,_readBufEnd,strnull);
    if(!strnull)
        _history.push_back(str2add);
    _historyIdx = (int)_history.size();
    return;
}




// 1. Replace current line with _history[_historyIdx] on the screen
// 2. Set _readBufPtr and _readBufEnd to end of line
//
// [Note] Do not change _history.size().
//
void
CmdParser::retrieveHistory()
{
   deleteLine();
   strcpy(_readBuf, _history[_historyIdx].c_str());
   cout << _readBuf;
   _readBufPtr = _readBufEnd = _readBuf + _history[_historyIdx].size();
}
