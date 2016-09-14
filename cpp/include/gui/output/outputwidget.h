/***********************************************************************
 *                                                                      *
 * (C) 2009, Frank Lemke, Computer Architecture Group,                  *
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

#ifndef OUTPUTWIDGET_H_
#define OUTPUTWIDGET_H_

// Includes
//---------------------

//-- Std
using namespace std;

//-- Qt
#include <QtGui>
#include <QtCore>

//-- Gui
#include <gui/scene/SceneRelatedObject.h>


class OutputWidget: public QTreeView, public SceneRelatedObject {

Q_OBJECT

protected:

    /** \defgroup CtxMenu Context Menu Actions */
    /** @{ */

    QAction* rename;
    QAction* newoutput;
    QAction* deleteoutput;
    QAction* deleteselectedoutput;
    QAction* outputup;
    QAction* outputdown;

    /** @} */
public:


	OutputWidget(QWidget* parent = NULL);
	virtual ~OutputWidget();

	/**
	 * Shows Context menu for adding/removing Stuff
	 * @param e
	 */
	virtual void contextMenuEvent(QContextMenuEvent* e);


public Q_SLOTS:

    /// To be Linked to the related scene to update content
    virtual void selectionUpdated();

    /// Reimplement from parent to propagate to model and delegate
    virtual void setRelatedScene(Scene * scene);

private:
	void createActions();

protected:



	/// Implements CTRL + UP/DOWN for fast move of outputs
	virtual void	keyPressEvent ( QKeyEvent * event ) ;


protected Q_SLOTS:

	void newOutput();

	void renameOutput() ;

	void deleteLast();

	void deleteSelected();

	void moveDown();

	void moveUp();



};

#endif
