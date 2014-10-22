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

//-- Gen VHDL
#include <generate/GeneratorFactory.h>
#include <genvhdl/VHDLGenerator.h>
#include <genverilog/VerificationPlanGenerator.h>
#include <genverilog/SimvisionMmapGenerator.h>

//-- Verify
#include <verification/Verificator.h>
#include <verification/StdoutVerificationListener.h>
#include <verification/StateOutputsRule.h>
#include <verification/StateTransitions.h>
#include <verification/OverlappingTransitionsRule.h>
#include <genverilog/VerilogGeneratorChecks.h>

using namespace std;

void cmdsyntaxerror() {
    cout
            << "Usage: fsmdesignerGenVHDL" << endl
            << " -p infile -fsm fsm (-vhdl outfile)?" << endl
            << " (-fmap)? : Generate a TCL States map for Simvision" << endl
            << " (-force)? : Force generation even if errors occured" << endl
            << " (-noChecks)? : Disable checks" << endl
            ;
}
;

int main(int argc, char ** argv, char** envp) {

    // Global return code
    int returnCode = 0;

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
        string vhdlDestination;
        string vplanDestination;
        string fsmToGenerate;
        bool generateMap = false;
        bool useVerilogGenerator2 = false;
        bool genverilog1RemoveIntersections = false;
        bool errorForce = false;
        bool noChecks = false;

        for (int i = 1; i < argc; i++) {

            // Project file
            if (strcmp(argv[i], "-p") == 0) {
                projectFile.append(argv[++i]);
            } else if (strcmp(argv[i], "-vhdl") == 0) {
                vhdlDestination.append(argv[++i]);
            } else if (strcmp(argv[i], "-vplan") == 0) {
                vplanDestination.append(argv[++i]);
            } else if (strcmp(argv[i], "-fsm") == 0) {
                fsmToGenerate.append(argv[++i]);
            } else if (strcmp(argv[i], "-fmap") == 0) {
                generateMap = true;
            }  else if (strcmp(argv[i], "-force") == 0) {

                errorForce = true;
            } else if (strcmp(argv[i], "-noChecks") == 0) {

                noChecks = true;
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
        GeneratorFactory::getInstance()->registerGenerator("VHDL",
                new VHDLGenerator());
        GeneratorFactory::getInstance()->registerGenerator("Simvision_Mmap",new SimvisionMmapGenerator());
        GeneratorFactory::getInstance()->registerGenerator("VPlan",new VerificationPlanGenerator());

        //-- Generate Verilog
        //------------
        if (vhdlDestination.size() > 0) {

            //-- Create Generator and generate
            Generator * generator = GeneratorFactory::getInstance()->newGenerator("VHDL");
            if (generator == NULL) {
                cerr
                        << "There are no Generator registered under the 'VHDL' name. No VHDL can generated"
                        << endl;
                return -1;
            }

            //-- Set Parameters

            //-- Open File
            QFile vhdlFile(QString::fromStdString(vhdlDestination));
            if (!vhdlFile.open(
                    QFile::Text | QFile::WriteOnly | QIODevice::Truncate)) {

                cerr
                        << "The provided file to generate verilog to cannot be opened for writing"
                        << endl;
                return -1;

            }

            //-- Error Check
            //--------------------------
            Verificator * verificator = new Verificator();
            verificator->addRule(new StateOutputsRule());
            verificator->addRule(new StateTransitions());
            verificator->addRule(new OverlappingTransitionsRule());

            //-- Start
            StdoutVerificationListener verificationListener;
            bool verifyResult = true;
            if (noChecks==false) {
                verifyResult = verificator->verify(fsm,&verificationListener);
            }

            //-- Generate only if no error and not force
            //--------------------------
            if (verifyResult==true || errorForce==true) {
                QDataStream outputStream(&vhdlFile);
                generator->generate(fsm, &outputStream);

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
                    QFile mmapFile(QString::fromStdString(vhdlDestination).replace(".vhdl",".svcf"));
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
                // EOF MMap

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
                // EOF VPlan

            } else {
                cerr << "An error occured while verifying FSM, so nothing was generated" << endl;
                returnCode = -1;
            }

            //-- Close
            delete generator;
            vhdlFile.close();

        }



        return returnCode;

    }
}
