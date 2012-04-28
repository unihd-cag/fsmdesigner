/*
 * SelectionAnimated.h
 *
 *  Created on: May 10, 2010
 *      Author: rleys
 */

#ifndef SELECTIONANIMATED_H_
#define SELECTIONANIMATED_H_


/**
 * Pure virtual, to be implemented by GraphicsObject whishing to implement animation on selection
 */
class SelectionAnimated {
public:
	SelectionAnimated();
	virtual ~SelectionAnimated();
};

#endif /* SELECTIONANIMATED_H_ */
