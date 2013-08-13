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
#include <generate/GeneratorFactory.h>
#include <genverilog/VerificationPlanGenerator.h>
#include <genverilog/VerilogGenerator.h>
#include <genverilog/VerilogSecuredGenerator.h>
#include <genverilog/VerilogGenerator2.h>
#include <genverilog/SimvisionMmapGenerator.h>

using namespace std;

void cmdsyntaxerror() {
    cout
            << "Usage: fsmdesignerGenVerilog"
            << " -p infile -fsm fsm (-vlog outfile)?"
            << " (-vplan vplanFile)?"
            << " (-forward delayed|async|sync)?"
            << " (-i include...)? (-fmap)? "
            << " (-v2)?"
            << " (-v1RemoveIntersections)"
            << " (-secured)?" 
            << " (-tmr)?"
            << " (-hamming)?"
            << endl;
}
;

int main(int argc, char ** argv, char** envp) {
    string st;
    string s;
    bool c1;
    bool c2;
    bool c3;

    if (argc < 7) {
        cmdsyntaxerror();
        return -1;
    } else {

        // Determine arguments
        //-----------------------
        QStringList includeArgs;
        string projectFile;
        string verilogDestination;
        string vplanDestination;
        string fsmToGenerate;
        bool forwardDelayed = false;
        bool forwardAsync = false;
        bool forwardState = false;
        bool generateMap = false;
        bool useVerilogGenerator2 = false;
        bool useVerilogSecuredGenerator = false;
        bool useVerilogSecuredGeneratorTMR = false;
        bool useVerilogSecuredGeneratorHamming = false;
        bool genverilog1RemoveIntersections = false;

        for (int i = 1; i < argc; i++) {

            // Project file
            if (strcmp(argv[i], "-p") == 0) {
                projectFile.append(argv[++i]);
            } else if (strcmp(argv[i], "-vlog") == 0) {
                verilogDestination.append(argv[++i]);
            } else if (strcmp(argv[i], "-vplan") == 0) {
                vplanDestination.append(argv[++i]);
            } else if (strcmp(argv[i], "-fsm") == 0) {
                fsmToGenerate.append(argv[++i]);
            } else if (strcmp(argv[i], "-fmap") == 0) {
                generateMap = true;
            } else if (strcmp(argv[i], "-forward") == 0) {
                char * nextarg = argv[++i];
                if (strcmp(nextarg, "delayed") == 0)
                    forwardDelayed = true;
                else if (strcmp(nextarg, "async") == 0)
                    forwardAsync = true;
                else if (strcmp(nextarg, "sync") == 0)
                    forwardState = true;
            } else if (strcmp(argv[i], "-i") == 0) {

                QString nextarg(argv[++i]);

                //-- Split
                includeArgs = nextarg.split(' ');

                //-- Includes, store all following arguments until a '-' start
                /*int fi = 0;
                for (fi = i + 1; fi < argc; fi++) {
                    if (argv[fi][0] == '-') {
                        // Stop
                        break;
                    } else {
                        // Add include to include args
                        includeArgs.push_back(QString(argv[fi]));
                    }
                }*/

                //-- Restart at forward looking -1 to balance the i++ in for definition
                //i = fi - 1;
            } else if (strcmp(argv[i], "-v2") == 0) {

                useVerilogGenerator2 = true;

            } else if (strcmp(argv[i], "-v1RemoveIntersections") == 0) {

                genverilog1RemoveIntersections = true;
            } else if (strcmp(argv[i], "-secured") == 0) {

                useVerilogSecuredGenerator = true;

            } else if (strcmp(argv[i], "-tmr") == 0) {

                useVerilogSecuredGeneratorTMR = true;

            } else if (strcmp(argv[i], "-hamming") == 0) {

                useVerilogSecuredGeneratorHamming = true;

            }

        }

        // Verify all arguments where provided
        //------------
        if (projectFile.size() == 0 || fsmToGenerate.size() == 0) {
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
        if (fsm == NULL) {
            cerr << "FSM to generate " << fsmToGenerate
                    << " doesn't exist in the project file" << endl;
            cmdsyntaxerror();
            return -1;
        }

        //-- Register Verilog Generator
        GeneratorFactory::getInstance()->registerGenerator("Verilog",
                new VerilogGenerator());

        // BB
        GeneratorFactory::getInstance()->registerGenerator("VerilogSecured",
                        new VerilogSecuredGenerator());

        GeneratorFactory::getInstance()->registerGenerator("Verilog2",
                        new VerilogGenerator2());
        GeneratorFactory::getInstance()->registerGenerator("Simvision_Mmap",new SimvisionMmapGenerator());
        GeneratorFactory::getInstance()->registerGenerator("VPlan",new VerificationPlanGenerator());

        //-- Generate Verilog
        //------------
        if (verilogDestination.size() > 0) {

        	// BB: Abfrage einfügen, wenn Commandline-Parameter "Secured", dann
        	// anderen Generator erstellen
            //-- Create Generator and generate
            Generator * generator = NULL;

            if (useVerilogSecuredGenerator) {
            	generator = GeneratorFactory::getInstance()->newGenerator("VerilogSecured");
            } else if (useVerilogGenerator2) {
            	generator = GeneratorFactory::getInstance()->newGenerator("Verilog2");
            } else {
            	generator = GeneratorFactory::getInstance()->newGenerator("Verilog");
            }

            if (generator == NULL) {
                cerr
                        << "There are no Generator registered under the 'Verilog' name. No Verilog can generated"
                        << endl;
                return -1;
            }

            //-- Set Parameters
            generator->setParameter("forward.delayed", forwardDelayed);
            generator->setParameter("forward.async", forwardAsync);
            generator->setParameter("forward.sync", forwardState);
            generator->setParameter("includes", includeArgs);

            if (useVerilogSecuredGenerator) {
                generator->setParameter("TMR", useVerilogSecuredGeneratorTMR);
                generator->setParameter("Hamming", useVerilogSecuredGeneratorHamming);
            }

            // v1 Parameters
            if (!useVerilogGenerator2) {
                generator->setParameter("removeIntersections", genverilog1RemoveIntersections);

            }

            //-- Open File
            QFile verilogFile(QString::fromStdString(verilogDestination));
            if (!verilogFile.open(
                    QFile::Text | QFile::WriteOnly | QIODevice::Truncate)) {

                cerr
                        << "The provided file to generate verilog to cannot be opened for writing"
                        << endl;
                return -1;

            }

            //-- Generate
            QDataStream outputStream(&verilogFile);
            generator->generate(fsm, &outputStream);

            //-- Close
            delete generator;
            verilogFile.close();

            //---- Generate MMap If necessary
            //-----------------------
            if (generateMap == true) {

                //-- Create Generator and generate
                Generator * mmapGenerator =
                        GeneratorFactory::getInstance()->newGenerator(
                                "Simvision_Mmap");
                if (mmapGenerator == NULL) {
                    cerr
                            << "There are no Generator registered under the 'Simvision_Mmap' name. No Simvision Mmap can generated"
                            << endl;
                    return -1;
                }

                //-- Open File
                QFile mmapFile(QString::fromStdString(verilogDestination).replace(".v",".svcf"));
                if (!mmapFile.open(
                        QFile::Text | QFile::WriteOnly | QIODevice::Truncate)) {

                    cerr
                            << "The provided file to generate a simvision mmap to cannot be opened for writing"
                            << endl;
                    return -1;

                }

                //-- Generate
                QDataStream mmapOutputStream(&mmapFile);
                mmapGenerator->generate(fsm, &mmapOutputStream);

                //-- Close
                delete mmapGenerator;
                mmapFile.close();

            }

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
        if (vplanDestination.size() > 0) {

            cout << "Generating VPlan to " << vplanDestination << endl;

            //-- Generate
            Generator * vplanGenerator =
                                    GeneratorFactory::getInstance()->newGenerator(
                                            "VPlan");

            //-- Open File
            QFile vplanFile(QString::fromStdString(vplanDestination.c_str()));
            if (!vplanFile.open(
                    QFile::Text | QFile::WriteOnly | QIODevice::Truncate)) {
                cerr
                        << "The provided file to generate a vplan to cannot be opened for writing"
                        << endl;
                return -1;

            }

            //-- Generate
            QDataStream vplanOutputStream(&vplanFile);
            vplanGenerator->generate(fsm, &vplanOutputStream);

            //-- Close
            delete vplanGenerator;
            vplanFile.close();

        }

        return 0;

    }
}

