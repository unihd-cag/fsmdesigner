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



// Includes
//----------

//-- STD
#include <string>
#include <iostream>
#include <exception>

using namespace std;

//-- QT
#include <QtGui/QMessageBox>
#include <QtCore/QList>

//-- Core
#include <core/Core.h>
#include <core/Fsm.h>
#include <core/LoadFactory.h>
#include <core/Project.h>

//-- Verif
#include <verification/errordetailswindow.h>

#include "verify.h"

// FIXME Check there was really a FSM
Verify::Verify(string filename,QWidget * parent) : QDialog(parent) {

	// Load File
	Core& c = *(Core::getInstance());
	Load_I& l = LoadFactory::getLoad_I();
	l.load(filename);

	if (c.hasFsm())
		this->fsm = c.getProject()->getFSMs().first();
	else
		throw new exception();
	warnings = 0;
	errors = 0;
	errordialog = true;
}

Verify::Verify(Fsm * fsm,QWidget * parent) : QDialog(parent) {

	this->fsm = fsm;
	this->warnings = 0;
	this->errors = 0;
	this->errordialog = true;
}

bool Verify::getVerified() {
	return verified;
}

bool Verify::getVerification() {
	verifyFsm();
	info();
	return verified;
}

void Verify::info() {

	if (errors == 0 && warnings == 0) {
		QMessageBox::question(this, "Verified",
				"FSM successfully passed all tests.", QMessageBox::Ok
						| QMessageBox::Escape, QMessageBox::NoButton,
				QMessageBox::NoButton);
	} else {
		ErrorDetailsWindow* edw;
		edw = new ErrorDetailsWindow(&Errlist);
		edw->exec();
	}
}

bool Verify::wanttoverify() {
	int ver = QMessageBox::question(this, "Verify",
			"Do you want to verify your FSM?", QMessageBox::Yes,
			QMessageBox::Cancel, QMessageBox::NoButton);

	if (ver == QMessageBox::Yes) {
		return true;
	} else {
		return false;
	}
}

void Verify::verifyFsm() {

	bool testpassed = true; //States connected test: PASSED
	bool defaultpassed = true; //One default outgoing Transition per State test: PASSED
	QList<int> defcountList;
	QList<string> stateList;
	int listAt = 0;

	this->verified = true;

/*
	/// States connected ... One default outgoing Transition per State
	if (f.getFirstState()) {
		bool end = false;
		bool start = false;
		int defaultcount = 0;
		listAt = 0;
		do {
			int id = f.getStateID();
			if (f.getFirstTrans()) {
				do {
					if (f.getTransStart() == id) {
						start = true;
					}
					if (f.getTransEnd() == id) {
						end = true;
					}
					if (f.getTransStart() == id && f.getTransDefault()) {
						defaultcount++;
					}
				} while (f.getNextTrans());
			}
			if (end && start && testpassed) {
				testpassed = true; //States connected test: PASSED
			} else {
				errors++;
				testpassed = false; // States connected test: FAILED
			}

			//defcountList[listAt] = defaultcount;
			//stateList[listAt] = f.getStateName();
			defcountList.append(defaultcount);
			stateList.append(f.getStateName());
			listAt++;
			if (defaultcount == 1) { // One default outgoing Transition per State test: PASSED
				defaultcount = 0;
			} else if (defaultcount == 0) {
				defaultpassed = false; // One default outgoing Transition per State test: FAILED
				warnings++;
			} else {
				defaultpassed = false; // One default outgoing Transition per State test: FAILED
				defaultcount = 0;
				warnings++;
			}
		} while (f.getNextState());
	}

	/// States connected

	if (testpassed) {
		//cout << "States connected test: PASSED" << endl;
		if (errordialog) {
			stringstream helpss;
			helpss
					<< "<b><font color='#008000'>States connected test: PASSED</font></b><br/>"
					<< endl;
			Errlist.push_back(QString::fromStdString(helpss.str()));
		}
	} else {
		//cout << "States connected test: FAILED" << endl;
		if (errordialog) {
			stringstream helpss;
			helpss
					<< "<b><font color='#ff0000'>States connected test: FAILED</font></b><br/>"
					<< endl;
			Errlist.push_back(QString::fromStdString(helpss.str()));
		}
	}
	testpassed = true;

	// One default outgoing Transition per State

	if (defaultpassed) {
		//cout << "One default outgoing Transition per State test: PASSED" << endl;
		if (errordialog) {
			stringstream helpss;
			helpss
					<< "<b><font color='#008000'>One default outgoing Transition per State test: PASSED</font></b><br/>"
					<< endl;
			Errlist.push_back(QString::fromStdString(helpss.str()));
		}
	} else {
		//cout << "One default outgoing Transition per State test: FAILED" << endl;
		if (errordialog) {
			stringstream helpss;
			helpss
					<< "<b><font color='#ff0000'>One default outgoing Transition per State test: FAILED</font></b><br/>";
			for (int i = 0; i < defcountList.size(); i++) {
				if (defcountList[i] < 1) {
					helpss
							<< "less than 1 default outgoing Transition at State: "
							<< stateList[i] << "<br/>" << endl;
				} else if (defcountList[i] > 1) {
					helpss
							<< "more than 1 default outgoing Transition at State: "
							<< stateList[i] << "<br/>" << endl;
				}
			}
			Errlist.push_back(QString::fromStdString(helpss.str()));
		}
	}

	/// States consistency

	if (f.getFirstState()) {

		string s1name;
		string s1output;
		string s2name;
		string s2output;
		int curid;

		do {
			curid = f.getStateID();
			s1name = f.getStateName();
			s1output = f.getStateOutput();

			while (f.getNextState()) {
				s2name = f.getStateName();
				s2output = f.getStateOutput();
				if (strcmp(s1name.c_str(), s2name.c_str()) == 0) {
					//cerr << "Consistency error!!! Statename: " << s1name << " is not unique." << endl;
					if (errordialog) {
						stringstream helpss;
						helpss
								<< "<b><font color='#ff0000'>Consistency error!!! Statename: "
								<< s1name << " is not unique.</font></b><br/>"
								<< endl;
						Errlist.push_back(QString::fromStdString(helpss.str()));
					}
					testpassed = false;
					errors++;
				}
				if (strcmp(s1output.c_str(), s2output.c_str()) == 0) {
					//cerr << "Consistency error!!! State: " << s1name << " and State: " << s2name << " have the same output." << endl;
					if (errordialog) {
						stringstream helpss;
						stringstream helpss2;
						helpss
								<< "<b><font color='#ff0000'>Consistency error!!! State: "
								<< s1name << " and State: " << s2name
								<< " have the same output.</font></b><br/>"
								<< endl;
						Errlist.push_back(QString::fromStdString(helpss.str()));
						helpss2
								<< "<b><font color='#ff0000'>               Stateoutput: "
								<< s1output << " and " << s2output
								<< ".</font></b><br/>" << endl;
						Errlist.push_back(QString::fromStdString(helpss2.str()));
					}
					testpassed = false;
					errors++;
				}
				// 	s1name = s2name;
				// 	s1output = s2output;
			}

			f.getStatebyID(curid);
		} while (f.getNextState());
	}

	/// States consistency

	if (testpassed) {
		//cout << "States consistency test: PASSED" << endl;
		if (errordialog) {
			stringstream helpss;
			helpss
					<< "<b><font color='#008000'>States consistency test: PASSED</font></b><br/>"
					<< endl;
			Errlist.push_back(QString::fromStdString(helpss.str()));
		}
	} else {
		//cout << "States consistency test: FAILED" << endl;
		if (errordialog) {
			stringstream helpss;
			helpss
					<< "<b><font color='#ff0000'>States consistency test: FAILED</font></b><br/>"
					<< endl;
			Errlist.push_back(QString::fromStdString(helpss.str()));
		}
	}
	testpassed = true;

	/// Transitions and Hypertransitions are complete.

	if (f.getFirstTrans()) {
		do {
			if (f.getTransStart() == -1 || f.getTransEnd() == -1) {
				testpassed = false;
				//cerr << "Transition " << f.getTransName() << " is incomplete."<< endl;
				if (errordialog) {
					stringstream helpss;
					helpss << "<b><font color='#ff0000'>Transition "
							<< f.getTransName()
							<< " is incomplete.</font></b><br/>" << endl;
					Errlist.push_back(QString::fromStdString(helpss.str()));
				}
				errors++;
			} else {
				if (!f.getTransDefault()) {
					if (!f.getTransFirstCondition()) {
						testpassed = false;
						//cerr << "Transition " << f.getTransName() << " is incomplete."<< endl;
						if (errordialog) {
							stringstream helpss;
							helpss << "<b><font color='#ff0000'>Transition "
									<< f.getTransName()
									<< " is incomplete.</font></b><br/>"
									<< endl;
							Errlist.push_back(QString::fromStdString(
									helpss.str()));
						}
						errors++;
					}
				}
			}
		} while (f.getNextTrans());
	}
	if (f.getFirstHypertrans()) {
		do {
			if (!f.getHypertransFirstCondition()) {
				testpassed = false;
				//cerr << "Hypertransition " << f.getHypertransID() << " is incomplete."<< endl;
				if (errordialog) {
					stringstream helpss;
					helpss << "<b><font color='#ff0000'>Hypertransition "
							<< f.getHypertransID()
							<< " is incomplete.</font></b><br/>" << endl;
					Errlist.push_back(QString::fromStdString(helpss.str()));
				}
				errors++;
			}
		} while (f.getNextHypertrans());
	}

	if (testpassed) {

		/// Transitions and Hypertransitions are complete

		//cout << "Transitions and Hypertransitions complete test: PASSED" << endl;
		if (errordialog) {
			stringstream helpss;
			helpss
					<< "<b><font color='#008000'>Transitions and Hypertransitions complete test: PASSED</font></b><br/>"
					<< endl;
			Errlist.push_back(QString::fromStdString(helpss.str()));
		}
		/// Transitions and Hypertransitions consistency

		if (f.getFirstTrans()) {

			int numberofinputs = f.getNumberOfInputs();
			string compare;
			string base;
			int basestart;
			int comparestart;
			int id;
			int compareid;
			string name;
			string comparename;

			do {
				if (!f.getTransDefault()) {
					id = f.getTransID();
					name = f.getTransName();
					if (f.getTransFirstCondition()) {
						basestart = f.getTransStart();
						do {
							base = f.getTransConditionValue();
							while (f.getNextTrans()) {
								comparestart = f.getTransStart();
								if (comparestart == basestart) {
									if (!f.getTransDefault()) {
										compareid = f.getTransID();
										comparename = f.getTransName();
										if (f.getTransFirstCondition()) {
											do {
												compare
														= f.getTransConditionValue();
												if (strcmp(base.c_str(),
														compare.c_str()) == 0) {
													testpassed = false;
													//cerr << "Consistency error!!! Between Transitions " << name << " and " << comparename  << "." << endl;
													if (errordialog) {
														stringstream helpss;
														stringstream helpss2;
														helpss
																<< "<b><font color='#ff0000'>Consistency error!!! Between Transitions: "
																<< name
																<< " and "
																<< comparename
																<< ".</font></b><br/>"
																<< endl;
														Errlist.push_back(
																QString::fromStdString(
																		helpss.str()));
														helpss2
																<< "<b><font color='#ff0000'>                             Inputvector: "
																<< base
																<< " and "
																<< compare
																<< ".</font></b><br/>"
																<< endl;
														Errlist.push_back(
																QString::fromStdString(
																		helpss2.str()));
													}
													errors++;
												} else {
													bool equal = true;
													char value;
													char valuecompare;
													for (int i = 0; i
															< numberofinputs; i++) {
														if (equal) {
															value = base[i];
															if (!((value == 'x')
																	|| (value
																			== 'X')
																	|| (value
																			== '-'))) {
																valuecompare
																		= compare[i];
																if (!((valuecompare
																		== 'x')
																		|| (valuecompare
																				== 'X')
																		|| (valuecompare
																				== '-'))) {

																	if (value
																			!= valuecompare) {
																		equal
																				= false;
																	}
																}
															}
														} else {
															i = numberofinputs;
														}
													}
													if (equal) {
														testpassed = false;
														//cerr << "Consistency error!!! Between Transitions " << name << " and " << comparename  << "." << endl;
														if (errordialog) {
															stringstream helpss;
															stringstream
																	helpss2;
															helpss
																	<< "<b><font color='#ff0000'>Consistency error!!! Between Transitions: "
																	<< name
																	<< " and "
																	<< comparename
																	<< ".</font></b><br/>"
																	<< endl;
															Errlist.push_back(
																	QString::fromStdString(
																			helpss.str()));
															helpss2
																	<< "<b><font color='#ff0000'>                             Inputvector: "
																	<< base
																	<< " and "
																	<< compare
																	<< ".</font></b><br/>"
																	<< endl;
															Errlist.push_back(
																	QString::fromStdString(
																			helpss2.str()));
														}
														errors++;
													}
												}
											} while (f.getTransNextCondition());
										}
									}
								}
							}
							if (f.getFirstHypertrans()) {
								do {
									compareid = f.getHypertransID();
									if (f.getHypertransFirstCondition()) {
										do {
											compare
													= f.getHypertransConditionValue();
											if (strcmp(base.c_str(),
													compare.c_str()) == 0) {
												testpassed = false;
												//cerr << "Consistency error!!! Between Transition " << name << " and Hypertransition " << compareid  << "." << endl;
												if (errordialog) {
													stringstream helpss;
													stringstream helpss2;
													helpss
															<< "<b><font color='#ff0000'>Consistency error!!! Between Transition: "
															<< name
															<< " and Hypertransition: "
															<< compareid
															<< ".</font></b><br/>"
															<< endl;
													Errlist.push_back(
															QString::fromStdString(
																	helpss.str()));
													helpss2
															<< "<b><font color='#ff0000'>                            Inputvector: "
															<< base << " and "
															<< compare
															<< ".</font></b><br/>"
															<< endl;
													Errlist.push_back(
															QString::fromStdString(
																	helpss2.str()));
												}
												errors++;
											} else {
												bool equal = true;
												char value;
												char valuecompare;
												for (int i = 0; i
														< numberofinputs; i++) {
													if (equal) {
														value = base[i];
														if (!((value == 'x')
																|| (value
																		== 'X')
																|| (value
																		== '-'))) {
															valuecompare
																	= compare[i];
															if (!((valuecompare
																	== 'x')
																	|| (valuecompare
																			== 'X')
																	|| (valuecompare
																			== '-'))) {
																if (value
																		!= valuecompare) {
																	equal
																			= false;
																}
															}
														}
													} else {
														i = numberofinputs;
													}
												}
												if (equal) {
													testpassed = false;
													//cerr << "Consistency error!!! Between Transition " << name << " and Hypertransition " << compareid  << "." << endl;
													if (errordialog) {
														stringstream helpss;
														stringstream helpss2;
														helpss
																<< "<b><font color='#ff0000'>Consistency error!!! Between Transition: "
																<< name
																<< " and Hypertransition: "
																<< compareid
																<< ".</font></b><br/>"
																<< endl;
														Errlist.push_back(
																QString::fromStdString(
																		helpss.str()));
														helpss2
																<< "<b><font color='#ff0000'>                            Inputvector: "
																<< base
																<< " and "
																<< compare
																<< ".</font></b><br/>"
																<< endl;
														Errlist.push_back(
																QString::fromStdString(
																		helpss2.str()));
													}
													errors++;
												}
											}
										} while (f.getHypertransNextCondition());
									}
								} while (f.getNextHypertrans());
							}
							f.getTransbyID(id);
						} while (f.getTransNextCondition());
					}
				}
			} while (f.getNextTrans());
		}

		if (f.getFirstHypertrans()) {

			int numberofinputs = f.getNumberOfInputs();
			string compare;
			string base;
			int id;
			int compareid;

			do {
				id = f.getHypertransID();
				if (f.getHypertransFirstCondition()) {
					do {
						base = f.getHypertransConditionValue();
						while (f.getNextHypertrans()) {
							if (f.getHypertransFirstCondition()) {
								compareid = f.getHypertransID();
								do {
									compare = f.getHypertransConditionValue();
									if (strcmp(base.c_str(), compare.c_str())
											== 0) {
										testpassed = false;
										//cerr << "Consistency error!!! Between Hypertransitions " << id << " and " << compareid  << "." << endl;
										if (errordialog) {
											stringstream helpss;
											stringstream helpss2;
											helpss
													<< "<b><font color='#ff0000'>Consistency error!!! Between Hypertransitions: "
													<< id << " and "
													<< compareid
													<< ".</font></b><br/>"
													<< endl;
											Errlist.push_back(
													QString::fromStdString(
															helpss.str()));
											helpss2
													<< "<b><font color='#ff0000'>                                  Inputvector: "
													<< base << " and "
													<< compare
													<< ".</font></b><br/>"
													<< endl;
											Errlist.push_back(
													QString::fromStdString(
															helpss2.str()));
										}
										errors++;
									} else {
										bool equal = true;
										char value;
										char valuecompare;
										for (int i = 0; i < numberofinputs; i++) {
											if (equal) {
												value = base[i];
												if (!((value == 'x') || (value
														== 'X') || (value
														== '-'))) {
													valuecompare = compare[i];
													if (!((valuecompare == 'x')
															|| (valuecompare
																	== 'X')
															|| (valuecompare
																	== '-'))) {
														if (value
																!= valuecompare) {
															equal = false;
														}
													}
												}
											} else {
												i = numberofinputs;
											}
										}
										if (equal) {
											testpassed = false;
											//cerr << "Consistency error!!! Between Hypertransitions " << id << " and " << compareid  << "." << endl;
											if (errordialog) {
												stringstream helpss;
												stringstream helpss2;
												helpss
														<< "<b><font color='#ff0000'>Consistency error!!! Between Hypertransitions: "
														<< id << " and "
														<< compareid
														<< ".</font></b><br/>"
														<< endl;
												Errlist.push_back(
														QString::fromStdString(
																helpss.str()));
												helpss2
														<< "<b><font color='#ff0000'>                                  Inputvector: "
														<< base << " and "
														<< compare
														<< ".</font></b><br/>"
														<< endl;
												Errlist.push_back(
														QString::fromStdString(
																helpss2.str()));
											}
											errors++;
										}
									}
								} while (f.getHypertransNextCondition());
							}
						}
						f.getHypertransbyID(id);
					} while (f.getHypertransNextCondition());
				}
			} while (f.getNextHypertrans());
		}

		/// Transitions and Hypertransitions consistency

		if (testpassed) {
			//cout << "Transitions and Hypertransitions consistency test: PASSED" << endl;
			if (errordialog) {
				stringstream helpss;
				helpss
						<< "<b><font color='#008000'>Transitions and Hypertransitions consistency test: PASSED</font></b><br/>"
						<< endl;
				Errlist.push_back(QString::fromStdString(helpss.str()));
			}
		} else {
			//cout << "Transitions and Hypertransitions consistency test: FAILED" << endl;
			if (errordialog) {
				stringstream helpss;
				helpss
						<< "<b><font color='#ff0000'>Transitions and Hypertransitions consistency test: FAILED</font></b><br/>"
						<< endl;
				Errlist.push_back(QString::fromStdString(helpss.str()));
			}
		}
		testpassed = true;

	} else {

		/// Transitions and Hypertransitions are complete

		//cout << "Transitions and Hypertransitions complete test: FAILED" << endl;
		if (errordialog) {
			stringstream helpss;
			helpss
					<< "<b><font color='#ff0000'>Transitions and Hypertransitions complete test: FAILED</font></b><br/>"
					<< endl;
			Errlist.push_back(QString::fromStdString(helpss.str()));
		}
		//cout << "Transitions and Hypertransitions consistency test: SKIPPED" << endl;
		if (errordialog) {
			stringstream helpss;
			helpss
					<< "<b><font color='#ff0000'>Transitions and Hypertransitions consistency test: SKIPPED</font></b><br/>"
					<< endl;
			Errlist.push_back(QString::fromStdString(helpss.str()));
		}
	}
	testpassed = true;

	if (warnings == 0 && errors == 0) {
		verified = true;
	} else {
		if (errordialog) {
			stringstream helpss;
			stringstream helpss2;
			helpss << "<b>Warnings:" << warnings << "</b><br/>" << endl;
			Errlist.push_back(QString::fromStdString(helpss.str()));
			helpss2 << "<b>Errors:" << errors << "</b><br/>" << endl;
			Errlist.push_back(QString::fromStdString(helpss2.str()));
		}
		//cout << "Warnings:" << warnings << endl;
		//cout << "Errors:" << errors << endl;
		verified = false;
	}*/
}
