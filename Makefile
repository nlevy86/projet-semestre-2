# Definitions de macros

CXX     = g++
CXXFLAGS = -g -Wall -std=c++11
CXXFILES = projet.cc shape.cc message.cc lifeform.cc
OFILES = projet.o shape.o message.o lifeform.o

# Definition de la premiere regle

all: projet 

projet: $(OFILES)
	$(CXX) $(OFILES) -o projet
	
projet.o: projet.cc shape.h
	$(CXX) $(CXXFLAGS) -c projet.cc -o projet.o
	
shape.o: shape.cc shape.h
	$(CXX) $(CXXFLAGS) -c shape.cc -o shape.o

message.o: message.cc message.h
	$(CXX) $(CXXFLAGS) -c message.cc -o message.o

lifeform.o: lifeform.cc lifeform.h message.h shape.h
	$(CXX) $(CXXFLAGS) -c lifeform.cc -o lifeform.o

# Definitions de cibles particulieres

depend:
	@echo " *** MISE A JOUR DES DEPENDANCES ***"
	@(sed '/^# DO NOT DELETE THIS LINE/q' Makefile && \
	  $(CXX) -MM $(CXXFLAGS) $(CXXFILES) | \
	  egrep -v "/usr/include" \
	 ) >Makefile.new
	@mv Makefile.new Makefile

clean:
	@echo " *** EFFACE MODULES OBJET ET EXECUTABLE ***"
	@/bin/rm -f *.o *.x *.cc~ *.h~ projet 

#
# -- Regles de dependances generees automatiquement

projet.o: projet.cc shape.h
shape.o: shape.cc shape.h
message.o: message.cc message.h
