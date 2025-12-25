chip8:
	g++ -g \
	`fltk-config --cxxflags` \
	src/main.cpp src/chip8.cpp src/ui.cpp src/runner.cpp src/audio.cpp \
	libs/miniaudio.c \
	-I libs \
	-o chip8 \
	`fltk-config --ldflags`
