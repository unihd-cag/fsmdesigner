// package fsmdesigner.nbtry
// 
// import java.awt.Color
// import java.awt.Dimension
// import java.awt.Graphics2D
// import java.awt.Point
// import java.awt.Rectangle
// import scala.collection.JavaConversions._
// import org.netbeans.api.visual.action.ActionFactory
// import org.netbeans.api.visual.action.ConnectProvider
// import org.netbeans.api.visual.action.ConnectorState
// import org.netbeans.api.visual.action.MoveControlPointProvider
// import org.netbeans.api.visual.action.TextFieldInplaceEditor
// import org.netbeans.api.visual.anchor.AnchorFactory
// import org.netbeans.api.visual.anchor.AnchorShape
// import org.netbeans.api.visual.anchor.PointShape
// import org.netbeans.api.visual.graph.GraphScene
// import org.netbeans.api.visual.router.RouterFactory
// import org.netbeans.api.visual.widget.ConnectionWidget
// import org.netbeans.api.visual.widget.FreeConnectionWidget
// import org.netbeans.api.visual.widget.LabelWidget
// import org.netbeans.api.visual.widget.LayerWidget
// import org.netbeans.api.visual.widget.Scene
// import org.netbeans.api.visual.widget.Widget
// import com.idyria.osi.vui.javafx.JavaFXRun
// import javafx.embed.swing.SwingNode
// import javafx.scene.control.ScrollPane
// import javafx.stage.Stage
// import javax.swing.SwingUtilities
// 
// /**
//  * State Widget
//  */
// class StateWidget(var scene: Scene) extends Widget(scene) {
// 
//   //-- Create Ellipse Widget
//   var ellipse = new Widget(scene) {
// 
//     this.setPreferredSize(new Dimension(20, 20))
// 
//     override def calculateClientArea(): Rectangle = {
// 
//       new Rectangle(0, 0, 20, 20)
// 
//     }
// 
//     override def paintWidget = {
// 
//       //println(s"Painting state widget "+hashCode())
//       var g2 = getGraphics().create.asInstanceOf[Graphics2D]
//       //g2.save
// 
//       //-- Set Painter
//       g2.setBackground(Color.BLACK)
//       g2.setColor(Color.BLACK)
// 
//       g2.drawOval(0, 0, 20, 20)
//       g2.fillOval(0, 0, 20, 20)
// 
//       g2.dispose
// 
//       super.paintWidget
//     }
//   }
//   addChild(ellipse)
// 
//   //-- Create Name Edit 
//   /*var nf = new JTextField
//   nf.setPreferredSize(new Dimension(50,20))
//   var nameField = new ComponentWidget(scene,nf)
//   nameField.setPreferredLocation(new Point(0,25))
//   
//   nameField.getActions().addAction(ActionFactory.createEditAction (new EditProvider {
//     def edit(w:Widget) : Unit = {
//       println(s"Editing Label")
//     }
//   }))
//   addChild(nameField)*/
// 
//   //-- Create Label Widget
//   var label = new LabelWidget(scene)
//   label.setLabel("State")
//   label.setPreferredLocation(new Point(0, 30))
//   /*  label.getActions().addAction(ActionFactory.createEditAction (new EditProvider {
//     def edit(w:Widget) : Unit = {
//       println(s"Editing Label")
//     }
//   }))*/
//   label.getActions().addAction(ActionFactory.createInplaceEditorAction(new TextFieldInplaceEditor {
// 
//     def isEnabled(widget: Widget): Boolean = {
//       return true;
//     }
//     def getText(widget: Widget): String = {
//       return widget.asInstanceOf[LabelWidget].getLabel
//     }
//     def setText(widget: Widget, text: String): Unit = {
//       widget.asInstanceOf[LabelWidget].setLabel(text);
//     }
//   }))
//   addChild(label)
// 
// }
// 
// /**
//  * FSM Scene
//  */
// class FSMScene extends GraphScene[String, String] {
// 
//   //-- Create layers
//   var mainLayer = new LayerWidget(this);
//   var connectionLayer = new LayerWidget(this);
//   var interactionLayer = new LayerWidget(this);
//   addChild(mainLayer)
//   addChild(connectionLayer)
//   addChild(interactionLayer)
// 
//   //-- Standard Components
//   //----------------------------
// 
//   //-- Satellite view
//   /*var satelliteView = this.createSatelliteView()
//   addChild(new ComponentWidget(this,satelliteView))*/
// 
//   //-- Add Scene Actions
//   //--------------------
// 
//   //-- Zoom
//   getActions().addAction(ActionFactory.createZoomAction());
// 
//   //-- Connection Provider
//   //--------------
// 
//   class MyConnectProvider extends ConnectProvider {
// 
//     /**
//      * Say if we can start connection from this widget
//      */
//     def isSourceWidget(source: Widget): Boolean = {
// 
//       println(s"In is source widget")
//       source match {
//         case null => false
//         case _    => true
//       }
//     }
// 
//     /**
//      * Say if we can connect to the target widget
//      */
//     def isTargetWidget(src: Widget, trg: Widget): ConnectorState = {
// 
//       trg match {
//         case null       => ConnectorState.REJECT
//         case trg: Scene => ConnectorState.REJECT
//         case _          => ConnectorState.ACCEPT
//       }
// 
//     }
// 
//     def hasCustomTargetWidgetResolver(scene: Scene): Boolean = {
//       return false;
//     }
// 
//     def resolveTargetWidget(scene: Scene, arg1: Point): Widget = {
//       return null;
//     }
// 
//     def createConnection(source: Widget, target: Widget): Unit = {
// 
//       var conn = new FreeConnectionWidget(FSMScene.this);
//       conn.setTargetAnchorShape(AnchorShape.TRIANGLE_FILLED);
//       conn.setTargetAnchor(AnchorFactory.createRectangularAnchor(target));
//       conn.setSourceAnchor(AnchorFactory.createRectangularAnchor(source));
// 
//       //-- Setup Control points
//       conn.setControlPointCutDistance(5)
//       conn.setPaintControlPoints(true)
//       conn.setControlPointShape(PointShape.SQUARE_FILLED_BIG)
//       conn.setRouter(RouterFactory.createFreeRouter())
// 
//       //-- Add control points
//       conn.getActions.addAction(ActionFactory.createAddRemoveControlPointAction())
// 
//       //-- Move control points
//       conn.getActions.addAction(ActionFactory.createMoveControlPointAction(new MoveControlPointProvider {
//         def locationSuggested(w: ConnectionWidget, index: Int, suggestedLocation: Point): java.util.List[Point] = {
// 
//           w.getControlPoints().get(index).setLocation(suggestedLocation)
//           w.getControlPoints()
// 
//         }
//       }))
// 
//       connectionLayer.addChild(conn);
//     }
// 
//   }
// 
//   protected def attachNodeWidget(arg0: String): Widget = {
// 
//     //-- Create Widget
//     //--------------------
//     var lw = new StateWidget(this)
//     //lw.setLabel(arg0)
// 
//     mainLayer.addChild(lw)
// 
//     //-- Connection Action
//     //----------------------
//     lw.getActions().addAction(
//       ActionFactory.createExtendedConnectAction(
//         connectionLayer, new MyConnectProvider()));
// 
//     //-- Moving action
//     //-----------------------
//     lw.getActions().addAction(
//       ActionFactory.createAlignWithMoveAction(
//         mainLayer, interactionLayer,
//         ActionFactory.createDefaultAlignWithMoveDecorator()));
// 
//     lw
//   }
// 
//   protected def attachEdgeWidget(arg0: String): Widget = {
//     throw new UnsupportedOperationException("Not supported yet.");
//   }
// 
//   protected def attachEdgeSourceAnchor(arg0: String, arg1: String, arg2: String) = {
//     throw new UnsupportedOperationException("Not supported yet.");
//   }
// 
//   protected def attachEdgeTargetAnchor(arg0: String, arg1: String, arg2: String) = {
//     throw new UnsupportedOperationException("Not supported yet.");
//   }
// }
// 
// object FSMDesignerNBTry extends App {
// 
//   println(s"Standard Swing try")
//   /*
//   //-- Main Frame
//   //----------------
//   var frame = new JFrame("Swing test")
//   frame.setSize(1024, 768)
//   frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE)
// 
//   //-- Scroll pane for scene
//   var sp = new JScrollPane
//   frame.setContentPane(sp)
// 
//   //-- Visual Scene
//   //----------------------
//   var fsmscene = new FSMScene
//   sp.setViewportView(fsmscene.createView())
//   
// 
// 
//   fsmscene.addNode("A").setPreferredLocation(new Point(100,100))
//   fsmscene.addNode("B").setPreferredLocation(new Point(200,200))
// 
//   frame.show()*/
// 
//   JavaFXRun.onJavaFX {
// 
//     var stage = new Stage
//     stage.setTitle("FSMDesigner 6")
//     stage.setWidth(1280)
//     stage.setHeight(1024)
// 
//     //-- Scene
//     var sn = new SwingNode
// var fsmscene = new FSMScene
//         sn.setContent(fsmscene.createView())
//     SwingUtilities.invokeLater(new Runnable {
//       def run = {
// 
//         
//         /*sn.getContent().setSize(640,480)
//       sn.getContent().revalidate()
//       sn.setVisible(true)*/
// 
//         fsmscene.addNode("A").setPreferredLocation(new Point(100, 100))
//         fsmscene.addNode("B").setPreferredLocation(new Point(200, 200))
//       }
//     })
//     //  sn.setContent(fsmscene.createView())
// 
//     //-- SP
//     var sc = new ScrollPane
//     sc.setFitToHeight(true)
//     sc.setFitToWidth(true)
//     sc.setContent(sn)
// 
//     var scene = new javafx.scene.Scene(sc)
//     stage.setScene(scene)
// 
//     stage.show
//   }
// 
// }