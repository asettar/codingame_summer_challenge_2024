
SCRIPT = makeCodingameFile

FILES = build.cpp

buildClean:
	make run clean

$(SCRIPT) : $(FILES)
	@make build

build :
	g++ -std=c++17 $(FILES) -o $(SCRIPT)

run : $(SCRIPT)
	@mkdir -p prod
	./$(SCRIPT)
	cat ./prod/code.cpp | pbcopy

clean:
	@rm -rf .vscode .DS_Store
	@rm -rf prod $(SCRIPT)

re : clean build

all : clean build run

.PHONY : run build clean re