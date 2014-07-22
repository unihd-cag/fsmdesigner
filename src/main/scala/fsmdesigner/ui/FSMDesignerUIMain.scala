package fsmdesigner.ui

import javafx.stage.Stage
import javafx.scene.Scene
import javafx.fxml.FXMLLoader
import javafx.scene.layout.Pane
import com.idyria.osi.vui.javafx.JavaFXRun
import javafx.scene.web.WebView
import javafx.scene.control.TabPane
import javafx.scene.control.ScrollPane
import java.io.File
import scala.io.Source
import fsmdesigner.model.project
import ooxoo.extras.JoinBufferIO
import com.idyria.osi.ooxoo.core.buffers.structural.io.sax.StAXIOBuffer

object FSMDesignerUIMain extends App {

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
    
  }
}