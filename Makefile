
all:
	protoc --cpp_out=. helloworld.proto
	g++ helloworld.cc helloworld.pb.cc -o helloworld -lprotobuf -pthread

run: all
	./helloworld

clean:
	-rm -f helloworld helloworld.pb.cc helloworld.pb.h
