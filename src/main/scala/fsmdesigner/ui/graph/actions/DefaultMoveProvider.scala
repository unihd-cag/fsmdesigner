/**
 *
 */
package fsmdesigner.ui.graph.actions

import org.netbeans.api.visual.action.MoveProvider
import org.netbeans.api.visual.widget.Widget
import java.awt.Point

/**
 * @author rleys
 *
 */
trait DefaultMoveProvider extends MoveProvider {

  var originalLoc : Point = _
  
  def getOriginalLocation(widget: Widget): Point = {
		widget.getPreferredLocation()
  }
  def movementFinished(widget: Widget): Unit = {

  }
  def movementStarted(widget: Widget): Unit = {
		originalLoc = widget.getPreferredLocation()
  }
  
  def setNewLocation(widget: Widget, location: Point): Unit = {
    //println(s"NEw loca: "+location+"//"+widget)
		//widget.setPreferredLocation(new Point(location.x-widget.getBounds().width/2,location.y-widget.getBounds().height/2))
    widget.setPreferredLocation(location)
  }

}