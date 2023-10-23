.PHONY: test app build
CXX=g++
CAR=ar
CRANLIB=ranlib

OS=$(shell uname)

ifeq ($(OS), Linux)
DEBIAN_FIX=-lstdc++fs
else
DEBIAN_FIX=
endif


FLAGS=-Wall -Wextra -std=c++17
# FLAGS=-Wall -Werror -Wextra -std=c++17

GTEST=-lgtest_main -lgtest -lpthread
GCOV=-fprofile-arcs -ftest-coverage

TARGETDIR=./
BDIR=../build/
INSTALL_DIR = ../trading

FILE=Trading
FILE_APP=trading
FILE_TEST=test
FILE_MODEL=model
FILE_NEWTON=newton_interpolation
FILE_SPLINE=spline_interpolation
FILE_APPROX=approximation
FILE_GAUSS=gauss

SRC =   ./main.cpp \
        ./test.cpp \
        ./types.h \
        ./controller.h \
        ./model.h \
        ./model.cpp \
        ./mainwindow.h \
        ./mainwindow.cpp \
        ./Approximation/*.* \
        ./NewtonInterpolation/*.* \
        ./SplineInterpolation/*.* \

all: app

app: build
ifeq ($(OS), Linux)
	$(BDIR)/$(FILE)
else
	open $(BDIR)/$(FILE).app
endif

build: clean
	-mkdir $(BDIR)
	cp $(FILE).pro $(BDIR)
	cp *.h *.cpp *.ui $(BDIR)
	cp -R Approximation $(BDIR)
	cp -R NewtonInterpolation $(BDIR)
	cp -R SplineInterpolation $(BDIR)
	cd $(BDIR); qmake $(FILE).pro
	make -C $(BDIR)
ifeq ($(OS), Darwin)
	-cp -R datasets $(BDIR)/datasets/
endif

tests: test

test:
	$(CXX) -c $(FLAGS) $(TARGETDIR)$(FILE_MODEL).cpp
	$(CXX) -c $(FLAGS) NewtonInterpolation/$(FILE_NEWTON).cpp
	$(CXX) -c $(FLAGS) SplineInterpolation/$(FILE_SPLINE).cpp
	$(CXX) -c $(FLAGS) Approximation/$(FILE_APPROX).cpp
	$(CXX) -c $(FLAGS) Approximation/$(FILE_GAUSS).cpp
	$(CXX) -c $(FLAGS) $(TARGETDIR)$(FILE_TEST).cpp $(GTEST)

	$(CXX) -o $(TARGETDIR)$(FILE_TEST) $(FLAGS) $(FILE_TEST).o $(FILE_MODEL).o \
			  $(FILE_NEWTON).o $(FILE_SPLINE).o $(FILE_APPROX).o $(FILE_GAUSS).o \
			  -L $(GTEST) $(DEBIAN_FIX)

	-$(TARGETDIR)$(FILE_TEST)


install: build
	rm -rf $(INSTALL_DIR)
	mkdir $(INSTALL_DIR)
ifeq ($(OS), Linux)
	cp $(BDIR)$(FILE) $(INSTALL_DIR)
	-cp -R datasets $(INSTALL_DIR)/datasets/
else
	cp -r $(BDIR)/$(FILE).app $(INSTALL_DIR)
	-cp -R datasets $(INSTALL_DIR)/datasets/
	-cp -R datasets $(INSTALL_DIR)/Trading.app/Contents/MacOS/datasets/
endif
	make clean
	echo "Installed to" $(INSTALL_DIR)

uninstall:
	rm -rf $(INSTALL_DIR)
	echo "Unistalled from" $(INSTALL_DIR)

dvi:
ifeq ($(OS), Linux)
	echo "Creating DVI file..." && \
	texi2dvi $(TARGETDIR)readme.tex && \
	echo "Create PDF file..." && \
	texi2pdf $(TARGETDIR)readme.tex && \
	echo "Done" || echo "\nSomething went wrong. Using the pre-built .pdf file"
	xdg-open $(TARGETDIR)readme.pdf
else
	echo "Using the pre-built .pdf file"
	open $(TARGETDIR)readme.pdf
endif

dist:
	rm -rf trading_dist/
	mkdir trading_dist/
	mkdir trading_dist/src
	cp Makefile Trading.pro trading_dist/src/
	cp *.cpp *.h *.ui trading_dist/src/
	-cp readme.tex readme.pdf trading_dist/src/
	-cp -R NewtonInterpolation trading_dist/src/
	-cp -R SplineInterpolation trading_dist/src/
	-cp -R Approximation trading_dist/src/
	-cp -R datasets trading_dist/src/
	tar cvzf ../trading_dist.tgz trading_dist/
	rm -rf trading_dist/


check:
	-rm -rf cpplint.py CPPLINT.cfg
	-ln -s ../materials/linters/cpplint.py
	-ln -s ../materials/linters/CPPLINT.cfg
	-python3 cpplint.py --extensions=cpp $(SRC)

	cppcheck --enable=all --check-config --suppress=missingIncludeSystem --language=c++ $(SRC)

clang:
	-rm -rf .clang-format
	-ln -s ../materials/linters/.clang-format
	clang-format -n $(SRC)


clang_fix:
	-rm -rf .clang-format
	-ln -s ../materials/linters/.clang-format
	clang-format -i *.cpp $(SRC)


leaks: test
ifeq ($(OS), Linux)
# 	Clear the log
	echo "Leaks check" > __leaks_check.out


	-CK_FORK=no valgrind --leak-check=full $(TARGETDIR)$(FILE_TEST) 2>> __leaks_check.out
	-grep -e leaks __leaks_check.out
	-grep -e error __leaks_check.out
else
	# Comment-out the 'ASSERT_THROW()' tests to fix false positives
	CK_FORK=no leaks -atExit -- $(TARGETDIR)$(FILE_TEST)
endif

clean:
	rm -rf $(BDIR)
	rm -rf *.gcno
	rm -rf *.gcda
	rm -rf *.gcov
	rm -rf *.info
	rm -rf $(REPORTDIR)
	rm -rf  *.o *.a *.out
	rm -rf $(TARGETDIR)$(FILE_TEST)
	rm -rf $(TARGETDIR)$(FILE_APP)
	rm -rf CPPLINT.cfg cpplint.py
	rm -rf readme.aux readme.dvi readme.log


# Frequent typos/synonyms
claen:	clean
clear:	clean
claer:	clean

rebuild:
	make clean
	make all
	rm -rf  *.o *.a
