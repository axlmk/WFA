.PHONY: all clean erase

# Folders
BUILD_DIR:=build
SRC_DIR:=src
HEADER_DIR=header
LOCAL_INCLUDE_DIR=include
IM_DIR=C:\msys64\mingw64\include\ImageMagick-7

# Compiler & linker options
LIB_DIR=-I$(LOCAL_INCLUDE_DIR) -I$(IM_DIR)
IMFLAGS:=-DMAGICKCORE_HDRI_ENABLE=0 -DMAGICKCORE_QUANTUM_DEPTH=16
CXXFLAGS:=-Wall -g $(LIB_DIR) $(IMFLAGS)
CXX:=g++
TARGET:=wfa.exe
LDLIBS:= -lMagick++-7.Q16HDRI -lole32 -lcurl

# List of .cpp files
SRCS:= $(wildcard $(SRC_DIR)/*.cpp)

# Automated object files list
OBJS:= $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDLIBS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR):
	powershell New-Item -ItemType Directory $(BUILD_DIR)

clean: erase
	-powershell Remove-Item -Path "$(BUILD_DIR)\*.o" -Force
	-powershell Remove-Item -Path "$(TARGET)" -Force
	-powershell Remove-Item -Path "$(BUILD_DIR)" -Recurse -Force
	-powershell Remove-Item -Path "temp"
	-powershell Remove-Item -Path "image"

erase:
	-powershell Remove-Item -Path "temp\*" -Force
	-powershell Remove-Item -Path "image\*" -Force