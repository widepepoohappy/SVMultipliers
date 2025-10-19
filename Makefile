
# Nome del compilatore
CXX = g++

# Opzioni di compilazione
CXXFLAGS = -Wall -Wextra -std=c++11
#CXXFLAGS = 

# Nome del file eseguibile
TARGET = main

# File sorgente
SRC = cpp/WallaceAlgorithm.cpp cpp/utils.cpp cpp/generateWallaceSV.cpp

#!/usr/local/bin/gnumake -f

VLOGAN_OPTS += +lint=all,noVCDE,noPCTI-L,noPCTIO-L -suppress=PCTI-L -suppress=PCTIO-L
VCS_OPTS += +lint=all,noPCTI-L -suppress=PCTI -suppress=PCTIO-L
VCS_OPTS += -notice
VLOGAN_OPTS += -timescale=1ns/1ps
NCELAB_OPTS += -TIMESCALE 1ns/1ps
VSIM_OPTS += -t 1ps
VSIM_OPTS += -suppress 3009

## E2 CPU specific options
TOP = MUL
DUT_MODULE_NAME = ${TOP}
HDL_TOP_SPEC    = ${TOP}
#DVT_VERSION  = 20.1.44

__TBX_CONFIG_OPTS := $(subst D2,STRATOM,${__TBX_CONFIG_OPTS})

# Regola principale
all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) 
ius:
	bsub -Ip -q short qbar -ius elab_dut
mti:
	bsub -Ip -q short qbar -mti elab_dut
vcs:
	bsub -Ip -q short qbar -vcs elab_dut
novas:
	bsub -Ip -q short qbar -novas elab_dut
dft:
	bsub -Ip -q interactive -R "select[type==LINUX64 && mem > 4000] rusage[mem=4000]" make tb_ide DUT_ECLIPSE=1 TESTCASE_TYPE=uvm HVL_TOP_SPEC=${TOP} DVT_VERSION=20.1.44 &
eucl:
	bsub -Ip -q interactive -R "select[type==LINUX64 && mem > 4000] rusage[mem=4000]" qbar -vcs elab_dut VCS_OPTS+="-lca" VLOGAN_OPTS+="-euclide" VCS_OPTS+="-euclide" TESTCASE_TYPE=uvm UNMANAGED_DIR=${unmanaged}/eucl unmanaged=${unmanaged}/eucl &



# Pulizia dei file generati
clean: $(TARGET)
	rm -rf main

