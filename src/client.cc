#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#include <iostream>
#include <fstream>

#include <curl/curl.h>
#include <glog/logging.h>
#include <gflags/gflags.h>

using namespace std;

DEFINE_string(input_file, "", "input json file.");
DEFINE_string(output_file, "", "output json file.");
DEFINE_string(URL, "", "sevice url.");
DEFINE_string(port, "", "service port.");
DEFINE_bool(do_not_stop, true, "whether do not stop post the request");


size_t WriteData(char* data, size_t size, size_t nmemb, string& content) {
  size_t sizes = size * nmemb;
  string temp(data, sizes);
  content = temp;
  return sizes;
} 

double GetDis(struct timeval* begin, struct timeval* end) {
  long usec_dis = end->tv_usec - begin->tv_usec;
  long sec_dis = end->tv_sec - begin->tv_sec;
  return sec_dis + usec_dis * 1.0 / 1000000;
}


int main(int argc, char** argv) {
  google::InitGoogleLogging(argv[0]);
  google::ParseCommandLineFlags(&argc, &argv, true);

  string url = FLAGS_URL + ":" + FLAGS_port;

  string content;
  CURLcode return_code;

  string line;

  vector<string> json_vec;
  ifstream infile;
  infile.open(FLAGS_input_file.c_str());
  if(!infile) {
    LOG(ERROR) << "can't open " << FLAGS_input_file;
    return -1;
  }
  
  while(!infile.eof()) {
    getline(infile, line);
    if(line.empty())
      continue;
    json_vec.push_back(line);
  }
  infile.close();

  // global init
  return_code = curl_global_init(CURL_GLOBAL_DEFAULT);
  if (return_code != CURLE_OK) {
    LOG(ERROR) << "init libcurl failed.";
    return -1;
  }

  // curl handler init
  CURL* curl = curl_easy_init();
  if (NULL == curl) {
    LOG(ERROR) << "handler failed.";
    curl_global_cleanup();
    return -1;
  }

  // set url
  return_code = curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
  if (return_code != CURLE_OK) {
    LOG(ERROR) << "set url failed.";
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    return -1;
  }


  // set write function
  return_code = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteData);
  if (return_code != CURLE_OK) {
    LOG(ERROR) << "set write function failed.";
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    return -1;
  }

  // set write data
  return_code = curl_easy_setopt(curl, CURLOPT_WRITEDATA, &content);
  if (return_code != CURLE_OK) {
    LOG(ERROR) << "set write data failed.";
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    return -1;
  }

  // set post method
  return_code = curl_easy_setopt(curl, CURLOPT_POST, 1);
  if (return_code != CURLE_OK) {
    LOG(ERROR) << "set post method failed.";
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    return -1;
  }


  int success = 0;

  double success_time = 0.0;

  struct timeval begin_time, end_time;

  double distime = 0;
  ofstream outfile;
  outfile.open(FLAGS_output_file.c_str());
  if (!outfile) {
    LOG(ERROR) << "can't open output file \"" << FLAGS_output_file << "\"";
    return -1;
  }

  bool has_output = false;

  while(1) {
    for (uint32_t i = 0; i < json_vec.size(); ++i) {
      return_code = curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_vec[i].c_str());
      if (return_code != CURLE_OK) {
        LOG(ERROR) << "set post fields failed.";
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        return -1;
      }
      while (1) {
        gettimeofday(&begin_time, NULL);
        return_code = curl_easy_perform(curl);
        gettimeofday(&end_time, NULL);
        if (return_code == 0) {
          ++success;
          distime = GetDis(&begin_time, &end_time);
          success_time += distime;
          LOG(INFO) << "this run time: " << distime << " | success: " << success << " | totol time: " << success_time;
          if (!has_output) {
            outfile << content;
          }
          break;
        }
        else {
          //LOG(ERROR) << "query error, status is " << curl_easy_strerror(return_code);
        }
      }
    }
    has_output = true;
    if (!FLAGS_do_not_stop)
      break;
  }

  outfile.close();

  curl_easy_cleanup(curl);
  curl_global_cleanup();
  return 0;

}
