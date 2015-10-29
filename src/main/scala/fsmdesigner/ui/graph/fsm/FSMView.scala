// /**
//  *
//  */
// package fsmdesigner.ui.graph.fsm
// 
// import fsmdesigner.model.projectfsm
// import javafx.scene.layout.Pane
// import javafx.scene.layout.HBox
// import javafx.embed.swing.SwingNode
// import javafx.scene.layout.VBox
// import javafx.scene.control.Label
// import javax.swing.SwingUtilities
// 
// /**
//  *
//  * This Component contains the setup to view an FSM in JFX
//  *
//  * @author rleys
//  *
//  */
// class FSMView(val fsm: projectfsm) extends SwingNode {
// 
//   // Title
//   //--------------
//   /*var lbl = new Label
//   lbl.setText(fsm.__fname)
//   this.getChildren().add(lbl)*/
// 
//   // Create Scene
//   //------------------
//   var sn = new SwingNode
//   var scene = new FSMScene(fsm)
//   
//   SwingUtilities.invokeAndWait(new Runnable {
//     def run = {
//      
//     		setContent(scene.createView())
//    
//      /* sn.setContent(scene.createView())
//       sn.getContent().setSize(640,480)
//       sn.getContent().revalidate()
//       sn.setVisible(true)*/
//     }
//   })
//   sn.setStyle("-fx-background-color:red")
//   sn.prefHeight(480)
//   sn.prefWidth(640)
//   //this.getChildren().add(sn)
// 
//   
//   // sn.setStyle("-fx-background-color:red")
// 
//   // Init Scene
//   //----------------
// 
// }