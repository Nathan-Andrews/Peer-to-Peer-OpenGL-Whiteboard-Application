# compile uic
#/usr/local/Cellar/qt/6.7.3/share/qt/libexec/uic clientinterface.ui -o ui_clientinterface.h

.PHONY: all clean run

# Build the project
all:
	mkdir -p build
	cd build && cmake .. && make

# Clean the build directory and remove the app executable
clean:
	rm -rf build
	rm -f app

# Run the application
run: all
	./app
