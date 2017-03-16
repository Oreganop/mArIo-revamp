#Makefile

CXX=g++
CXX_FLAGS=-std=gnu++11 -Wall -pedantic -g -O2
LIBS=#-lsfml-graphics -lsfml-window -lsfml-system

#Executable name
EXEC=genetics

#Directory Names
GA=Genetic_Algorithm
ANN=Network
OBJ=obj

$(info ***** Your friendly AI maker *****)

all: $(OBJ) $(SAVES_DIR) $(EXEC) 

$(OBJ):
	mkdir $(OBJ)

$(SAVES_DIR):
	mkdir Saves
	mkdir Saves/cur

$(EXEC): $(OBJ)/driver.o $(OBJ)/ga.o $(OBJ)/ann.o $(OBJ)/functions.o
	$(CXX) $(CXX_FLAGS) -o $(EXEC) \
		$(OBJ)/driver.o \
		$(OBJ)/ga.o \
	    $(OBJ)/ann.o \
 		$(OBJ)/functions.o \
		$(LIBS)

$(OBJ)/driver.o: $(GA)/driver.cpp $(GA)/ga.h
	$(CXX) $(CXX_FLAGS) -c -o $@ \
		$(GA)/driver.cpp 

$(OBJ)/ga.o: $(GA)/ga.cpp $(GA)/ga.h $(ANN)/ann.h
	$(CXX) $(CXX_FLAGS) -c -o $@ \
		$(GA)/ga.cpp

$(OBJ)/ann.o: $(ANN)/ann.cpp $(ANN)/ann.h $(ANN)/functions.h
	$(CXX) $(CXX_FLAGS) -c -o $@ \
		$(ANN)/ann.cpp 

$(OBJ)/functions.o: $(ANN)/functions.cpp $(ANN)/functions.h 
	$(CXX) $(CXX_FLAGS) -c -o $@ \
		$(ANN)/functions.cpp 

clean:
	rm -rf $(OBJ) $(EXEC)
