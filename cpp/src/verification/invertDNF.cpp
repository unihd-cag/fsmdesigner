/***********************************************************************
*                                                                      *
* (C) 2008-12, Frank Lemke, Computer Architecture Group,               *
* University of Heidelberg, Germany                                    *
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
*                 lsra_admin@ziti.uni-heidelberg.de        *
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
  overlap = false;
  overlapall = false;
  intersection = true;
  currentMaxNumber = 0;
  
}


string InvertDNF::invert(vector <string> inputDNF) {

  vector <string>::iterator iter;
  vector <string>::iterator ite;
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

  // Substract restTerms from complementDNF to receive complete set of inverted values

  


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

  // Remove intersections


  while (intersection) {

    // Find maximum intersection (ite is maximum)

    intersection = false;
    for(iter=complementDNF.begin(); iter!=complementDNF.end(); iter++){
      for(it=complementDNF.begin(); it!=complementDNF.end(); it++){
        cnt = 0;

        // Calculate the intersection distance

        for(int i=0; i<len; i++) {
          help=(*iter)[i];
          if(help!='-') {
        helpvalue=(*it)[i];
        if(helpvalue!='-') {
           if(help!=helpvalue){
              cnt = 0;
              i = len;
           }
        } else {
           cnt++;
        }
          }
        }

        // There is an intersection with distance > 0
        //cout << "intersection: "<< intersection << endl;
        if(cnt!=0) {
          intersection = true;
        }

        // New maximum distance ?

        if(cnt>currentMaxNumber) {
          currentMaxNumber = cnt;
          ii=iter;
          ite=it;
        }
      }
    }

    // remove maximum intersection and add substitution terms without intersection

    /*for(iter=complementDNF.begin(); iter!=complementDNF.end(); iter++){
    for(int i=0; i<len; i++) {
      cout << (*iter)[i];
       }
       cout << " CDNF" << endl;
    }*/

    if(intersection) {

        for(int i=0; i<len; i++) {
          help=(*ii)[i];
          if(help!='-') {
        helpvalue=(*ite)[i];
        if(helpvalue=='-') {
          curres="";
          for(int z=0; z<len; z++) {
            if(i!=z) {
              curres+=(*ite)[z];
            } else {
              if(help=='0') {
            curres+="1";
              } else {
            curres+="0";
              }
            }
          }
          //cout << "curres " << curres << endl;
          currentResult.push_back(curres);
        }
          }
        }

      complementDNF.erase(ite);
      if(!currentResult.empty()){
      for(ii=currentResult.begin(); ii!=currentResult.end(); ii++){
        complementDNF.push_back(*ii);
      }
    }
    ii=currentResult.begin();
    while(!currentResult.empty()) {
      currentResult.erase(ii);
      ii=currentResult.begin();
    }
      currentMaxNumber = 0;
    }


  }

  // Remove duplicates


  for(iter=complementDNF.begin(); iter!=complementDNF.end(); iter++){
    for(it=complementDNF.begin(); it!=complementDNF.end(); it++){
    if(iter!=it) {

        // Duplicate ?

        overlap = true;
        for(int i=0; i<len; i++) {
          help=(*iter)[i];
          helpvalue=(*it)[i];
        if(helpvalue!=help) {
             overlap = false;
        }
        }

        // Delete duplicate and restart

        if(overlap) {
          complementDNF.erase(iter);
          iter=complementDNF.begin();
          it=complementDNF.begin();
        }
    }
      }
    }


  // Compact code (add don't cares)

  overlapall = true;

  while(overlapall) {
    overlapall = false;
    for(iter=complementDNF.begin(); iter!=complementDNF.end(); iter++){
      for(it=complementDNF.begin(); it!=complementDNF.end(); it++){
        currentBitsSet=0;
        cnt=0;
        for(int i=0; i<len; i++) {
          help=(*iter)[i];
          helpvalue=(*it)[i];
        if(helpvalue!=help) {
          if((help=='0' && helpvalue=='1') || (help=='1' && helpvalue=='0')) {
            if(cnt==0) {
              overlap = true;
              currentBitsSet=i;
            } else {
              overlap = false;
            }
            cnt++;
          } else {
            overlap = false;
            i = len;
          }
        }
        }

        // Create new Value

        if(overlap) {
            curres ="";
            for(int z=0; z<len; z++) {
              if(z==currentBitsSet) {
            curres+="-";
              } else {
            curres+=(*iter)[z];
              }

              }
            //cout << "curres " << curres << endl;
            currentResult.push_back(curres);
          }

        // Delete duplicate and restart

        if(overlap) {
        overlapall = true;
         curres = (*it);
        complementDNF.erase(iter);
        intersection = false;
        for(it=complementDNF.begin(); it!=complementDNF.end(); it++){
          for(int i=0; i<len; i++) {
            help = (*it)[i];
            helpvalue = curres [i];
            if(help==helpvalue) {
              intersection = true;
            } else {
              intersection = false;
              i = len;
            }
          }
          if(intersection) {
              complementDNF.erase(it);
              it=complementDNF.begin();
              intersection = false;
          }
        }
        if(!currentResult.empty()){
          for(ii=currentResult.begin(); ii!=currentResult.end(); ii++){
              complementDNF.push_back(*ii);
          }
        }
        ii=currentResult.begin();
        while(!currentResult.empty()) {
          currentResult.erase(ii);
          ii=currentResult.begin();
        }
        iter=complementDNF.begin();
        it=complementDNF.begin();
        }
      }
    }
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
