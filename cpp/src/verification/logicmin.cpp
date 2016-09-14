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

#include <verification/logicmin.h>

ostream& operator<<(ostream& os, const minTerm& min){
    min.term2stream(os);
    return os;
  }


// self-explaining;
void
logicMin::copySet2Vector(set<string,less<string> >& setOfAllInputTermS){
  unsigned int index(0);
  for(set<string,less<string> >::const_iterator const_setIter(setOfAllInputTermS.begin());
	  const_setIter!=setOfAllInputTermS.end();
	  ++const_setIter){
	_allInputTermS[index++]=*const_setIter;
  }
}

// dto;
void
logicMin::copyStrPair2SetS(pair<string,string>& strPair,
						   set<string,less<string> >& normalResult,
						   set<string,less<string> >& invertedResult){
  normalResult.clear();
  invertedResult.clear();
  string oneTerm, emptyTerm;
  for(string::const_iterator const_strIter(strPair.first.begin());
	  const_strIter!=strPair.first.end();
	  ++const_strIter){
	if(*const_strIter==OR_SYMBOL){
	  normalResult.insert(oneTerm);
	  oneTerm=emptyTerm;
	}
	else oneTerm+=*const_strIter;
  }
  normalResult.insert(oneTerm);
  oneTerm=emptyTerm;

  for(string::const_iterator const_strIter(strPair.second.begin());
	  const_strIter!=strPair.second.end();
	  ++const_strIter){
	if(*const_strIter==OR_SYMBOL){
	  invertedResult.insert(oneTerm);
	  oneTerm=emptyTerm;
	}
	else oneTerm+=*const_strIter;
  }
  invertedResult.insert(oneTerm);
  oneTerm=emptyTerm;
}

//dto;
void
logicMin::copyStrPair2Set(pair<string,string>& strPair,
						  set<string,less<string> >& normalResult){
  normalResult.clear();
  string oneTerm, emptyTerm;
  for(string::const_iterator const_strIter(strPair.first.begin());
	  const_strIter!=strPair.first.end();
	  ++const_strIter){
	if(*const_strIter==OR_SYMBOL){
	  normalResult.insert(oneTerm);
	  oneTerm=emptyTerm;
	}
	else oneTerm+=*const_strIter;
  }
  normalResult.insert(oneTerm);
  oneTerm=emptyTerm;
}
//dto;
void
logicMin::copyList2Vector(list<string>& minTermList){
  unsigned int index(0);
  for(list<string>::const_iterator I(minTermList.begin());
	  I!=minTermList.end();
	  ++I)
	_allInputTermS[index++]=*I;
}
//dto;
void
logicMin::copyStrPair2List(pair<string,string>& strPair,
						   list<string>& normalResultList){
  normalResultList.clear();
  string oneTerm, emptyTerm;
  for(string::const_iterator const_strIter(strPair.first.begin());
	  const_strIter!=strPair.first.end();
	  ++const_strIter){
	if(*const_strIter==OR_SYMBOL){
	  normalResultList.push_back(oneTerm);
	  oneTerm=emptyTerm;
	}
	else oneTerm+=*const_strIter;
  }
  normalResultList.push_back(oneTerm);
  oneTerm=emptyTerm;
}
//dto;
void
logicMin::copyStrPair2ListS(pair<string,string>& strPair,
							list<string>& normalResultList,
							list<string>& invertedResultList){
  normalResultList.clear();
  string oneTerm, emptyTerm;
  for(string::const_iterator const_strIter(strPair.first.begin());
	  const_strIter!=strPair.first.end();
	  ++const_strIter){
	if(*const_strIter==OR_SYMBOL){
	  normalResultList.push_back(oneTerm);
	  oneTerm=emptyTerm;
	}
	else oneTerm+=*const_strIter;
  }
  normalResultList.push_back(oneTerm);
  oneTerm=emptyTerm;

  invertedResultList.clear();
  for(string::const_iterator const_strIter(strPair.second.begin());
	  const_strIter!=strPair.second.end();
	  ++const_strIter){
	if(*const_strIter==OR_SYMBOL){
	  invertedResultList.push_back(oneTerm);
	  oneTerm=emptyTerm;
	}
	else oneTerm+=*const_strIter;
  }
  invertedResultList.push_back(oneTerm);
  oneTerm=emptyTerm;
}
//------------------------------------------;
// allListOfTermS2Free():                   ;
// frees all list<minTerm> in _allTermS     ;
//------------------------------------------;
void 
logicMin::allListOfTermS2Free(){
  for(alterable_allTermSIter I(allTermSBegin());
	  I!=allTermSEnd();
	  ++I)
	minTermS2Free(*I);
}
//------------------------------------------;
// minTermS2Free():                         ;
// clears the passed list                   ;
//------------------------------------------;
void
logicMin::minTermS2Free(list<minTerm>& minTermList){
  if(!minTermList.empty())
	minTermList.clear();
}
//------------------------------------------;
// maxLen():                                ;
// returns the maximal length of the terms  ;
// in list<minTerm>& l                      ;
//------------------------------------------;
unsigned int
logicMin::maxLen(list<minTerm>& list){
  unsigned int max(0);
  if(!list.empty()){
	for(const_minTermListIter J(list.begin());
		J!=list.end();
		++J){
	  max=(max>J->termLen())?max:J->termLen();
	}
  }
  return max;
}
//-------------------------------------------;
// reduceTermS();                            ;
// erase all terms which has fewer           ;
// dont cares AND which are fully covered by ;
// the smaller ones                          ;
// these terms are deleted                   ;
//                                           ;
// strategy: imagine a term x                ;
// onePos stores the pos of TRUE_SYMBOLS of x;
// then all other terms with the same len    ;
// are checked for their position of         ;
// TRUE_SYMBOLS                              ;
// if these checked terms has a FALSE_SYMBOL ;
// where x has a TRUE_SYMBOL no reduction is ;
// possible                                  ;
//-------------------------------------------;
void
logicMin::reduceTermS(list<minTerm>& minTermList,
					  unsigned int lenMargin){
  //cerr<<"/reduTermS"<<endl;
  if(!minTermList.empty()){
	bool reduce(false);
	vector<string::size_type> nonDCPos;
	nonDCPos.resize(_numOfMinTermS);
	for(alterable_minTermListIter reduceIter(minTermList.begin());
	    reduceIter!=minTermList.end();
	    ++reduceIter){
	  if(reduceIter->termLen()==lenMargin){//search a term with a length of lenMargin;
		string::size_type pos=0;
		//store the TRUE_SYMBOLS and FALSE_SYMBOLS of reduceIter in nonDCPos;
		for(string::size_type stringIndex(0);
		    stringIndex<reduceIter->termSize();
		    ++stringIndex){
		  if((reduceIter->get(stringIndex))!=DC_SYMBOL)nonDCPos[pos++]=stringIndex;//store the positions of the TRUE_SYMBOLS;
		}
		nonDCPos[pos]=string::npos;//end-marker;

		//compare the positions of ones and zeros with the other terms in minTermList;
		//if one of this terms has a FALSE_SYMBOL or TRUE_SYMBOL at a position appearing in onePos[];
		//-> no reduction;
		for(alterable_minTermListIter searchIter(minTermList.begin());
		    searchIter!=minTermList.end();
		    ++searchIter){
		  ////cerr<<"compare: "<<*minTermListIter0<<" with "<<*minTermListIter1<<endl;
		  if(reduceIter!=searchIter&&searchIter->termLen()>=lenMargin){
		    reduce=true;
			for(pos=0;
				nonDCPos[pos]!=string::npos;
				++pos){
			  if(searchIter->get(nonDCPos[pos])==DC_SYMBOL){
				//remember, that in onePos was stored the position of the TRUE_SYMBOLS and FALSE_SYMBOLS;
				reduce=false;//in this case NO REDUCTION POSSIBLE, because the term behind searchIter is at this position DC;
				break;// stop traversing the term;
			  }
			}
			if(reduce){
			  ////cerr<<"delete "<<*searchIter<<endl;
			  //*reduceIter covers *searchIter, therefore erase searchIter;
			  assert(!minTermList.empty());
			  searchIter=minTermList.erase(searchIter);//executes the ++minTermListIter1 statement implicit;
			  if(!minTermList.empty())--searchIter;
			}
		  }
		}
	  }
	}
  }
  //cerr<<"/reduTermS_done"<<endl;
}
//------------------------------------------;
// reduceAll():                             ;
// reduces all terms in _allTermS           ;
// by calling reduceTermS()                 ;
// this function is equivalent to the       ;
// reduction of the Quine McCluskey Algo.   ;
//------------------------------------------;
void
logicMin::reduceAll(){
  //cerr<<"/rA";
  for(alterable_allTermSIter I(allTermSBegin());
      I!=allTermSEnd();
      ++I){
	if(!(I->empty())){
	  for(unsigned int i(2);
		  i<=maxLen(*I);
		  ++i)
		reduceTermS(*I,i);
	}
  }
  //cerr<<"- done"<<endl;
}
//------------------------------------------;
// redundant():                             ;
// compares ONE line of the input matrix    ;
// (allInputTermSIndex) with a given        ;
// minTermList                              ;
// if the line is redundant, return true    ;
//                                          ;
// called by:                               ;
// discarTerm()                            ;
//------------------------------------------;
bool
logicMin::redundant(list<minTerm>& minTermList,
					unsigned int allInputTermSIndex){
  //cerr<<"/redundant\n";
  if(!minTermList.empty()){
	for(const_minTermListIter minTermListIter(minTermList.begin());
		minTermListIter!=minTermList.end();
		++minTermListIter){
	  for(string::size_type stringIndex(0);
		  stringIndex<_inpVarS;
		  ++stringIndex){
		assert(stringIndex<_allInputTermS[allInputTermSIndex].size());
		if(minTermListIter->get(stringIndex)!=DC_SYMBOL){
		  // NOT REDUNDANT;
		  if(minTermListIter->get(stringIndex)!=_allInputTermS[allInputTermSIndex][stringIndex]){
			//cerr<<"false"<<endl;
			//cerr<<"/redu_done"<<endl;
			return false;	
		  }
		  break;
		}
	  } 
	}
  }
  //cerr<<"true"<<endl;
  //cerr<<"/redundant_done"<<endl;
  return true;
}
//------------------------------------------;
// discardTerm():                           ;
// looks for lines with output value        ;
// TRUE_SYMBOL to check them for redundancy.;
// All redundant terms are marked as DISCARD;
// in _allTermS                             ;
// this method decimates the number of terms;
// to handle                                ;
// if an already calculated term is         ;
// recognized as redundant it is inserted   ;
// into the _freeList                       ;
//                                          ;
// called by:                               ;
// cmpTermS()                               ;
//------------------------------------------;
void
logicMin::discardTerm(unsigned int coveringIndex){
  //cerr<<"discardTerm"<<endl;
  //cerr<<"discardTerm : coveringIndex ="<<coveringIndex<<endl;
  for(unsigned int checkForRedundanceIndex(0);
      checkForRedundanceIndex<allTermSSize();
      ++checkForRedundanceIndex){
	//cerr<<"discardTerm : i="<<checkForRedundanceIndex<<" "<<endl;
	if(!_allTermS[checkForRedundanceIndex].empty()){
		if(_outpValueS[checkForRedundanceIndex]!=TRUE_SYMBOL||
		   _allTermS[checkForRedundanceIndex].front()==DISCARD||
		   coveringIndex==checkForRedundanceIndex){
		  //cerr<<"discardTerm : i="<<checkForRedundanceIndex<<" ";
		  //cerr<<"continued with i="<<checkForRedundanceIndex<<"in discardTerm"<<endl;
		  continue;
		}
	}
	if(redundant(_allTermS[coveringIndex],checkForRedundanceIndex)){
	  minTermS2Free(_allTermS[checkForRedundanceIndex]);//Free all terms in the entry with number termCnt;
	  _allTermS[checkForRedundanceIndex].push_front(DISCARD);
	}
  }
  //cerr<<"discardTerm_done"<<endl;
}
//------------------------------------------;
// markDifferentInpVarS():                  ;
// compares a oneterm with a zeroterm and   ;
// marks the different vars                 ;
// for every zeroterm a new term will be    ;
// append at _allTermS[zeroTermIndex]       ;
//                                          ;
// called by:                               ;
// cmpTermS()                               ;
//------------------------------------------;
void
logicMin::markDifferentInpVarS(unsigned int oneTermIndex,
							   unsigned int zeroTermIndex,
							   unsigned int &lastZeroTermIndex, //call by reference!! NO COPY;
							   string::size_type valuePos,
							   char value){
  //cerr<<"/mDiffInpVarS"<<endl;
  if((zeroTermIndex!=lastZeroTermIndex)||(_allTermS[oneTermIndex].empty())){
	minTerm mT(_inpVarS);
	assert(mT.termNum()==0);
	_allTermS[oneTermIndex].push_front(mT);
	lastZeroTermIndex=zeroTermIndex;
  }
  (_allTermS[oneTermIndex].begin())->incTermLen();
  (_allTermS[oneTermIndex].begin())->set(valuePos,value);
  
  //DEBUG;
  //cerr<<endl<<"valuePos: "<<valuePos<<endl;
  //cerr<<"z0: "<<zeroTermIndex<<endl;
  //cerr<<"z1: "<<oneTermIndex<<endl;
  //cerr<<*_allTermS[oneTermIndex].begin()<<endl;
  //cerr<<"-----------------------------"<<endl;
  //END_DEBUG;
  //cerr<<"mDIV_done"<<endl;
}
//------------------------------------------;
// cmpTermS():                              ;
// compares all oneterms with all zeroterms ;
// oneterms and detects logic mismatch      ;
// found terms are -if possible- reduced to ;
// a length of 1 to detect redundant terms  ;
//                                          ;
// this method works on the _allInputTermS  ;
// container and fills the _allTermS        ;
// container by calling                     ;
// markDifferentInpVarS()                   ;
//                                          ;
// called by:                               ;
// minimize()                               ;
//                                          ;
// calls:                                   ;
// markDifferentInpVarS                     ;
//------------------------------------------;
void
logicMin::cmpTermS(){
  //cerr<<"/cmpTermS"<<endl;
  unsigned int lastZeroTermIndex(allTermSSize());
  //lastZeroTermIndex marks the last handled zeroterm in markDifferentInpVarS(..);
  //the initial value is an ILLEGAL INDEX;
  bool contradiction(false);
  // Search a oneterm, a term for which the equation is true;
  for(unsigned int oneTermIndex(0);
      oneTermIndex<outpValueSSize();
      ++oneTermIndex){
    if(_outpValueS[oneTermIndex]!=TRUE_SYMBOL)continue;
	if(!_allTermS[oneTermIndex].empty())
	  if(_allTermS[oneTermIndex].front()==DISCARD)continue;
    //oneTermIndex marks a oneTerm;
    //now search a zeroTerm;
    for(unsigned int zeroTermIndex(0);
	zeroTermIndex<outpValueSSize();
	++zeroTermIndex){
      if(_outpValueS[zeroTermIndex]!=FALSE_SYMBOL)continue;
	  if(!_allTermS[zeroTermIndex].empty())
		if(_allTermS[zeroTermIndex].front()==DISCARD)continue;//modified;
      //zeroTermIndex marks a zeroterm;
      contradiction=true;//initialize with true;
	  //convince me that there is no contradiction;
      for(string::size_type varPos(0);
		  varPos<_inpVarS;
		  ++varPos){
		//0:0, 1:1, *:DC, DC:* cases -> no difference -> nothing to mark;
	   if(_allInputTermS[zeroTermIndex][varPos]==_allInputTermS[oneTermIndex][varPos]||
	   _allInputTermS[zeroTermIndex][varPos]==DC_SYMBOL||
	   _allInputTermS[oneTermIndex][varPos]==DC_SYMBOL)continue;
	   markDifferentInpVarS(oneTermIndex,
							zeroTermIndex,
							lastZeroTermIndex,
							varPos,
							_allInputTermS[oneTermIndex][varPos]);
	   //attention: firstZeroTermIndex modified in markDifferentInpVarS(..);
	   contradiction=false;
      }
      assert(!contradiction);
      //if programm exits here there is a contradiction in the logic equation;
	  //what means that a oneterm is also a zeroterm ... ;
    }
    reduceTermS(_allTermS[oneTermIndex],1);//reduce all terms with len of one;
    if(maxLen(_allTermS[oneTermIndex])==1){
      //cerr<<"discard oneTermIndex: "<<oneTermIndex<<endl;
	  discardTerm(oneTermIndex);
    }
  }
  for(alterable_allTermSIter discardEliminationIter(allTermSBegin());
	  discardEliminationIter!=allTermSEnd();
	  ++discardEliminationIter){
    if(!discardEliminationIter->empty())
	  if(*(discardEliminationIter->begin())==DISCARD){
		discardEliminationIter->clear();
		assert(discardEliminationIter->empty());
	  }
  }
  //cerr<<"cmpTermS_done"<<endl;
}
//------------------------------------------------------------;
// mult():                                                    ;
// transforms an expression in conjunctive                    ;
// normal form to disjunctive normal form                     ;
//                                                            ;
// called by:                                                 ;
// allMult()                                                  ;
// e.g.: inputs: abcdef                                       ;
// a+b * c+d * e+f  => 10****, **01**, ****11                 ;
// lTermList : 1*****, *0****                                 ;
// rTermList : 1*0***, 1**1**,                                ;
//             *01***, *0*1**                                 ;
// (combination of lTermList and c+d (**11**))                ;
// -> lTermList=rTermList; rTermList.clear();                 ;
// lTermList : 1*0***, 1**1**,                                ;
//             *01***, *0*1**                                 ;
// rTermList : 1*0*1*, 1*0**1,                                ;
//             1**11*, 1**1*1,                                ;
//             *01*1*, *01**1,                                ;
//             *0*11*, *0*1*1                                 ;
//(combination of lTermList and e+f (****11)                  ;
//                                                            ;
// called by:                                                 ;
// allMult()                                                  ;
//                                                            ;
// calls:                                                     ;
// reduceTermS(), minTermS2Free()                             ;
//------------------------------------------------------------;
void
logicMin::mult(alterable_allTermSIter allTermSIter){
  //cerr<<"/mult"<<endl;
  //first initialize lTermList with the first splitted Term (see above);
  list<minTerm> lTermList;
  const_minTermListIter minTermListIter(minTermListBegin(allTermSIter));
  for(string::size_type inpVarIndex(0);
	  inpVarIndex<_inpVarS;
	  ++inpVarIndex){
	if(minTermListIter->get(inpVarIndex)!=DC_SYMBOL){//each one results in a new term;
	  minTerm term(_inpVarS);
	  term.set(inpVarIndex,minTermListIter->get(inpVarIndex));
	  term.incTermLen();//TermLen=1;
	  lTermList.push_front(term);
	}
  }

  if(!lTermList.empty()){
	//then reduce the number of terms to handle with every while-loop step;
	list<minTerm> rTermList;
	for(++minTermListIter;
		minTermListIter!=allTermSIter->end();
		++minTermListIter){
	  for(string::size_type inpVarIndex(0);
		  inpVarIndex<_inpVarS;
		  ++inpVarIndex){
		if(minTermListIter->get(inpVarIndex)!=DC_SYMBOL){
		  for(alterable_minTermListIter lTermListIter(lTermList.begin());
			  lTermListIter!=lTermList.end();
			  ++lTermListIter){
			minTerm term(_inpVarS);
			term.setTerm(lTermListIter->term());//get a copy of lTermList item;
			term.set(inpVarIndex,minTermListIter->get(inpVarIndex));
			rTermList.push_front(term);
		  }
		}
	  }
	  minTermS2Free(lTermList);
	  assert(lTermList.empty());
	  //change list content;
	  //change the rolls of the two lists;
	  lTermList.swap(rTermList);//rTermList is empty now (minTermS2Free() -> swap());
	  //initialization of margins for reduceTermS;
	  unsigned int lower_margin(_inpVarS);
	  unsigned int upper_margin(0);
	  if(!lTermList.empty()){
		for(alterable_minTermListIter lTermListIter(lTermList.begin());
			lTermListIter!=lTermList.end();
			++lTermListIter){
		  unsigned int termLen=0;
		  for(string::size_type inpVarIndex(0);
			  inpVarIndex<_inpVarS;
			  ++inpVarIndex)
			if(lTermListIter->get(inpVarIndex)!=DC_SYMBOL)++termLen;
		  if(termLen<lower_margin)lower_margin=termLen;
		  if(termLen>upper_margin)upper_margin=termLen;
		  lTermListIter->setTermLen(termLen);
		}
	  }
	  for(unsigned int numOfOnesInTerm=lower_margin;
		  numOfOnesInTerm<=upper_margin;
		  ++numOfOnesInTerm)
		reduceTermS(lTermList,numOfOnesInTerm);
	} 
	//free the old minTermList (_allTermS[allTermSIter]);
	minTermS2Free(*allTermSIter);
	assert(allTermSIter->empty());
	allTermSIter->swap(lTermList);
  }
}
//------------------------------------------;
// allMult():                               ;
// calls mult for each list of minterms in  ;
// _allTermS                                ;
//------------------------------------------;
void
logicMin::allMult(){
  //cerr<<"/allmult"<<endl;
  for(alterable_allTermSIter allTermSIter(allTermSBegin());
      allTermSIter!=allTermSEnd();
      ++allTermSIter){
	if(!allTermSIter->empty())
	  mult(allTermSIter);
  }
  //cerr<<"allmult_done"<<endl;
}
//------------------------------------------;
// cntEqualTermS():                         ;
// counts the number of equal terms in      ;
// different lists of _allTermS             ;
// the number of equal terms is returned    ;
//------------------------------------------;
int
logicMin::cntEqualTermS(alterable_minTermListIter minTermListIter0,
						unsigned int indexMargin){
  //cerr<<"/cntEqTrm"<<endl;
  string emptyTerm(_inpVarS,DC_SYMBOL);	
  int equalNum(0);
#warning had to comment out following line because of compiler? 13.03.2007 MN
  //  assert(minTermListIter0!=0&&indexMargin<=allTermSSize());
  if(minTermListIter0->term()!=emptyTerm){//dont count empty terms;
	for(unsigned int allTermSIndex(indexMargin);
		allTermSIndex<allTermSSize();
		++allTermSIndex){
	  if(!_allTermS[allTermSIndex].empty()){
		  for(alterable_minTermListIter minTermListIter1(minTermListBegin(allTermSIndex));
			  minTermListIter1!=minTermListEnd(allTermSIndex);
			  ++minTermListIter1){
			//COMPARISON between two Minterms;
			if(*minTermListIter0==*minTermListIter1&&!(minTermListIter1->termNumAssigned())){
			  minTermListIter1->setTermNum(minTermListIter0->termNum());//unique term number;
			  minTermListIter1->setTermNumAssigned();
			  ++equalNum;
			}
			//minTermListIter1->print();
		  }
		}
	  }
	}
  //cerr<<"/cntEqTrm_done"<<endl;
  return equalNum;//zero if minTermListIter0 is empty or discard;
}
//------------------------------------------;
// getTermNum():                            ;
// - numbers essential terms found          ;
// - then the terms are compared with the   ;
// following terms                          ;
// - equal terms get equal numbers          ;
// in _allMinTermInfo the number of equal   ;
// terms and the length of the terms will be;
// stored                                   ;
//------------------------------------------;
void
logicMin::getTermNum(){
  //cerr<<"/gTermNum"<<endl;
  if(_numOfMinTermS>allMinTermInfoSize())
	cerr<<"_numOfMinTermS: "<<_numOfMinTermS<<" allMinTermInfoSize :"<<allMinTermInfoSize()<<endl;
  assert(_numOfMinTermS<=allMinTermInfoSize());
  unsigned int termNum(0);
  for(unsigned int allTermSIndex(0);
	  allTermSIndex<allTermSSize();
	  ++allTermSIndex){
    if(_allTermS[allTermSIndex].empty())continue;

	// first enumerate the essential terms!;
	if(_allTermS[allTermSIndex].size()==1&&!(_allTermS[allTermSIndex].begin()->termNumAssigned())){
	  //cerr<<endl<<"mainTerm : "<<_allTermS[allTermSIndex].begin()->term()<<" at position"<<allTermSIndex<<endl;
	  // This is a non-reduceable, essential term!!;
	 assert(termNum<allTermSSize());
	 _allTermS[allTermSIndex].begin()->setTermNum(termNum++);
	 _allTermS[allTermSIndex].begin()->setTermNumAssigned();// mark term as term with assigned termNum;
	 continue;// the termNum++ below is not executed;
	}
	for(alterable_minTermListIter minTermListIter(minTermListBegin(allTermSIndex));
		minTermListIter!=minTermListEnd(allTermSIndex);
		++minTermListIter){
	  if(!(minTermListIter->termNumAssigned())){// term not discussed yet, initial value of termNum is 0;
		minTermListIter->setTermNum(termNum);
		minTermListIter->setTermNumAssigned();// mark term as term with assigned termNum;
		minTermInfo termInfo(cntEqualTermS(minTermListIter,allTermSIndex+1),
							 minTermListIter->termLen(),
							 allTermSIndex);
		_allMinTermInfo[termNum++]=termInfo;
	  }
	}
  }
  //cerr<<"/gTermNum_done"<<endl;
}
//------------------------------------------;
// getFirstTerm():                          ;
// returns an Index  to _allTermS           ;
// the term number of this term has to be   ;
// equal to termNum                         ;
//------------------------------------------;
unsigned int
logicMin::getFirstTerm(unsigned int allTermIndex,
					   unsigned int bestIndex){
  for(unsigned int allTermSIndex(allTermIndex);
      allTermSIndex<allTermSSize();
      ++allTermSIndex)
    for(alterable_minTermListIter minTermListIter(minTermListBegin(allTermSIndex));
		minTermListIter!=minTermListEnd(allTermSIndex);
		++minTermListIter)
      if(minTermListIter->termNum()==bestIndex)return allTermSIndex;
  // if the first is found, return this index!;
  // ->getFirstTerm(..);
  //cerr<<"Internal Error : getFirstTerm() : NO FIRST TERM IN _allTermS Container"<<endl;
  return allTermSSize();//allTermSSize() is a forbidden Index -> assert(..) after returning from routine;
  // THIS CASE SHOULD NEVER APPEAR!!!, but if it should be handled ... ;
}
//-------------------------------------------------------;
// removeInessentialTermS()                              ;
// removes all inessential terms in the list of bestIndex;
// the criteria is that the term-numbers in              ;
// of the deleted minTerms                               ;
// are not equal to bestIndex                            ;
//-------------------------------------------------------;
void
logicMin::removeInessentialTermS(unsigned int firstTermIndex,
								 unsigned int bestIndex){
  //cerr<<"/remI"<<endl;
  if(!_allTermS[bestIndex].empty()){
    for(alterable_minTermListIter minTermListIter(minTermListBegin(bestIndex));
		minTermListIter!=minTermListEnd(bestIndex);
		++minTermListIter){
	  if(minTermListIter->termNum()!=firstTermIndex){
		if(_allMinTermInfo[minTermListIter->termNum()].equalTermS())//if there equal terms in other minTermLists;
		  _allMinTermInfo[minTermListIter->termNum()].decrEqualTermS(); //decrement counter, to update info-structure;
		//cerr<<"Index : "<<bestIndex<<" erase : "<<minTermListIter->term()<<endl;
		_allTermS[bestIndex].erase(minTermListIter);//implicit increment of iterator;
		--minTermListIter;//Undo increment, its the job of the for-loop;
	  }
	}
  }
  //cerr<<"/remI_done"<<endl;
}
//--------------------------------------------------;
// eraseIdentical:                                  ;
// all double Terms with termNum will be erased     ;
// therefore the minTermLists in _allTermS will be  ;
// traversed from allTermIndex to allTermSSize()    ;
// and each term will be checked for doubles        ;
// (-> _minTermInfo[])                              ;
// every minTermList with doubles in other list(s)  ;
// will be cleared                                  ;
// the lists will be cleared since equalTermS() is 0;
//--------------------------------------------------; 
void
logicMin::eraseIdentical(unsigned int bestIndex,
						 unsigned int nextTermIndex){
  //cerr<<"/eraI"<<endl;
  for(unsigned int allTermSIndex(nextTermIndex);
      allTermSIndex<allTermSSize();
      ++allTermSIndex){
	if(!_allTermS[allTermSIndex].empty()){
	  //the DISCARD case is checked 4 lines down;
	  for(const_minTermListIter minTermListIter(minTermListBegin(allTermSIndex));
	      minTermListIter!=minTermListEnd(allTermSIndex);//!
	      ++minTermListIter){
		if(minTermListIter->termNum()==bestIndex&&minTermListIter->termNumAssigned()){
		  // only erase terms with the same termNum -> equal terms are redundant;
		  // if termNums are equal check the list for further terms with termNum;
		  // there exists one ore more duplicates in OTHER list(s) -> see _allMinTermInfo[<termNum>].equalTermS();
		  // because of the current list will be cleared, update all other minTermInfo structures indexed with termNum;
		  for(const_minTermListIter updateMinTermListIter(minTermListBegin(allTermSIndex));
			  updateMinTermListIter!=minTermListEnd(allTermSIndex);
			  ++updateMinTermListIter){
			assert(updateMinTermListIter->termNum()<allMinTermInfoSize());
			if(_allMinTermInfo[updateMinTermListIter->termNum()].equalTermS()){//if there are equal terms;
			  _allMinTermInfo[updateMinTermListIter->termNum()].decrEqualTermS();//update the info-data-structure;
			}
		  }
		  minTermS2Free(_allTermS[allTermSIndex]);//clear if minTermListIter->termNum()==termNum;
		  // the _allMinTermInfo data structure contains the information about the double terms
		  // to each termNum -> if _allMinTermInfo[<termNum>].equalTermS() is 0, there are NO DOUBLE TERMS;
		  // of <termNum> -> return;
		  if(!(_allMinTermInfo[bestIndex].equalTermS()))return;//no double terms in _allTermS;
		  break;//++minTermListIter, because the CURRENT LIST WAS CLEARED!;
		}
	  }//according to breaks;
	}
  }//next minTermList;
  //cerr<<"/eraI_done"<<endl;
}
//------------------------------------------;
// selectTermS():                           ;
// reduces every expression (list<string>   ;
// in _allTermS) about one Term             ;
// (class minTerm) which covers most other  ;
// Terms. This term will be erased          ;
//                                          ;
// is called by:                            ;
// minimize()                               ;
//                                          ;
// calls:                                   ;
// getFirstTerm()                           ;
// removeInessentialTerm()                  ;
// eraseIdentical()                         ;
//------------------------------------------;
void
logicMin::selectTermS(){
  //cerr<<"/selTermS"<<endl;
  unsigned int bestIndex;
  do{
	bestIndex=allTermSSize();// forbidden Index, only to mark end of while-loop;
	for(unsigned  int runningIndex(0);
	    runningIndex<allTermSSize();
	    ++runningIndex){
	  if(_allTermS[runningIndex].empty())continue;
	  // terms with more equals are prefered, if two terms have the same number of equals;
	  // the shorter term will be taken!;
	  // equal to the Quine McCluskey Algo.;
	  if(_allMinTermInfo[runningIndex].equalTermS()>_allMinTermInfo[bestIndex].equalTermS()||
	     (_allMinTermInfo[runningIndex].equalTermS()==_allMinTermInfo[bestIndex].equalTermS()&&
	      _allMinTermInfo[runningIndex].termLen()<_allMinTermInfo[bestIndex].termLen())){
	    bestIndex=runningIndex;
		assert(bestIndex!=allTermSSize());
	  }
	}
	if(bestIndex!=allTermSSize()){//if true -> bestIndex was modified;
	  // Remember: _allTermS is always one entry smaller than _allMinTermInfo!;
	  // see : buildComplement();
	  unsigned int firstTermIndex=getFirstTerm(_allMinTermInfo[bestIndex].termIndex(),
											   bestIndex);
	  assert(firstTermIndex<allTermSSize());//internal Error (-> there was no term found with bestIndex;
	  // Rem:;
	  // bestIndex is the index of the term in the _allTermS structure;

      // the term indexed by bestIndex marks the best term;
	  // (the most covering, and from these the shortest term);
      // this term covers other terms. These terms are eliminated by removeInessentialTermS(..);
      removeInessentialTermS(firstTermIndex,bestIndex);

	  // the term indexed by bestIndex may appear in other lists as doubles generated by reduceTermS(..);
	  // these doubles are eliminated by eraseIdentical(..);
	  // the parameter allTermIndex+1 prevents, that all terms of bestIndex will be deleted;
      eraseIdentical(bestIndex,firstTermIndex+1);
	}
  }while(bestIndex!=allTermSSize());//if bestIndex==allTermSSize(), no term could be found (see ll. 641);
 
  // now there are some terms left which are not essential,;
  // what means that they can be replaced by a combination of other terms ;
  // the choice of these terms influence the quality of the result;
  // to get the very best result you have to try all combinations of these terms;
  // but this brute force solution is nondeterministic polynomial complete (NPC);
  // hence a heurictic is implemented: choose the shortest terms!;

  unsigned int heuristicIndex=0;

  for(unsigned int runningIndex(0);
	  runningIndex<allTermSSize();
	  ++runningIndex){
	if(_allTermS[runningIndex].empty())continue;
	if(_allTermS[runningIndex].front()==DISCARD)continue;
	if(_allTermS[runningIndex].size()>1){
	  // this list contains no essential term;
	  // lists with essential terms have a length of 1!;
	  // now the heuristic has to be used to get on in a acceptable time;
	  unsigned int shortest=_inpVarS; // _inpVarS is the max length of a term;
	  for(const_minTermListIter  minTermListIter(minTermListBegin(runningIndex));
		  minTermListIter!=minTermListEnd(runningIndex);
		  ++minTermListIter){
		// here is the heuristic implemented;
		if(minTermListIter->termLen()<shortest){
		  shortest=minTermListIter->termLen();
		  heuristicIndex=minTermListIter->termNum();
		}
	  }
	  removeInessentialTermS(runningIndex,heuristicIndex);
	}
  }
  //cerr<<"selTermS_done"<<endl;
}

//------------------------------------------;
// generateDNF():                           ;
// traverses _allTermS and prints the       ;
// Disjunctive Normal Form                  ;
//------------------------------------------;
void
logicMin::generateDNF(string& dnfString){
  //cerr<<"/genDNF"<<endl;
  //cerr<<"generateDNF():"<<endl;
  bool isFirstTerm(true);
  for(const_allTermSIter allTermSIter(allTermSBegin());
      allTermSIter!=allTermSEnd();
      ++allTermSIter){
	if(!(allTermSIter->empty())){
	  if(!isFirstTerm){
		dnfString+=OR_SYMBOL;
	  }
		dnfString+=allTermSIter->begin()->term();
		isFirstTerm=false;
	}
  }
  //cerr<<"/genDNF_done"<<endl;
}
//------------------------------------------;
// invert():                                ;
// inverts the _outpValueS string           ;
// e.g.:                                    ;
// 0101 gets 1010                           ;
//------------------------------------------;
void
logicMin::invert(){
  //cerr<<"/inv"<<endl;
  assert(allInputTermSSize()==outpValueSSize());

  for(string::size_type idx(0);
	  idx<outpValueSSize();
	  ++idx){
	assert(_outpValueS[idx]==TRUE_SYMBOL^_outpValueS[idx]==FALSE_SYMBOL);
	if(_outpValueS[idx]==TRUE_SYMBOL)
	  _outpValueS.replace(idx,static_cast<string::size_type>(1),1,FALSE_SYMBOL);
//Here the old functions seems to be deprecated in stl-version 3.0:
//	  _outpValueS.replace(idx,static_cast<string::size_type>(1),FALSE_SYMBOL);
	else
	  _outpValueS.replace(idx,static_cast<string::size_type>(1),1,TRUE_SYMBOL);
//Here the old functions seems to be deprecated in stl-version 3.0:
//	  _outpValueS.replace(idx,static_cast<string::size_type>(1),TRUE_SYMBOL);
  }
}
//------------------------------------------;
// buildComplement():                       ;
// completes the truth-table _allInputTermS ;
// with its zeroterms to define the boolean ;
// function completely                      ;
// In the following steps the function will ;
// minimized in its negated form.           ;
// the result will be inserted into this    ;
// truth-table.                             ;
// an optimization as a result of memory    ;
// overflow is that before the completeion  ;
// of the truth-table all redundant         ;
// positions will be detected in a mask     ;
// Then only the relevant cases will be     ;
// inserted                                 ;
//------------------------------------------;
void
logicMin::buildComplement(){
  //cerr<<"/BuCom"<<endl;
  unsigned int mask(0);
  for(unsigned int numOfMinTermIndex(0);
      numOfMinTermIndex<_numOfMinTermS;
      ++numOfMinTermIndex){
    //initialize with 0 to get correct shift operations;
    complementInfo mTI((unsigned int)0,(unsigned int)0);
    _complementInfo[numOfMinTermIndex]=mTI;
    for(string::size_type inputVarIndex(0);
	inputVarIndex<_inpVarS;
	++inputVarIndex){
      assert(inputVarIndex<_inpVarS);
      //bitwise OR stores the position of the Dont Cares;
      _complementInfo[numOfMinTermIndex]=complementInfo(((_complementInfo[numOfMinTermIndex].val())|
							 ((NUMVAL(_allInputTermS[numOfMinTermIndex][inputVarIndex])<<inputVarIndex))),
							((_complementInfo[numOfMinTermIndex].dc())|
							 (DCVAL(_allInputTermS[numOfMinTermIndex][inputVarIndex])<<inputVarIndex))
							);
    }
    mask|=_complementInfo[numOfMinTermIndex].dc();
    //cerr<<"mask : "<<mask<<endl;
  }
  unsigned int complementInfoNum=_numOfMinTermS;
  bool skip;
  
  for(unsigned int nt(0);
      nt<=mask;
      ++nt){
    skip=false;
    if((nt&mask)!=nt)continue;//nt == mask;
    for(unsigned int numOfMinTermIndex(0);
	numOfMinTermIndex<complementInfoNum;
	++numOfMinTermIndex){
      if(((_complementInfo[numOfMinTermIndex].dc())&nt)==(_complementInfo[numOfMinTermIndex].val())){
	skip=true;
      }
    }
    if(skip){
      continue;
    }
    string zeroTerm(_inpVarS,FALSE_SYMBOL);
    string::iterator sI(zeroTerm.begin());

    //generate the new zeroTerm;
    for(unsigned int inpVarIndex(0);
	inpVarIndex<_inpVarS;
	++inpVarIndex){
      *sI++=((mask&(1<<inpVarIndex))==0)?DC_SYMBOL:CHARVAL(nt&(1<<inpVarIndex));
    }

    ////cerr<<"insert zeroTerm : "<<zeroTerm<<"at position "<<_numOfMinTermS<<endl;

    _allInputTermS.resize(allInputTermSSize()+1);

	_allTermS.resize(allTermSSize()+1);//get space to handle all terms during minimization;
	_allMinTermInfo.resize(allTermSSize()+1);//get space to handle all terms during minimization;
	// REMARK: THE ALLMINTERMINFOSIZE IS ONE ENTRY LARGER THAN THE ALLTERMSSIZE BECAUS OF in the selectTermS() method;
	// the overlapping entry is the zero-entry to initialize bestIndex;
	// the initial values of this zero-entry are all zero;
	// for each entry in _allTermS exists one entry in _allTermInfo;

    _allInputTermS[_numOfMinTermS]=zeroTerm;
    _outpValueS+=FALSE_SYMBOL;
    _numOfMinTermS++;
  }
  //cerr<<"/BuCom_done"<<endl;
}
//------------------------------------------;
// minimize():                              ;
// executes the minimization after          ;
// buildComplenemt() was called             ;
//------------------------------------------;
void
logicMin::minimize(bool invertedMini,
				   string& result){
  //cerr<<"/mini"<<endl;
  if(invertedMini)invert();

  // cout<<"ALLTERMS BEFORE CMPTERMS"<<endl;
  //   printAlTermS();
  cmpTermS();
  // cout<<"ALLTERMS AFTER CMPTERMS"<<endl;
  //   printAllTermS();

  // cout<<"ALLTERMS BEFORE REDUCEALL"<<endl;
  //   printAllTermS();
  reduceAll();
  // cout<<"ALLTERMS AFTER REDUCEALL"<<endl;
  //   printAllTermS();

  // cout<<"ALLTERMS BEFORE ALLMULT "<<endl;
  //   printAllTermS();
  allMult();
  // cout<<"ALLTERMS AFTER ALLMULT"<<endl;
  //   printAllTermS();

  // cout<<"ALLTERMS BEFORE GETTERMNUM "<<endl;
  //   printAllTermS();
  getTermNum();
  // cout<<"ALLTERMS AFTER GETTERMNUM "<<endl;
  //   printAllTermS();

  // cout<<"ALLTERMS BEFORE SELECTTERMS "<<endl;
  //   printAllTermS();
  selectTermS();
  // cout<<"ALLTERMS AFTER SELECTTERMS "<<endl;
  //   printAllTermS();
  
  generateDNF(result);
  allListOfTermS2Free();
  if(invertedMini)invert();
}
//------------------------------------------;
// checkMinTermSize():                      ;
// takes no relevant part in minimization   ;
// algorithm. it is only a routine to check ;
// if all minTermS has the same number of   ;
// inputs                                   ;
//------------------------------------------;
void
logicMin::checkMinTermSize() const {
  for(vector<string>::const_iterator allInputTermSIter(_allInputTermS.begin());
      allInputTermSIter!=_allInputTermS.end();
      ++allInputTermSIter){
	if(allInputTermSIter->size()!=_inpVarS)
	  cerr<<"term:"<<*allInputTermSIter<<" size?: "<<_inpVarS<<endl;
    assert(allInputTermSIter->size()==_inpVarS);
  }
}
//------------------------------------------;
// minimize2Pair():                         ;
// - self-introductive -                    ;
//------------------------------------------;
void
logicMin::minimize2Pair(pair<string,string>& strPair, bool onlyNormal){
  //cerr<<"/M2P";
  unsigned int tmpNumOfMinTermS = _numOfMinTermS;
  buildComplement();
  if(tmpNumOfMinTermS!=_numOfMinTermS){
    minimize(false,strPair.first);
	if(!onlyNormal)
	  minimize(true,strPair.second);
    }
  else{
    //THE KARNAUGH DIAGRAMM HAS ONLY ONEs!!;
    string dcTerm(_inpVarS,DC_SYMBOL);
    pair<string, string> dcPair(dcTerm, dcTerm);
    strPair=dcPair;
  }
  return;
}
//------------------------------------------;
// getAllTermSCharSize():                   ;
// returns the size of result by counting   ;
// the chars in _allTermS after all reducing;
// and erasing steps                        ;
//------------------------------------------;
unsigned int
logicMin::getAllTermSCharSize() const {
  unsigned int CharSize(0);
  for(const_allTermSIter I(allTermSBegin());
	  I!=allTermSEnd();
	  ++I){
	if(!I->empty()){
	  if(!(*(I->begin())==DISCARD)){
		CharSize+=(I->begin()->term()).size();
		++CharSize;
	  }
	}
  }  
  return --CharSize;
}
//------------------------------------------;
// printAllTermS():                         ;
// - self-explaining -                      ;
//------------------------------------------;
void
logicMin::printAllTermS() const {
  for(const_allTermSIter I(allTermSBegin());
	  I!=allTermSEnd();
	  ++I){
	if(minTermListBegin(I)==minTermListEnd(I)){
	  cerr<<"<empty>"<<endl;
	  cerr<<"----------"<<endl;
	  continue;
	}
	for(const_minTermListIter J(minTermListBegin(I));
		J!=minTermListEnd(I);
		++J){
	  cerr<<J->term()<<endl;
	}
	cerr<<"----------"<<endl;
  }
}
//------------------------------------------;
// printAllInputTermS():                    ;
// - self-explaining -                      ;
//------------------------------------------;
void
logicMin::printAllInputTermS() const {
  for(const_allInputTermSIter I(allInputTermSBegin());
	  I!=allInputTermSEnd();
	  ++I)
	cerr<<*I<<endl;
  cerr<<"_outpValueS="<<_outpValueS<<endl;
}
//------------------------------------------;
// printMinTermList():                      ;
// - self-explaining -                      ;
//------------------------------------------;
void
logicMin::printMinTermList(const list<minTerm>& minTermList) const {
  for(const_minTermListIter I(minTermList.begin());
	  I!=minTermList.end();
	  ++I){
	cerr<<I->term()<<endl;
  }
}
//------------------------------------------;
// printAllMinTermInfo():                   ;
// - self-explaining -                      ;
//------------------------------------------;
void
logicMin::printAllMinTermInfo() const {
  for(const_allMinTermInfoIter I(allMinTermInfoBegin());
	  I!=allMinTermInfoEnd();
	  ++I){
	cerr<<"equalTermS(): "<<I->equalTermS()<<endl;
	cerr<<"int termLen(): "<<I->termLen()<<endl;
	cerr<<"int termIndex(): "<<I->termIndex()<<endl;
  }
}
//------------------------------------------;
// printInfoOfMinTerm():                    ;
// - self-explaining -                      ;
//------------------------------------------;
void
logicMin::printInfoOfMinTerm(unsigned int index) const {
  	cerr<<"equalTermS(): "<<_allMinTermInfo[index].equalTermS()<<endl;
	cerr<<"int termLen(): "<<_allMinTermInfo[index].termLen()<<endl;
	cerr<<"int termIndex(): "<<_allMinTermInfo[index].termIndex()<<endl;
}
//EOF;


