package fsmdesigner.ui.graph.actions

import org.netbeans.api.visual.action.WidgetAction
import fsmdesigner.ui.graph.fsm.FSMScene
import org.netbeans.api.visual.widget.Widget
import java.awt.event.KeyEvent

class FSMSceneUndoRedoKey(var scene: FSMScene) extends WidgetAction.Adapter {

  override def keyTyped(widget: Widget, event: WidgetAction.WidgetKeyEvent): WidgetAction.State = {

   
    var zCode = 'z'.toInt & 0x3F
     println(s"Got key" + event.getKeyChar().toInt+" Zcode is "+zCode+s"// can redo: ${scene.undoManager.canRedo()}")
    
    (scene.undoManager.canUndo(),scene.undoManager.canRedo(), event.isControlDown(), event.getKeyChar().toInt) match {
      case (true, _,true, 26) =>

        scene.undoManager.undo()

        WidgetAction.State.CONSUMED
        
      case (_,true,true,25) => 
        
        scene.undoManager.redo()

        WidgetAction.State.CONSUMED
        
      case _ => WidgetAction.State.REJECTED
    }

  }
  /*
  override def keyReleased( widget : Widget,event : WidgetAction.WidgetKeyEvent ): WidgetAction.State = {
    
    println(s"Got key2 "+event.getKeyChar())
    println(s"")
    event.getKeyChar() match {
        case 'z' if(event.isControlDown()) => 
          
          println(s"Can undo")
          scene.undoManager.undo()
          
          WidgetAction.State.CONSUMED
        case _ => 
          
          
          WidgetAction.State.REJECTED
      }
   // WidgetAction.State.REJECTED
  }*/

}