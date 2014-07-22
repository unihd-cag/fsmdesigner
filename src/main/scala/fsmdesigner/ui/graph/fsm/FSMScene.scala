/**
 *
 */
package fsmdesigner.ui.graph.fsm

import java.awt.event.ActionEvent
import java.awt.event.ActionListener
import scala.collection.JavaConversions._
import org.netbeans.api.visual.action.ActionFactory
import org.netbeans.api.visual.action.WidgetAction
import org.netbeans.api.visual.graph.GraphScene
import org.netbeans.api.visual.widget.ComponentWidget
import org.netbeans.api.visual.widget.LayerWidget
import org.netbeans.api.visual.widget.Widget
import fsmdesigner.model.projectfsm
import fsmdesigner.model.projectfsmstate
import fsmdesigner.nbtry.StateWidget
import fsmdesigner.ui.graph.UIState
import fsmdesigner.ui.graph.UITransition
import javax.swing.JButton
import javax.swing.undo.UndoManager
import fsmdesigner.ui.graph.actions.FSMSceneUndoRedoKey

/**
 * @author rleys
 *
 */
class FSMScene(val fsm: projectfsm) extends GraphScene[Object, Object] {

  //-- Create layers
  var mainLayer = new LayerWidget(this);
  var connectionLayer = new LayerWidget(this);
  var interactionLayer = new LayerWidget(this);
  addChild(mainLayer)
  addChild(connectionLayer)
  addChild(interactionLayer)
  
  //-- Edition Layers
  var transitionEditLayer = new LayerWidget(this)
  addChild(transitionEditLayer)

  //-- Standard Components
  //----------------------------

  //-- Satellite view
  /*var satelliteView = this.createSatelliteView()
  addChild(new ComponentWidget(this,satelliteView))*/

  
  //-- Undo/Redo Stack
  //---------------------------
  var undoManager = new UndoManager
  var undoRedoKeyAction = new FSMSceneUndoRedoKey(this)
  
 /* this.mainLayer.getActions().addAction(new FSMSceneUndoRedoKey(this))
  this.connectionLayer.getActions().addAction(new FSMSceneUndoRedoKey(this))
  this.interactionLayer.getActions().addAction(new FSMSceneUndoRedoKey(this))*/
  this.getActions().addAction(new FSMSceneUndoRedoKey(this))
 
  
  
  //-- Add Scene Actions
  //--------------------

  //-- Zoom
  getActions().addAction(ActionFactory.createZoomAction());

  //-- Connection Provider
  //--------------

  

  /*class MyReConnectProvider extends ReconnectProvider {

    def  	hasCustomReplacementWidgetResolver : Boolean = false
    
    /**
     * Say if we can start connection from this widget
     */
    def  isTargetReconnectable(source: Widget): Boolean = {

      //println(s"In is source widget")
      source match {
        case null => false
        case _    => true
      }
    }

    
    /**
     * Say if we can start connection from this widget
     */
    def  isSourceReconnectable(source: Widget): Boolean = {

      //println(s"In is source widget")
      source match {
        case null => false
        case _    => true
      }
    }
    
    /**
     * Say if we can connect to the target widget
     */
    def  	isTargetReconnectable(src: Widget, trg: Widget): ConnectorState = {

      trg match {
        case null       => ConnectorState.REJECT
        case trg: Scene => ConnectorState.REJECT
        case _          => ConnectorState.ACCEPT
      }

    }

    def  	resolveReplacementWidget(scene: Scene, arg1: Point): Widget = {
      return null;
    }
    
    def reconnectingFinished( connectionWidget : ConnectionWidget , reconnectingSource : Boolean) = {
      
    }
    
     def reconnectingStarted( connectionWidget : ConnectionWidget , reconnectingSource : Boolean) = {
      
    }

    def createConnection(source: Widget, target: Widget): Unit = {

      var conn = new FreeConnectionWidget(FSMScene.this);
      conn.setTargetAnchorShape(AnchorShape.TRIANGLE_FILLED);
      conn.setTargetAnchor(AnchorFactory.createRectangularAnchor(target));
      conn.setSourceAnchor(AnchorFactory.createRectangularAnchor(source));

      //-- Setup Control points
      conn.setControlPointCutDistance(5)
      conn.setPaintControlPoints(true)
      conn.setControlPointShape(PointShape.SQUARE_FILLED_BIG)
      conn.setRouter(RouterFactory.createFreeRouter())

      //-- Add control points
      conn.getActions.addAction(ActionFactory.createAddRemoveControlPointAction())

      //-- Move control points
      conn.getActions.addAction(ActionFactory.createMoveControlPointAction(new MoveControlPointProvider {
        def locationSuggested(w: ConnectionWidget, index: Int, suggestedLocation: Point): java.util.List[Point] = {

          w.getControlPoints().get(index).setLocation(suggestedLocation)
          w.getControlPoints()

        }
      }))

      connectionLayer.addChild(conn);
    }

  }*/

  protected def attachNodeWidget(model: Object): Widget = {

    //-- Create Widget
    //--------------------
    var lw = new UIState(model.asInstanceOf[projectfsmstate], this)
    model.asInstanceOf[projectfsmstate].setNextBuffer(lw)
    //lw.setLabel(arg0)

    mainLayer.addChild(lw)

    

    lw
  }

  protected def attachEdgeWidget(arg0: Object): Widget = {
    throw new UnsupportedOperationException("Not supported yet.");
  }

  protected def attachEdgeSourceAnchor(arg0: Object, arg1: Object, arg2: Object) = {
    throw new UnsupportedOperationException("Not supported yet.");
  }

  protected def attachEdgeTargetAnchor(arg0: Object, arg1: Object, arg2: Object) = {
    throw new UnsupportedOperationException("Not supported yet.");
  }

  // FSM Init
  //--------------------

  // Add test button
  var b = new JButton("init")
  var bw = new ComponentWidget(this, b)
  mainLayer.addChild(bw)
  b.addActionListener(new ActionListener {
    override def actionPerformed(e: ActionEvent) = {
      println(s"Hello!")

      // Add transitions
      fsm.transs.foreach {
        t =>

          //-- Connection
          var conn = new UITransition(t, FSMScene.this);
          connectionLayer.addChild(conn);
         // conn.setRouter(RouterFactory.createOrthogonalSearchRouter(connectionLayer))
          // println(s"CP"+conn.getControlPoints())
          
         /* SwingUtilities.invokeLater(new Runnable {
            def run = {*/

              //-- Init list of control points         
              /*var cl = t.trackpoints.map {
                tp =>

                  var p = new Point(tp.posx.toInt, tp.posy.toInt)
                  println(s"Added point " + p)

                  conn.addRemoveControlPoint(p)
                 // conn.addAction.
                  p
              }
              conn.setControlPoints(cl, false)*/
              

         /*   }
          })*/

          /*conn.setRouter(new Router {
            def routeConnection( connectionWidget : ConnectionWidget) : java.util.List[Point] = {
              println(s"Routing for widget with points: "+connectionWidget.getControlPoints())
              
              var s = connectionWidget.getSourceAnchor ().compute(connectionWidget.getSourceAnchorEntry ()).getAnchorSceneLocation ()
              var t  = connectionWidget.getTargetAnchor ().compute(connectionWidget.getTargetAnchorEntry ()).getAnchorSceneLocation ()
              var r = List(s) ::: connectionWidget.getControlPoints().toList ::: List(t) 
              //var r = List(widget.getSourceAnchor().getRelatedSceneLocation()) ::: widget.getControlPoints().toList ::: List(widget.getTargetAnchor().getRelatedSceneLocation()) 
              //widget.getControlPoints().toList
              conn.setRouter(RouterFactory.createFreeRouter())
              
              r
            // List(widget.getSourceAnchor().getRelatedSceneLocation())
            }
          })*/

          /*conn.calculateRouting()
          conn.reroute()
          conn.repaint()
          conn.revalidate()*/

      }

      FSMScene.this.revalidate()
      FSMScene.this.repaint()

    }
  })

  // Add only states for now
  fsm.states.foreach {
    state =>
      attachNodeWidget(state)
    //println(s"Added state")
  }

}