OUT = projet
CXX = g++
CXXFLAGS = -Wall -std=c++17
LINKING = `pkg-config --cflags gtkmm-4.0`
LDLIBS = `pkg-config --libs gtkmm-4.0`
CXXFILES = graphic.cc shape.cc lifeform.cc simulation.cc projet.cc gui.cc \
 message.cc
OFILES = $(CXXFILES:.cc=.o)

all: $(OUT)

depend:
	@echo " *** MISE A JOUR DES DEPENDANCES ***"
	@(sed '/^# DO NOT DELETE THIS LINE/q' Makefile && \
	  $(CXX) -MM $(CXXFLAGS) $(CXXFILES) | \
	  egrep -v "/usr/include" \
	 ) >Makefile.new
	@mv Makefile.new Makefile

clean:
	@echo " *** EFFACE MODULES OBJET ET EXECUTABLE ***"
	@/bin/rm -f *.o *.x *.cc~ *.h~ $(OUT)

shape.o: shape.cc shape.h graphic.h
	$(CXX) $(CXXFLAGS) -c $< -o $@ 

message.o: message.cc message.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

lifeform.o: lifeform.cc lifeform.h message.h constantes.h shape.h \
graphic.h
	$(CXX) $(CXXFLAGS) -c $< -o $@ 

simulation.o: simulation.cc simulation.h lifeform.h message.h \
constantes.h shape.h graphic.h
	$(CXX) $(CXXFLAGS) -c $< -o $@ 

graphic.o: graphic.cc graphic.h graphic_gui.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ 

gui.o: gui.cc graphic.h graphic_gui.h gui.h simulation.h lifeform.h \
 shape.h constantes.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ 

projet.o: projet.cc gui.h simulation.h lifeform.h message.h shape.h \
graphic.h constantes.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ 

$(OUT): $(OFILES)
	$(CXX) $(CXXFLAGS) $(LINKING) $(OFILES) -o $@ $(LDLIBS)


# DO NOT DELETE THIS LINE
graphic.o: graphic.cc graphic.h graphic_gui.h
shape.o: shape.cc shape.h graphic.h graphic_gui.h
lifeform.o: lifeform.cc lifeform.h message.h constantes.h shape.h \
 graphic.h graphic_gui.h
simulation.o: simulation.cc simulation.h lifeform.h message.h \
 constantes.h shape.h graphic.h graphic_gui.h
projet.o: projet.cc gui.h simulation.h lifeform.h message.h constantes.h \
 shape.h graphic.h graphic_gui.h
gui.o: gui.cc gui.h simulation.h lifeform.h message.h constantes.h \
 shape.h graphic.h graphic_gui.h
message.o: message.cc message.h
