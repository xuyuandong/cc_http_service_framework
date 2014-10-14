#ifndef _USER_DATA_H_
#define _USER_DATA_H_

#include <string>

namespace fuyi {

class UserData {
  public:
    std::string engine_host;
    int         engine_port;
    std::string engine_type;
};

}

#endif  // _USER_DATA_H_
