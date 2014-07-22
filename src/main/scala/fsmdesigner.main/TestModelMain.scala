package fsmdesigner.main

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

object TestModelMain extends App {

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
  transition.target = firstState
  
  //-- Transition from two to two
  transition = new Transition
  secondState.transitions += transition
  transition.target = secondState
  
  var outString = StAXIOBuffer(fsm, true)

  println(s"FSM Out: " + outString)

  // Streamin
  //-------------------
  var reReadFsm = new Fsm
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

class JoinBufferTargetId(var target : JoinBufferTarget) extends XSDStringBuffer {


  data = this.hashCode().toHexString
  
  
  override def streamIn(du: DataUnit) = {

    
  // println(s"In Import data unit")
    //-- Keep parent behavior
    super.streamIn(du)

    //-- Register Join Target to available ones
    JoinBuffer.makeAvailable(data, target)

  }

  

}

trait JoinBufferTarget extends BaseBufferTrait {

  @xattribute
  var id = new JoinBufferTargetId(this)
  

}

class JoinBuffer[T <: JoinBufferTarget] extends BaseBufferTrait {

  var targetObject: T = _

  /**
   * Record Target Object
   */
  def apply(target: T): Unit = {

    targetObject = target

  }

  def apply() : T = targetObject
  
  override def streamIn(du: DataUnit) = {

    var id = du.value
    println(s"ID to narrow: " + id)

    JoinBuffer.narrow[T](id) match {
      case Some(result) =>
        println(s"--> Found")
        targetObject = result
      case None =>

        println(s"--> Waiting")
        JoinBuffer.onMatch("available.id") {
          case addedid if (id == addedid) =>
            println(s"--> Now available")
            targetObject = JoinBuffer.narrow[T](id).get
          case _ => 
        }

    }

    println("Done join buffer streamin")
    super.streamIn(du)
  }

  /**
   * Set the value of this buffer
   */
  override def streamOut(du: DataUnit) = {

    println(s"In StreamOut of JoiBuffer")

    //-- Set Value
    du.value = targetObject match {
      case null => "-1"
      case obj  => targetObject.id
    }

    super.streamOut(du)

  }

}
object JoinBuffer extends ListeningSupport {

  var joinsMap = Map[String, JoinBufferTarget]()

  def narrow[T <: JoinBufferTarget](id: String): Option[T] = {

    //-- Look into map
    joinsMap.get(id) match {

      //-- Target already registered -> return
      case Some(result) =>
        //joinsMap = joinsMap - id.toString
        Some(result.asInstanceOf[T])

      //-- Target Not Available,
      case None => None

    }

  }

  def makeAvailable[T <: JoinBufferTarget](id: String, target: T): Unit = {

    //-- Add
    joinsMap = joinsMap + (id -> target)

    //-- Signal
    @->("available.id", id)
    @->("available.object", target)
  }

  implicit def objectToJoinBuffer[T <: JoinBufferTarget](obj: T): JoinBuffer[T] = {
    var b = new JoinBuffer[T]
    b(obj)
    b
  }

}
