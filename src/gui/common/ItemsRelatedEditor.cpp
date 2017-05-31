/*
 * ItemsRelatedEditor.cpp
 *
 *  Created on: May 24, 2011
 *      Author: rleys
 */

// Includes
//----------------

//-- Std
#include <iostream>
using namespace std;

//-- Qt
#include <QtGui>

//-- Core
#include <core/FSMDesigner.h>
#include <core/State.h>
#include <core/Trans.h>

//-- Gui
#include <gui/items/StateItem.h>
#include <gui/items/LinkDeparture.h>
#include <gui/items/Transline.h>
#include <gui/items/HyperTransition.h>

#include "ItemsRelatedEditor.h"

ItemsRelatedEditor::ItemsRelatedEditor() {
	this->editingItem = NULL;
	this->editingItemType = FSMDesigner::NONE;

}

ItemsRelatedEditor::~ItemsRelatedEditor() {
	// TODO Auto-generated destructor stub
}

void ItemsRelatedEditor::editNone() {
	this->editingItem = NULL;
	this->editingItemType = FSMDesigner::NONE;
}

void ItemsRelatedEditor::editState(StateItem* state) {
	this->editingItem = state;
	this->editingItemType = FSMDesigner::STATE;
}
void ItemsRelatedEditor::editTransline(Transline* transition) {
	this->editingItem = transition;
	this->editingItemType = FSMDesigner::TRANS;
}

void ItemsRelatedEditor::editLinkDeparture(LinkDeparture* link) {
	this->editingItem = link;
	this->editingItemType = FSMDesigner::LINKDEPARTURE;
}
void ItemsRelatedEditor::editHypertrans(HyperTransition* hypertrans) {
	this->editingItem = hypertrans;
	this->editingItemType = FSMDesigner::HYPERTRANS;
}
