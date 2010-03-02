///js///////////////////////////////////////////////////////////////////////////
// Copyleft Schell Scivally Enterprise - Civilian Software
// All rights reversed
////////////////////////////////////////////////////////////////////////////////

/**
 *	Lists the (at least enumerable) properties of an object.
 *
 *	@param	obj	the object to list
 *	@param	lvl	the level to print at (num tabs)
 *	@param	pf	whether or not to print function code
 *	@author	Schell Scivally
 *	@since	Wed Feb 24 11:10:24 PST 2010
 */
this.dump = function (obj, lvl, pf) {

	if(lvl == null)
		lvl = 0;
	if(pf == null)
		pf = false;

	var s = "";

	//The padding given at the beginning of the line.
	var tabs = "	";
	var i = lvl;
	while(i-- > 0)
		tabs += "	";
	if(obj === null)
		s += "null\n";
	else if (obj.constructor.toString().indexOf("Array") == -1)
		s += obj.toString() + "\n";
	else
		s += "[array]\n";
	if( typeof(obj) == 'object') {
		for(var item in obj) {
			var value = obj[item];

			if(typeof(value) == 'object') {
				s += tabs + item + " => ";
				s += this.dump(value, lvl + 1, pf);
			} else {
				s += tabs + item + " => " + this.dump(value, 0, pf) + "\n";
			}
		}
	} else {
		var type = typeof(obj);
		if(type != 'function')
			s = "["+ typeof(obj) + " '" + obj + "']";
		else if(type == 'function' && pf)
			s = "[function] : \n\n" + obj + "\n";
		else
			s = "[function]";
	}
	return s;
}