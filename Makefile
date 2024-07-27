#######################################################################################
.PHONY: help
help:
	@echo "Makefile Usage:"
	@echo "  make all TARGET=<sw_emu/hw_emu/hw> DEVICE=<FPGA platform>"
	@echo "      Command to generate the design for specified Target and Device."
	@echo ""
	@echo "  make exe TARGET=<sw_emu,hw_emu,hw>"
	@echo "      Command to generate host."
	@echo ""
	@echo "  make xclbin "
	@echo "      Command to generate hardware platform files(xo,xclbin)."
	@echo ""
	@echo "  make image "
	@echo "      Command to create the boot image"
	@echo ""
	@echo "  make clean "
	@echo "      Command to remove the generated files."
	@echo ""

#######################################################################################
TARGET := hw_emu
#DEVICE := xilinx_zcu102_base_202320_1
DEVICE := xilinx_zcu102_base_dfx_202320_1

BASE_INCLUDES=-Iinclude/ -I$(XILINX_HLS)/include

####   SYSROOT and related paths
SYSROOT=/opt/xilinx/platforms/sysroots/cortexa72-cortexa53-xilinx-linux/
ROOTFS=/opt/xilinx_local/xilinx-zynqmp-common-v2023.2

ifeq ($(TARGET),sw_emu)
	XO := krnl.sw_emu.xo
else
	XO := krnl.hw.xo
endif

XCLBIN := krnl.$(TARGET).xclbin

# Host building global settings
CXXFLAGS += -g -std=c++17 -Wall -O0 $(BASE_INCLUDES)

# Kernel compiler & linker global settings
CLFLAGS := -t $(TARGET) --platform $(DEVICE) $(BASE_INCLUDES)
   
# Host executable customization
EXECUTABLE := host
LDCLFLAGS := -t $(TARGET) --platform $(DEVICE) --config configs/randGen.cfg $(BASE_INCLUDES)
TEST_EXE := testbench

# Package : SD CARD Boot Image
SD_IMAGE=./package

##  HOST build switches
ifeq ($(TARGET),hw)
	CXX=aarch64-linux-gnu-g++
	EXECUTABLE=host.aarch64
endif

all: $(EXECUTABLE) $(XO) $(XCLBIN) emconfig

exe: $(EXECUTABLE)

xclbin: $(XO) $(XCLBIN)

image: $(SD_IMAGE)

test: $(TEST_EXE)

# Building kernel
$(XO): ./src/lsfr.cpp ./include/lsfr.h
ifeq ($(TARGET),sw_emu)
	v++ $(CLFLAGS) -c -k randWordGen16Bit -g -o'$@' '$<'
else
	v++ --platform $(DEVICE) -c --mode hls --config configs/hls_config.cfg 
endif

$(XCLBIN): $(XO) 
	v++ $(LDCLFLAGS) -l -o'$@' $(+)

# Building Executable
ifeq ($(TARGET),hw)
$(EXECUTABLE): ./src/host_randGen.cpp
	echo "Making exe for aarch64"
	${CXX} $(CXXFLAGS) -o '$@' '$<'  \
		--sysroot=${SYSROOT} \
		-I${XILINX_VIVADO}/include/ \
		-I${SYSROOT}/usr/include/xrt/ \
		-I${SYSROOT}/usr/include/ \
		-L$(SYSROOT)/usr/lib \
		-L$(SYSROOT)/usr/xrt/lib \
		-lxrt_coreutil -pthread
else
$(EXECUTABLE): ./src/host_randGen.cpp
	echo "Making exe for the current environment"
	${CXX} $(CXXFLAGS) -o '$@' '$<'  \
		-I${XILINX_XRT}/include/ \
                -L$(XILINX_XRT)/lib -lxrt_coreutil -pthread
endif



.PHONY: emconfig
emconfig:
	emconfigutil --platform $(DEVICE)

.PHONY: run
run: all
ifeq ($(TARGET),$(filter $(TARGET),sw_emu hw_emu))
	XCL_EMULATION_MODE=$(TARGET) ./$(EXECUTABLE) $(XCLBIN)
else
	./$(EXECUTABLE) $(XCLBIN)
endif

${SD_IMAGE}:
	echo "Making the SD card Image"
	v++ -p -t hw --platform ${DEVICE} --config configs/zcu102.cfg  $(XCLBIN) --package.out_dir $(SD_IMAGE) --package.sd_file configs/xrt.ini \
		--package.rootfs $(ROOTFS)/rootfs.ext4 \
		--package.sd_file $(ROOTFS)/Image 

${TEST_EXE}: src/lsfr_test.cpp  src/lsfr.cpp include/lsfr.h
	${CXX} $(CXXFLAGS)  -o '$@' '$<'  src/lsfr.cpp
  


# Cleaning stuff
.PHONY: clean

RMDIR = rm -rf

clean:
	-$(RMDIR) $(EXECUTABLE) $(XCLBIN)/{*sw_emu*,*hw_emu*} $(TEST_EXE)
	-$(RMDIR) TempConfig system_estimate.xtxt *.rpt
	-$(RMDIR) *.protoinst _v++_* .Xil emconfig.json dltmp* xmltmp* *.log *.jou 

cleanall: clean
	-$(RMDIR) $(XCLBIN) *.xo *.xclbin* *.wcfg *.wdb *.csv *.compile_summary *.run_summary host* $(TEST_EXE)
	-$(RMDIR) _x* .run/
	-$(RMDIR) $(SD_IMAGE)

