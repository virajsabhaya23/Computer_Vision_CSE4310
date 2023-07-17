//
//    Copyright 2023 Viraj V. Sabhaya
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

/*******************************************************************************************************************/ /**
 * @file cv_Traffic_Counter.cpp
 * @brief C++ Traffic Counter example for OpenCV
 * @author Viraj V. Sabhaya
**********************************************************************************************************************/

// include necessary dependencies
#include <iostream>
#include <cstdio>
#include "opencv2/opencv.hpp"
#include <opencv2/tracking.hpp>
#include <opencv2/core/ocl.hpp>
#include <opencv2/highgui.hpp>

// Global variables
using namespace std;
using namespace cv;

// configuration parameters
#define NUM_COMMAND_LINE_ARGUMENTS 1
#define DISPLAY_WINDOW_NAME "Traffic Counter! @Viraj V. Sabhaya"

/*******************************************************************************************************************/ /**
 * @brief program entry point
 * @param[in] argc number of command line arguments
 * @param[in] argv string array of command line arguments
 * @return return code (0 for normal termination)
 * @author Viraj V. Sabhaya
**********************************************************************************************************************/
int main(int argc, char *argv[])
{
    string videoFileName;

    if (argc != NUM_COMMAND_LINE_ARGUMENTS + 1)
    {
        printf("Usage: %s <video_file>\n", argv[0]);
        return 0;
    }
    else
    {
        videoFileName = argv[1];
    }

    // open video file
    VideoCapture capture(videoFileName);
    if (!capture.isOpened())
    {
        cout << "Unable to open video source, terminating program!" << endl;
        return 0;
    }
    
    int captureWidth = capture.get(CAP_PROP_FRAME_WIDTH);
    int captureHeight = capture.get(CAP_PROP_FRAME_HEIGHT);
    int captureFPS = capture.get(CAP_PROP_FPS);
    cout << "Video source opened successfully!" << endl;
    cout << "Width: " << captureWidth << endl;
    cout << "Height: " << captureHeight << endl;
    cout << "FPS: " << captureFPS << endl;

    // created display window
    namedWindow(DISPLAY_WINDOW_NAME, WINDOW_AUTOSIZE);

    bool tracking = true;
    while(tracking)
    {
        Mat Frame;

        // read frame from video source
        bool captureSuccess = capture.read(Frame);

        if(captureSuccess)
        {
            imshow(DISPLAY_WINDOW_NAME, Frame);
        }

        if(waitKey(1) == 27)
        {
            tracking = false;
        }
    }

    // release captured video and destryoing all windows
    capture.release();
    destroyAllWindows();
}
