CFLAGS = -lGL -lglfw -lGLEW -lSOIL

game: src/*.cpp
	g++ -o ExamCrusher src/*.cpp $(CFLAGS)

clean:
	rm ExamCrusher
