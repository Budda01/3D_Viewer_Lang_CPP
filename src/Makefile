CC = g++ -std=c++17 -Wall -Werror -Wextra -lstdc++ 
LIBS = -lgtest -pthread
GCOV = --coverage
TARGET = 3DViewTK_v2
SRC = ./model/model.cc test.cc

all: clean install

install: 
	@mkdir -p ../build
	@cd view && qmake && make && make clean && rm -rf Makefile && mv view ../../build/$(TARGET)
	@cp -R ../build/$(TARGET) ~/Desktop/$(TARGET)

uninstall:
	@rm -rf ~/Desktop/3DViewTK_v2.app ../build
	@rm ~/Desktop/$(TARGET)

dvi: 
	@xdg-open instruction.md

dist: install
	@mkdir -p ../dist
	@cp -R ../build/$(TARGET) ../dist
	@cd ../dist && tar cvzf $(TARGET).tgz *
	@cd ../dist && rm -rf $(TARGET)

tests:
	@$(CC) $(SRC) -o test.out $(LIBS) 
	@./test.out

gcov_report: clean
	@$(CC) $(GCOV) $(SRC) -o test.out $(LIBS) 
	@./test.out
	@lcov -t "test" -o test.info -c -d . 
	@genhtml -o report test.info 
	@open ./report/index.html
	@make clean_test

check:
	@cp ../materials/linters/.clang-format ./
	@clang-format -n ./*/*.cc ./*/*.cpp ./*/*.h *.cc *.cpp
	@rm -rf .clang-format

clean_test:
	@rm -rf *.gcda *.gcno *.info test.out test

clean: clean_test uninstall
	@rm -rf ./*.o ./*.a report