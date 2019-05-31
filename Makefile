
all:
	protoc --cpp_out=. helloworld.proto
	g++ main.cc helloworld.pb.cc -o main -lprotobuf -pthread

run: all
	./main

clean:
	-rm -f main helloworld.pb.cc helloworld.pb.h
