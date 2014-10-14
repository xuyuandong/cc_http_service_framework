#ifndef _REQUEST_HANDLER_H_
#define _REQUEST_HANDLER_H_

#include <stdint.h>
#include <string>
#include "user_data.h"

namespace fuyi {

//class EngineClient;

class RequestHandler {
  public:
    RequestHandler(); 
    ~RequestHandler();

    void Init(const UserData* ud);
    void Process(const UserData* ud);
    void LogMessage(const char* message);

    const std::string& GetJsonResult() const {
      return json_str_;
    }

    char* GetRequestBuffer() {
      return request_buf_;
    }
    static uint32_t GetRequestBufferSize() {
      return request_buffer_size;
    }

  private:

    // 存放 post json串缓存
    char* request_buf_;  
    static const uint32_t request_buffer_size;

    // json 结果
    std::string json_str_;

    // thrift RPC engine
    //EngineClient *engine_client_;
};

}

#endif  // _REQUEST_HANDLER_H_
