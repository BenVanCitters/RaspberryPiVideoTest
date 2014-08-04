#ifndef _THREADED_OBJECT
#define _THREADED_OBJECT

#include "ofMain.h"

// this is not a very exciting example yet
// but ofThread provides the basis for ofNetwork and other
// operations that require threading.
//
// please be careful - threading problems are notoriously hard
// to debug and working with threads can be quite difficult


class ThreadedIndexUpdate : public ofThread
{
    int m_lookupSz;
    float* sinlkup;
    int m_width;
    int m_height;
    float m_updateDuration;
public:
    //public for easy, fast access to the array
    int* m_indecies;

    //provides interface to the cached sine float array
    float sn(double t);
    ThreadedIndexUpdate();
    ~ThreadedIndexUpdate();
    void setup(int width, int height);
    void start();
    void stop();
    void threadedFunction();
    float getUpdateDuration();
};

#endif
