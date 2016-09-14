/*
 * ConditionWidget.h
 *
 *  Created on: Nov 24, 2011
 *      Author: rleys
 */

#ifndef CONDITIONWIDGET_H_
#define CONDITIONWIDGET_H_

// Includes
//---------------------

//-- Qt
#include <QtGui>
#include <QtCore>

//-- GUI
#include <gui/scene/SceneRelatedObject.h>
class Scene;

#include <gui/input/inputwidget.h>

class Transline;

/**
 * Edits the conditions of a transition
 *
 * <combobox>Various conditions</combobox>
 * <inputwidget/>
 *
 */
class ConditionWidget : public QWidget , public SceneRelatedObject {

	Q_OBJECT

protected:

	/// Condition Selector
	QComboBox	comboBox;

	/// Input widget
	InputWidget	inputwidget;

	/// The actual edited transition
	Transline * currentTransition;

public:
	ConditionWidget(QWidget * parent = NULL);
	virtual ~ConditionWidget();



public Q_SLOTS:

	/// If scene selection changed, and it is a transition, update conditions and so
	void sceneSelectionChanged();

	/**
	 * If the combo box selection changed
	 * @param index The new selected index of -1 if empty
	 */
	void conditionSelected(int index);

	/**
     * Sets the related scene and propagates to sub components
     * @param
     */
    virtual void setRelatedScene(Scene *);


};

#endif /* CONDITIONWIDGET_H_ */
