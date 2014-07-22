package fsmdesigner.ui.undoredo


// Special Property Change


trait UndoRedoAction {
  
  
  def doRedo
  
  def doUndo
}


class AddRemoveState {
  
}

class ChangeStateName {
  
}

class ChangeStateColor



