/***********************************************************************
*                                                                      *
* (C) 2007, Frank Lemke, Computer Architecture Group,                  *
* University of Mannheim, Germany                                      *
*                                                                      *
* This program is free software; you can redistribute it and/or modify *
* it under the terms of the GNU General Public License as published by *
* the Free Software Foundation; either version 2 of the License, or    *
* (at your option) any later version.                                  *
*                                                                      *
* This program is distributed in the hope that it will be useful,      *
* but WITHOUT ANY WARRANTY; without even the implied warranty of       *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        *
* GNU General Public License for more details.                         *
*                                                                      *
* You should have received a copy of the GNU General Public License    *
* along with this program; if not, write to the Free Software          *
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 *
* USA                                                                  *
*                                                                      *
* For informations regarding this file contact                         *
*			      office@mufasa.informatik.uni-mannheim.de *
*                                                                      *
***********************************************************************/

#ifndef LOGICMIN_H
#define LOGICMIN_H

#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <assert.h>
#include <string.h>
#include <string>
#include <cstring>
#include <set>

#define alive {cerr << "Still Alive: " << __FILE__ << " " << __LINE__ << endl;}

#define DISCARD minTerm(string("DISCARD"))
#define OR_SYMBOL '+'
#define DC_SYMBOL '-'
#define TRUE_SYMBOL '1'
#define FALSE_SYMBOL '0'
#define	NUMVAL(param)   ((param) == TRUE_SYMBOL?1:0)
#define	DCVAL(param)	((param) == DC_SYMBOL?0:1)
#define	CHARVAL(index)	((index) == 0?FALSE_SYMBOL:TRUE_SYMBOL)

using namespace std;

class minTerm
{
public:
  typedef string::const_iterator const_termIter;
  const_termIter termBegin() const {return _term.begin();}
  const_termIter termEnd() const {return _term.end();}
  const unsigned int termSize() const {return _term.size();}
  //constructor is used to mark discard terms;
  minTerm():
	_termNum(0),
	_termNumAssigned(false),
	_termLen(0),
	_term(0,DC_SYMBOL)
	{}
  minTerm(const string& discard):
	_termNum(0),
	_termNumAssigned(false),
	_termLen(0),
	_term(discard)
	{}
  minTerm(unsigned int numOfInputS=0):
	_termNum(0),
	_termNumAssigned(false),
	_termLen(0),
	_term(numOfInputS,DC_SYMBOL)
    {}
  ~minTerm(){}
  //Modification;
  void setTermLen(unsigned int& termLen){_termLen=termLen;}
  void setTermNumAssigned(){_termNumAssigned=true;}
  void incNumOfOnes(){++_termNum;}
  void incTermLen(){++_termLen;}
  void set(string::size_type p, char c){_term[p]=c;}
  void setTerm(const string& term){_term=term;}
  void setTermNum(unsigned int termNum){_termNum=termNum;}
  //Accessors;
  const bool termNumAssigned() const {return _termNumAssigned;}
  char get(string::size_type p) const {return _term[p];}
  const string& term() const {return _term;}
  unsigned int termNum() const {return _termNum;}
  unsigned int termLen() const {return _termLen;}//describes the number of ones or zeros in term;

  void print(ostream& os) const {
    os<<"-----------"<<endl\
      <<"_termNum : "<<_termNum<<endl\
      <<"_termLen : "<<_termLen<<endl\
      <<"_term : "<<_term<<endl;
  }
  void term2stream(ostream& os) const {
    os<<"term: "<<_term<<endl;
  }

  //Operators;
  //---------------------------------------------------------;
  bool operator==(const minTerm& min) const {
	if(this!=&min)
	  //_termNum is only an administrative help;
	  //and not involved in compares;
	  return (_termLen==min.termLen()&&
			  _term==min.term());
	  return true;
  }
  //---------------------------------------------------------;
  
private:
  unsigned int _termNum;
  bool _termNumAssigned;
  unsigned int _termLen;
  string _term;
};

ostream& operator<<(ostream& os, const minTerm& min);

class minTermInfo{
public:
  minTermInfo():
	_equalTermS(),
	_termLen(),
	_termIndex()
	{}
  minTermInfo(unsigned int equalTermS,
	      unsigned int termLen,
	      unsigned int termIndex):
    _equalTermS(equalTermS),
    _termLen(termLen),
    _termIndex(termIndex)
    {}
  ~minTermInfo(){}
  //Accessors;
  unsigned int equalTermS() const {return _equalTermS;}
  unsigned int termLen() const {return _termLen;}
  int termIndex() const {return _termIndex;}
  //Modification; 
  void decrEqualTermS(){--_equalTermS;}
  void setEqualTermS(unsigned int equalTermS){_equalTermS=equalTermS;}
  void setTermLen(unsigned int termLen){_termLen=termLen;}
  void setTermIndex(unsigned int termIndex){_termIndex=termIndex;}
  minTermInfo& operator=(const minTermInfo& mTI){
	if(this!=&mTI){
	  _equalTermS=mTI._equalTermS;
	  _termLen=mTI._termLen;
	  _termIndex=mTI._termIndex;
	}
	return *this;
  }
  bool operator==(const minTermInfo& mTI) const {
	if(this!=&mTI)
	  return ((_equalTermS==mTI.equalTermS())&&
			  (_termLen==mTI.termLen())&&
			  (_termIndex==mTI.termIndex()));
	return false;
  }
private:
  unsigned int _equalTermS;
  unsigned int _termLen;
  int _termIndex;
};

class complementInfo
{
public:
  complementInfo():
	_val(),
	_dc(){}
  complementInfo(unsigned int val,
				 unsigned int dc):
	_val(val),
	_dc(dc){}
  ~complementInfo(){}
  //Accessors;
  const unsigned int& val() const {return _val;}
  const unsigned int& dc() const {return _dc;}
  //Modification;
  void setVal(unsigned int val){_val=val;}
  void setDc(unsigned int dc){_dc=dc;}
  //Operators;
  bool operator==(complementInfo cI) const {
	return((_val==cI.val())&&(_dc==cI.dc()));
  }
private:
  unsigned int _val;
  unsigned int _dc;
};

class logicMin
{
public:
  typedef vector<list<minTerm> >::const_iterator const_allTermSIter;
  const_allTermSIter allTermSBegin() const {return _allTermS.begin();}
  const_allTermSIter allTermSEnd() const {return _allTermS.end();}
  const unsigned int allTermSSize() const {return _allTermS.size();}

  typedef list<minTerm>::const_iterator const_minTermListIter;
  const_minTermListIter minTermListBegin(const_allTermSIter I) const {return I->begin();}
  const_minTermListIter minTermListEnd(const_allTermSIter I) const {return I->end();}
  const unsigned int minTermListSize(const_allTermSIter I) const {return I->size();}
  const_minTermListIter minTermListBegin(unsigned int i) const {return _allTermS[i].begin();}
  const_minTermListIter minTermListEnd(unsigned int i) const {return _allTermS[i].end();}
  const unsigned int minTermListSize(unsigned int i) const {return _allTermS[i].size();}

  typedef vector<string>::const_iterator const_allInputTermSIter;
  const_allInputTermSIter allInputTermSBegin() const {return _allInputTermS.begin();}
  const_allInputTermSIter allInputTermSEnd() const {return _allInputTermS.end();}
  const unsigned int allInputTermSSize() const {return _allInputTermS.size();}

  typedef vector<minTermInfo>::const_iterator const_allMinTermInfoIter;
  const_allMinTermInfoIter allMinTermInfoBegin() const {return _allMinTermInfo.begin();}
  const_allMinTermInfoIter allMinTermInfoEnd() const {return _allMinTermInfo.end();}
  const unsigned int allMinTermInfoSize() const {return _allMinTermInfo.size();}
 
  //----------------------Constructors--------------------------------------;
  logicMin(set<string,less<string> >& setOfAllInputTermS,
		   set<string,less<string> >& normalResult,
		   set<string,less<string> >& invertedResult):
	_numOfMinTermS(setOfAllInputTermS.size()),
	_outpValueS(setOfAllInputTermS.size(),TRUE_SYMBOL){

	_allInputTermS.resize(_numOfMinTermS);
	_allTermS.resize(_numOfMinTermS);
	_allMinTermInfo.resize(_numOfMinTermS);
	_complementInfo.resize(_numOfMinTermS);
	
	_inpVarS=(_allInputTermS.begin())->size();

	copySet2Vector(setOfAllInputTermS);

	//----------------------------;
	pair<string,string> strPair;
	checkMinTermSize();
	minimize2Pair(strPair,false);
	//----------------------------;

	copyStrPair2SetS(strPair,normalResult,invertedResult);
  }

  //------------------------------------------------------------------------;
  logicMin(set<string,less<string> >& setOfAllInputTermS,
		   set<string,less<string> >& normalResult):
	_numOfMinTermS(setOfAllInputTermS.size()),
	_outpValueS(setOfAllInputTermS.size(),TRUE_SYMBOL){

	_allInputTermS.resize(_numOfMinTermS);
	_allTermS.resize(_numOfMinTermS);
	_allMinTermInfo.resize(_numOfMinTermS);
	_complementInfo.resize(_numOfMinTermS);

	_inpVarS=(_allInputTermS.begin())->size();

	copySet2Vector(setOfAllInputTermS);

	//----------------------------;
	pair<string,string> strPair;
	checkMinTermSize();
	minimize2Pair(strPair,true);
	//----------------------------;
	copyStrPair2Set(strPair,normalResult);
  }
  //------------------------------------------------------------------------;
  logicMin(list<string>& minTermList,
	   list<string>& normalResultList):
	_numOfMinTermS(minTermList.size()),
	_outpValueS(minTermList.size(),TRUE_SYMBOL){

	_allInputTermS.resize(_numOfMinTermS);
	_allTermS.resize(_numOfMinTermS);
	_allMinTermInfo.resize(_numOfMinTermS);
	_complementInfo.resize(_numOfMinTermS);

	_inpVarS=(minTermList.begin())->size();

	copyList2Vector(minTermList);

	//----------------------------;
	pair<string,string> strPair;
	checkMinTermSize();
	minimize2Pair(strPair,true);
	//----------------------------;
	copyStrPair2List(strPair,normalResultList);
  }
  //------------------------------------------------------------------------;
  logicMin(list<string>& minTermList,
		   list<string>& normalResultList,
		   list<string>& invertedResultList):
	_numOfMinTermS(minTermList.size()),
	_outpValueS(minTermList.size(),TRUE_SYMBOL){

	_allInputTermS.resize(_numOfMinTermS);
	_allTermS.resize(_numOfMinTermS);
	_allMinTermInfo.resize(_numOfMinTermS);
	_complementInfo.resize(_numOfMinTermS);

	_inpVarS=(minTermList.begin())->size();

	copyList2Vector(minTermList);

	//----------------------------;
	pair<string,string> strPair;
	checkMinTermSize();
	minimize2Pair(strPair,false);
	//----------------------------;
	copyStrPair2ListS(strPair,normalResultList,invertedResultList);
  }
  //------------------------------------------------------------------------;
  logicMin(vector<string>& allInputTermS,
		   pair<string,string>& strPair):
    _numOfMinTermS(allInputTermS.size()),
    _outpValueS(allInputTermS.size(),TRUE_SYMBOL){
	
	_allInputTermS.swap(allInputTermS);
	_allTermS.resize(_numOfMinTermS);
	_allMinTermInfo.resize(_numOfMinTermS);
	_complementInfo.resize(_numOfMinTermS);

	_inpVarS=(_allInputTermS.begin())->size();
	
	//----------------------------;
	checkMinTermSize();
	minimize2Pair(strPair,false);
	//----------------------------;
  }
  ~logicMin(){}
private: 
  typedef vector<list<minTerm> >::iterator alterable_allTermSIter;
  alterable_allTermSIter allTermSBegin(){return _allTermS.begin();}
  alterable_allTermSIter allTermSEnd(){return _allTermS.end();}
  
  typedef list<minTerm>::iterator alterable_minTermListIter;
  alterable_minTermListIter minTermListBegin(alterable_allTermSIter I){return I->begin();}
  alterable_minTermListIter minTermListEnd(alterable_allTermSIter I){return I->end();}
  alterable_minTermListIter minTermListBegin(unsigned int index){return _allTermS[index].begin();}
  alterable_minTermListIter minTermListEnd(unsigned int index){return _allTermS[index].end();}
  
  typedef vector<string>::iterator alterable_allInputTermSIter;
  alterable_allInputTermSIter allInputTermSBegin(){return _allInputTermS.begin();}
  alterable_allInputTermSIter allInputTermSEnd(){return _allInputTermS.end();}
  
  typedef string::iterator outpValueSIter;
  outpValueSIter outpValueSBegin(){return _outpValueS.begin();}
  outpValueSIter outpValueSEnd(){return _outpValueS.end();}
  const unsigned int outpValueSSize() const {return _outpValueS.size();}

  void copySet2Vector(set<string,less<string> >& setOfAllInputTermS);
  void copyStrPair2SetS(pair<string,string>& strPair,
					   set<string,less<string> >& normalResult,
					   set<string,less<string> >& invertedResult);
  void copyStrPair2Set(pair<string,string>& strPair,
					   set<string,less<string> >& normalResult);
  void copyList2Vector(list<string>& minTermList);
  void copyStrPair2List(pair<string,string>& strPair,
						list<string>& normalResultList);
  void copyStrPair2ListS(pair<string,string>& strPair,
						 list<string>& normalResultList,
						 list<string>& invertedResultList);

  void allListOfTermS2Free();
  void minTermS2Free(list<minTerm>& list);
  unsigned int maxLen(list<minTerm>& l);
  void reduceTermS(list<minTerm>& minTermList,
				   unsigned int lenMargin);
  void reduceAll();
  bool redundant(list<minTerm>& minTermList,
				 unsigned int allInputTermSIndex);
  void discardTerm(unsigned int Index);
  void markDifferentInpVarS(unsigned int oneTermIndex,
							unsigned int zeroTermIndex,
							unsigned int &firstZeroTermIndex,//call by reference essential;
							string::size_type valuePos,
							char value);
  void cmpTermS();	  
  void mult(alterable_allTermSIter allTermSIter);
  void allMult();
  int cntEqualTermS(alterable_minTermListIter minTermListIter0,
					unsigned int lower_margin);
  void getTermNum();
  unsigned int getFirstTerm(unsigned int allTermIndex,
							unsigned int bestIndex);
  void removeInessentialTermS(unsigned int firstTermIndex,
							  unsigned int bestIndex);
  void eraseIdentical(unsigned int bestIndex,
					  unsigned int nextTermIndex);
  void selectTermS();
  void generateDNF(string& dnfString);
  void invert();
  void buildComplement();
  void minimize(bool invertedMini,
				string& result);
  void minimize2Pair(pair<string,string>& strPair, bool onlyNormal);
  unsigned int getAllTermSCharSize() const ;
  void checkMinTermSize() const ;
  void printAllInputTermS() const ;
  void printAllTermS() const ;
  void printMinTermList(const list<minTerm>& minTermList) const ;
  void printAllMinTermInfo() const ;
  void printInfoOfMinTerm(unsigned int index) const ;
  //Container and VarS;
  unsigned int _inpVarS;
  unsigned int _numOfMinTermS;
  vector<list<minTerm> > _allTermS;
  vector<string> _allInputTermS;
  string _outpValueS; 
  vector<minTermInfo> _allMinTermInfo;
  vector<complementInfo> _complementInfo;
};

#endif


