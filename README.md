# Computer_Vision_CSE4310

---

## Project 1: Raster Graphics Editor

- created a primitive image manipulation program using OpenCV. The goal is to become familiar with basic OpenCV data structures and manipulation, such as pixel access, ROIs, copying, visualization, etc.

The program implements the following image editing tools:
**EYEDROPPER**: When active, a left mouse click changes the current color value stored in memory (the "eyedropper" value). The eyedropper value is initialized to white (255, 255, 255) when the program is loaded. The BGR values of the new color are printed to the console.

**CROP**: When active, dragging the left mouse button from a click location to a release location defines a rectangular area that is immediately cropped. The cropped area replaces whatever is currently displayed in the image window, and the window automatically resizes to fit the cropped area.

**PENCIL**: When active, any left mouse clicks or left mouse button drags change the color of the target pixel to the current eyedropper value.

**PAINT BUCKET**: When active, a left mouse click at a pixel location with color X changes it to the eyedropper value. Any 4-connected neighbors (top, bottom, left, right, but NOT diagonals) with color value X are also changed to the eyedropper value. This process repeats until no new suitable connected pixels can be added to the fill area.

**RESET**: When active, a left mouse double click in the image window replaces the window contents with the original, unedited image as it was when the program was initially loaded.

Please note that the program console displays the currently activated tool, but there are no changes to the appearance of the cursor or any buttons/GUI controls.

---

