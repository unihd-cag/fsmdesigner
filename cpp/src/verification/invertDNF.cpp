/***********************************************************************
*                                                                      *
* (C) 2008, Frank Lemke, Computer Architecture Group,                  *
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
*                 office@mufasa.informatik.uni-mannheim.de *
*                                                                      *
***********************************************************************/

#include <stdio.h>
#include <cstdlib>
#include <vector>
#include <iterator>
#include <string>
#include <iostream>
#include <sstream>

#include "invertDNF.h"

using namespace std;


InvertDNF::InvertDNF() {
  numberOfInputterms=0;
  currentPosition=0;
  len=0;
  currentBitsSet=0;
  cnt=0;
  mainCTerm="";
  result="";
  currentCALCTerm="";
  
}


string InvertDNF::invert(vector <string> inputDNF) {

  vector <string>::iterator iter;
  vector <string>::iterator it;
  vector <string>::iterator ii;
  

  //Find Term with least bits set to non don't care value and check length of all conjunction terms


  for(iter=inputDNF.begin(); iter!=inputDNF.end(); iter++){
    if(iter==inputDNF.begin()) {
      len=(*iter).length();
      cnt=0;
      for(int i=0; i<len; i++) {
    help=(*iter)[i];
    if(help!='-') {
      cnt++;
    }
      }
      currentBitsSet=cnt;
      mainCTerm=(*iter);
      it=iter;
    } else {
      if(len!=(*iter).length()){
    cerr << "Error: Input vector is invalid!" << endl;
    return "error";
      }
      len=(*iter).length();
      cnt=0;
      for(int i=0; i<len; i++) {
    help=(*iter)[i];
    if(help!='-') {
      cnt++;
    }
      }
      if(cnt<currentBitsSet) {
    currentBitsSet=cnt;
    mainCTerm=(*iter);
    it=iter;
      }
    }
  }

  if(currentBitsSet==0) {
    return "none";
  }

  for(iter=inputDNF.begin(); iter!=inputDNF.end(); iter++){
    if(it!=iter) {
      restTerms.push_back(*iter);
    }
  }

  //cout << "MCTerm: "  << mainCTerm << " and length: " << len << endl;

  // Invert mainCTerms

  
  string CTerm = "";
  for(int i=0; i<len; i++) {
    help=mainCTerm[i];
    if(help!='-') {
      if(help=='0') {
    for(int l=0; l<len ;l++) {
      if(l==i) {
        CTerm+="1";
      } else {
        CTerm+="-";
      }
    }
    complementDNF.push_back(CTerm);
    CTerm="";
      } else {
    for(int l=0; l<len ;l++) {
      if(l==i) {
        CTerm+="0";
      } else {
        CTerm+="-";
      }
    }
    complementDNF.push_back(CTerm);
    CTerm="";
      }
    } 
  }    
  
  /*cout << "inverse:" << endl;
    for(iter=complementDNF.begin(); iter!=complementDNF.end(); iter++){
    cout << (*iter) << endl;
    }
    cout << "rest:" << endl;
    for(it=restTerms.begin(); it!=restTerms.end(); it++){
    cout << (*it) << endl;
    }
  */

  // Substract restTerms from complementDNF to receive

  
  bool overlap;
  bool overlapall;

  for(it=restTerms.begin(); it!=restTerms.end(); it++){
    iter=complementDNF.begin();
    while(iter!=complementDNF.end()) {
      overlap=false;
      overlapall=true;
      for(int i=0; i<len; i++) {
    help=(*it)[i];
    if(help!='-') {
      helpvalue=(*iter)[i];
      if(helpvalue!='-') {
        if(help==helpvalue){
          overlap=true;
        } else {
          overlapall=false;
        }
      } else {
        overlap=true;
      }
    }
      }
      //cout << "CURRENT CALC VALUE: " << (*it) << endl;
      if(overlap & overlapall) {
    //cout << "CURRENT CALC VALUE OVERLAP: " << (*it) << endl;
    //cout << "yes" << (*iter) << endl;
    for(int i=0; i<len; i++) {
      help=(*it)[i];
      if(help!='-') {
        helpvalue=(*iter)[i];
        if(help!=helpvalue) {
          curres="";
          for(int l=0; l<len ;l++) {
        if(l==i) {
          if(help=='0') {
            curres+="1";
          } else {
            curres+="0";
          }
        } else {
          curres+=(*iter)[l];
        }
          }
          //cout << curres << endl;
          currentResult.push_back(curres);
        } else {
          curres+=help;
        }
      }
    }
    //cout << "erase: " << (*iter) << endl;
    complementDNF.erase(iter);
    if(!currentResult.empty()){
      for(ii=currentResult.begin(); ii!=currentResult.end(); ii++){
        //cout << "Daten: "<< (*ii) << endl;
        complementDNF.push_back(*ii);
      }
    }
    ii=currentResult.begin();
    while(!currentResult.empty()) {
      currentResult.erase(ii);
      ii=currentResult.begin();
    }
    //for(ii=complementDNF.begin(); ii!=complementDNF.end(); ii++){
    //  cout << "compDaten: "<< (*ii) << endl;
    //}
    iter=complementDNF.begin();
      } else {
    iter++;
      }
      overlap=false;
      overlapall=true;
    }
    minimizeComplementDNF();
  }

  
  // Return result

 
  result="";
  for(iter=complementDNF.begin(); iter!=complementDNF.end(); iter++){
    if(iter==complementDNF.begin()) {
      result+=(*iter);
    } else {
      result+="+";
      result+=(*iter);
    }
  }

  return result;
 
}

void InvertDNF::minimizeComplementDNF() {
  
  vector <string>::iterator ii;
  pair<string, string> resu;
  
  if (complementDNF.size()>0) {
    logicMin(complementDNF,resu);
    ii=complementDNF.begin();
    while(!complementDNF.empty()) {
      complementDNF.erase(ii);
      ii=complementDNF.begin();
    }
    string minres;
    minres = resu.first;
    //cout << "result: " << minres <<endl;
    cnt = minres.length();
    for(int i=0; i<cnt; i++) {
      curres="";
      for(int l=0; l<len; l++) {
    curres+=minres[i+l];
      }
      i=i+len; //delimiter is added in during loopback
      //cout << "pushback" << curres << endl;
      complementDNF.push_back(curres);
    }
  }
}
