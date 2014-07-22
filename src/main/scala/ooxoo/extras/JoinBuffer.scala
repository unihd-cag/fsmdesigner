package ooxoo.extras

import com.idyria.osi.ooxoo.core.buffers.structural.DataUnit
import com.idyria.osi.ooxoo.core.buffers.structural.BaseBufferTrait
import com.idyria.osi.ooxoo.core.buffers.datatypes.XSDStringBuffer
import com.idyria.osi.ooxoo.core.buffers.structural.io.BaseIOBuffer
import com.idyria.osi.ooxoo.core.buffers.structural.xattribute
import com.idyria.osi.tea.listeners.ListeningSupport


class JoinBufferTargetId(var target : JoinBufferTarget) extends XSDStringBuffer {


  data = this.hashCode().toHexString
  
  
  override def streamIn(du: DataUnit) = {

    
  // println(s"In Import data unit with chain: "+this.getNextBuffer)
    //-- Keep parent behavior
    super.streamIn(du)

    //-- Register Join Target to available ones
    this.findBufferType[JoinBufferIO] match {
     case Some(joinIO) => joinIO.makeAvailable(data, target)
     case _ => 
   }
  //  this.lastBuffer.getPreviousBuffer.asInstanceOf[JoinBufferIO]

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

    du.value match {
      case null => 
        
      case v =>
        
          var id = du.value
   // println(s"ID to narrow: " + id)

    //-- Get Join IO
    var joinio = this.findBufferType[JoinBufferIO] match {
      case Some(io) => io
      case _ => throw new RuntimeException("Cannot use a JoinBuffer without JoinBufferIO in the IO Chain for streamin")
    }
    
    joinio.narrow[T](id) match {
      case Some(result) =>
        println(s"--> Found")
        targetObject = result
      case None =>

       // println(s"--> Waiting")
        joinio.onMatch("available.id") {
          case addedid if (id == addedid) =>
       //     println(s"--> Now available")
            targetObject = joinio.narrow[T](id).get
          case _ => 
        }

    }

    //println("Done join buffer streamin")
    }
    
  
    super.streamIn(du)
  }

  /**
   * Set the value of this buffer
   */
  override def streamOut(du: DataUnit) = {

    //println(s"In StreamOut of JoinBuffer")

    //-- Set Value
    du.value = targetObject match {
      case null => "-1"
      case obj  => targetObject.id
    }

    super.streamOut(du)

  }

}
class JoinBufferIO extends BaseIOBuffer with ListeningSupport   {

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

  override def cloneIO = this
 

}
object JoinBufferIO {
  
   implicit def objectToJoinBuffer[T <: JoinBufferTarget](obj: T): JoinBuffer[T] = {
    var b = new JoinBuffer[T]
    b(obj)
    b
  }
} 