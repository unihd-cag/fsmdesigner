// /**
//  *
//  */
// package fsmdesigner.ui
// 
// import javafx.stage.Stage
// import javafx.scene.Scene
// import javafx.scene.layout.Pane
// import javafx.scene.control.TabPane
// import javafx.fxml.FXMLLoader
// import javafx.scene.web.WebView
// import javafx.scene.control.ScrollPane
// import scala.io.Source
// import fsmdesigner.model.project
// import fsmdesigner.ui.graph.fsm.FSMView
// import javafx.scene.control.Tab
// 
// /**
//  * @author rleys
//  *
//  */
// class FSMDesignerStage extends Stage {
// 
//   /////////////////////////////////
//   // INit
//   ///////////////////////////////
//   
//   //var stage = new Stage
//   setTitle("FSMDesigner 6")
//   setWidth(1280)
//  setHeight(1024)
// 
//   // Load FXML
//   //---------------
//   //var scene = new Scene
//   var loader = new FXMLLoader
//   var pane = loader.load(getClass.getClassLoader().getResourceAsStream("fsmdesigner/ui/MainStage.fxml")).asInstanceOf[Pane]
// 
//   var scene = new Scene(pane)
//   setScene(scene)
//   //pane.requestLayout()
// 
//   // Search certain components from FXML
//   //---------------
//   var tabsPane = pane.lookup("#tabs").asInstanceOf[TabPane]
//   
//   
//   // Load Welcome page
//   //-------------------
//   /*println(s"Found: "+pane.lookup("#tabs").asInstanceOf[TabPane].getTabs.get(0).getGraphic()) 
//     sys.exit*/
// 
//   var welcomewww = pane.lookup("#tabs").asInstanceOf[TabPane].getTabs.get(0).getContent().asInstanceOf[ScrollPane].getContent().lookup("#www").asInstanceOf[WebView]
//   //welcomewww.getEngine().load("http://www.google.com")
//   // println(welcomewww.getParent().get)
//   // welcomewww.setPrefSize(640, 480)
//   //welcomewww.setMinSize(640, 480)
//   //println(welcomewww.getBoundsInParent())
// 
//   //pane.requestLayout()
// 
//   //-- Load
//   // var welcomePage = Source.fr
//   welcomewww.getEngine.loadContent(Source.fromURL(getClass.getClassLoader().getResource("welcome/welcome.html"), "UTF-8").mkString)
// 
//   /////////////////////////////////
//   // Control
//   /////////////////////////////////
//   
//   // Project Management
//   //------------------------
//   
// 
//   def openProject(project: project) = {
//     
//     //-- Load FSMs
//     project.fsms.map(new FSMView(_)).foreach {
//       fsmView => 
//         
//         //-- Create Tab
//         var fsmTab = new Tab
//         fsmTab.setText(fsmView.fsm.fname)
//  
//         //-- Create Scrollpane for FSMView
//         var sc = new ScrollPane
//         sc.setFitToHeight(true)
//         sc.setFitToWidth(true)
//         sc.setContent(fsmView)
//         fsmTab.setContent(sc)
//  
//         
//         
//         //--
//         tabsPane.getTabs().add(fsmTab)
//     }
//     
//     
//     //-- Enable Buttons
//   }
//   
// }