///js///////////////////////////////////////////////////////////////////////////
// Copyleft Schell Scivally Enterprise - Civilian Software
// All rights reversed
////////////////////////////////////////////////////////////////////////////////

var sys = require('sys'), rtaudio = require('./rtaudio_binding');

sys.puts("\nRtAudio Settings:\n");
sys.puts("  All APIs: " + sys.inspect(rtaudio.apiMap));
sys.puts("  Compiled APIs:  " + rtaudio.getCompiledApis());
sys.puts("  Devices: " + sys.inspect(rtaudio.enumerateDevices()));
sys.puts("  Creating new RtAudio instance:");
var audio = rtaudio.New();
sys.puts("  " + audio);
sys.puts("      Current API: " + audio.getCurrentApi());
sys.puts("      Connected Devices: " + audio.getDeviceCount());
sys.puts("      Device 0 info: " + sys.inspect(audio.getDeviceInfo(0)));
sys.puts("      Default output device:  "); sys.puts(sys.inspect(audio.getDefaultOutputDevice()));
sys.puts("      Default input device:   ");
sys.puts(sys.inspect(audio.getDefaultInputDevice()));

