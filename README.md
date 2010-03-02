node.rtaudio
=============
RtAudio bindings for node.js - these bindings are premature, i've only made wrappers for two (non sound making) functions in the RtAudio class.
install and run
---------------
install [node](http://github.com/ry/node "node at github") 
compile [RtAudio](http://www.music.mcgill.ca/~gary/rtaudio/ "The RtAudio Home Page") into librtaudio.a (i've done this step for you OS X users out there) 
place your compiled librtaudio.a in the rtaudio/ folder 
make 
node test.js
