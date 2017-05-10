
LIBS:= -L$(LIBROOTFFTWWRAPPER_DIR) -lRootFftwWrapper -L$(ARA_UTIL_INSTALL_DIR)/lib -lAraEvent -lfftw3 -L$(ROOTSYS)/lib/root -lMathMore -L$(SQLITE)/lib -lsqlite3

OBS_DIR:=objects

ROOT_CONFIG:=`root-config --libs --cflags` -lMinuit -lCore # to add the requried root libraries
CFLAGS:= -Wl,--no-as-needed -fPIC $(ROOT_CONFIG) -DWITH_SQLITE3=ON

INCLUDES:= -I. -Iincludes -I$(LIBROOTFFTWWRAPPER_DIR) -I$(ARA_UTIL_INSTALL_DIR)/include -I$(SQLITE)/include # include some of the generic headers needed
# -I/home/cbora/Software/Install/Eigen/eigen-eigen-1306d75b4a21/Eigen  -I/usr/include

MODULES := Geometry InputOutput TimeFinding VertexFinding # Filtering EventViewer Calibration

INCLUDES += $(patsubst %,-I%, $(MODULES)) # include the different module directories in the include directive
VPATH+=$(subst :, , $(MODULES))

# this list will get much shorter as we move everything into module directories... 
# SOURCES := Reconstruction.cc # Driver.cc

include $(patsubst %, %/module.mk, $(MODULES)) # this includes the makefile pieces inside each module directory

#expect the module.mk files to add the correct filename.cc to the SOURCES variable
OBJECTS += $(patsubst %.cc, $(OBS_DIR)/%.o, $(filter %.cc, $(SOURCES))) # figure out what object files to make
HEADERS += $(patsubst %.cc, %.h, $(filter %.cc, $(SOURCES))) # figure out what header files to use as dependencies


ifneq ($(ARASIM),) # only if ARASIM is installed (need to have invironmental variable set)

LIBS+= -L$(ARASIM) -lSim -L$(ARA_UTIL_INSTALL_DIR)/lib -lAraEvent
INCLUDES+= -I$(ARASIM) -I$(ARA_UTIL_INSTALL_DIR)/include -I$(BOOST_ROOT)
CFLAGS+= -DARASIM_EXISTS 
endif

libReco.so: $(OBJECTS) $(HEADERS) recoClassDict.o 
	$(CC) -g -O0 $(INCLUDES) $(OBJECTS) $(LIBS) $(CFLAGS) recoClassDict.o -shared  -o libReco.so 

l2.exe: mainLoop.cxx $(OBJECTS) $(HEADERS) recoClassDict.o
	$(CXX) mainLoop.cxx recoClassDict.o $(INCLUDES) $(CFLAGS) $(OBJECTS) $(LIBS)  -o l2.exe


times.exe: main.cxx $(OBJECTS) $(HEADERS) recoClassDict.o
	$(CXX) $(CFLAGS) main.cxx recoClassDict.o $(INCLUDES) $(OBJECTS) $(LIBS) -o times.exe

genLoop.exe: 
	$(CXX) $(CFLAGS) scripts/genLoop.cxx recoClassDict.o $(INCLUDES) $(OBJECTS) $(LIBS) -o run_script.exe


# we are still missing a way to account for dependencies between different classes (e.g. if Channel.h changes ChannelCollection.o doesn't automatically recompile as well)
	
$(OBS_DIR)/%.o : %.cc %.h
	@echo "compiling "$<
	@mkdir -p $(OBS_DIR)
	$(CC) -c -g -O0 $(LIBS) $< $(INCLUDES)  $(CFLAGS) -o $@

recoClassDict.o : recoClassDict.C $(OBJECTS)
	$(CC) -c -g -O0 $< $(CFLAGS) $(INCLUDES) $(LIBS) -o $@
	
recoClassDict.C : $(HEADERS) $(OBJECTS) LinkDef.h
	@echo "generating dictionary"
	@rm -f *Dict*
	rootcint -f $@ -c -g $(INCLUDES) $(HEADERS) LinkDef.h

	
clean: 
	-@rm -f $(OBS_DIR)/*.o
	-@rm -f recoClassDict*
	-@rm -f libReco.so
	-@rm -f times.exe
	-@rm -f l2.exe
