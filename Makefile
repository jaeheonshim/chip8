chip8:
	g++ -g \
	`fltk-config --cxxflags` \
	src/main.cpp src/chip8.cpp src/ui.cpp \
	-o chip8 \
	`fltk-config --ldflags`
