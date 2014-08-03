//
//  ThreadedIndexUpdate.cpp
//  RaspberryPiVideo
//
//  Created by Ben Van Citters on 8/2/14.
//
//
#include "ThreadedIndexUpdate.h"
#include <ofMain.h>

ThreadedIndexUpdate::ThreadedIndexUpdate()
{
    lookupSz = 600;
    sinlkup = new float[(int)((lookupSz)*M_PI*2)];
    for(int i = 0; i < lookupSz; i++)
    {
        sinlkup[i] = sin(i*2.f*M_PI/lookupSz);
    }
}

//cached sin fetching function
float ThreadedIndexUpdate::sn(double t)
{
    //    t += M_PI*2*(abs((int)(t/(2*M_PI))) + 1);
    t =fmodl(t,2*M_PI) + 2*M_PI;
    t = fmodl(t,2*M_PI);
    t /= 2*M_PI;
    t *= lookupSz;
    return sinlkup[(int)t];
}

//clean up dynamic memory allocation
ThreadedIndexUpdate::~ThreadedIndexUpdate()
{
    cout << "deleting arrays" << endl;
    delete m_indecies;
    delete sinlkup;
}


void ThreadedIndexUpdate::start()
{
    startThread(true, false);   // blocking, verbose
}

void ThreadedIndexUpdate::stop()
{
    stopThread();
}

void ThreadedIndexUpdate::setup(int width, int height)
{
    m_width = width;
    m_height = height;
    m_indecies = new int[width*height*3];
}

//--------------------------
void ThreadedIndexUpdate::threadedFunction()
{
    cout << "we got in the thread" << endl;
    int totalPixels = m_width*m_height*3;
    float wxh =totalPixels/3;
    //loop over all 'pixels' and determine a new displacement for each as long as thie thread is running
    while( isThreadRunning() != 0 )
    {
   
        float curTime = ofGetElapsedTimef();
        float startTime =curTime;
         cout << "are we running the  thread" <<  curTime << endl;
        ofVec3f rotCtr(m_width*(sn(curTime/4)+1)/2,m_height*(sn(500+curTime/5)+1)/2);
        float denom = (20.*(1+sn(curTime/2.1))/2);
        float dMult =sn(curTime/8)/30.f;
        for (int i = 0; i < totalPixels; i++)
        {
            float x = fmod(i,m_width*3.0f);
            float y = i/(m_width*3.f);
            
            float ang = atan2(y-rotCtr[0],x-rotCtr[1]);
            float dist = 10.6+ofDist(x,y,rotCtr[0],rotCtr[1]);
            float d = dMult*dist;
            x += (d*sn(curTime/3.2+ang));
            y += (d*sn(curTime/4+ang+HALF_PI));
            y = (y+ 1.*sn((x+curTime)/denom));
            ////            x = (x- 4.*sn((y+curTime/2)/(18.*(1+sn(curTime/5))/2)));
            //            float d = 25;
            //            x += (d*sn(HALF_PI+y/7.f+curTime*1.5));
            //            y += (d*sn(x/10.f+curTime*2));
            int ny = (int)(y);
            int nx = (int)(x);
            ny = (ny%m_height) * m_width*3;
            nx = nx%(m_width*3);
            int tmpIndex = (ny + nx);
            m_indecies[i] =(tmpIndex%(totalPixels) + (totalPixels))%(totalPixels);
        }
    }
    
}