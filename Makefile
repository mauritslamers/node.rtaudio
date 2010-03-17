rtaudio_binding.node: rtaudio_binding.o Makefile
	g++ -bundle -bundle_loader /usr/local/bin/node -o rtaudio_binding.node rtaudio_binding.o -L./rtaudio -lrtaudio -lpthread -framework CoreAudio -framework CoreFoundation
	
rtaudio_binding.o: rtaudio.cc Makefile
	g++ `/usr/local/bin/node --cflags` -I./rtaudio -I. rtaudio.cc -c -o rtaudio_binding.o
	
clean:
	rm -f ./*.o ./*.node