# BMP_ReaderWriter

Welcome to my own Bitmap reading writing library! Developed by Onkar Parag Kulkarni

---

## Instructions to use

Step 1) Copy the file inside include folder and put it in your own project's include folder where you put all the dependencies

Step 2) Open the lib folder and copy the Debug folder's files and paste it into your own project's library dependencies debug folder and same for the release as well.

Step 3) (Optional) You can generate the lib files by building the solution in required configuration.

---

## Functions in static library

sColor GetColor(int x, int y) const;	 --> Returns color at (x, y) location of image.bmp
	
void SetColor(const sColor& color, int x, int y);	--> Sets RGBA color at location (x, y) of image.bmp

void ReadBMP(const char* path)		--> Reads the .bmp file and stores it in cBMPImage's object and you can export it later

void ExportBMP(const char* path) const;		--> Exports/Saves the bitmap image with path as name in solution directory.

---

## Thank You For Using My Library!