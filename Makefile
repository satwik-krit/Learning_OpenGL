source = src
include = include
output = out
interms = interms
src_assets = src\res
build_assets = out\res

CC = gcc


build: $(output)/build.exe assets tags

$(output)/build.exe: $(interms)/main.o $(interms)/util.o $(interms)/gl_funcs.o $(interms)/stb_image.o $(interms)/handmademath.o
	@$(CC) -o $(output)/build.exe $(interms)/main.o $(interms)/util.o $(interms)/gl_funcs.o $(interms)/stb_image.o -luser32 -lgdi32 -lopengl32
	@echo Build completed.

$(interms)/main.o: $(source)/main.c $(source)/util.h $(source)/gl_funcs.h
	@echo main.c
	@$(CC) -I$(include) $(source)/main.c -c -g -o $(interms)/main.o

$(interms)/util.o: $(source)/util.c $(source)/util.h
	@echo util.c
	@$(CC) -I$(include) $(source)/util.c -c -g -o $(interms)/util.o

$(interms)/gl_funcs.o: $(source)/gl_funcs.c $(source)/gl_funcs.h
	@echo gl_funcs.c
	@$(CC) -I$(include) $(source)/gl_funcs.c -c -g -o $(interms)/gl_funcs.o

$(interms)/stb_image.o: $(source)/stb_image.c $(include)/stb_image.h
	@echo stb_image.c
	@$(CC) -I$(include) $(source)/stb_image.c -c -g -o $(interms)/stb_image.o

$(interms)/handmademath.o: $(include)\HandmadeMath.h
	@echo HandmadeMath.h
	@$(CC) $(include)/HandmadeMath.h -c -g -o $(interms)/HandmadeMath.o

assets: $(wildcard $(src_assets)/*.*)
	echo $(wildcard $(src_assets)/*.*)
	@rmdir $(build_assets) /S /Q
	@mkdir $(build_assets)
	@copy $(src_assets)\* $(build_assets)\* /Y
	@echo Copied assets.

prereq: 
	@mkdir $(interms) 
	@mkdir $(output) 
	@mkdir $(build_assets) 

tags:
	@echo Building tags...
	@ctags $(include)/* $(src)/* -R

clean:
	@rmdir $(interms) /S /Q
	@rmdir $(output) /S /Q

.PHONY: prereq clean tags build assets
