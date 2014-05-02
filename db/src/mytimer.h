#ifndef __MY_TIME__H__
#define __MY_TIME__H__

#include<sys/time.h>
#include<string>

extern "C" {
#include <libtaomee/log.h>
}

class MyTimer {

private:
    struct timeval start_;
    struct timeval end_;
    std::string method_;

public:    
    MyTimer(const std::string& method); 
    ~MyTimer(); 
};

#endif
