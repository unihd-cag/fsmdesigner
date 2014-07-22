package fsmdesigner.model

import com.idyria.osi.ooxoo.model._
import com.idyria.osi.ooxoo.model.out.markdown.MDProducer
import com.idyria.osi.ooxoo.model.out.scala.ScalaProducer

@producers(Array(
  new producer(value = classOf[ScalaProducer]),
  new producer(value = classOf[MDProducer])
))
object FSMModel extends ModelBuilder {

  parameter("scalaProducer.targetPackage", "fsmdesigner.model")

  // Generic Model
  //------------------------

  //-- FSM
  //-------------

  //-- Project
  //----------------
  "project" is {
    attribute("pname")
    attribute("version")

    "ToolsParameters" is {
      "Parameters" is {
        attribute("userid")

        "Parameter" multiple {
          attribute("key")
          ofType("cdata")
        }
      }
    }

    "fsm" multiple {

      // Properties
      //------------
      attribute("fname")
      attribute("resetstate") is classType("ooxoo.extras.JoinBuffer[projectfsmstate]")
      
      //-- IO
      //-------------
      "inputnames" is {

        "inputname" multiple ofType("cdata")
      }
      "outputnames" is {

        "ouputname" multiple ofType("cdata")
      }

      //-- States
      //--------------
      "state" multiple {

        withTrait("ooxoo.extras.JoinBufferTarget")

        attribute("posx") ofType ("float")
        attribute("posy") ofType ("float")
        attribute("color") ofType ("long")

        "sname" is ofType("string")
        "output" is ofType("string")
      }

      //-- Transitions
      //-----------------
      "trans" multiple {

        withTrait("ooxoo.extras.JoinBufferTarget")

        //-- Text
        attribute("textposx") ofType ("float")
        attribute("textposy") ofType ("float")
        "name" is ofType("cdata")
        
        //-- From - To
        "start" is classType("ooxoo.extras.JoinBuffer[projectfsmstate]")
        "end" is classType("ooxoo.extras.JoinBuffer[projectfsmstate]")

        //-- Conditions
        "condition" multiple {
          "cname" is ofType("cdata")
          "input" is ofType("string")

        }

        //-- Trackpoints
        "trackpoint" multiple {
          attribute("posx") ofType ("float")
          attribute("posy") ofType ("float")
        }
      }

    }
  }

  // Generic Impl
  //-------------------
  /*
    "Fsm" is {

        withDescription("""Base FSM Description""")

        attribute("name")

        "State" multiple {


            "Transition" multiple {

                //-- Direct target to a state, or using a Link jump
                attribute("linkTo")
                attribute("target")

                //-- Conditions
                "Condition" multiple {

                }

            }
        }





    }
*/
}
