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

// Global variables
using namespace std;
using namespace cv;

enum Tools
{
    EYEDROPPER,
    CROP,
    PENCIL,
    PAINT_BUCKET,
    RESET
};
Tools selectedTools = EYEDROPPER;
Point mouse_X_Y_Coordinates;

Point crop_start_point;
Point crop_end_point;

bool is_drawing_rect = false;
bool is_drawing_line = false;
bool is_filling_color = false;

// configuration parameters
#define NUM_COMMAND_LINE_ARGUMENTS 1
#define DISPLAY_WINDOW_NAME "Raster Graphic Editor! @Viraj V. Sabhaya"

// EYEDROPPER
Vec3b eyeDropperValue(255, 255, 255); // initialized default value of WHITE

// Input image and Reset image
Mat imageInput;
Mat imageReset;

/*******************************************************************************************************************//**
 * @brief handler for image click callbacks
 * @param[in] event mouse event type
 * @param[in] x x coordinate of event
 * @param[in] y y coordinate of event
 * @param[in] flags additional event flags
 * @param[in] param user data passed as void* (in this case, the input image)
 * @author Christoper D. McMurrough
 **********************************************************************************************************************/
static void clickCallback(int event, int x, int y, int flags, void *param)
{
    // cast userdata to a Mat
    Mat& imageIn = *(Mat *)param;

    // EYEDROPPER ****************************************************************************************************
    if (event == EVENT_LBUTTONDOWN && selectedTools == EYEDROPPER)
    {
        cout << "LEFT CLICK (" << x << ", " << y << ")" << endl;

        // get the color value at the clicked pixel location and print to console
        Vec3b pixel = imageIn.at<Vec3b>(y, x);
        // display the color value B, G, R
        cout << "B: " << static_cast<int>(pixel[0]) << endl;
        cout << "G: " << static_cast<int>(pixel[1]) << endl;
        cout << "R: " << static_cast<int>(pixel[2]) << endl;

        // updating the color value
        eyeDropperValue = pixel;
    }

    // CROP *********************************************************************************************************
    else if (event == EVENT_LBUTTONDOWN && selectedTools == CROP)
    {
        cout << "Crop selected ---" << endl;
        crop_start_point = Point(x, y);
    }
    else if (event == EVENT_LBUTTONUP && selectedTools == CROP)
    {
        crop_end_point = Point(x, y);

        Rect region(crop_start_point, crop_end_point);
        // rectangle(imageIn, region, Scalar(0, 0, 0), 5);
        imageIn = imageIn(region).clone();
        imshow(DISPLAY_WINDOW_NAME, imageIn);
    }

    // PENCIL *******************************************************************************************************
    else if (event == EVENT_LBUTTONDOWN && selectedTools == PENCIL)
    {
        cout << "Pencil selected --- " << endl;
        is_drawing_line = true;

        // Change the color of the target pixel to the eyedropper value
        imageIn.at<Vec3b>(y, x) = eyeDropperValue;
        line(imageIn, Point(x, y), Point(x, y), eyeDropperValue, 2);
        imshow(DISPLAY_WINDOW_NAME, imageIn);
    }
    else if (event == EVENT_MOUSEMOVE && selectedTools == PENCIL && is_drawing_line)
    {
        cout << "Pencil drawing ... " << endl;

        // Change the color of the target pixel to the eyedropper value
        imageIn.at<Vec3b>(y, x) = eyeDropperValue;
        line(imageIn, Point(x, y), Point(x, y), eyeDropperValue, 2);
        imshow(DISPLAY_WINDOW_NAME, imageIn);
    }
    else if (event == EVENT_LBUTTONUP && selectedTools == PENCIL)
    {
        is_drawing_line = false;
        cout << "Pencil drawing finished." << endl;
    }

    // PAINT_BUCKET *************************************************************************************************
    else if (event == EVENT_LBUTTONDOWN && selectedTools == PAINT_BUCKET)
    {
        is_filling_color = true;

        // Get the current color value of the clicked pixel
        Vec3b targetColor = imageIn.at<Vec3b>(y, x);

        // Fill color using the floodFill algorithm. SOURCE: https://docs.opencv.org/4.x/d1/d17/samples_2cpp_2ffilldemo_8cpp-example.html 
        floodFill(imageIn, Point(x, y), eyeDropperValue, 0, Scalar(0,0,0), Scalar(0,0,0), 4);

        imshow(DISPLAY_WINDOW_NAME, imageIn);
    }
    else if (event == EVENT_LBUTTONUP && selectedTools == PAINT_BUCKET)
    {
        is_filling_color = false;
        cout << "Paint Bucket filled !" << endl;
    }

    // RESET ********************************************************************************************************
    else if (event == EVENT_LBUTTONDBLCLK && selectedTools == RESET)
    {
        // Reset functionality: Restore original image
        imageInput = imageReset.clone();
        imshow(DISPLAY_WINDOW_NAME, imageInput);
    }

    // RIGHT CLICK ***************************************************************************************************
    else if (event == EVENT_RBUTTONDOWN)
    {
        cout << "RIGHT CLICK (" << x << ", " << y << ")" << endl;
        // Handle right-click event
        selectedTools = static_cast<Tools>((selectedTools + 1) % (RESET + 1));

        switch (selectedTools)
        {
        case EYEDROPPER:
            cout << "Tool: Eyedropper selected" << endl;
            cout << "USAGE: Left click on the image to get the color value" << endl;
            break;
        case CROP:
            cout << "Tool: Crop selected" << endl;
            cout << "USAGE: Drag the Left mouse button from a click location to another to crop" << endl;
            break;
        case PENCIL:
            cout << "Tool: Pencil selected" << endl;
            cout << "USAGE: Left click/drag on the image to draw a line" << endl;
            break;
        case PAINT_BUCKET:
            cout << "Tool: Paint Bucket selected" << endl;
            break;
        case RESET:
            cout << "Tool: Reset selected" << endl;
            cout << "USAGE: Left double click to reset the image" << endl;
            break;
        }
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
    if (argc != NUM_COMMAND_LINE_ARGUMENTS + 1)
    {
        printf("Usage: %s <image_file>\n", argv[0]);
        return 0;
    }
    else
    {
        // Mat imageInput;                             // input image
        imageInput = imread(argv[1], IMREAD_COLOR); // read the input image

        // check for file error
        if (!imageInput.data)
        {
            cout << "Error while opening file " << argv[1] << endl;
            return 0;
        }
        else
        {
            // Cloning the image to use later for reset functionality
            imageReset = imageInput.clone();

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
    return 0;
}
