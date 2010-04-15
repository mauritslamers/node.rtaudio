////////////////////////////////////////////////////////////////////////////////
// Copyleft 
//  |      _| |    |   
//    \    _|  _|    \ 
// _| _| _|  \__| _| _| 
// All rights reversed
// Written by Schell Scivally (efnx.com, happyfuntimehacking.com)
// Thu Apr 15 15:28:42 PDT 2010 
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

