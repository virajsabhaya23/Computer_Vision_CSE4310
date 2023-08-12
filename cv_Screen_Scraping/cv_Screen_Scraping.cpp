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
 * @brief C++ Screen Scraping example for OpenCV
 * @author Viraj V. Sabhaya
**********************************************************************************************************************/

// include necessary dependencies
#include <iostream>
#include <cstdio>
#include "opencv2/opencv.hpp"
#include <opencv2/tracking.hpp>
#include <opencv2/core/ocl.hpp>

// Global variables
using namespace std;
using namespace cv;

Scalar GREEN_COLOR(0, 255, 0);
Scalar RED_COLOR(0, 0, 255); 
Scalar BLUE_COLOR(255, 0, 0);
Scalar YELLOW_COLOR(0, 255, 255);

// configuration parameters
#define NUM_COMMAND_LINE_ARGUMENTS 1

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

    // created displaying windows
    namedWindow("capturedFrame", WINDOW_AUTOSIZE);

    bool tracking = true;

    while(tracking)
    {
        Mat capturedFrame;
        // hsv frame for color detection
        Mat hsvFrame;

        // read frame from video source
        bool captureSuccess = capture.read(capturedFrame);

        // converting to hsc frame if the caputured frame is valid
        if (captureSuccess)
        {
            cvtColor(capturedFrame, hsvFrame, COLOR_BGR2HSV);

            // Thresholding to read only green color from the frame
            Mat greenMask;
            inRange(hsvFrame, Scalar(40, 40, 40), Scalar(70, 255, 255), greenMask);

            // Morphological operations to remove noise
            Mat kernel = getStructuringElement(MORPH_RECT, Size(5, 5));
            morphologyEx(greenMask, greenMask, MORPH_OPEN, kernel);
            morphologyEx(greenMask, greenMask, MORPH_CLOSE, kernel);

            // finding contours in the mask
            vector<vector<Point> > contours;
            vector<Vec4i> hierarchy;
            findContours(greenMask, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

            // drawing contours on the captured frame
            for (int i = 0; i < contours.size(); i++)
            {
                double area = contourArea(contours[i]);
                if(area > 1200 && area < 20000)
                {
                    drawContours(capturedFrame, contours, i, GREEN_COLOR, 2, LINE_8, hierarchy, 0);
                }
            }

            // finding the bounding box around the contours
            vector<Rect> boundRect(contours.size());
            for (int i = 0; i < contours.size(); i++)
            {
                boundRect[i] = boundingRect(contours[i]);
            }

            // drawing the bounding box on the captured frame
            for (int i = 0; i < contours.size(); i++)
            {   
                double area = contourArea(contours[i]);
                if(area > 1200 && area < 20000)
                {
                    rectangle(capturedFrame, boundRect[i].tl(), boundRect[i].br(), GREEN_COLOR, 2);
                }
            }

            // finding the center of the bounding box
            vector<Point> center(contours.size());
            for (int i = 0; i < contours.size(); i++)
            {
                center[i] = Point(boundRect[i].x + boundRect[i].width / 2, boundRect[i].y + boundRect[i].height / 2);
            }
        }

        // updating GUI window
        if (captureSuccess)
        {
            imshow("capturedFrame", capturedFrame);
        }
        else
        {
            cout << "Video playback finished! " << endl;
            tracking = false;
            continue;
        }

        // added because the viedo was playing too fast
        char waitTime = waitKey(500 / captureFPS);
        if(((char)waitKey(1) == 'q'))
        {
            tracking = false;
        }
    }

    // releasing the captured video and destryoing all windows
    capture.release();
    destroyAllWindows();
}
