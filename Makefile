CC = g++
TARGET = nuovo
SRC = main.cpp app.cpp render.cpp text.cpp module.cpp controller.cpp sequencer.cpp
LFLAGS = -lSDL2 -lSDL2_ttf -lSDL2_mixer

$(TARGET) : $(SRC)
	@$(CC) -o $(TARGET) $(SRC) $(LFLAGS)
	@./$(TARGET)
