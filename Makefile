

project_name := Template
uvproj_path := .
day_version := 99
.PHONY: default build cformat format clean flash debug assets

# Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope CurrentUser

default: macro2path build path2macro

.PHONY: dist clear

clear:
	@echo clear
	@- rmdir /Q /S "./_build"

dist:
	make
	./utils_bin/tcc/tcc.exe .\utils_bin\tcc\sha1\sha1.c -run .\utils_bin\dist.c $(day_version) WELCOME

build:
	@echo [START]----- UV build -----
	@- UV4 -b $(uvproj_path)/$(project_name).uvprojx -j0 -o ./Obj/Build_Output.txt
	@- powershell -c "cat $(uvproj_path)./Obj/Build_Output.txt | sls 'error'"
	@echo [ END ]----- UV build -----

clean:
	@echo [START]----- UV clean -----
	UV4 -c $(uvproj_path)/$(project_name).uvprojx -j0
	@echo [ END ]----- UV clean -----

flash:
	@echo [START]----- UV flash -----
	@- UV4 -b $(uvproj_path)/$(project_name).uvprojx -j0 -o ./Obj/Build_Output.txt
	@- UV4 -f $(uvproj_path)/$(project_name).uvprojx -j0 -o ./Obj/Build_Output.txt
	@- powershell -c cat $(uvproj_path)/Obj/Build_Output.txt
	@echo [ END ]----- UV flash -----

debug:
	@echo [START]----- UV debug -----
	UV4 -d $(uvproj_path)/$(project_name).uvprojx
	@echo [ END ]----- UV debug -----

path2macro:
	@echo [START]----- Path TO MACRO -----
	./utils_bin/KeilUtils-win-amd64.exe path2macro
	@echo [ END ]----- Path TO MACRO -----

macro2path:
	@echo [START]----- MACRO TO Path -----
	./utils_bin/KeilUtils-win-amd64.exe macro2path
	@echo [ END ]----- MACRO TO Path -----

format:
	@echo Astyle source code format
	./utils_bin/astyle.exe --project="./utils_bin/.astylerc" -r **.c,**.h --ignore-exclude-errors --exclude=_build --exclude=utils_bin --exclude=dist --exclude=utils -v -Q

assets:
	@echo Create Assets
	./utils_bin/p2a-win-amd64.exe -c rgb565 -in ./APP/UI -out ./APP/assets

