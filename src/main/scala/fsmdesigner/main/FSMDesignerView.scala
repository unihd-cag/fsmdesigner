package fsmdesigner.main

import com.idyria.osi.vui.html.standlone.DefaultStandaloneHTMLBuilder
import com.idyria.osi.vui.html.standlone.StandaloneHTMLView
import com.idyria.osi.vui.html.basic.BasicHTMLView

/**
 * This is the main View for FSM designer
 * A View represents a UI Component. In this case, it will be the main GUI.
 *
 * The View API is designed to support Navigation, composition, automatic reload etc...
 *
 * The FSMDesignerView inherits the BasicHTMLView to be a view...and the DefaultStandaloneHTML Builder to have access
 * to the HTML building functions
 *
 * TheFSMDesignerView gets a Standalone HTML Builder
 * It is a standard HTML builder with overrides so that objects created answer to the Standalone API
 * The Data types seen are however standard HTML, which is ok to build generic applications
 */
class FSMDesignerView extends BasicHTMLView with DefaultStandaloneHTMLBuilder {

  /**
   * The content method is used to set a closure that will produce this view's UI on request
   * It is supposed to return an HTML node because the View type inherited requires production of an HTML node
   */
  this.content {
    
    //-- Let's create a normal HTML
    html {
      head {
        
        // Here we can link Stylesheets and Scripts
      }
      
      body {
        
        // Add A Title
        h1("FSM Designer") {
          
        }
        
        // Here you can create HTML
        
      }
    }
  }
  
}