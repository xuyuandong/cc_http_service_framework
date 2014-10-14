#include "glog/logging.h"
#include "json/json.h"

#include "http_proxy.h"
#include "request_handler.h"

using namespace std;

namespace fuyi {

#define RESPONSE_ERROR(err) do { \
  Json::Value root;              \
  root["code"] = 404;            \
  root["message"] = err;         \
  std::string res = root.toStyledString();  \
  mg_printf(conn, "HTTP/1.1 200 \r\n" \
      "Content-Type: text/plain\r\n"  \
      "Content-Length: %lu\r\n"       \
      "\r\n"  \
      "%s\n", \
      res.length(), res.c_str()); \
  return 0; \
} while (0)

UserData* HttpProxy::user_data_ = NULL;

void HttpProxy::Start(const char** options) {
  memset(&callbacks_, 0, sizeof(callbacks_));
  callbacks_.thread_start = &HttpProxy::thread_start;
  callbacks_.begin_request = &HttpProxy::begin_request;
  callbacks_.thread_stop = &HttpProxy::thread_stop;
  ctx_ = mg_start(&callbacks_, user_data_, options);
}

void HttpProxy::Stop() {
  mg_stop(ctx_);
}

void HttpProxy::thread_start(void *server_data, void **conn_data) {
  if (NULL != conn_data) { // worker thread
    RequestHandler** cd = (RequestHandler**)conn_data;
    *cd = new(nothrow) RequestHandler();
    if (NULL == *cd) {
      LOG(WARNING) << "failed to alloc memory.";
      exit(1);
    }
    (*cd) -> Init(user_data_);
  }
  LOG(INFO) << "start thread.";
}

void HttpProxy::thread_stop(void *server_data, void **conn_data) {
  RequestHandler* cd = (RequestHandler*)conn_data;
  delete cd;
  cd = NULL;
  LOG(INFO) << "stop thread.";
}

int HttpProxy::begin_request(struct mg_connection *conn) {
  const struct mg_request_info *request_info = mg_get_request_info(conn);
  RequestHandler* cd = (RequestHandler*)request_info->conn_data;

  // read request content
  char* request_buffer = cd->GetRequestBuffer();
  if (!strcmp(request_info->request_method, "GET")) {
    LOG(INFO) << "recv get request '" << request_info->query_string << "'"; 

  } else { // post request
    int read_size = 0;
    char* buf_beg = request_buffer;
    int buf_size = RequestHandler::GetRequestBufferSize();
    do {
      read_size = mg_read(conn, buf_beg, buf_size);
      if (read_size > 0) {
        buf_beg += read_size;
        buf_size -= read_size;
      }
    } while(read_size > 0);
    (*buf_beg) = '\0';

    LOG(INFO) << "recv post request '" << request_buffer << "'";
  }
  
  //const UserData* ud = (UserData*)request_info->user_data;
  cd->Process(user_data_);

  // Send HTTP reply to the client
  const std::string& json_result = cd->GetJsonResult();
  mg_printf(conn,
      "HTTP/1.1 200 OK\r\n"
      "Content-Type: text/plain\r\n"
      "Content-Length: %lu\r\n"        // Always set Content-Length
      "\r\n"
      "%s\n",
      json_result.length(), json_result.c_str());

  return 1;
}

int HttpProxy::log_message(struct mg_connection *conn, const char* message) {
  const struct mg_request_info *request_info = mg_get_request_info(conn);
  RequestHandler* cd = (RequestHandler*)request_info->conn_data;
  cd -> LogMessage(message);
  return 1;
}

} // end namespace
