#include "glog/logging.h"
#include "json/json.h"

//#include <thrift/protocol/TBinaryProtocol.h>
//#include <thrift/protocol/TProtocol.h>
//#include <thrift/transport/TTransport.h>
//#include <thrift/transport/TSocket.h>
//#include <thrift/transport/TTransportUtils.h>

#include "request_handler.h"

using std::string;

namespace fuyi {

const uint32_t RequestHandler::request_buffer_size = 32 * 1024;    // 32 KB

RequestHandler::RequestHandler() {
  request_buf_ = new (std::nothrow) char[request_buffer_size];
  if (request_buf_ == NULL) {
    LOG(FATAL) << "Request Handler construct failed!" ;
    exit(1);
  }
}

RequestHandler::~RequestHandler() {
  delete[] request_buf_;
  request_buf_ = NULL;
}

void RequestHandler::LogMessage(const char* message) {
  LOG(INFO) << message;
}

void RequestHandler::Init(const UserData* user) {
  /*
  shared_ptr<TTransport> socket(new TSocket(user->engine_host, user->engine_port));
  shared_ptr<TTransport> transport;
  if ((user->engine_type).compare("block")) {
    shared_ptr<TTransport> tmp(new TBufferedTransport(socket));
    transport = tmp;
  } else {  // nonblock
    shared_ptr<TTransport> tmp(new TFramedTransport(socket));
    transport = tmp;
  }

  shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
  engine_client_ = new RecommEngineClient(protocol);

  while (1) {
    try {
      transport->open();
      LOG(INFO) << "transport open successed";
      break;
    } catch(...) {
      LOG(ERROR) << "Failed connect to recomm engine. reconnecting...";
    }
    sleep(5);
  }
  */
}

void RequestHandler::Process(const UserData* user) {
  //EngineRequest req;
  //EngineResponse res;
  //engine_client_ -> Query(req, res);
  // save to json string
} 

}  // end namespace
