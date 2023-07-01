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
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>

// Global variables
using namespace std;
using namespace cv;

// configuration parameters
#define NUM_COMMAND_LINE_ARGUMENTS 1
#define DISPLAY_WINDOW_NAME_INPUT "Show me the Money! - Input Window"
#define DISPLAY_WINDOW_NAME_GRAY "Show me the Money! - GrayScale Window"
#define DISPLAY_WINDOW_NAME_OUTPUT "Show me the Money! - OUTPUT Window"

// Input image, GrayScale image and Coins Image(Circles the coins)
Mat imageInput;
Mat imageGray;
// Mat imageCoins;

/*******************************************************************************************************************/ /**
 * @brief program entry point
 * @param[in] argc number of command line arguments
 * @param[in] argv string array of command line arguments
 * @return return code (0 for normal termination)
 * @author Viraj V. Sabhaya
**********************************************************************************************************************/
int main(int argc, char *argv[])
{
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
    
    // display the Input Image size (Width, Height) and channels
    cout << "Image size: " << imageInput.size().width << endl;
    cout << "Image size: " << imageInput.size().height << endl;
    cout << "Image channels: " << imageInput.channels() << endl;

    // Converting the Color image to GrayScale image
    cvtColor(imageInput, imageGray, COLOR_BGR2GRAY);
    
    // display the images
    imshow(DISPLAY_WINDOW_NAME_INPUT, imageInput);
    imshow(DISPLAY_WINDOW_NAME_GRAY, imageGray);
    // imshow(DISPLAY_WINDOW_NAME_OUTPUT, imageCoins);
    
    waitKey();

    return 0;
}