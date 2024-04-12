# Definitions de macros

CXX     = g++
CXXFLAGS = -g -Wall -std=c++17
LINKING = `pkg-config --cflags gtkmm-4.0`
LDLIBS = `pkg-config --libs gtkmm-4.0`
CXXFILES = projet.cc shape.cc message.cc lifeform.cc simulation.cc gui.cc graphic.cc
OFILES = projet.o shape.o message.o lifeform.o simulation.o gui.o graphic.o

# Definition de la premiere regle

all: projet 

projet: $(OFILES)
	$(CXX) $(CXXFLAGS) $(LINKING) $(OFILES) -o $@ $(LDLIBS)
	
projet.o: projet.cc simulation.h gui.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)
	
shape.o: shape.cc shape.h
	$(CXX) $(CXXFLAGS) -c $< -o $@ 

message.o: message.cc message.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

lifeform.o: lifeform.cc lifeform.h message.h shape.h
	$(CXX) $(CXXFLAGS) -c $< -o $@ 

simulation.o: simulation.cc simulation.h lifeform.h message.h shape.h
	$(CXX) $(CXXFLAGS) -c $< -o $@ 
	
gui.o: gui.cc gui.h simulation.h graphic.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)
	
graphic.o: graphic.cc graphic.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)

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

projet.o: projet.cc simulation.h lifeform.h message.h constantes.h \
 shape.h
shape.o: shape.cc shape.h
message.o: message.cc message.h
lifeform.o: lifeform.cc lifeform.h message.h constantes.h shape.h
simulation.o: simulation.cc simulation.h lifeform.h message.h \
 constantes.h shape.h
gui.o: gui.cc gui.h simulation.h lifeform.h message.h constantes.h \
 shape.h graphic.h
graphic.o: graphic.cc graphic.h
