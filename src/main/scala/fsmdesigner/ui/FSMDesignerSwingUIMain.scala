package fsmdesigner.ui

import java.io.File
import com.idyria.osi.vui.lib.gridbuilder.GridBuilder
import fsmdesigner.model.project
// import javafx.embed.swing.JFXPanel
// import javafx.scene.Scene
// import javafx.scene.control.ScrollPane
// import javafx.scene.web.WebView
import ooxoo.extras.JoinBufferIO
import com.idyria.osi.ooxoo.core.buffers.structural.io.sax.StAXIOBuffer
import fsmdesigner.ui.graph.fsm.FSMScene
import com.idyria.osi.vui.impl.swing.builders.SwingNodeWrapper
// import javafx.application.Platform
import scala.io.Source

object FSMDesignerSwingUIMain extends App with GridBuilder {

  var ui = frame 
  ui {
    f =>
      f title ("FSMDesigner 6")
      f size (1280, 1024)

      var tabPane = tabpane
      
      f.onWith("openProject") {
        f: File =>

          //-- parse
          var project = new project
          project.appendBuffer(new JoinBufferIO)
          project.appendBuffer(StAXIOBuffer(f.toURI().toURL()))

          project.lastBuffer.streamIn

          //-- Pass to Gui
          openProject(project)
      }

      
      def openProject(p: project) = {

        //-- Load FSMs
        p.fsms.foreach {
          fsm =>

            tabPane.addTab(fsm.fname) {
 
                scrollpane {
                  sp => 
                    var v = new FSMScene(fsm).createView()
                    //v.grabFocus()
                    sp <= SwingNodeWrapper(v)
                }
            }
            /*//-- Create Tab
            var fsmTab = new Tab
            fsmTab.setText(fsmView.fsm.fname)

            //-- Create Scrollpane for FSMView
            var sc = new ScrollPane
            sc.setFitToHeight(true)
            sc.setFitToWidth(true)
            sc.setContent(fsmView)
            fsmTab.setContent(sc)

            //--
            tabsPane.getTabs().add(fsmTab)*/
        }

      }

      f <= grid {

        "Toolbar" row {

          var open = button("Open")

        }

        "Tabs" row {

          tabPane {
            tp =>

              tp(expand)
              //-- Welcome Tab
//               tp.addTab("Welcome") {
// 
//                 //-- Create WWW JFX View
//                 var wwwPanel = new JFXPanel
// 
//                 Platform.runLater(new Runnable {
//                   def run = {
//                     var welcomeBrowser = new WebView
//                     welcomeBrowser.getEngine.loadContent(Source.fromURL(getClass.getClassLoader().getResource("welcome/welcome.html"), "UTF-8").mkString)
// 
//                     wwwPanel.setScene(new Scene(welcomeBrowser))
//                   }
//                 })
// 
//                 //-- Add to Scroll pane for tab
//                 scrollpane {
//                   sp => sp <= SwingNodeWrapper(wwwPanel)
//                 }
// 
//               }
          }

        }

      }

  }

  ui.show
  ui.@->("openProject",new File("src/test/scala/smfu2_pipelined_128.pro.xml"))
  /*
  // Load Standard project
  //---------------------------

  //-- parse
  var project = new project
  project.appendBuffer(new JoinBufferIO)
  project.appendBuffer(StAXIOBuffer(getClass.getClassLoader().getResource("smfu2_pipelined_128.pro.xml")))

  project.lastBuffer.streamIn

  //-- Pass to Gui
  openProject(project)

  ui.show

  JavaFXRun.onJavaFX {

    // Create Frame
    //-----------------
    var ui = new FSMDesignerStage
    ui.show()

    println(s"load project now")

    // Load Standard project
    //---------------------------

    //-- parse
    var project = new project
    project.appendBuffer(new JoinBufferIO)
    project.appendBuffer(StAXIOBuffer(getClass.getClassLoader().getResource("smfu2_pipelined_128.pro.xml")))

    project.lastBuffer.streamIn

    //-- Pass to Gui
    ui.openProject(project)

  }*/
}