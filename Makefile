SOURCE_DIRS = src
INCLUDE_DIRS = include external/cglm/include/
OUTPUT_DIR = out
INTERMS_DIR = interms
SRC_ASSETS = res
BUILD_ASSETS = out\res

CC = gcc

INCLUDE_CMD =  $(foreach dir, $(INCLUDE_DIRS), -I$(dir))

BUILD_FLAGS = -Wall -Wno-missing-braces

.PHONY: prereq clean tags build assets

build: $(OUTPUT_DIR)/build.exe assets

objectFiles = $(wildcard $(INTERMS_DIR)/*.o)
$(OUTPUT_DIR)/build.exe: dirs $(INTERMS_DIR)/main.o $(INTERMS_DIR)/util.o $(INTERMS_DIR)/gl_funcs.o $(INTERMS_DIR)/stb_image.o assets
	@$(CC) -o $(OUTPUT_DIR)/build.exe $(objectFiles) -luser32 -lgdi32 -lopengl32

$(INTERMS_DIR)/main.o: $(SOURCE_DIRS)/main.c $(SOURCE_DIRS)/util.h $(SOURCE_DIRS)/gl_funcs.h
	@echo main.c
	@$(CC) $(INCLUDE_CMD) $(SOURCE_DIRS)/main.c -c -g -o $(INTERMS_DIR)/main.o $(BUILD_FLAGS)

$(INTERMS_DIR)/util.o: $(SOURCE_DIRS)/util.c $(SOURCE_DIRS)/util.h
	@echo util.c
	@$(CC) $(INCLUDE_CMD) $(SOURCE_DIRS)/util.c -c -g -o $(INTERMS_DIR)/util.o $(BUILD_FLAGS)

$(INTERMS_DIR)/gl_funcs.o: $(SOURCE_DIRS)/gl_funcs.c $(SOURCE_DIRS)/gl_funcs.h
	@echo gl_funcs.c
	@$(CC) $(INCLUDE_CMD) $(SOURCE_DIRS)/gl_funcs.c -c -g -o $(INTERMS_DIR)/gl_funcs.o $(BUILD_FLAGS)

$(INTERMS_DIR)/stb_image.o: $(SOURCE_DIRS)/stb_image.c include/stb_image.h
	@echo stb_image.c
	@$(CC) $(INCLUDE_CMD) $(SOURCE_DIRS)/stb_image.c -c -g -o $(INTERMS_DIR)/stb_image.o $(BUILD_FLAGS)

$(INTERMS_DIR)/math.o: $(SOURCE_DIRS)/math.c
	@echo math.c
	@$(CC) $(SOURCE_DIRS)/math.c -c -g -o $(INTERMS_DIR)/math.o -MATH_ONLY=1 $(BUILD_FLAGS)

assets: $(wildcard $(SRC_ASSETS)/*.*)
	@if exist $(BUILD_ASSETS) rmdir $(BUILD_ASSETS) /S /Q
	@mkdir $(BUILD_ASSETS)
	@copy $(SRC_ASSETS)\* $(BUILD_ASSETS)\* /Y
	@echo Copied assets.

dirs: 
	@if not exist $(INTERMS_DIR) mkdir $(INTERMS_DIR) 
	@if not exist $(OUTPUT_DIR) mkdir $(OUTPUT_DIR) 
	@if not exist $(BUILD_ASSETS) mkdir $(BUILD_ASSETS)

tags:
	@echo Building tags...
	@ctags -R $(src)/* $(INCLUDE_DIRS)/*

clean:
	@if exist $(INTERMS_DIR) rmdir $(INTERMS_DIR) /S /Q
	@if exist $(OUTPUT_DIR) rmdir $(OUTPUT_DIR) /S /Q
