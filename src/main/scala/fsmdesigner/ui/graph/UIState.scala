/**
 *
 */
package fsmdesigner.ui.graph

import java.awt.Color
import java.awt.Dimension
import java.awt.Graphics2D
import java.awt.Point
import java.awt.Rectangle
import org.netbeans.api.visual.action.ActionFactory
import org.netbeans.api.visual.action.ConnectProvider
import org.netbeans.api.visual.action.ConnectorState
import org.netbeans.api.visual.action.MoveControlPointProvider
import org.netbeans.api.visual.action.TextFieldInplaceEditor
import org.netbeans.api.visual.action.WidgetAction
import org.netbeans.api.visual.anchor.AnchorFactory
import org.netbeans.api.visual.anchor.AnchorShape
import org.netbeans.api.visual.anchor.PointShape
import org.netbeans.api.visual.layout.LayoutFactory
import org.netbeans.api.visual.router.RouterFactory
import org.netbeans.api.visual.widget.ConnectionWidget
import org.netbeans.api.visual.widget.FreeConnectionWidget
import org.netbeans.api.visual.widget.LabelWidget
import org.netbeans.api.visual.widget.Scene
import org.netbeans.api.visual.widget.Widget
import org.netbeans.modules.visual.action.AlignWithMoveStrategyProvider
import org.netbeans.modules.visual.action.SingleLayerAlignWithWidgetCollector
import com.idyria.osi.ooxoo.core.buffers.structural.BaseBufferTrait
import fsmdesigner.model.projectfsmstate
import fsmdesigner.ui.graph.actions.DefaultMoveProvider
import fsmdesigner.ui.graph.fsm.FSMScene
import fsmdesigner.ui.graph.actions.AlignWithMove
import javax.swing.undo.UndoableEdit
import javax.swing.undo.AbstractUndoableEdit
import fsmdesigner.model.projectfsmtrans
import java.awt.BasicStroke
import java.awt.RadialGradientPaint
import java.awt.geom.Point2D
import scala.collection.JavaConversions._
import java.awt.MultipleGradientPaint.CycleMethod

/**
 * @author rleys
 *
 */
class UIState(val state: projectfsmstate, val scene: FSMScene) extends Widget(scene) with BaseBufferTrait {

  //-- Create Ellipse Widget
  //-------------------------------
  var ellipseRadius = 50
  var ellipse = new Widget(scene) {

    this.setPreferredSize(new Dimension(ellipseRadius, ellipseRadius))

    override def calculateClientArea(): Rectangle = {

      new Rectangle(0, 0, ellipseRadius, ellipseRadius)

    }

    override def paintWidget = {

      //println(s"Painting state widget "+hashCode())
      var g2 = getGraphics().create.asInstanceOf[Graphics2D]
      //g2.save

      //-- Set Painter
      g2.setBackground(new Color(state.__color.toInt, true))
      g2.setColor(new Color(state.__color.toInt, true))

      //-- Gradient paint
      var grCenter = new Point2D.Float(ellipseRadius/2,ellipseRadius/2)
      var grFocus = new Point2D.Float(ellipseRadius-(ellipseRadius*10/100),ellipseRadius*10/100)
      var radius = ellipseRadius.toFloat
      var dist = Array(0.0f,1.0f)
      var colors = Array(new Color(state.__color.toInt, true),Color.WHITE)
      var p =
        new RadialGradientPaint(grCenter, radius, grFocus,
          dist, colors,
          CycleMethod.NO_CYCLE);
      
      

      scene.fsm.resetstate.toString match {
        case rid if (rid == state.id.toString) =>
          
          g2.setColor(Color.BLACK)
          g2.setStroke(new BasicStroke(2))
          g2.drawOval(0, 0, ellipseRadius, ellipseRadius)
          
          g2.setColor(new Color(state.__color.toInt, true))
          g2.setPaint(p)
          g2.fillOval(5, 5, ellipseRadius - 10, ellipseRadius - 10)
        case _ =>

          g2.setColor(Color.BLACK)
          g2.setStroke(new BasicStroke(2))
          g2.drawOval(0, 0, ellipseRadius, ellipseRadius)

          g2.setColor(new Color(state.__color.toInt, true))
          g2.setPaint(p)
          g2.fillOval(0, 0, ellipseRadius, ellipseRadius)
      }

      g2.dispose

      super.paintWidget
    }
  }
  addChild(ellipse)

  //-- Create Label Widget
  //--------------------------------
  var label = new LabelWidget(scene)
  label.setLabel(state.sname)

  // label.setPreferredLocation(new Point(0, ellipseRadius + 5))
  /*  label.getActions().addAction(ActionFactory.createEditAction (new EditProvider {
    def edit(w:Widget) : Unit = {
      println(s"Editing Label")
    }
  }))*/
  label.getActions().addAction(ActionFactory.createInplaceEditorAction(new TextFieldInplaceEditor {

    def isEnabled(widget: Widget): Boolean = {
      return true;
    }
    def getText(widget: Widget): String = {
      return widget.asInstanceOf[LabelWidget].getLabel
    }
    def setText(widget: Widget, text: String): Unit = {

      //-- Create UNDO/REDO Action
      var edit = new AbstractUndoableEdit {

        var saveText = widget.asInstanceOf[LabelWidget].getLabel()

        override def undo = {
          var tmp = widget.asInstanceOf[LabelWidget].getLabel()
          widget.asInstanceOf[LabelWidget].setLabel(saveText)
          saveText = tmp
          super.undo

        }

        override def redo = {
          var tmp = widget.asInstanceOf[LabelWidget].getLabel()
          widget.asInstanceOf[LabelWidget].setLabel(saveText)
          saveText = tmp
          super.redo
          //this.h
        }

      }

      //-- Register
      widget.asInstanceOf[LabelWidget].setLabel(text)

      //-- perform initial change
      UIState.this.scene.undoManager.addEdit(edit)

    }
  }))
  addChild(label)

  //-- Model Connections
  //-------------------------
  this.setPreferredLocation(new Point(state.posx.data.toInt, state.posy.data.toInt))

  // Position

  //-- Layout
  //---------------
  this.setLayout(LayoutFactory.createVerticalFlowLayout(LayoutFactory.SerialAlignment.CENTER, 3))

  //-- Actions and events
  //--------------------------------

  //----- Connection Action
  //----------------------
  class MyConnectProvider extends ConnectProvider {

    /**
     * Say if we can start connection from this widget
     */
    def isSourceWidget(source: Widget): Boolean = {

      println(s"In is source widget")
      source match {
        case null => false
        case _    => true
      }
    }

    /**
     * Say if we can connect to the target widget
     */
    def isTargetWidget(src: Widget, trg: Widget): ConnectorState = {

      trg match {
        case null         => ConnectorState.REJECT
        case trg: Scene   => ConnectorState.REJECT
        case trg: UIState => ConnectorState.ACCEPT
        case _            => ConnectorState.REJECT
      }

    }

    def hasCustomTargetWidgetResolver(scene: Scene): Boolean = {
      return false;
    }

    def resolveTargetWidget(scene: Scene, arg1: Point): Widget = {
      return null;
    }

    def createConnection(source: Widget, target: Widget): Unit = {

      //-- Create Model
      var transition = new projectfsmtrans
      transition.name = "Unnamed"
      UIState.this.scene.fsm.transs += transition

      //-- Set states
      transition.start.targetObject = source.asInstanceOf[UIState].state
      transition.end.targetObject = target.asInstanceOf[UIState].state

      //-- Create UI
      var conn = new UITransition(transition, UIState.this.scene);

      UIState.this.scene.connectionLayer.addChild(conn);
      UIState.this.scene.repaint()
      UIState.this.scene.connectionLayer.revalidate()
      UIState.this.scene.connectionLayer.repaint()

      /*var conn = new FreeConnectionWidget(UIState.this.scene);
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
      }))*/

    }

  }
  this.getActions().addAction(
    ActionFactory.createExtendedConnectAction(
      scene.connectionLayer, new MyConnectProvider()));

  //---- Moving action
  //-----------------------
  var maction = new AlignWithMove(new SingleLayerAlignWithWidgetCollector(scene.mainLayer, false), scene.interactionLayer, ActionFactory.createDefaultAlignWithMoveDecorator(), false) {

    var originalLoc: Point = _

    override def movementStarted(widget: Widget): Unit = {
      originalLoc = widget.getPreferredLocation()
      super.movementStarted(widget)
    }

    override def movementFinished(widget: Widget): Unit = {

      println(s"Move undo/redo can be set")
      if (!originalLoc.equals(widget.getLocation())) {
        UIState.this.scene.undoManager.addEdit(new AbstractUndoableEdit {

          var saveLoc = originalLoc

          override def undo = {
            var tmp = widget.getPreferredLocation()
            widget.setPreferredLocation(saveLoc)
            saveLoc = tmp
            super.undo

          }

          override def redo = {
            var tmp = widget.getPreferredLocation()
            widget.setPreferredLocation(saveLoc)
            saveLoc = tmp
            super.redo
          }

        })
      }

      super.movementFinished(widget)
    }

  }

  this.getActions().addAction(ActionFactory.createMoveAction(maction, maction));

}