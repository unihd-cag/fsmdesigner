package fsmdesigner.main.io

import com.idyria.osi.ooxoo.core.buffers.structural.XList
import com.idyria.osi.ooxoo.core.buffers.structural.xelement
import com.idyria.osi.ooxoo.core.buffers.datatypes.XSDStringBuffer
import com.idyria.osi.ooxoo.core.buffers.structural.xattribute
import com.idyria.osi.ooxoo.core.buffers.structural.ElementBuffer
import com.idyria.osi.ooxoo.core.buffers.structural.BaseBufferTrait
import com.idyria.osi.ooxoo.core.buffers.structural.DataUnit
import scala.language.implicitConversions
import com.idyria.osi.ooxoo.core.buffers.structural.io.sax.StAXIOBuffer
import com.idyria.osi.tea.listeners.ListeningSupport
import com.idyria.osi.ooxoo.core.buffers.structural.io.IOBuffer
import com.idyria.osi.ooxoo.core.buffers.structural.io.BaseIOBuffer
import ooxoo.extras.JoinBufferIO
import ooxoo.extras.JoinBuffer
import ooxoo.extras.JoinBufferTarget

object TestModelIO extends App {

  implicit def objectToJoinBuffer[T <: JoinBufferTarget](obj: T): JoinBuffer[T] = {
    var b = new JoinBuffer[T]
    b(obj)
    b
  }
  
  println("Testing Model")

  // Streamout 
  //----------------
  var fsm = new Fsm
  fsm.states += new State
  fsm.states += new State

  var firstState = fsm.states.head
  var secondState = fsm.states.last

  //-- Transition from one to two
  var transition = new Transition
  firstState.transitions += transition

  transition.target = secondState  

  //-- Transition from two to one
  transition = new Transition
  secondState.transitions += transition
  
  //transition.target = firstState
  
  //-- Transition from two to two
  transition = new Transition
  secondState.transitions += transition
  
 // transition.target = secondState
  
  var outString = StAXIOBuffer(fsm, true)

  println(s"FSM Out: " + outString)

  // Streamin
  //-------------------
  var reReadFsm = new Fsm
  reReadFsm.appendBuffer(new JoinBufferIO)
  reReadFsm.appendBuffer(StAXIOBuffer(outString))

  reReadFsm.lastBuffer.streamIn
  
  //-- Make a few checks
  firstState = reReadFsm.states.head
  secondState = reReadFsm.states.last
  
  assert(firstState.transitions.head.target() == secondState)

}

// Model Test 
//-----------------

@xelement(name = "Fsm")
class Fsm extends ElementBuffer {

  @xattribute
  var name: XSDStringBuffer = null

  @xelement(name = "State")
  var states = XList[State]

}

class State extends ElementBuffer with JoinBufferTarget {

  @xelement(name = "Transition")
  var transitions = XList[Transition]

}

class Transition extends ElementBuffer {

  @xattribute
  var target: JoinBuffer[State] = null

}

// Join Buffer Test
//---------------------

