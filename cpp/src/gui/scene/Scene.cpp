// Includes
//---------------

//-- Std
#include <iostream>
using namespace std;

//-- Items
#include <gui/items/FSMGraphicsItem.h>
#include <gui/items/StateItem.h>
#include <gui/items/TrackpointItem.h>
#include <gui/items/Transline.h>
#include <gui/items/TranslineText.h>
#include <gui/items/LinkArrival.h>
#include <gui/items/LinkDeparture.h>
#include <gui/items/JoinItem.h>
#include <gui/items/HyperTransition.h>

//-- Undo
#include <gui/actions/DeleteJoinAction.h>
#include <gui/actions/DeleteLinkArrivalAction.h>
#include <gui/actions/DeleteTransitionAction.h>
#include <gui/actions/DeleteHyperTransitionAction.h>

//-- Others
#include <gui/common/GUIUtils.h>

//-- Verification
#include <gui/verify/FSMVerificator.h>
#include <verification/Verificator.h>
#include <verification/StateOutputsRule.h>
#include <verification/VerificatorRule.h>

// -- Qt
#include <QtGui>
#include <QtScript/QScriptEngine>
#include <QtCore>

//-- Core
#include <core/FSMDesigner.h>
#include <core/Fsm.h>
#include <core/Core.h>
#include <core/State.h>
#include <core/Link.h>
#include <core/Join.h>
#include <core/Trackpoint.h>
#include <core/Trans.h>

using namespace FSMDesigner;

#include "Scene.h"
Scene::Scene(Fsm * fsm, QObject *parent) :
	QGraphicsScene(-5000,-5000,10000,10000,parent) {

	//this->setBackgroundBrush(Qt::blue);
	//this->setBackgroundBrush(QPixmap());
	//this->setStickyFocus(true);

	// Variables
	//-------------
	this->fsm = fsm;

	//-- Place
	this->placeMode = FSMDesigner::NONE;
	this->placeLock = FSMDesigner::UNLOCKED;

	// Verification
	//---------------
	this->setFSMVerificator(new FSMVerificator(this));

}

void Scene::initializeScene() {

	// Clean scene
	//----------
	this->clearSelection();
	this->clear();
	//	QList <QGraphicsItem *> cil = this->items()  ;
	//	while (!cil.isEmpty()) {
	//		QGraphicsItem* i = cil.takeFirst();
	//		this->removeItem(i);
	//		delete i;
	//	}


	// Draw FSM
	//------------------

	//-- Get current FSM to draw
	Fsm * currentFSM =this->fsm;

	// Prepare some datas
	//------------------------------

	//-- Created items map (id<->object), and Join map
	map<int, StateItem*> itemsMap;
	map<int,JoinItem*> joinsMap;



	// Place Joins
	//--------------------
	FOREACH_JOIN(this->fsm)

		//-- Prepare Item
		JoinItem * joinItem = new JoinItem((*it).second);
		joinsMap[(*it).first] = joinItem;

		//-- Use Undo to place on the scene
		DeleteJoinAction * undoJoin = new DeleteJoinAction(joinItem, NULL);

		//-- If target state is defined, set transition and trackpoints to it
		//----------------------------
		if (joinItem->getModel()->getTargetState()!=NULL) {

			//-- Target State
			State * endState = joinItem->getModel()->getTargetState();
			StateItem * endStateItem = itemsMap[endState->getId()];
			if (endStateItem == NULL) {
				endStateItem = new StateItem(endState);
				itemsMap[endState->getId()] = endStateItem;
				this->addItem(endStateItem);
			}

			//-- Use Undo to place on the scene
			//undoJoin->setEndItem(endStateItem);

		} else {
		    qDebug() << "No target state onto join";
		}

		//-- place
		undoJoin->setRelatedScene(this);
		undoJoin->undo();

	END_FOREACH_JOIN

	// Loop over states until no more
	//-------------------
	FOREACH_STATE(currentFSM)

		// Get Current State
		State * currentState = state;
		if (currentState == NULL)
			continue;

		//-- Create a GUI state if necessary
		StateItem * stateItem = itemsMap.find(currentState->getId())!=itemsMap.end() ? itemsMap[currentState->getId()] : NULL;
		if (stateItem == NULL) {
			stateItem = new StateItem(currentState);
			itemsMap[currentState->getId()] = stateItem;
			this->addItem(stateItem);
		}

		// Create transitions from this item
		//-----------
		Transline * lastTransitionLine = NULL;
		list<Trans*> stateStartingTransitions =
				currentState->getStartingTransitions();
		list<Trans*>::iterator i;
		for (i = stateStartingTransitions.begin(); i
				!= stateStartingTransitions.end(); i++) {

			// Get Trans
			Trans * transition = *i;
			lastTransitionLine = NULL;

			//-- Calculate the start and end as a trackpoints

			// Start
			Trackpoint * start = new Trackpoint(currentState->getPosition().first,
					currentState->getPosition().second, 0);

			// End
			/*currentFSM->getStatebyID(transition->end);
			State * endState = currentFSM->getCurrentState();
			currentFSM->getStatebyID(currentState->sid);*/
			State * endState = transition->getEndState();

			//-- Find/create end Gui item
			StateItem * endStateItem = itemsMap.find(endState->getId())!=itemsMap.end() ? itemsMap[endState->getId()] : NULL;
			if (endStateItem == NULL) {
				endStateItem = new StateItem(endState);
				itemsMap[endState->getId()] = endStateItem;
				this->addItem(endStateItem);
			}

			//-- Use Undo to place on the scene
			Transline * lineToAdd = new Transline((TransitionBase*)transition);
			DeleteTransitionAction * undoTransition = new DeleteTransitionAction(lineToAdd);
			//undoTransition->setStartItem(stateItem);
			//undoTransition->setEndItem(endStateItem);
			undoTransition->setRelatedScene(this);
			undoTransition->undo();
			delete undoTransition;
			delete lineToAdd;

		} // ENd OF transitions loop


	END_FOREACH_STATE
	//-- END LOOP OVER STATES

	// Loop over links entrances
	//----------------------------
	FOREACH_LINKS(currentFSM)

        //-- Use Undo Class to place
        LinkArrival * linkArrival = new LinkArrival(link,itemsMap[link->getTargetState()->getId()]);
        DeleteLinkArrivalAction * undoLinkArrival = new DeleteLinkArrivalAction(linkArrival);
        undoLinkArrival->setRelatedScene(this);
        undoLinkArrival->undo();

	END_FOREACH_LINKS

	// Place Hypertransitions
    //---------------------------
    FOREACH_HYPERTRANSITIONS(this->fsm)

        //-- Prepare Item
        HyperTransition * hypertransitionItem = new HyperTransition(hypertransition);

        //-- Use undo to place on the scene
        DeleteHyperTransitionAction * undoHypertransition = new DeleteHyperTransitionAction(hypertransitionItem);
        undoHypertransition->setRelatedScene(this);
        undoHypertransition->undo();
        delete undoHypertransition;

    END_FOREACH_HYPERTRANSITIONS

    this->placeMode = FSMDesigner::CHOOSE;

}

QUndoStack * Scene::getUndoStack() {
	return &(this->undoStack);
}

/**
 * Propagate Undo Redo
 */
void Scene::undo() {
    this->setPlaceMode(FSMDesigner::NONE);
    this->undoStack.undo();
    this->setPlaceMode(FSMDesigner::CHOOSE);
}

/**
 * Propagate Undo Redo
 */
void Scene::redo() {
    this->setPlaceMode(FSMDesigner::NONE);
    this->undoStack.redo();
    this->setPlaceMode(FSMDesigner::CHOOSE);
}

void Scene::addToToPlaceStack(QGraphicsItem * item) {

	//-- If the stack is not empty,
	//---------------------------

	//-- Apply rules to items

	// Not visible and not enabled
	this->addItem(item);
	item->setVisible(false);
	item->setEnabled(false);
	//item->setSelected(true);


	//-- Add to stack
	this->toPlaceStack.push_front(item);
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *e) {


	QGraphicsScene::mousePressEvent(e);



	//-- First element has to be placed, removed from stack and reenabled
	if (this->toPlaceStack.size() > 0) {

		qDebug() << "Mouse Press, releasing to be placed element:"
				<< e->scenePos();

		//-- If the first is a transline, we don't really want to place it
		//-- Invisible elements are not yet to be considered. They wil be made visible after first movement
		if (!this->toPlaceStack.first()->isVisible()) {

		} else if (this->toPlaceStack.first()->type() == Transline::Type) {

			QGraphicsItem * tr = this->toPlaceStack.takeFirst();

			/*
			//-- Delete only if the transline is not from a trackpoint
			if (!FSMGraphicsItem<>::isTrackPoint(
					FSMGraphicsItem<>::toTransline(tr)->getStartItem())) {
				this->removeItem(tr);
				delete tr;
			}*/

			//this->toPlaceStack.first()->setVisible(false);


			//-- only ensure the line is enabled
			tr->setEnabled(true);


			//-- If the transline does not lead to anywhere, and don't be long to a trackpoint delete
			if (FSMGraphicsItem<>::toTransline(tr)->getEndItem()==NULL &&
					!FSMGraphicsItem<>::isTrackPoint(FSMGraphicsItem<>::toTransline(tr)->getStartItem() )) {
				delete tr;

			}


		} else {

			// Get and remove
			QGraphicsItem * first = this->toPlaceStack.takeFirst();

			// Reenable
			first->setEnabled(true);

			// Position
			this->placeUnderMouse(first, e->scenePos());

		}

	}


}

void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent* e) {

	// Placing if not in choose mode
	//------------------
	if (this->placeMode != CHOOSE) {

		// Do action depending on place mode
		switch (this->placeMode) {

		// State
		//----------------------
		case STATE: {

			//-- Get Item under
			QList<QGraphicsItem*> itemsUnder = this->items(e->scenePos(),
					Qt::IntersectsItemShape, Qt::AscendingOrder);
			QGraphicsItem* itemUnder =
					itemsUnder.size() > 0 ? itemsUnder.front() : NULL;

			//-- Create a new State
			State * state = this->fsm->addState();

			//-- Create new Item or take the one on the stack
			StateItem * stateItem = NULL;
			if (itemUnder != NULL && itemUnder->type() == StateItem::Type) {

				qDebug() << "Adding state from placed element " << QString::fromStdString(state->getName());
				stateItem = dynamic_cast<StateItem*> (itemUnder);
				stateItem->setModel(state);

			} else if(this->selectedItems().size()>0 && this->selectedItems().first()->type()== StateItem::Type) {
				stateItem = dynamic_cast<StateItem*> (this->selectedItems().first());
				stateItem->setModel(state);
			}

			else {
				qDebug() << "Adding new state";
				stateItem = new StateItem(state);
				this->addItem(stateItem);
			}

			//-- Place centered on mouse
			state->setPosition(pair<double,double>(e->scenePos().x(),e->scenePos().y()));
			stateItem->setPos(e->scenePos().x(),e->scenePos().y());

			// Close interaction
			this->toPlaceStack.clear();
			this->setPlaceMode(CHOOSE);

		}
			break;
			//-- END OF State -------//

        // Transition
        //--------------------------
		case TRANS: {

			//-- Get Item under
			QList<QGraphicsItem*> itemsUnder = this->items(e->scenePos(),
					Qt::IntersectsItemShape, Qt::AscendingOrder);

			// Wipe transline if on top and already started the transaction
			if (this->placeTransitionStack.size() > 0 && itemsUnder.size() > 0
					&& itemsUnder.front()->type() == Transline::Type) {
				itemsUnder.pop_front();
			}

			QGraphicsItem* itemUnder =
					itemsUnder.size() > 0 ? itemsUnder.front() : NULL;

			// Place trackpoint (not for hypertransition!)
			//---------------
			if (itemUnder == NULL && this->placeTransitionStack.size() > 0
			                      && this->placeTransitionStack.back()->type()!=FSMGraphicsItem<>::HYPERTRANS ) {

				//-- Nothing under, add a trackpoint between last item on stack, and nothing
				TrackpointItem * item = new TrackpointItem(new Trackpoint(e->scenePos().x(), e->scenePos().y(), 0),
						this->placeTransitionStack.first(), NULL);
				item->setPos(e->scenePos());
				this->addItem(item);

				//-- Trackpoint is last element on the transition stack
				this->placeTransitionStack.push_front(item);

				//-- We want to place its next line (to have the mouse follow the next line)
				//this->addToToPlaceStack(item->getNextTransline());
			}
			// Use Item under (Ok if Join or State)
			//---------------
			else if (itemUnder != NULL && (itemUnder->type() == StateItem::Type || itemUnder->type() == JoinItem::Type)) {


				//-- If there is a previous trackpoint, it ends on this item item
				if (this->placeTransitionStack.size() > 0
						&& FSMGraphicsItem<>::isTrackPoint(
								this->placeTransitionStack.first())) {
					dynamic_cast<TrackpointItem*> (this->placeTransitionStack[0])->setEndItem(
							itemUnder);
				} else {

				}
				this->placeTransitionStack.push_front(itemUnder);

			}

			// If nothing on the stack, do nothing further
			if (this->placeTransitionStack.size() == 0)
				break;

			// Check that if we should end the transition
			// The objects are commited to model
			//-----------------------------------------------

			// If we have found that we can end the transition
			bool endOfTransition = false;

			//-- Determine type of transition
			bool stateToState = this->placeTransitionStack.size() > 1
					&& this->placeTransitionStack.front()->type()
							== StateItem::Type
					&& this->placeTransitionStack.back()->type()
							== StateItem::Type;

			bool joinToState = this->placeTransitionStack.size() > 1
					&& this->placeTransitionStack.front()->type()
							== StateItem::Type
					&& this->placeTransitionStack.back()->type()
							== JoinItem::Type;

			bool stateToJoin = this->placeTransitionStack.size() > 1
								&& this->placeTransitionStack.front()->type()
										== JoinItem::Type
								&& this->placeTransitionStack.back()->type()
										== StateItem::Type;

			bool hypertransitionToState = this->placeTransitionStack.size() > 1
			                                && this->placeTransitionStack.front()->type()
			                                        == StateItem::Type
			                                && this->placeTransitionStack.back()->type()
			                                        == HyperTransition::Type;

			//-- Transition from state to state
			//-----------------
			if (stateToState || joinToState || stateToJoin) {

			    //-- Keep added/referenced transition
			    Trans * transition= NULL;

				//---- Add to model depending on type
				//---------------------
				if (stateToState) {

					//-- Add Transition
				    transition = this->fsm->addTrans(
							dynamic_cast<StateItem*> (this->placeTransitionStack.back())->getModel(),
							dynamic_cast<StateItem*> (this->placeTransitionStack.front())->getModel());

				} else if (joinToState) {

					//-- Update join model to target the state
					State * targetState = FSMGraphicsItem<>::toStateItem(this->placeTransitionStack.front())->getModel();
					FSMGraphicsItem<>::toJoinItem(this->placeTransitionStack.back())->getModel()->setTargetState(targetState);

					//-- Update eventual transition going to this join to the new target
					QList<Transline*>  incoming = FSMGraphicsItem<>::toJoinItem(this->placeTransitionStack.back())->getIncomingTransitions();
					for (QList<Transline*>::iterator it = incoming.begin();it != incoming.end(); it++) {
						((Trans *)(*it)->getModel())->setEndState(FSMGraphicsItem<>::toStateItem(this->placeTransitionStack.front())->getModel());
					}

				} else if (stateToJoin) {

					//-- Target is target of the joint
					State * targetState = FSMGraphicsItem<>::toJoinItem(this->placeTransitionStack.front())->getModel()->getTargetState();
					int joinId = FSMGraphicsItem<>::toJoinItem(this->placeTransitionStack.front())->getModel()->getId();

					//-- Add a transition pointing to the join point destination
					transition = this->fsm->addTrans(dynamic_cast<StateItem*> (this->placeTransitionStack.back())->getModel(),targetState);

					//-- Add one last trackpoint representing the join before the join graphic item
					TrackpointItem * joinTrackpoint = new TrackpointItem(new Trackpoint(0,0,NULL));
					joinTrackpoint->getModel()->setJoinID(joinId);
					//this->placeTransitionStack.insert(this->placeTransitionStack.indexOf(this->placeTransitionStack.front())-1,joinTrackpoint);
					this->placeTransitionStack.insert(1,joinTrackpoint);

					qDebug() << "Join target: " << targetState->getId() << " is id: " << joinTrackpoint->getModel()->getJoinID();

				}

				//---- Trackpoints (Go from end to start because of stacking)
				//-------------------
				Transline* lastTransline = NULL;
				for (int i = this->placeTransitionStack.size() - 1; i >= 0; i--) {

					//-- Add Trackpoint model to transition
					if (FSMGraphicsItem<>::isTrackPoint(
							this->placeTransitionStack[i])) {

						TrackpointItem * trackPoint =
								FSMGraphicsItem<>::toTrackPoint(
										this->placeTransitionStack[i]);

						//-- Add to transition if we are placing a transition
						if (stateToJoin|| stateToState) {

							transition->appendTrackpoint(
									trackPoint->getModel());
						}
						//-- Add to JoinItem if we are starting on a Join
						else if (joinToState) {
							FSMGraphicsItem<>::toJoinItem(this->placeTransitionStack.back())->getModel()->addTrackpoint(trackPoint->getModel());
						}

						//-- If trackpoint points to join, it is not designed to go on the scene
						if (trackPoint->getModel()->getJoinID()==0) {
							trackPoint->modelChanged();
							lastTransline = trackPoint->getPreviousTransline();
						}
					}

				} // End trackpoints --//

				//-- If no last transline, we have to create a new one
				if (lastTransline == NULL) {
					lastTransline = new Transline(transition,
							this->placeTransitionStack.back(), this->placeTransitionStack.front());
					this->addItem(lastTransline);
				}

				//-- Mark end of transition
				endOfTransition = true;


				//-- Add Text
				//--------------------------
				if (stateToState || stateToJoin) {
					TranslineText * translineText = new TranslineText(QString(
					        transition->getName().c_str()),(Trans*)lastTransline->getModel());


					//-- Place in between the two state
					lastTransline->setEnabled(true);
					qreal textx = this->placeTransitionStack.back()->x() -(this->placeTransitionStack.back()->x() - this->placeTransitionStack.front()->x())/2;
					qreal texty = this->placeTransitionStack.back()->y() -(this->placeTransitionStack.back()->y() - this->placeTransitionStack.front()->y())/2;
					translineText->setPos(textx,texty);
					this->addItem(translineText);
				}

				// Clean
				//----------------------
				// All items to be placed have been placed
				this->placeTransitionStack.clear();
				this->setPlaceMode(CHOOSE);

			} // End State to State finish--/
			// hypertransition to State
			//--------------------
			else if (hypertransitionToState) {

			    //-- Update Hypertransition model to target the state
                State * targetState = FSMGraphicsItem<>::toStateItem(this->placeTransitionStack.front())->getModel();
                FSMGraphicsItem<>::toHyperTransition(this->placeTransitionStack.back())->getModel()->setTargetState(targetState);

                //-- Add Transline
                Transline * transline = new Transline(NULL,
                                            this->placeTransitionStack.back(), this->placeTransitionStack.front());
                this->addItem(transline);

                //-- Clean
                // All items to be placed have been placed
                this->placeTransitionStack.clear();
                this->setPlaceMode(CHOOSE);

                //-- Mark end of transition
                endOfTransition = true;

			}

			// End of transition or add a new Transition that follow the mouse
			//------------------------------------------------------
			if (!endOfTransition) {

				// Not end of transition, add a transition item from last element (state or trackpoint) to the possible next
				//--------------

				//-- If last stacked element, is a trackpoint, the transline to move is the next transline
				if (this->placeTransitionStack.first()->type()
						== TrackpointItem::Type) {

					//-- Should follow the mouse
					this->addToToPlaceStack(
							dynamic_cast<TrackpointItem*> (this->placeTransitionStack.first())->getNextTransline());

				} else {

					//-- Otherwise, create a new transline
					//-- Prepare transline
					Transline * transition = new Transline(NULL,
							this->placeTransitionStack.first(), NULL);

					//-- Add to transition stack
					//this->placeTransitionStack.push_front(transition);

					//-- Should follow the mouse
					this->addToToPlaceStack(transition);
				}
			}
			break;
		}
        //-- END OF Transition -------//

        // Link
        //------------------------------
		case LINKDEPARTURE: {

			//-- Get Item under
			QList<QGraphicsItem*> itemsUnder = this->items(e->scenePos(),
					Qt::IntersectsItemShape, Qt::AscendingOrder);
			QGraphicsItem* itemUnder =
					itemsUnder.size() > 0 ? itemsUnder.front() : NULL;

			//-- Action if clicked on a state
			if (itemUnder == NULL || itemUnder->type() != StateItem::Type) {
				break;
			}
			StateItem * clickedState = dynamic_cast<StateItem*> (itemUnder);

			//-- First state To be clicked is the source
			if (this->placeLinkStartState == NULL) {

				//-- Record and unselect
				this->placeLinkStartState = clickedState;
				this->placeLinkStartState->setSelected(false);
			}
			//-- Second state to be clicked is the destination
			else if (this->placeLinkEndState == NULL) {

				//-- Record and unselect
				this->placeLinkEndState = clickedState;
				this->placeLinkEndState->setSelected(false);
			}

			//-- If both states are set, proceed
			//-----------
			if (this->placeLinkEndState != NULL && this->placeLinkStartState
					!= NULL) {

				// Prepare Model
				//------------------------

			    Link * link = this->getFsm()->getLinkbyGoal(this->placeLinkEndState->getModel());

				//-- Add Link to destination if necessary
				if (link==NULL) {
				    link = this->getFsm()->addLink(this->placeLinkEndState->getModel(),(double) e->scenePos().x(),
							(double) e->scenePos().y());
				}



				//-- Add Link transition from start
				Trans * linkToTransition = this->getFsm()->addTrans(
						this->placeLinkStartState->getModel()->getId(),
						this->placeLinkEndState->getModel()->getId());
				Trackpoint * tp = linkToTransition->appendTrackpoint(
						(double) e->scenePos().x(), (double) e->scenePos().y());

				tp->setTargetLink(link->getId());

				// Add Items to GUI using toPlace stack
				//-----------------

				//-- Prepare destination
				LinkArrival* linkToEndStateItem = this->placeLinkToState(
										this->placeLinkEndState, link);

				//-- Prepare Start
				LinkDeparture * linkFromStartItem = new LinkDeparture(tp,
						this->placeLinkStartState);

				//-- Sync colors
				linkFromStartItem->setBrush(linkToEndStateItem->brush());


				//-- End (At first because it cleans everything)
				//-- && Place
				this->setPlaceMode(CHOOSE);

				//-- Place Link to State if necessary
				if (linkToEndStateItem->scene() == NULL)
					this->addToToPlaceStack(linkToEndStateItem);

				//-- Place transition from start
				this->addToToPlaceStack(linkFromStartItem);

				// Clean
				//--------------------
				this->placeLinkStartState = NULL;
				this->placeLinkEndState = NULL;
			}

			break;
			//-- END OF Link -------------//

			/*default:
			 break;*/
		}
		//-- EOF Link --//

		// Place a JointPoint
		//--------------------------
		case JOIN: {

			//-- Get Item under
			QList<QGraphicsItem*> itemsUnder = this->items(e->scenePos(),
					Qt::IntersectsItemShape, Qt::AscendingOrder);
			QGraphicsItem* itemUnder =
					itemsUnder.size() > 0 ? itemsUnder.front() : NULL;

			JoinItem * JoinItem = FSMGraphicsItem<>::toJoinItem(itemUnder);

			//-- Add to FSM
			this->getFsm()->addJoin(JoinItem->getModel());


			//-- END
			this->setPlaceMode(CHOOSE);


			break;
		}
		//-- EOF JoinItem

		// Place an HyperTransition
		//---------------------------------
		case HYPERTRANS: {

		    //-- Get Hypertransition item (under the mouse because just released)
            HyperTransition * hypertransition = FSMGraphicsItem<>::toHyperTransition(this->items(e->scenePos(),
                    Qt::IntersectsItemShape, Qt::AscendingOrder).front());


            //-- Add To FSM
            Hypertrans * hypertransitionModel = this->getFsm()->addHypertrans();
            hypertransition->setModel(hypertransitionModel);
            this->getFsm()->addHypertrans(hypertransition->getModel());

            //-- Place centered on mouse
            hypertransitionModel->setPosition(pair<double,double>(e->scenePos().x(),e->scenePos().y()));
            hypertransition->setPos(e->scenePos().x(),e->scenePos().y());

            // Ask user to target a state
            //----------------------------------------

            // Start Transline Placement
            //--------------------

            //-- Stakc hypertransition on transitin placement stack
            this->placeTransitionStack.push_front(hypertransition);

            //-- Add a transline to position, and start transition placement modus
            Transline * beginTransline = new Transline(NULL,hypertransition,NULL);
            this->addToToPlaceStack(beginTransline);

            //-- END
            this->setPlaceMode(TRANS);

            break;

		}


		}



		//-- EOF Place Mode --------------//

	}

	// To Place elements Stack
	//------------------------------



	//-- Parent job
	// Only now, to avoid selection update on an item whose model has not been prepared
	QGraphicsScene::mouseReleaseEvent(e);

}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent* e) {

	//----------------- Mouse Following stacks --------------//

	// Mouse following
	//---------------------------------
	if (this->toPlaceStack.size() > 0) {

		QGraphicsItem * stackFirst = this->toPlaceStack.first();
		stackFirst->setVisible(true);
		stackFirst->setEnabled(true);
		this->placeUnderMouse(stackFirst, e->scenePos());

	}


	QGraphicsScene::mouseMoveEvent(e);
}

void Scene::placeUnderMouse(QGraphicsItem * item, QPointF mousePosition) {

	/*item->setPos(mousePosition.x() - (item->boundingRect().width() / 2),
	 mousePosition.y() - (item->boundingRect().height() / 2));*/

	//-- If we have a transline, we only want to position the endpoint
	if (item->type() == Transline::Type) {

		Transline * line = dynamic_cast<Transline*> (item);
		line->setEndPoint(mousePosition);
	} else {
		item->setPos(mousePosition);
	}

}

void Scene::keyReleaseEvent(QKeyEvent * keyEvent) {

	// Call Parent
	//-------------------
	QGraphicsScene::keyReleaseEvent(keyEvent);

	//qDebug() << "Krelease in scene widget: " << keyEvent->isAccepted();


	//-- IF event was already accepted, don't process it
	if (keyEvent->isAccepted())
		return;

	// ESC sets place mode to choose
	//---------------
	if (keyEvent->key() == Qt::Key_Escape && this->getPlaceMode() != CHOOSE) {
		this->setPlaceMode(CHOOSE);
	}
	// Suppr deletes items
	//---------------------
	else if (keyEvent->key() == Qt::Key_Delete) {

		QList<QGraphicsItem*> selectedItems = this->selectedItems();
		for (QList<QGraphicsItem*>::iterator it = selectedItems.begin(); it
				< selectedItems.end(); it++) {

			//--  Check item is still on the scene
			if ((*it)->scene() == NULL) {
				continue;
			}

			//-- remove / delete from scene
			//this->removeItem(*it);

			QList<QUndoCommand*> undoCommands;
			if ((*it)->type() == StateItem::Type) {

				undoCommands = dynamic_cast<StateItem*> (*it)->remove();

			} else if ((*it)->type() == Transline::Type) {

				undoCommands = dynamic_cast<Transline*> (*it)->remove();

			} else if ((*it)->type() == LinkDeparture::Type) {

				undoCommands = dynamic_cast<LinkDeparture*> (*it)->remove();

			} else if ((*it)->type() == TrackpointItem::Type) {

				undoCommands = dynamic_cast<TrackpointItem*> (*it)->remove();

			} else if ((*it)->type() == LinkArrival::Type) {

				undoCommands = dynamic_cast<LinkArrival*> (*it)->remove();

			} else if ((*it)->type() == HyperTransition::Type) {

                undoCommands = dynamic_cast<HyperTransition*> (*it)->remove();

            }

			//-- Add gathered Undo commands to stack
			for (QList<QUndoCommand*>::iterator cit = undoCommands.begin(); cit
					< undoCommands.end(); cit++) {
				this->undoStack.push(*cit);
			}

			// Only do once
			break;

		}

	} else if (keyEvent->key() == Qt::Key_Z && keyEvent->modifiers()
			== Qt::CTRL) {

		//-- Do undo
		//cout << "*I Undoing" << endl;
		//this->undo();

	}  else if (keyEvent->key() == Qt::Key_Y && keyEvent->modifiers()
            == Qt::CTRL) {

        //-- Do Redo
       // cout << "*I Redoing" << endl;
        //this->redo();

    }

}

Fsm * Scene::getFsm() {
	return this->fsm;
}
void Scene::setFsm(Fsm * fsm) {

	this->fsm = fsm;
}

#define CLEAN_POINTER_QLIST(list)\
		while (!list.isEmpty()) \
			 delete list.takeFirst();

void Scene::setPlaceMode(FSMDesigner::Item mode) {

	//-- If back to choose requested, then perform cleaning and such
	//-------------------------------------
	if (mode==FSMDesigner::CHOOSE) {

		//-- Restore any cursor change
		QApplication::restoreOverrideCursor();

		//-- Clear selected elements
		this->clearSelection();



		// Clean to place stack
		//-----------------------------------

		//-- Clean things to be placed as they will never come up
		while (!this->toPlaceStack.isEmpty()) {

			QGraphicsItem* firstItem = this->toPlaceStack.takeFirst();
			//-- Transline that belong to a trackpoint are removed by trackpoint
			if (FSMGraphicsItem<>::isTransline(firstItem) &&
				FSMGraphicsItem<>::isTrackPoint(FSMGraphicsItem<>::toTransline(firstItem)->getStartItem())) {

			} else
				delete firstItem;
		}
		//CLEAN_POINTER_QLIST(this->toPlaceStack)

		// Clean Transition place stack
		//---------------------------------------

		//-- Clear Trackpoints that were in placement
		while (!this->placeTransitionStack.isEmpty()) {

			QGraphicsItem* firstItem = this->placeTransitionStack.takeFirst();
			//-- Transline that belong to a trackpoint are removed by trackpoint
			if (FSMGraphicsItem<>::isTrackPoint(firstItem)) {
				this->removeItem(firstItem);
				delete firstItem;
			}
		}

		// Reset variables
		//---------------------
		this->placeLinkStartState = NULL;
		this->placeLinkEndState = NULL;
	}


	//-- If In Lock mode, don't change back to choose and repeat
	if (this->getPlaceModeLock() != FSMDesigner::LOCKED ) {
		//-- Record
		this->placeMode = mode;
	}

	//-- Make some preparation
	switch (this->placeMode) {

        case LINKDEPARTURE: {
            //-- Change cursor
            QApplication::setOverrideCursor(Qt::CrossCursor);

            //-- Highligh all the states that don't already have a link
            this->clearSelection();
            QList<QGraphicsItem*> items = this->items();
            for (QList<QGraphicsItem*>::iterator it = items.begin(); it
                    < items.end(); it++) {

                if ((*it)->type() == StateItem::Type) {
                    (*it)->setSelected(true);
                }

            }

            break;
        }
        case STATE: {
            //-- Create an Item to navigate under the mouse
            StateItem * st = new StateItem();
            this->addToToPlaceStack(st);

            break;
        }

        case JOIN: {

            //-- Create an Item to navigate under the mouse
            JoinItem * joinItem = new JoinItem();
            this->addToToPlaceStack(joinItem);

            break;
        }

        case HYPERTRANS: {

            //-- Create an Item to navigate under the mouse
            HyperTransition * hypertransition = new HyperTransition();
            this->addToToPlaceStack(hypertransition);

            break;
        }

	default:
		break;

	}

}

LinkArrival * Scene::placeLinkToState(StateItem * endState, Link * link) {

	// Find An eventual LinkArrival to this state, using the StateItem incoming transitions
	//------------------------------
	LinkArrival * linkArrival = endState->findLinkArrival();


	//-- Create GUI Item if not found
	if (linkArrival==NULL) {
	    linkArrival = new LinkArrival(link);

		//-- Add to scene
		//this->addItem(LinkArrival);

		//-- Create transition Line to state
		Transline * transLine = new Transline(NULL, linkArrival, endState);
		linkArrival->setNextTransline(transLine);
		this->addItem(transLine);

	}


	return linkArrival;
}

void Scene::alignSelectionVertical() {

	qreal firstPosx = -1;

	//-- Foreach selection
	QList<QGraphicsItem*> selectedItems = this->selectedItems();
	for (QList<QGraphicsItem*>::iterator it = selectedItems.begin(); it
			< selectedItems.end(); it++) {

		//-- Only states and trackpoints
		if ((*it)->type() != StateItem::Type && (*it)->type() != TrackpointItem::Type )
			continue;

		//-- Record X first position
		if (firstPosx == -1) {
			firstPosx = (*it)->pos().x();
		} else {
			//-- If already, give it to the others
			(*it)->setPos(firstPosx, (*it)->y());
		}

	}

}
void Scene::alignSelectionHorizontal() {

}

void Scene::startPlaceState() {

	//-- Set Place Mode
	this->setPlaceMode(FSMDesigner::STATE);

}

void Scene::startPlaceTransition() {

	//-- Set Place Mode
	this->setPlaceMode(FSMDesigner::TRANS);
}

void Scene::startPlaceHyperTransition() {

    //-- Set Place Mode
    this->setPlaceMode(FSMDesigner::HYPERTRANS);
}

void Scene::startPlaceLink() {

	//-- Set Place Mode
	this->setPlaceMode(FSMDesigner::LINKDEPARTURE);
}

void Scene::startPlaceJoin() {

	//-- Set Place Mode
	this->setPlaceMode(FSMDesigner::JOIN);
}


void Scene::verify() {

	if (this->getFSMVerificator()!=NULL) {
		this->getFSMVerificator()->reset();
	}

    Verificator * verificator = new Verificator();
    verificator->addRule(new StateOutputsRule());
    verificator->verify(this->fsm,this);

    delete verificator;


}

void Scene::enteredRule(VerificatorRule * rule) {

}

void Scene::ruleSuccessful(VerificatorRule * rule) {

}

void Scene::ruleFailed(VerificatorRule * rule,QList<RuleError*>& errors) {

    qDebug() << "Rule failed: " << rule->getName();

    // Map Errors to Elements
    //--------------------------------
    for (int i = 0 ; i<errors.size(); i++) {

        // Pick Error
        RuleError * error = errors.at(i);

        // Create an FSMVerifyError
        FSMVerifyError * guiError = new FSMVerifyError(FSMVerifyError::DEFAULT);
        guiError->setMessage(error->getMessage());

        // Map To GUI Item
        //-----------------------
        switch (error->getConcernedObjectType()) {

            //-- State Item
            case FSMDesigner::STATE: {

                if (error->getConcernedObject<State>()==NULL)
                    break;

                // Find back and Set
                StateItem * stateItem = this->findStateItem(dynamic_cast<State*>(error->getConcernedObject<State>()));
                if (stateItem!=NULL) {
                    guiError->addConcernedItem(stateItem);
                    stateItem->addVerificationError(guiError);
                }

                break;
            }
            default:

                break;

        }


    }


}

QList<Transline *> Scene::findTransline(Trans * transitionModel) {

	QList<QGraphicsItem*> allItems = this->items();
	QList<Transline *> result;

	//-- Search for the transline
	for (QList<QGraphicsItem*>::iterator it = allItems.begin();it!=allItems.end();it++) {

		if (FSMGraphicsItem<>::isTransline((*it))
				&& FSMGraphicsItem<>::toTransline(*it)->getModel()!= NULL
				&& FSMGraphicsItem<>::toTransline(*it)->getModel()==transitionModel) {
			result+=(FSMGraphicsItem<>::toTransline(*it));
		}
	}

	return result;

}

QList<TrackpointItem *> Scene::findTransitionBaseTrackpoint(TransitionBase * transitionBaseModel) {

    QList<QGraphicsItem*> allItems = this->items();
    QList<TrackpointItem *> result;

    //-- Search for the transline
    for (QList<QGraphicsItem*>::iterator it = allItems.begin();it!=allItems.end();it++) {

        TransitionBase * itemTransitionModel = NULL;
        if (FSMGraphicsItem<>::isTrackPoint((*it))
                && FSMGraphicsItem<>::toTrackPoint(*it)->getModel()!= NULL
                && FSMGraphicsItem<>::toTrackPoint(*it)->getModel()->getTransition()==transitionBaseModel) {
            result+=FSMGraphicsItem<>::toTrackPoint(*it);
        }
    }

    return result;

}

TranslineText * Scene::findTranslineText(Trans * transitionModel) {

    QList<QGraphicsItem*> allItems = this->items();
    TranslineText * result = NULL;

    //-- Search for the transline text
    for (QList<QGraphicsItem*>::iterator it = allItems.begin();it!=allItems.end();it++) {
        if (FSMGraphicsItem<>::isTranslineText((*it))
                && FSMGraphicsItem<>::toTranslineText(*it)->getTransition()!= NULL
                && FSMGraphicsItem<>::toTranslineText(*it)->getTransition()==transitionModel) {
            result=(FSMGraphicsItem<>::toTranslineText(*it));
            break;
        }
    }

    return result;
}

StateItem * Scene::findStateItem(State * stateModel) {

	QList<QGraphicsItem*> allItems = this->items();
	QList<Transline *> result;

	//-- Search for the transline
	for (QList<QGraphicsItem*>::iterator it = allItems.begin();it!=allItems.end();it++) {

		if (FSMGraphicsItem<>::isStateItem((*it))
				&& FSMGraphicsItem<>::toStateItem(*it)->getModel()!= NULL
				&& FSMGraphicsItem<>::toStateItem(*it)->getModel()==stateModel) {
			return FSMGraphicsItem<>::toStateItem(*it);
		}
	}

	return NULL;

}

JoinItem * Scene::findJoinItem(Join * joinModel) {

    QList<QGraphicsItem*> allItems = this->items();
    JoinItem * result;

    //-- Search for the transline
    for (QList<QGraphicsItem*>::iterator it = allItems.begin();it!=allItems.end();it++) {

        if (FSMGraphicsItem<>::isJoinItem((*it))
                && FSMGraphicsItem<>::toJoinItem(*it)->getModel()!= NULL
                && FSMGraphicsItem<>::toJoinItem(*it)->getModel()==joinModel) {
            return FSMGraphicsItem<>::toJoinItem(*it);
        }
    }

    return NULL;

}

QList<LinkDeparture*> Scene::findLinkDepartures(Link * link) {

	QList<QGraphicsItem*> allItems = this->items();
	QList<LinkDeparture *> result;

	//-- Search for the LinkDepartures
	for (QList<QGraphicsItem*>::iterator it = allItems.begin();it!=allItems.end();it++) {

		if (FSMGraphicsItem<>::isLinkDeparture((*it))
				&& FSMGraphicsItem<>::toLinkDeparture(*it)->getModel()!= NULL
				&& FSMGraphicsItem<>::toLinkDeparture(*it)->getModel()->getTargetLink() == link->getId()) {
			result << FSMGraphicsItem<>::toLinkDeparture(*it);
		}
	}

	return result;
}




