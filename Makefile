source = src
include = include
output = out
interms = interms

DIR_GUARD = mkdir $(interms) && mkdir $(output)


$(output)/build.exe: $(interms)/main.o $(interms)/util.o $(interms)/gl_funcs.o $(interms)/stb_image.o
	ctags include/* src/*
	@echo Building ...
	@gcc -o $(output)/build.exe $(interms)/main.o $(interms)/util.o $(interms)/gl_funcs.o $(interms)/stb_image.o -luser32 -lgdi32 -lopengl32

$(interms)/main.o: $(source)/main.c $(source)/util.h $(source)/gl_funcs.h
	@gcc -I$(include) $(source)/main.c -c -g -o $(interms)/main.o
	@echo main.c

$(interms)/util.o: $(source)/util.c $(source)/util.h
	@gcc -I$(include) $(source)/util.c -c -g -o $(interms)/util.o
	@echo util.c

$(interms)/gl_funcs.o: $(source)/gl_funcs.c $(source)/gl_funcs.h
	@gcc -I$(include) $(source)/gl_funcs.c -c -g -o $(interms)/gl_funcs.o
	@echo gl_funcs.c

$(interms)/stb_image.o: $(source)/stb_image.c $(include)/stb_image.h
	@gcc -I$(include) $(source)/stb_image.c -c -g -o $(interms)/stb_image.o
	@echo stb_image.c

prereq: 
	@$(DIR_GUARD)
	@echo Done!

clean:
	@rmdir $(interms) /S /Q
	@rmdir $(output) /S /Q
	@echo Done! $<

.PHONY: prereq clean
