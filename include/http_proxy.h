#ifndef _HTTP_PROXY_H_
#define _HTTP_PROXY_H_

#include "common.h"
#include "mongoose.h"

namespace fuyi {

class UserData;

class HttpProxy {
  public:
    HttpProxy(){}
    ~HttpProxy(){}

    static void Init(UserData* ud){
      user_data_ = ud;
    }

    void Start(const char** options);
    void Stop();

  private:
    static void thread_start(void *server_data, void **conn_data);
    static void thread_stop(void *server_data, void **conn_data);
    static int begin_request(struct mg_connection *conn);
    static int log_message(struct mg_connection *conn, const char* message);
 
  private:
    static UserData* user_data_;

    struct mg_context *ctx_;
    struct mg_callbacks callbacks_;
    
  DISALLOW_COPY_AND_ASSIGN(HttpProxy);
};


}

#endif  // _HTTP_PROXY_H_
