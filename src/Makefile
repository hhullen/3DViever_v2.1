MAIN_PROJ_NAME=3DViewer_v2
MODEL_TEST_NAMES=model
MODEL_PATH=$(MAIN_PROJ_NAME)/Model_module
CONTROLLER_PATH=$(MAIN_PROJ_NAME)/Controller_module
VIEV_PATH=$(MAIN_PROJ_NAME)/View_module
TEST_PATH=$(MAIN_PROJ_NAME)/Test_module
TESTBUILD_FILES=$(TEST_PATH)/$(MAIN_PROJ_NAME)_test.cc $(MODEL_PATH)/*$(MODEL_TEST_NAMES)*.h $(MODEL_PATH)/*$(MODEL_TEST_NAMES)*.cc
EXECUTABLE=$(MAIN_PROJ_NAME)_test.out
COMPILER=g++
STD=-std=c++17
TEST_FLAGS=-lgtest -pthread
CLANG_FILE=.clang-format
CLANG_FILE_WAY=../materials/linters/$(CLANG_FILE)
CHECK_FILES=$(MODEL_PATH)/*.cc $(MODEL_PATH)/*.h $(CONTROLLER_PATH)/*.cc $(CONTROLLER_PATH)/*.h $(VIEV_PATH)/*.cc $(VIEV_PATH)/*.h $(TEST_PATH)/*.cc
CPPCH_SETUP=--enable=warning,performance,portability  -v --language=c++ -$(STD)
VALGRIND_SETUP=--tool=memcheck --leak-check=full --show-leak-kinds=all
BUILD_DIR=../build
APP=$(MAIN_PROJ_NAME).app
APP_PACKAGE=$(APP)/Contents/MacOS/$(MAIN_PROJ_NAME)
BUILD_FILES_TO_DELETE=`ls | grep -v $(APP)`
ICON_RESOURCES=$(MAIN_PROJ_NAME)/Resources
PLIST=$(MAIN_PROJ_NAME)/Info.plist
ICON_RESOURCES_TARGET=$(BUILD_DIR)/$(APP)/Contents/
DOC_DIR=../documentation
TO_DELETE=*.o *.out *.dSYM *.gch .DS_Store $(EXECUTABLE) $(CLANG_FILE) $(DOC_DIR)

all: clean dvi check uninstall install tests dist

install:
	mkdir -p $(BUILD_DIR)
	cmake -S $(MAIN_PROJ_NAME) -B $(BUILD_DIR)
	cd $(BUILD_DIR) && make && rm -rf $(BUILD_FILES_TO_DELETE)
	chmod -R u+w $(BUILD_DIR)
	cp -r $(ICON_RESOURCES) $(ICON_RESOURCES_TARGET)
	cp -r $(PLIST) $(ICON_RESOURCES_TARGET)
	open $(BUILD_DIR)

uninstall:
	rm -rf $(BUILD_DIR)/$(APP)

dvi: get_html
	open $(DOC_DIR)/index.html

get_html:
	makeinfo --html -o $(DOC_DIR) dvi/info.texi

dist:	
	tar -cvf $(BUILD_DIR)/$(MAIN_PROJ_NAME).tar $(MAIN_PROJ_NAME)

check:
	cppcheck $(CPPCH_SETUP) $(CHECK_FILES)
	cp $(CLANG_FILE_WAY) $(CLANG_FILE)
	clang-format -i --style=Google $(CHECK_FILES)
	clang-format -n --style=Google $(CHECK_FILES)

tests: clean
	$(COMPILER) $(STD) $(TESTBUILD_FILES) -o $(EXECUTABLE) $(TEST_FLAGS)
	./$(EXECUTABLE)

valgrind: clean
	$(COMPILER) -g $(STD) $(TESTBUILD_FILES) -o $(EXECUTABLE) $(TEST_FLAGS)
	CK_FORK=no valgrind $(VALGRIND_SETUP) ./$(EXECUTABLE)

leaks:
	$(COMPILER) -g $(STD) $(TESTBUILD_FILES) -o $(EXECUTABLE) $(TEST_FLAGS)
	CK_FORK=no leaks -atExit -- ./$(EXECUTABLE)

run:
	./$(BUILD_DIR)/$(APP_PACKAGE)

clean:
	rm -rf $(TO_DELETE)
