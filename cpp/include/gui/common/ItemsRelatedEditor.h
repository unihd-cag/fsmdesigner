/*
 * ItemsRelatedEditor.h
 *
 *  Created on: May 24, 2011
 *      Author: rleys
 */

#ifndef ITEMSRELATEDEDITOR_H_
#define ITEMSRELATEDEDITOR_H_

// Includes
//----------------

//-- Qt
#include <QtGui>
#include <QtCore>

//-- Core
#include <core/FSMDesigner.h>
class StateItem;
class Transline;
class LinkDeparture;
class HyperTransition;

/**
 * This class provies common fields for classes that
 * are to provide edition functions for Items, like the properties table to edit states and so on values
 */
class ItemsRelatedEditor {

protected:

	/// Defines what kind of Item we are editing
	FSMDesigner::Item editingItemType;

	/// Pointer to the currently edited Item
	void * editingItem;

public:
	ItemsRelatedEditor();
	virtual ~ItemsRelatedEditor();

	FSMDesigner::Item getEditingItemType() {
		return editingItemType;
	}

	void * getEditingItem() {
		return this->editingItem;
	}

	/** \defgroup Methods to choose the kind of item to edit */
	/** @{ */

	/// Say we are not editing anything particular. Sets pointer back to NULL
	virtual void editNone();
	virtual void editState(StateItem *);
	virtual void editTransline(Transline*);
	virtual void editLinkDeparture(LinkDeparture*);
	virtual void editHypertrans(HyperTransition*);

	/**@}*/

};

#endif /* ITEMSRELATEDEDITOR_H_ */
