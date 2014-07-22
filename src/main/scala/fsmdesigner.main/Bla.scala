package fsmdesigner.main

object Bla extends App {

  var input = "";
  var inputFlt : java.lang.Double = 0.1;
  
  var test = 0.7999
  println(f"Read value(f): $test%.2f")
  
  var phase  : java.lang.Double = 0.0
  for ( i <- 0 to 20) {
    phase += inputFlt
    println(f"Read value(f): ${phase.toFloat}")
  }

  /*input = Console.readLine;

  try {
    inputFlt = java.lang.Double.parseDouble(input);
    //inputFlt = input.toFloat
  } catch {
    case _: Throwable => inputFlt = 0.0F;
  }

  if (input == "") {
    inputFlt = 0.0F;
  }
  
  var dres = inputFlt.toFloat
  println(s"Read value(s): $dres")
  println(f"Read value(f): $dres")*/

}