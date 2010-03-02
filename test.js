///js///////////////////////////////////////////////////////////////////////////
// Copyleft Schell Scivally Enterprise - Civilian Software
// All rights reversed
////////////////////////////////////////////////////////////////////////////////

var sys = require('sys'), rtaudio = require('./rtaudio_binding'), tools = require('./tools');
sys.puts("\nRtAudio Settings:\n");
sys.puts("  All APIs: " + tools.dump(rtaudio.apiMap));
sys.puts("  Compiled APIs:  " + rtaudio.getCompiledApis());
sys.puts("  Devices: " + tools.dump(rtaudio.enumerateDevices(), 1));
sys.puts("  Creating new RtAudio instance:");
var audio = rtaudio.New();
sys.puts("      Current API: " + audio.getCurrentApi());
sys.puts("      Connected Devices: " + audio.getDeviceCount());