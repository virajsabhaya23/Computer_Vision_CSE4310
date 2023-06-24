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

/*******************************************************************************************************************//**
 * @file cv_Raster_Graphic_Editor.cpp
 * @brief C++ Raster Graphic Editor example for OpenCV
 * @author Viraj V. Sabhaya
 **********************************************************************************************************************/

// include necessary dependencies
#include <iostream>
#include <opencv2/opencv.hpp>

// Global variables
using namespace std;
using namespace cv;

enum Tools {
    EYEDROPPER,
    CROP,
    PENCIL,
    PAINT_BUCKET,
    RESET
};
Tools selectedTools = EYEDROPPER;
Point mouse_X_Y_Coordinates;

// configuration parameters
#define NUM_COMMAND_LINE_ARGUMENTS 1
#define DISPLAY_WINDOW_NAME "Raster Graphic Editor! @Viraj V. Sabhaya"

// EYEDROPPER
Vec3b eyeDropperValue(255, 255, 255); //initialized default value of WHITE


/*******************************************************************************************************************//**
 * @brief handler for image click callbacks
 * @param[in] event mouse event type
 * @param[in] x x coordinate of event
 * @param[in] y y coordinate of event
 * @param[in] flags additional event flags
 * @param[in] param user data passed as void* (in this case, the input image)
 * @author Viraj V. Sabhaya
 **********************************************************************************************************************/
static void clickCallback(int event, int x, int y, int flags, void* param)
{
    // cast userdata to a Mat
    Mat imageIn = *(Mat *)param;

    // handle the mouse event types
    // LEFT CLICK ****************************************************************************************************
    if(event == EVENT_LBUTTONDOWN)
    {
        cout << "LEFT CLICK (" << x << ", " << y << ")" << endl;

        if(selectedTools == EYEDROPPER)
        {
            // get the color value at the clicked pixel location and print to console
            Vec3b pixel = imageIn.at<Vec3b>(y, x);
            // display the color value B, G, R
            cout << "B: " << static_cast<int>(pixel[0]) << endl;
            cout << "G: " << static_cast<int>(pixel[1]) << endl;
            cout << "R: " << static_cast<int>(pixel[2]) << endl;
            // // update the color value
            // eyeDropperValue = pixel;
            // // update the clicked pixel color
            // imageIn.at<Vec3b>(y, x) = eyeDropperValue;
            // // display the updated image
            // imshow(DISPLAY_WINDOW_NAME, imageIn);
        }
        else if(selectedTools == CROP)
        {
            cout << "CROP" << endl;
        }
    }
    // RIGHT CLICK ***************************************************************************************************
    else if(event == EVENT_RBUTTONDOWN)
    {
        cout << "RIGHT CLICK (" << x << ", " << y << ")" << endl;
        // Handle right-click event
        selectedTools = static_cast<Tools>((selectedTools + 1) % (RESET + 1));

        switch (selectedTools)
        {
            case EYEDROPPER:
                cout << "Tool: Eyedropper" << endl;
                break;
            case CROP:
                cout << "Tool: Crop" << endl;
                break;
            case PENCIL:
                cout << "Tool: Pencil" << endl;
                break;
            case PAINT_BUCKET:
                cout << "Tool: Paint Bucket" << endl;
                break;
            case RESET:
                cout << "Tool: Reset" << endl;
                break;
        }
    }
    // else if(event == EVENT_MBUTTONDOWN)
    // {
    //     cout << "MIDDLE CLICK (" << x << ", " << y << ")" << endl;
    // }
    // MOUSE MOVE ****************************************************************************************************
    else if(event == EVENT_MOUSEMOVE)
    {
        // cout << "MOUSE OVER (" << x << ", " << y << ")" << endl;
    }
}



/*******************************************************************************************************************//**
 * @brief program entry point
 * @param[in] argc number of command line arguments
 * @param[in] argv string array of command line arguments
 * @return return code (0 for normal termination)
 * @author Viraj V. Sabhaya
 **********************************************************************************************************************/
int main(int argc, char *argv[])
{
    if(argc != NUM_COMMAND_LINE_ARGUMENTS + 1)
    {
        printf("Usage: %s <image_file>\n", argv[0]);
        return 0;
    }
    else
    {
        Mat imageInput; // input image
        imageInput = imread(argv[1], IMREAD_COLOR); // read the input image

        // check for file error
        if(!imageInput.data)
        {
            cout << "Error while opening file " << argv[1] << endl;
            return 0;
        }
        else
        {
            imshow(DISPLAY_WINDOW_NAME, imageInput);

            // display the Image size (Width, Height) and channels
            cout << "Image size: " << imageInput.size().width << endl;
            cout << "Image size: " << imageInput.size().height << endl;
            cout << "Image channels: " << imageInput.channels() << endl;

            // set the mouse callback function
            setMouseCallback(DISPLAY_WINDOW_NAME, clickCallback, &imageInput);

            waitKey();
        }
    }
}
