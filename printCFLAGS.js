////////////////////////////////////////////////////////////////////////////////
// Copyleft 
//  |      _| |    |   
//    \    _|  _|    \ 
// _| _| _|  \__| _| _| 
// All rights reversed
// Thu Apr 15 15:12:42 PDT 2010 
// Prints whatever nodes CFLAGS are to stdout
////////////////////////////////////////////////////////////////////////////////
var sys = require('sys'), spwn = require('child_process').spawn;

var varstring = '';
var nodeVars = spwn('node', ['--vars']);
nodeVars.stdout.addListener('data', function (out) {
    varstring += String(out);
});
nodeVars.addListener('exit', function (code) {
    sys.print(varstring.match(/NODE_CFLAGS: (.*)\n/)[1]); 
});