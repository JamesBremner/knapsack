#source file search paths
VPATH = ../../src

LIBS = -lgdiplus -lgdi32 -lcomdlg32 -lstdc++fs \

static_link_flags = -static-libstdc++ -static-libgcc -static

# folders
ODIR = ./.vscode/obj

OPT = -g
CCflags = -std=c++17 $(OPT) -D_mingw_

# sources
OBJ_DP3UK = \
	$(ODIR)/DP3UKInstance.o \
	$(ODIR)/DP3UK.o \
	$(ODIR)/knapsack.o \
	$(ODIR)/DDP.o 

OBJ_DP3SUK = \
	$(ODIR)/DP3SUKInstance.o \
	$(ODIR)/DPS3UK.o \
	$(ODIR)/knapsack.o \
	$(ODIR)/DDP.o 

OBJ_H3CS = \
	$(ODIR)/H3CSInstance.o \
	$(ODIR)/H3CS.o \
	$(ODIR)/knapsack.o \
	$(ODIR)/RK2FFG.o \
	$(ODIR)/DDP.o 

### COMPILE

$(ODIR)/%.o: %.cpp  
	g++ -c $< \
	-o $@ $(INCS) $(CCflags)

### LINK

DP3UK: $(OBJ_DP3UK)
	g++ -o bin/DP3UK.exe \
	 $^ $(OPT)  \
	$(static_link_flags) $(LIBS)

DP3SUK: $(OBJ_DP3SUK)
	g++ -o bin/DP3SUK.exe \
	 $^ $(OPT)  \
	$(static_link_flags) $(LIBS)

H3CS: $(OBJ_H3CS)
	g++ -o bin/H3CS.exe \
	 $^ $(OPT)  \
	$(static_link_flags) $(LIBS)

.PHONY: clean
clean:
	del /q .vscode\\obj\\*.o

