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

//-- Std
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <list>

//-- Core
#include <core/LoadFactory.h>
#include <core/Core.h>
#include <core/Fsm.h>
#include <core/Project.h>

//-- Gen Verilog
#include <genverilog/VerificationPlanGenerator.h>


using namespace std;

void cmdsyntaxerror()
{
	cout<<"Usage: fsmdesignerGenVerilog -p infile -fsm fsm (-vlog outfile)? (-vplan vplanFile)? (-forward delayed|async|sync)? (-i include...)? (-fmap)? "<< endl;
};

int main( int argc, char ** argv ,  char** envp ) {
  string st;
  string s;
  bool c1;
  bool c2;
  bool c3;

  if(argc<7) {
    cmdsyntaxerror();
    return -1;
  } else {

	  // Determine arguments
	  //-----------------------
	  list<QString> includeArgs;
	  string projectFile;
	  string verilogDestination;
	  string vplanDestination;
	  string fsmToGenerate;
	  bool forwardDelayed=false;
	  bool forwardAsync=false;
	  bool forwardState = false;
	  bool generateMap = false;

	  for (int i=1;i<argc;i++) {

		  // Project file
		  if (strcmp(argv[i],"-p")==0) {
			  projectFile.append(argv[++i]);
		  } else if (strcmp(argv[i],"-vlog")==0) {
			  verilogDestination.append(argv[++i]);
		  } else if (strcmp(argv[i],"-vplan")==0) {
			  vplanDestination.append(argv[++i]);
		  } else if (strcmp(argv[i],"-fsm")==0) {
			  fsmToGenerate.append(argv[++i]);
		  } else if (strcmp(argv[i],"-fmap")==0) {
			  generateMap = true;
		  } else if (strcmp(argv[i],"-forward")==0) {
			  char * nextarg = argv[++i];
			  if (strcmp(nextarg,"delayed")==0)
				  forwardDelayed = true;
			  else if (strcmp(nextarg,"async")==0)
				  forwardAsync = true;
			  else if (strcmp(nextarg,"sync")==0)
				  forwardState = true;
		  } else if (strcmp(argv[i],"-i")==0) {

			  //-- Includes, store all following arguments until a '-' start
			  int fi = 0;
			  for (fi = i+1;fi < argc ; fi++) {
				  if (argv[fi][0]=='-') {
					  // Stop
					  break;
				  } else {
					  // Add include to include args
					  includeArgs.push_back(QString(argv[fi]));
				  }
			  }

			  //-- Restart at forward looking -1 to balance the i++ in for definition
			  i = fi-1;
		  }
	  }

	  // Verify all arguments where provided
	  //------------
	  if (projectFile.size()==0 ||
		  fsmToGenerate.size()==0) {
		  cmdsyntaxerror();
		  return -1;
	  }



	  // OK Proceed to generation
	  //----------------------------

	  //-- Load FSMs
	  LoadFactory::getLoad_I().load(projectFile);

	  //-- Find the right one
	  Core& core = *(Core::getInstance());
	  Fsm * fsm = core.getProject()->findFSM(fsmToGenerate);

	  // Not found -> fail
	  if (fsm==NULL) {
		  cerr << "FSM to generate " << fsmToGenerate << " doesn't exist in the project file" << endl;
		  cmdsyntaxerror();
		  return -1;
	  }

	  //-- Generate Verilog
	  //------------
	  if (verilogDestination.size()>0) {

		 /* GenerationOfVerilog genver(fsm,(QWidget *)NULL,true,true,QString(verilogDestination.c_str()));
		  genver.setForwardStateAsync(forwardAsync);
		  genver.setForwardStateDelayed(forwardDelayed);
		  genver.setForwardState(forwardState);

		  //-- Copy environment
		  genver.setEnvp(envp);

		  //-- Includes
		  list<QString>::iterator it;
		  for (it = includeArgs.begin(); it != includeArgs.end(); it++) {
			  genver.addGenerateInclude(*it);
		  }

	  	  genver.generateVerilog(verilogDestination.c_str());

	  	  //-- Generate map if requested
	  	  if (generateMap) {

	  		  // Extract basename (no .v) from filename and then add .f
	  		  stringstream fFile;
	  		  fFile << verilogDestination.substr(0,verilogDestination.size()-2) << ".f";

	  		  genver.generateFFile(fFile.str(),true);
	  	  }*/

	  }

	  //-- Generate Verification Plan if necessary
	  //-------------
	  if (vplanDestination.size()>0) {

		  cout << "Generating VPlan to " << vplanDestination << endl;

		  //-- Prepare output file
		  ofstream vplanStream(vplanDestination.c_str(), ios_base::out | ios_base::trunc);

		  //-- Generate
		  VerificationPlanGenerator generator(fsm);
		  generator.generate(vplanStream);

		  vplanStream.close();

	  }


	  return 0;


  }
}

