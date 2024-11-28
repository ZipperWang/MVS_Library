
#include "opencv2/opencv.hpp"
#include <X11/Xlib.h> 
#include <assert.h>  
#include "math.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include "MVS_WORK.h"
using namespace cv;


int main()
{     
    
    MVS_WORK* mvs_work = new MVS_WORK(0);
    namedWindow("video",WINDOW_AUTOSIZE);
    while (true)
    {
        imshow("video",mvs_work->img);
        mvs_work->WaitForKeyPress();
    }
    delete mvs_work;
    destroyAllWindows();
    return 0;
}
