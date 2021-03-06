#include <iostream>
#include <fstream>
#include <string>
#include <google/protobuf/util/json_util.h>

#include "helloworld.pb.h"

using namespace helloworld;

// Basic implementation of the server.
class HelloWorldServiceImpl : public HelloWorldService {
 public:
  void Print(google::protobuf::RpcController* controller,
             const HelloWorldRequest* request,
             HelloWorldResponse* response,
             google::protobuf::Closure* done) {
    response->set_world("world");
    done->Run();
  }
};

// Basic implementation of a FCM channel.
class RpcChannel : public google::protobuf::RpcChannel {
  void CallMethod(const google::protobuf::MethodDescriptor * method,
                  google::protobuf::RpcController* controller,
                  const google::protobuf::Message* request,
                  google::protobuf::Message* response,
                  google::protobuf::Closure* done) {
    // Serializes the request to JSON.
    std::string buffer;
    google::protobuf::util::JsonPrintOptions options;
    options.add_whitespace = true;
    options.always_print_primitive_fields = true;
    options.preserve_proto_field_names = true;
    google::protobuf::util::MessageToJsonString(*request, &buffer, options);

    // TODO(goto): insert network request here.
    std::cout << "Sending request to server: " << std::endl;
    std::cout << buffer << std::endl;

    // Unserializes the JSON request to a message.
    HelloWorldRequest parsed;
    google::protobuf::util::JsonParseOptions options2;
    google::protobuf::util::JsonStringToMessage(buffer, &parsed, options2);

    impl.CallMethod(method, controller, &parsed, response, done);
  }

 private:

  HelloWorldServiceImpl impl;
};

class RpcController : public google::protobuf::RpcController  {
  void Reset() {
  }
  bool Failed() const {
    return false;
  }
  std::string ErrorText() const {
    return "";
  }
  void StartCancel() {
  }
  void SetFailed(const std::string& reason) {
  }
  bool IsCanceled() const {
    return false;
  }
  void NotifyOnCancel(google::protobuf::Closure * callback) {
  }
};

void Done(const HelloWorldResponse* response) {
  std::cout << "Done: " << response->world() << std::endl;
}

int main(int argc, char* argv[]) {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  // Uses our custom channels.
  RpcChannel channel;
  RpcController controller;

  // Creates the client stub.
  HelloWorldService_Stub service(&channel);

  HelloWorldRequest request;
  HelloWorldResponse response;

  request.set_hello("foo bar");

  google::protobuf::Closure* callback = google::protobuf::NewCallback(
      &Done, static_cast<const HelloWorldResponse*>(&response));

  // Sends the RPC.
  service.Print(&controller,
                &request,
                &response,
                callback);

  google::protobuf::ShutdownProtobufLibrary();

  return 0;
}
