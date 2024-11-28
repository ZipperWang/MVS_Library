#include "MVS_WORK.h"
#include "MvCameraControl.h"
#include <thread>
#include <iostream>
#include <unistd.h>

using namespace std;
using namespace cv;


MVS_WORK::MVS_WORK(unsigned int nIndex){
	this->nIndex = nIndex;
	this->Start();
}
MVS_WORK::~MVS_WORK(){
	this->Stop();
}
void MVS_WORK::openDevice(){
		
		this->nRet = MV_CC_OpenDevice(this->handle);
        if (MV_OK != this->nRet)
        {
            printf("MV_CC_OpenDevice fail! nRet [%x]\n", this->nRet);
            exit(114514);
        }
}
void MVS_WORK::creatHandle(){
	
	
}
void MVS_WORK::enumDevice(){
	this->nRet = MV_CC_Initialize();
		if (MV_OK != nRet)
		{
			printf("Initialize SDK fail! nRet [0x%x]\n", nRet);
			exit(114514);
		}
		
		MV_CC_DEVICE_INFO_LIST stDeviceList;
		memset(&stDeviceList, 0, sizeof(MV_CC_DEVICE_INFO_LIST));
	  this->nRet = MV_CC_EnumDevices(MV_GIGE_DEVICE | MV_USB_DEVICE | MV_GENTL_CAMERALINK_DEVICE | MV_GENTL_CXP_DEVICE | MV_GENTL_XOF_DEVICE, &stDeviceList);
        if (MV_OK != this->nRet)
        {
            printf("MV_CC_EnumDevices fail! nRet [%x]\n", this->nRet);
            exit(114514);
        }

        if (stDeviceList.nDeviceNum > 0)
        {
            for (int i = 0; i < stDeviceList.nDeviceNum; i++)
            {
                MV_CC_DEVICE_INFO* pDeviceInfo = stDeviceList.pDeviceInfo[i];
                if (NULL == pDeviceInfo)
                {
                    break;
                } 
				
				printf("[device %d]:\n", i);
			       
            }  
        } 
        else
        {
            printf("Find No Devices!\n");
            exit(114514);
        }
		printf("Input index :%d\n",this->nIndex);

        this->nRet = MV_CC_CreateHandle(&this->handle, stDeviceList.pDeviceInfo[nIndex]);
        if (MV_OK != this->nRet)
        {
            printf("MV_CC_CreateHandle fail! nRet [%x]\n", this->nRet);
            exit(114514);
        }
	
}
void MVS_WORK::WaitForKeyPress(){
	if (waitKey(5)==27)
	{
		exit(114514);
	}
	
	
}
void MVS_WORK::TempWorkThread(void* pUser)
{
    MV_FRAME_OUT stImageInfo = {0};
	MV_CC_IMAGE  stImage = {0};
   
    while(1)
    {
        nRet = MV_CC_GetImageBuffer(pUser, &stImageInfo, 1000);
        if (nRet == MV_OK)
        {
        
			stImage.nWidth = stImageInfo.stFrameInfo.nExtendWidth;
			stImage.nHeight = stImageInfo.stFrameInfo.nExtendHeight;
			stImage.enPixelType = stImageInfo.stFrameInfo.enPixelType;
			stImage.pImageBuf = stImageInfo.pBufAddr;
			stImage.nImageBufLen = stImageInfo.stFrameInfo.nFrameLenEx;
            this->img = Mat(stImageInfo.stFrameInfo.nHeight, stImageInfo.stFrameInfo.nWidth, CV_8UC1, stImageInfo.pBufAddr);
            //cv::imshow("Captured Image", img);
			//this->WaitForKeyPress();
            this->nRet = MV_CC_FreeImageBuffer(pUser, &stImageInfo);
            if(this->nRet != MV_OK)
            {
                printf("Free Image Buffer fail! nRet [0x%x]\n", this->nRet);
            }
        }
        else
        {
            printf("Get Image fail! nRet [0x%x]\n", this->nRet);
        }
    }

}


void MVS_WORK::Start(){

	this->enumDevice();

	this->creatHandle();

	this->openDevice();

	this->nRet = MV_CC_StartGrabbing(this->handle);
        if (MV_OK != nRet)
        {
            printf("MV_CC_StartGrabbing fail! nRet [%x]\n", nRet);
            exit(114514);
        }
         this->WorkThread = thread(&MVS_WORK::TempWorkThread,this,handle);

}
void MVS_WORK::Stop(){
     
	    // 停止取流
        // stop grab image
        this->nRet = MV_CC_StopGrabbing(this->handle);
        if (MV_OK != this->nRet)
        {
            printf("MV_CC_StopGrabbing fail! nRet [%x]\n", this->nRet);
            exit(114514);
        }

        // 关闭设备
        // close device
        this->nRet = MV_CC_CloseDevice(this->handle);
        if (MV_OK != this->nRet)
        {
            printf("MV_CC_CloseDevice fail! nRet [%x]\n", this->nRet);
            exit(114514);
        }

        // 销毁句柄
        // destroy handle
        this->nRet = MV_CC_DestroyHandle(this->handle);
        if (MV_OK != this->nRet)
        {
            printf("MV_CC_DestroyHandle fail! nRet [%x]\n", this->nRet);
            exit(114514);
        }
        this->handle = NULL;
		if (this->handle != NULL)
    	{
        	MV_CC_DestroyHandle(this->handle);
        	this->handle = NULL;
    	}
    	// ch:反初始化SDK | en:Finalize SDK
		MV_CC_Finalize();


    	printf("exit.\n");
}
