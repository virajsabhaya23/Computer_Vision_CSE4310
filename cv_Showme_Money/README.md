# Computer Vision - Show me the Money

### Introduction

- This program is a computer vision application that identifies and counts United States coins (penny, nickel, dime, and quarter) in an input image. The goal is to determine the total monetary value of the coins in the image.

### Overview of the functions used

- **Canny:** This function is used to detect edges in an image using the Canny edge detection algorithm. It takes the grayscale image, threshold values, and aperture size as parameters and returns an image with detected edges.
  
- **dilate and erode: **These functions are used for morphological operations to remove noise from the edges. dilate is used to dilate the edges, making them thicker, and erode is used to erode the dilated edges, reducing their thickness.

- **findContours: **This function is used to find contours in an image. It takes the edge image, a vector to store the contours, a retrieval mode (e.g., RETR_EXTERNAL to retrieve only the external contours), a contour approximation mode, and an optional offset parameter.

- **drawContours: **This function is used to draw contours on an image. It takes the image, the vector of contours, the contour index (-1 for all contours), the color, and an optional thickness parameter.

- **minAreaRect:** This function is used to fit a rotated rectangle to a contour. It takes a contour and returns a RotatedRect object representing the minimum area rectangle that encloses the contour.

- **ellipse:** This function is used to draw an ellipse on an image. It takes the image, a RotatedRect object representing the ellipse parameters, the color, and an optional thickness parameter.

#### The program should be able to be compiled and executed by running the following set of commands in the program directory:

```bash
mkdir build && cd build
cmake ..
make
./cv_Showme_Money ../coins1.jpeg
```
or
```bash
cmake .
make
./cv_Showme_Money coins1.jpeg
```

### Output

![Output of the program](<Screenshot 2023-07-09 at 20.30.32.png>)