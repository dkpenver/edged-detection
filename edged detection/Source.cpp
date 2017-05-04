#include <opencv2\opencv.hpp>
#include <cmath>

using namespace cv;
using namespace std;

void removeGrey(Mat *sup, int x, int y);
float pi = 3.1415926535897;
int wl = 70;
int bl = 20;

int main(int arg) {

	Mat imgIn, imgOut, pic, yCor, xCor, mag, angle, sup, fin;
	imgIn = imread("Image1.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	if (imgIn.empty()) return -1;
	imgOut = Mat(imgIn.rows, imgIn.cols, CV_32F);
	xCor = Mat(imgIn.rows, imgIn.cols, CV_32F);
	yCor = Mat(imgIn.rows, imgIn.cols, CV_32F);
	mag = Mat(imgIn.rows, imgIn.cols, CV_32F);
	angle = Mat(imgIn.rows, imgIn.cols, CV_32F);
	sup = Mat(imgIn.rows, imgIn.cols, CV_32F);
	fin = Mat(imgIn.rows, imgIn.cols, CV_32F);
	GaussianBlur(imgIn, imgOut, Size(5, 5), 0, 0);
	imgOut.convertTo(imgOut, CV_32F);
	
	Sobel(imgOut, xCor, CV_32F, 1, 0, 3);
	Sobel(imgOut, yCor, CV_32F, 0, 1, 3);
	
	int cols = imgIn.cols;
	int rows = imgIn.rows;
	int x = 0;
	int y = 0;
	
	while (x < cols) {
		while (y < rows) {
			float gxCor = xCor.at<float>(y,x);
			float gyCor = yCor.at<float>(y,x);
			mag.at<float>(y,x) = sqrt(pow(gxCor, 2) + pow(gyCor, 2));
			angle.at<float>(y,x)= atan2(gyCor, gxCor) * 360 / (2 * pi) + 180;
			y++;
		}
		x++;
		y = 0;
	}
	x = 1;
	y = 1;
	while (x < cols - 1) {
		while (y < rows - 1) {
			float m = mag.at<float>(y, x);
			float a = angle.at<float>(y, x);
			
			if (((m <= 22.5) || (m > 337.5)) || ((m > 157.5) && (m <= 202.5))) {
				if ((m > mag.at<float>(y, x - 1)) && (m > mag.at<float>(y, x + 1))) {
					sup.at<float>(y, x) = m;
				}
				else{
					sup.at<float>(y, x) = 0;
				}
			}
			else if (((m <= 67.5) && (m > 22.5)) || ((m > 202.5) && (m <= 247.5))) {
				if ((m > mag.at<float>(y-1, x+1)) && (m > mag.at<float>(y+1, x-1))) {
					sup.at<float>(y, x) = m;
				}
				else {
					sup.at<float>(y, x) = 0;
				}
			}
			else if (((m <= 112.5) && (m > 67.5)) || ((m > 247.5) && (m <= 292.5))) {
				if ((m > mag.at<float>(y-1, x)) && (m > mag.at<float>(y+1, x))) {
					sup.at<float>(y, x) = m;
				}
				else {
					sup.at<float>(y, x) = 0;
				}
			}
			else if (((m <= 157.5) && (m > 112.5)) || ((m > 292.5) && (m <= 337.5))) {
				if ((m > mag.at<float>(y - 1, x - 1)) && (m > mag.at<float>(y + 1, x + 1))) {
					sup.at<float>(y, x) = m;
				}
				else {
					sup.at<float>(y, x) = 0;
				}
			}
			y++;
		}
		x++;
		y = 1;
	}
	x = 1;
	while (x < cols - 1) {
		
		while (y < rows - 1) {
			float lux = sup.at<float>(y, x);
			if (lux >= wl) {
				removeGrey(&sup, y, x);
			}
			y++;
		}
		x++;
		y = 1;
	}
	x = 1;
	while (x < cols - 1) {

		while (y < rows - 1) {
			float lux = sup.at<float>(y, x);
			if (lux == 0xffff) {
				fin.at<float>(y, x) = lux;
			}
			y++;
		}
		x++;
		y = 1;
	}
	imwrite("imageInput.jpg", imgIn);
	imwrite("mag.jpg", mag);
	imwrite("xCor.jpg", xCor);
	imwrite("YCorr.jpg", yCor);
	imwrite("angle.jpg", angle);
	imwrite("SupressedImage.jpg", sup);
	imwrite("FinalOutput.jpg", fin);
	return 0;
}

void removeGrey(Mat *sup, int x, int y) {
	sup[0].at<float>(x, y) = 0xffff;
	if ((sup[0].at<float>(x - 1, y - 1) > bl) && (sup[0].at<float>(x - 1, y - 1) < wl)) {
		removeGrey(sup, x - 1, y - 1);
	}
	else if ((sup[0].at<float>(x, y - 1) > bl) && (sup[0].at<float>(x, y - 1) < wl)) {
		removeGrey(sup, x, y - 1);
	}
	else if ((sup[0].at<float>(x + 1, y - 1) > bl) && (sup[0].at<float>(x + 1, y - 1) < wl)) {
		removeGrey(sup, x + 1, y - 1);
	}
	else if ((sup[0].at<float>(x + 1, y) > bl) && (sup[0].at<float>(x + 1, y) < wl)) {
		removeGrey(sup, x + 1, y);
	}
	else if ((sup[0].at<float>(x + 1, y + 1) > bl) && (sup[0].at<float>(x + 1, y + 1) < wl)) {
		removeGrey(sup, x + 1, y + 1);
	}
	else if ((sup[0].at<float>(x, y + 1) > bl) && (sup[0].at<float>(x, y + 1) < wl)) {
		removeGrey(sup, x, y + 1);
	}
	else if ((sup[0].at<float>(x - 1, y + 1) > bl) && (sup[0].at<float>(x - 1, y + 1) < wl)) {
		removeGrey(sup, x - 1, y + 1);
	}
	else if ((sup[0].at<float>(x - 1, y) > bl) && (sup[0].at<float>(x - 1, y) < wl)) {
		removeGrey(sup, x - 1, y);
	}
}