build:
	mkdir -p build
	cd build && cmake ..
	cd build && make

run:
	./build/meowc example.meow
	clang out.ll -o app
	./app
