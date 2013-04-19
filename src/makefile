
objects = main.o ALConstraintGraph.o \
		  ConstraintNetwork.o FullBitStringTable.o \
		  FullCompositionTable.o FullInverseTable.o \
		  LanguageModel.o Vertex.o \
		  Bridge.o util.o \
		  ConnectComponent.o WeakConnectComponent.o \
		  MultiCSPFile.o

main :  $(objects)
	g++ -o main $(objects)

main.o : main.cpp ALConstraintGraph.h \
		ConstraintNetwork.h FullBitStringTable.h \
		FullCompositionTable.h FullInverseTable.h \
		LanguageModel.h Vertex.h \
		Bridge.h util.h \
		ConnectComponent.h WeakConnectComponent.h \
		MultiCSPFile.h
	g++ -c main.cpp

MultiCSPFile.o : MultiCSPFile.cpp MultiCSPFile.h ALConstraintGraph.h 
	g++ -c MultiCSPFile.cpp

WeakConnectComponent.o : WeakConnectComponent.h WeakConnectComponent.cpp \
		ConstraintNetwork.h ALConstraintGraph.h util.h
	g++ -c WeakConnectComponent.cpp

FullBitStringTable.o : FullBitStringTable.cpp FullBitStringTable.h BitStringTable.h
	g++ -c FullBitStringTable.cpp

FullInverseTable.o : FullInverseTable.cpp FullInverseTable.h InverseTable.h
	g++ -c FullInverseTable.cpp

FullCompositionTable.o : FullCompositionTable.cpp FullCompositionTable.h CompositionTable.h
	g++ -c FullCompositionTable.cpp

LanguageModel.o : LanguageModel.cpp LanguageModel.h Constraint.h CompositionTable.h InverseTable.h BitStringTable.h
	g++ -c LanguageModel.cpp

ConstraintNetwork.o : ConstraintNetwork.h ConstraintNetwork.cpp  Triple.h  Constraint.h util.h
	g++ -c ConstraintNetwork.cpp

ALConstraintGraph.o : ALConstraintGraph.cpp ALConstraintGraph.h \
		Constraint.h Vertex.h Arc.h \
		ConstraintNetwork.h Triple.h \
		LanguageModel.h 
	g++ -c ALConstraintGraph.cpp 

Vertex.o : Vertex.cpp Vertex.h \
		Arc.h Constraint.h
	g++ -c Vertex.cpp

Bridge.o : Bridge.cpp Bridge.h ALConstraintGraph.h
	g++ -c Bridge.cpp

util.o : util.cpp util.h
	g++ -c util.cpp

ConnectComponent.o : ConnectComponent.cpp ConnectComponent.h util.h ALConstraintGraph.h ConstraintNetwork.h
	g++ -c ConnectComponent.cpp

clean : 
	rm *.o



