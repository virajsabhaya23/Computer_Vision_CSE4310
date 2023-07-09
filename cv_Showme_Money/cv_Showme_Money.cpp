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
 * @file cv_Raster_Graphic_Editor.cpp
 * @brief C++ Raster Graphic Editor example for OpenCV
 * @author Viraj V. Sabhaya
**********************************************************************************************************************/

// include necessary dependencies
#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

// Global variables
using namespace std;
using namespace cv;

// configuration parameters
#define NUM_COMMAND_LINE_ARGUMENTS 1

// Color Constants
Scalar COLOR_RED = CV_RGB(255, 0, 0); // For pennies
Scalar COLOR_GREEN = CV_RGB(0, 255, 0); // For quarters
Scalar COLOR_BLUE = CV_RGB(0, 0, 255); // For dimes
Scalar COLOR_YELLOW = CV_RGB(255, 255, 0); // For nickels

/*******************************************************************************************************************/ /**
 * @brief program entry point
 * @param[in] argc number of command line arguments
 * @param[in] argv string array of command line arguments
 * @return return code (0 for normal termination)
 * @author Viraj V. Sabhaya
**********************************************************************************************************************/
int main(int argc, char *argv[])
{
    // Input image, GrayScale image and Coins Image(Circles the coins)
    Mat imageInput;
    Mat imageGray;
    Mat imageEdges;
    Mat imageResult;

    if (argc != NUM_COMMAND_LINE_ARGUMENTS + 1)
    {
        printf("Usage: %s <image_file>\n", argv[0]);
        return 0;
    }
    else
    {
        imageInput = imread(argv[1], IMREAD_COLOR);

        // check for file error
        if (!imageInput.data)
        {
            cout << "Error while opening file " << argv[1] << endl;
            return 0;
        }
    }

    // Copy the input image to the result image
    imageResult = imageInput.clone();

    Mat imageEllipse = imageInput.clone();
    Mat imageRectangles = imageInput.clone();
    
    // display the Input Image size (Width, Height) and channels
    cout << "Input Image details ... " << endl;
    cout << "Image width: " << imageInput.size().width << endl;
    cout << "Image height: " << imageInput.size().height << endl;
    cout << "Image channels: " << imageInput.channels() << endl;

    // Converting the Color image to GrayScale image
    cvtColor(imageInput, imageGray, COLOR_BGR2GRAY);

    // Finding edges in the image using canny edge detection
    const double cannyThreshold1 = 100;
    const double cannyThreshold2 = 200;
    const int cannyAperture = 3;
    Canny(imageGray, imageEdges, cannyThreshold1, cannyThreshold2, cannyAperture);

    // Removing noise from the edges using ERODE and DILATE
    Mat imageEnD;
    int morphologySize = 1;
    dilate(imageEdges, imageEnD, Mat(), Point(-1, -1), morphologySize);
    erode(imageEnD, imageEnD, Mat(), Point(-1, -1), morphologySize);
    
    // Locating image contours by applying threshold or canny
    vector<vector<Point> > contours;
    findContours(imageEnD, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));

    // Drawing contours around the coins
    for(int i = 0; i < contours.size(); i++)
    {
        drawContours(imageResult, contours, i, COLOR_RED, 2);
    }

    // rectangles
    vector<RotatedRect> minAreaRectangles(contours.size());
    for (int i = 0; i < contours.size(); i++)
    {
        minAreaRectangles[i] = minAreaRect(contours[i]);
    }
    
    for(int i = 0; i < contours.size(); i++)
    {
        Point2f rectanglePoints[4];
        minAreaRectangles[i].points(rectanglePoints);
        for(int j = 0; j < 4; j++)
        {
            line(imageRectangles, rectanglePoints[j], rectanglePoints[(j+1)%4], COLOR_GREEN);
        }
    }
    // fit ellipses to contours 
    vector<RotatedRect> fittedEllipses(contours.size());
    for (int i = 0; i < contours.size(); i++)
    {
        if(contours.at(i).size() > 5)
        {
            fittedEllipses[i] = fitEllipse(contours[i]);
        }
    }
    
    const int minEllipseInliers = 5;
    for(int i = 0; i < contours.size(); i++)
    {
        if(contours.at(i).size() > minEllipseInliers)
        {
            ellipse(imageEllipse, fittedEllipses[i], COLOR_BLUE, 2);
        }
    }
    

    // display the images
    imshow("input image", imageInput);
    imshow("image GRAY w/ BLUR", imageGray);
    imshow("image EDGES", imageEdges);
    imshow("image EDGES w/ erode and dilate", imageEnD);
    imshow("image contours", imageResult);
    imshow("image rectangles", imageRectangles);
    imshow("image ellipses", imageEllipse); 
    // imshow(DISPLAY_WINDOW_NAME_GRAY, imageResult);

    waitKey();

}
