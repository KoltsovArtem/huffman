TARGET = hw-02_huffman
CC = g++
CFLAGS = -Wall -Werror -Wextra

PREF_SRC = ./src/
PREF_OBJ = ./obj/

SRC = $(wildcard $(PREF_SRC)*.cpp)
OBJ = $(patsubst $(PREF_SRC)%.cpp, $(PREF_OBJ)%.o, $(SRC))

$(TARGET) : $(OBJ)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(OBJ) -I include -o $(TARGET)

$(PREF_OBJ)%.o : $(PREF_SRC)%.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -I include -o $@

.PHONY: clean
clean :
	rm $(TARGET) $(PREF_OBJ)*.o