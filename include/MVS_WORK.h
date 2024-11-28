
#include <opencv2/opencv.hpp>
#include <thread>
#include <stdio.h>
#include "string.h"



using namespace cv;

enum CONVERT_TYPE
{
	OpenCV_Mat = 0,    // Most of the time, we use 'Mat' format to store image data after OpenCV V2.1
	OpenCV_IplImage = 1,   //we may also use 'IplImage' format to store image data, usually before OpenCV V2.1
};

class MVS_WORK
{
public:
	MVS_WORK(unsigned int nIndex);
	~MVS_WORK();
	void WaitForKeyPress(void);
	Mat img;

protected:
	void Start();
	void Stop();
	void openDevice();
	void creatHandle(void);
	void enumDevice(void);
	void TempWorkThread(void* pUser);
	


private:
	unsigned int g_nPayloadSize;
	int nRet;
	void* handle;
	unsigned char* pData;
	unsigned int nFormat;
	unsigned int nIndex;
	std::thread WorkThread;
};