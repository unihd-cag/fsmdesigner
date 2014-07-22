/**
 *
 */
package fsmdesigner.ui.graph

import org.netbeans.api.visual.widget.FreeConnectionWidget
import fsmdesigner.model.projectfsmtrans
import org.netbeans.api.visual.widget.Scene
import org.netbeans.api.visual.anchor.AnchorShape
import org.netbeans.api.visual.action.ActionFactory
import org.netbeans.api.visual.action.MoveControlPointProvider
import java.awt.Point
import org.netbeans.api.visual.widget.ConnectionWidget
import org.netbeans.api.visual.anchor.AnchorFactory
import org.netbeans.api.visual.anchor.PointShape
import org.netbeans.api.visual.router.RouterFactory
import org.netbeans.api.visual.widget.ConnectionWidget.RoutingPolicy
import org.netbeans.modules.visual.action.AddRemoveControlPointAction
import org.netbeans.api.visual.action.WidgetAction
import org.netbeans.api.visual.widget.Widget
import java.util.ArrayList
import scala.collection.JavaConversions._
import org.netbeans.api.visual.widget.LabelWidget
import org.netbeans.api.visual.layout.LayoutFactory

/**
 * @author rleys
 *
 */
class UITransition(val transition: projectfsmtrans, scene: Scene) extends FreeConnectionWidget(scene, 0, 0) {

  //-- Target and source UI
  var source = transition.start.targetObject.findBufferType[UIState].get
  var target = transition.end.targetObject.findBufferType[UIState].get

  //-- Connection
  this.setTargetAnchorShape(AnchorShape.TRIANGLE_FILLED);
  /*this.setTargetAnchor(AnchorFactory.createRectangularAnchor(target));
  this.setSourceAnchor(AnchorFactory.createRectangularAnchor(source));
  this.setTargetAnchor(AnchorFactory.createCircularAnchor(target,target.getBounds().width/2));
  this.setSourceAnchor(AnchorFactory.createCircularAnchor(source,source.getBounds().width/2));*/

  this.setTargetAnchor(AnchorFactory.createFreeRectangularAnchor(target, true));
  this.setSourceAnchor(AnchorFactory.createFreeRectangularAnchor(source, true));

  //-- Add control points action
  var addAction: AddRemoveControlPointAction = ActionFactory.createAddRemoveControlPointAction().asInstanceOf[AddRemoveControlPointAction]
  this.getActions.addAction(addAction)

  //-- Move control points action
  this.getActions.addAction(ActionFactory.createMoveControlPointAction(new MoveControlPointProvider {
    def locationSuggested(w: ConnectionWidget, index: Int, suggestedLocation: Point): java.util.List[Point] = {

      w.getControlPoints().get(index).setLocation(suggestedLocation)
      w.getControlPoints()

    }
  }))

  //-- Setup Control points
  //conn.setControlPointCutDistance(10)
  this.setPaintControlPoints(true)
  this.setControlPointShape(PointShape.SQUARE_FILLED_BIG)
  this.setRouter(RouterFactory.createFreeRouter())
  this.setRoutingPolicy(RoutingPolicy.UPDATE_END_POINTS_ONLY)

  //-- Add trackpoints
  var rl = new ArrayList[Point]();
  rl.add(this.source.getPreferredLocation())
  rl.addAll(transition.trackpoints.map(tp => new Point(tp.posx.toInt, tp.posy.toInt)).toList)
  rl.add(this.target.getPreferredLocation())
  setControlPoints(rl, false)

  //-- Add name label
  this.transition.name match {
    case null                          =>
    case name if (name.toString == "") =>
    case name =>
    
      //-- Create
      //----------
      var lbl = new LabelWidget(scene)
      this.addChild(lbl)
      lbl.setLabel(name)
      
      //-- Set Position or init it if not set
      (this.transition.textposx.toInt,this.transition.textposy.toInt) match {
        case (0,0) => 
          
          // Calculate middle of source and target by doing target-source to go in the right direction
          var middleX = (target.getPreferredLocation().getX() - source.getPreferredLocation().getX())/2
          var middleY = (target.getPreferredLocation().getY() - source.getPreferredLocation().getY())/2
          
          // Set From source position
          lbl.setPreferredLocation(new Point(source.getPreferredLocation().getX().toInt+middleX.toInt,source.getPreferredLocation().getY().toInt+middleY.toInt))
        case (x,y) => lbl.setPreferredLocation(new Point(x,y))
      }
      

      //-- Moving action
      //-----------------------
      lbl.getActions().addAction(
         ActionFactory.createMoveAction());

    //this.setConstraint(lbl, LayoutFactory.ConnectionWidgetLayoutAlignment.TOP_SOURCE, 50.0f)

  }

}