#ifndef RENAMEINPUT_H_
#define RENAMEINPUT_H_

// Includes
//----------------------

//-- Std
#include <string>
#include <iostream>
using namespace std;

//-- Qt
#include <QtGui>
#include <QtCore>

//-- Gui
#include <gui/scene/SceneRelatedObject.h>

//-- Ui
#include <ui_inputname.h>


class RenameInput : public QDialog, public Ui_inputname, public SceneRelatedObject {

protected:
	  int index;

public:

  /**
   *
   * @param fsm
   * @param inputIndex Index of the input to rename
   * @param parent
   */
  RenameInput(Scene * scene,int inputIndex,QWidget* parent = NULL);

  void accept();


};

#endif
