#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <string>
#include <vector>

using namespace cv;
using namespace std;

Scalar yellow_right = Scalar(36, 255, 255);
Scalar yellow_left = Scalar(23, 253, 248);
Scalar blue_right = Scalar(110, 255, 242);
Scalar blue_left = Scalar(90, 253, 226);
Scalar green_right = Scalar(72, 208, 180);
Scalar green_left = Scalar(67, 201, 173);
Scalar red_right = Scalar(180, 227, 238);
Scalar red_left = Scalar(177, 224, 235);

int main()
{
	string coordinates;
	vector<vector<Point>>contours;
	// Read the image file
	Mat picture = imread("C:/Program Files (x86)/picture.jpg");
	Mat background;
	picture.copyTo(background);

	cvtColor(picture, picture, COLOR_BGR2HSV);

	Mat mask;
	inRange(picture, red_left, red_right, mask);
	findContours(mask, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	drawContours(background, contours, -1, (0, 255, 0), 2);
	RotatedRect rot_rect1 = minAreaRect(contours[0]);
	Point center1 = rot_rect1.center;

		circle(background, center1, 1, (0, 255, 0), 2);
		coordinates = to_string(center1.x) + "," + to_string(center1.y);
		putText(background, coordinates, Point(center1.x + 5, center1.y - 10), FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0));

	inRange(picture, green_left, green_right, mask);
	findContours(mask, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	drawContours(background, contours, -1, (0, 255, 0), 2);
	RotatedRect rot_rect2 = minAreaRect(contours[0]);
	Point center2 = rot_rect2.center;

		circle(background, center2, 1, (0, 255, 0), 2);
		coordinates = to_string(center2.x) + "," + to_string(center2.y);
		putText(background, coordinates, Point(center2.x + 5, center2.y - 10), FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0));

	inRange(picture, blue_left, blue_right, mask);
	findContours(mask, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	drawContours(background, contours, -1, (0, 255, 0), 2);
	RotatedRect rot_rect3 = minAreaRect(contours[0]);
	Point center3 = rot_rect3.center;

		circle(background, center3, 1, (0, 255, 0), 2);
		coordinates = to_string(center3.x) + "," + to_string(center3.y);
		putText(background, coordinates, Point(center3.x + 5, center3.y - 10), FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0));

	inRange(picture, yellow_left, yellow_right, mask);
	findContours(mask, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	drawContours(background, contours, -1, (0, 255, 0), 1);
	RotatedRect rot_rect4 = minAreaRect(contours[0]);
	Point center4 = rot_rect4.center;

		circle(background, center4, 1, (0, 255, 0), 2);
		coordinates = to_string(center4.x) + "," + to_string(center4.y);
		putText(background, coordinates, Point(center4.x + 5, center4.y - 10), FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0));

	imshow("background", background);
	waitKey(0);
}