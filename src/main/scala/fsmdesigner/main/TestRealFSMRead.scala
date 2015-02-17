package fsmdesigner.main

import fsmdesigner.model.project
import ooxoo.extras.JoinBufferIO
import com.idyria.osi.ooxoo.core.buffers.structural.io.sax.StAXIOBuffer

object TestRealFSMRead extends App {

  //-- Project
  //-----------------
  
  var project = new project
  project.appendBuffer(new JoinBufferIO)
  project.appendBuffer(StAXIOBuffer(getClass.getClassLoader().getResource("smfu2_pipelined_128.pro.xml")))

  project.lastBuffer.streamIn
  
  //-- Make a few checks
  //firstState = reReadFsm.states.head
  //secondState = reReadFsm.states.last
  
  
  //-- Stats
  project.fsms.foreach {
    fsm => 
      
      println(s"Found FSM: "+fsm)
  }
  
}