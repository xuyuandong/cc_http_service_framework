#include <signal.h>
#include <iostream>

#include "gflags/gflags.h"
#include "glog/logging.h"

#include "http_proxy.h"
#include "user_data.h"

using namespace std;
using namespace fuyi;

DEFINE_string(port, "8080", "The server listen port.");
DEFINE_string(num_threads, "2", "Num of worker threads of server.");
DEFINE_string(access_log_file, "", "The server access log file.");
DEFINE_string(error_log_file, "", "The server error log file.");

int main(int argc, char* argv[]) {
  ::google::InitGoogleLogging(argv[0]);
  ::google::ParseCommandLineFlags(&argc, &argv, true);
  
  // Setup signal handler: quit on Ctrl-C 
  signal(SIGPIPE, SIG_IGN);
  FLAGS_logbufsecs  = 0; 

  // List of mongoose options. Last element must be NULL.
  const char *options[] = {
    "listening_ports", FLAGS_port.c_str(),
    "num_threads", FLAGS_num_threads.c_str(),
    "access_log_file", FLAGS_access_log_file.c_str(),
    "error_log_file", FLAGS_error_log_file.c_str(),
    NULL};

  UserData ud;

  HttpProxy hproxy;
  hproxy.Init(&ud);

  // Start the web server.
  hproxy.Start(options);
  LOG(INFO) << "Server start running.";

  // Looping to check new data and updating.
  while (1) {
    sleep(60);
  }

  // Stop the server.
  hproxy.Stop();
  LOG(INFO) << "Server has stoped.";

  return 0;
}

