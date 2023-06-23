#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

// Global variables
Mat image;
Scalar eyedropperValue(255, 255, 255);
enum Tool
{
    EYEDROPPER,
    CROP,
    PENCIL,
    PAINT_BUCKET,
    RESET
};
Tool currentTool = EYEDROPPER;
Rect cropRect;
bool isDragging = false;

// Mouse event handler
void onMouse(int event, int x, int y, int flags, void *userdata)
{
    switch (event)
    {
    case EVENT_RBUTTONDOWN:
        // Toggle between available tools
        currentTool = static_cast<Tool>((currentTool + 1) % 5);
        cout << "Current Tool: " << currentTool << endl;
        break;

    case EVENT_LBUTTONDOWN:
        if (currentTool == EYEDROPPER)
        {
            // Change eyedropper value and print BGR values
            eyedropperValue = image.at<Vec3b>(y, x);
            cout << "Eyedropper Value: B = " << eyedropperValue[0]
                << ", G = " << eyedropperValue[1]
                << ", R = " << eyedropperValue[2] << endl;
        }
        else if (currentTool == CROP)
        {
            // Start dragging for crop
            cropRect = Rect(x, y, 0, 0);
            isDragging = true;
        }
        else if (currentTool == PENCIL)
        {
            // Change target pixel color to eyedropper value
            image.at<Vec3b>(y, x) = Vec3b(eyedropperValue[0], eyedropperValue[1], eyedropperValue[2]);
            imshow("Image", image);
        }
        else if (currentTool == PAINT_BUCKET)
        {
            // Perform paint bucket fill operation
            // Implement this functionality based on the requirements
            // described in the assignment
        }
        else if (currentTool == RESET)
        {
            // Restore the original image
            image = imread(static_cast<const char *>(userdata));
            imshow("Image", image);
        }
        break;

    case EVENT_LBUTTONUP:
        if (currentTool == CROP)
        {
            // Perform crop operation
            cropRect.width = x - cropRect.x;
            cropRect.height = y - cropRect.y;
            Mat croppedImage = image(cropRect);
            image = croppedImage.clone();
            namedWindow("Image", WINDOW_NORMAL);
            setWindowProperty("Image", WND_PROP_AUTOSIZE, WINDOW_AUTOSIZE);
            imshow("Image", image);
            isDragging = false;
        }
        break;

    case EVENT_MOUSEMOVE:
        if (currentTool == CROP && isDragging)
        {
            // Update crop rectangle size during dragging
            cropRect.width = x - cropRect.x;
            cropRect.height = y - cropRect.y;
            Mat tempImage = image.clone();
            rectangle(tempImage, cropRect, Scalar(0, 255, 0), 2);
            imshow("Image", tempImage);
        }
        break;
    }
}

int main(int argc, char *argv[])
{
    // Parse command line argument for image file path
    if (argc != 2)
    {
        cout << "Usage: " << argv[0] << " <PATH TO FILE>" << endl;
        return 1;
    }

    // Load the image
    image = imread(argv[1]);
    if (image.empty())
    {
        cout << "Failed to load image" << endl;
        return 1;
    }

    // Create a named window and display the image
    namedWindow("Image", WINDOW_NORMAL);
    setWindowProperty("Image", WND_PROP_AUTOSIZE, WINDOW_AUTOSIZE);
    imshow("Image", image);

    // Set the mouse event callback
    setMouseCallback("Image", onMouse, static_cast<void *>(argv[1]));

    // Main event loop
    while (true)
    {
        int key = waitKey(10);
        if (key == 27) // ESC key
            break;
    }

    return 0;
}
