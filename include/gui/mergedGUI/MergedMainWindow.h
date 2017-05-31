/**
 *
 *
 * \file MergedMainWindow.h
 *
 *
 *
 *
 */

// Includes
//-------------------------

//-- Std

//-- Qt
#include <QtGui>
#include <QtCore>


//-- Ui
#include <ui_MergedMainWindow.h>

/**
 * This class is the new Main GUI. It is the old GUI with a tabbed FSMs component in place of the scene
 */
class MergedMainWindow: public QMainWindow, public Ui_MergedMainWindow  {

    Q_OBJECT


    private:

        /// The Last path opened, to restore File dialog
        QString  openFileLastPath;


    public:

        /** Default Constructor
         *
         */
        MergedMainWindow();

        /** Default Destructor
         *
         */
        virtual ~MergedMainWindow();

    protected:


        /// Save Geometry state
        virtual void resizeEvent ( QResizeEvent * event );

        /**
         * F11: Toggle fullscreen
         * @param event
         */
        virtual void keyReleaseEvent ( QKeyEvent * event );


        /// Save Geometry state
        void closeEvent(QCloseEvent *event);

        /** \defgroup FileOpen Methods to open project files */
        /** @{

        /**
         * Reimplements Drop event to fast open Project files
         * @param event
         */
        virtual void    dropEvent(QDropEvent * event);

        /**
         * Determines if the entering drag qualifies for files opening
         * @param event
         */
        virtual void    dragEnterEvent(QDragEnterEvent * event) ;

        /** @} */

    public slots:

        /** \defgroup Project Project Manipulation Interface */
        /** @{ */

       /**
        * Creates a new project
        * Ask for confirmation if a project exists
        */
       void newProject();

       /** Opens the provided project through Core interface
        *
        */
       void openProject();

       /**
        * Closes the project and resets the Core interface
        */
       void closeProject();


       /**
        * Creates a new project by taking name from name text line editor
        */
       void createProject();

       /**
        * Create a new Project
        * @param name
        */
       void createProject(QString name);

       /**
        * Rename the current Project
        * If the name is not provided, asks for one
        */
       void renameProject(QString newName = "");


       /**
        * Saves the project to a new location
        */
       void saveProjectAs();

       /**
        * Saves the project to previous location or call #saveProjectAs if none defined
        */
       void saveProject();

        /** @} */




        /** \addToGroup FileOpen Project File Opening*/
        /** @{ */

        /// Open A recent file from recent files list index
        void openRecentFile( QListWidgetItem * item);

        /**
         * Open the given file, or ask for one
         * @param path if not provided or "", ask for a file to open
         */
        void openFile(QString path = "");

        /**
         * Open the given file, or ask for one
         * @param path if not provided or "", ask for a file to open
         */
        void openFile(QAction *  sourceAction);

        /** @} */



        /** \defgroup Gui Gui Connections */
        /** @{ */

        /**
         * Display Global Editing table
         */
        void showGlobalTable();

        /** @} */

        /** \defgroup Export Exportation Functions */
        /** @{ */

        /**
         * Calls up a print preview window
         * Relies on #print(QPainter*) for actual painting
         */
        void print();

        /**
         * Do the actual painting of the selected scene on a painter
         * @param painter
         */
        void print(QPrinter * painter);

        void exportAsSVG();

        /** @} */




        /// If the UI settings have changed
        void settingsChanged();

        void about();

};

