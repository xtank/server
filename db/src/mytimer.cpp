#include "mytimer.h"

MyTimer::MyTimer(const std::string& method) {
    gettimeofday(&start_,NULL);
    method_ = method;
}

MyTimer::~MyTimer() {
    gettimeofday(&end_,NULL);

    float timeuse = 1000000*(end_.tv_sec - start_.tv_sec)+end_.tv_usec - start_.tv_usec;

    KINFO_LOG(0, "method %s timer=%f s", method_.c_str(), timeuse/1000000);
}

