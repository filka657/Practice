#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <ostream>
#include <algorithm>
#include <string>
#include <vector>
#include <map>
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

class Markers
{
public:
	string color_str;
	Scalar left, right;
	//Point coordinates;
	Markers();
	Markers(string color_str, Scalar left, Scalar right);
	Mat detect(Mat picture, Scalar left, Scalar right);
	void find(Mat mask);
	void draw(Mat background, vector<vector<Point>>contours);
	Point min(vector<vector<Point>>contours);
	void point(Mat background, Point center);
	void text(Point center, Mat background);
	Mat show(Mat background);
};

Markers::Markers(string color_str, Scalar left, Scalar right)
{
	this->color_str = color_str;
	this->left = left;
	this->right = right;
}

Mat Markers::detect(Mat picture, Scalar left, Scalar right)
{
	Mat mask;
	inRange(picture, left, right, mask);
	return mask;
}

void Markers::find(Mat mask)
{
	vector<vector<Point>>contours;
	return findContours(mask, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
}

void Markers::draw(Mat background, vector<vector<Point>>contours)
{
	return drawContours(background, contours, -1, (0, 255, 0), 2);
}

Point Markers::min(vector<vector<Point>>contours)
{
	RotatedRect rot_rect1 = minAreaRect(contours[0]);
	Point center = rot_rect1.center;
}

void Markers::point(Mat background, Point center)
{
	return circle(background, center, 1, (0, 255, 0), 2);
}

void Markers::text(Point center, Mat background)
{
	string coordinates;
	coordinates = to_string(center.x) + "," + to_string(center.y);
	return putText(background, coordinates, Point(center.x + 5, center.y - 10), FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0));
}

Mat Markers::show(Mat background)
{
	imshow("background", background);
	return background;
}

int main()
{
	map<int, Markers> dict;
	map<int, Markers>::iterator it = dict.begin();
	dict.insert(make_pair(1, Markers("red", red_left, red_right)));
	dict.insert(make_pair(2, Markers("yellow", yellow_left, yellow_right)));
	dict.insert(make_pair(3, Markers("green", green_left, green_right)));
	dict.insert(make_pair(4, Markers("blue", blue_left, blue_right)));
	int flower[4] = { 1,2,3,4 };
	string coordinates;
	vector<vector<Point>>contours;
	// Read the image file
	Mat picture = imread("C:/Program Files (x86)/picture.jpg");
	Mat background;
	picture.copyTo(background);
	cvtColor(picture, picture, COLOR_BGR2HSV);
	Mat mask;
	Markers M;
	Point center;
	for (it; it != dict.end(); it++)
	{
		M.detect(picture, it->second.left, it->second.right);
		M.find(mask);
		M.draw(background, contours);
		M.min(contours);
		M.point(background, center);
		M.text(center, background);
		M.show(background);
	}
}