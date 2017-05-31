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
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>

int print_usage()
{
  cout<<endl<<"Logicminimizer. (C)2006 LSRA"<<endl;
  cout<<"------------------------------"<<endl<<endl;
  cout<<"Usage:"<<endl;
  cout<<"logicmin <term0> {<termi>}* "<<endl<<endl;
  cout<<"where term is a string of 1s,0s or -s"<<endl<<endl;
  exit(1);
}

int main(int argc, char** argv)
{
  
  if (argc<2)
    print_usage();


  vector<string> input;
  for (int i=1;i<argc;i++)
    {
      input.push_back(argv[i]);      
//       cout<<"Input "<<i<<" is "<<input[i-1]<<endl;
    }

  //call minimizer
  if (input.size()>0) {
    pair<string, string> result;
    logicMin(input,result);

    
    cout<<"Minimized output: "<<result.first<<endl;
    cout<<"Inverse function: "<<result.second<<endl;
  }
  else
    {
      cout<<"nothing do to"<<endl;
      return -1;
    }
  return 0;
}
