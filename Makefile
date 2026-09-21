all:
	cmake --build build
	./build/pb

run:
	./build/pb

prepare:
	cmake -B build

clean:
	rf -r build