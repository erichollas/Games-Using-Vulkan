/*
* TITLE:	Vulkan Render Engine
* BY:		Eric Hollas	
*
* FILE:		Clock.h	
* DETAILS:	This file defines the clock object. For the implementation of this game	
*				it will be used to print the frame statistics to the window title bar.
*/
#pragma once
#include <time.h>
#include <iostream>

class Clock {
public:
	Clock() {}
	~Clock() {}

	/*
	* Function: calcFrameStats
	*
	* Paramters: none
	*
	* Return Type: void
	*
	* Description: the algorithm to calculate the fps and mspf variables
	*
	*/
	void calcFrameStats() {
		frameCount++;
		endTimeStamp = clock();
		totalTime += endTimeStamp - beginTimeStamp;
		if (clockConvert(totalTime) > 1000.0f) { //every second
			fps = (float)frameCount * 0.5f + fps * 0.5f; //more stable
			frameCount = 0;
			totalTime -= CLOCKS_PER_SEC;
			mspf = 1000.0f / (fps == 0.0f ? 0.001f : fps);
		}
	}
	/*
	* Function: startTimer
	*
	* Paramters: none
	*
	* Return Type: void
	*
	* Description: sets the starting time
	*
	*/
	void startTimer() {
		beginTimeStamp = clock();
	}

	/*
	* the following two functions are accessor functions
	*
	*/
	float getFPS() const {
		return fps;
	}
	float getMSPF() const {
		return mspf;
	}

private:
	clock_t beginTimeStamp;
	clock_t endTimeStamp;
	clock_t totalTime;
	uint32_t frameCount;
	float fps;
	float mspf;

	/*
	* Function: clockConvert
	*
	* Paramters: clock_t time
	*
	* Return Type: float
	*
	* Description: a private method, ensures the calcFrameStats method
	*				calcs resonable values
	*
	*/
	float clockConvert(clock_t time) {
		return (((float)time) / ((float)CLOCKS_PER_SEC)) * 1000.0f;
	}
};