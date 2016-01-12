package fsmdesigner.main

import com.idyria.osi.vui.html.standalone.StandaloneBasicHTMLBuilderTrait
import com.idyria.osi.vui.html.standlone.DefaultStandaloneHTMLBuilder
import com.idyria.osi.vui.implementation.javafx.JavaFXRun
import com.idyria.osi.vui.core.definitions.VUIBuilder

/**

 * 
 * The HTML API in VUI is a standalone API
 * We need the standard VUI Builder trait to have access to the classical GUI builder functions (window etc...)
 * 
 */
object FSMDesigner extends App with DefaultStandaloneHTMLBuilder with VUIBuilder {
  
  // FSM Designer Main
  //----------------------
  
  
  // Create A Web View Using VUI2 API
  // For now the code is quite explicit to learn 
  //------------------------
  
  //-- The GUI Code runs in a JavaFXRun so that the underlying toolkit will be JavaFX
  //-- This syntax is required because JavaFX startup code structure is not very flexible
  JavaFXRun.onJavaFX {
    
    // The Normal VUIBuilder has a rather explicit API: call a function to create an object and pass a closure to configure the created object
    frame {
      f => 
        f.title ="FSM Designer"
        f.size = (1024,768)
        
        //-- Create A Web browser
        var web = webbrowser 
        f <= web
        
        //-- Now we can add the main FSMDesigner View to the web browser
        //-- We can just instantiate the object, or call upon the embedded compiler
        var view = new FSMDesignerView
        web.view = view
        
        f.visible = true
        
    }
  }
}