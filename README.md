to compile it 
g++ test.cpp -o test $(sdl2-config --cflags --libs) -lGLEW -lGL
