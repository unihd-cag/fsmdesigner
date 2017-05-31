
// Items Sorting
// Sort All Outputs in a map
//--------------------------
var allItems = this.scene.items();
var stateItems;
var outputsMap;

//-- Sort All State Items
for (var it = allItems.begin();it!=allItems.end();it++) {

	if (it.type()==FSMGraphicsItem<>::FSMGraphicsItemTypes.STATEITEM) {
		
		stateItems.append(FSMGraphicsItem<>::toStateItem(*it));
		
		
		//-- Do we find Output in map
		if (map[(*it)->getModel()->getOutput()]==NULL) {
			
			//-- No, it is unique up to now
			map[(*it)->getModel()->getOutput()] = *it;
			
		} else {
			
			//-- Already found, put an error to the one in the map, and the processed one
			
			//-- Create new error, or use one already on the item in the map
			
			qDebug() << "Found two states with same output";
			return false;
		}
		
		
	}
}




return true;