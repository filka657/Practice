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
	vector<vector<Point>>contours;
	RotatedRect rot_rect;
	Point center;
	Mat mask;
	string coordinates;
	
	Markers(string color_str, Scalar left, Scalar right);
	void detect(Mat picture, Scalar left, Scalar right);
	void find();
	void draw(Mat background);
	void min();
	void point(Mat background);
	void text(Mat background);
	void show(Mat background);
};

Markers::Markers(string color_str, Scalar left, Scalar right)
{
	this->color_str = color_str;
	this->left = left;
	this->right = right;
}

void Markers::detect(Mat picture, Scalar left, Scalar right)
{
	inRange(picture, left, right, mask);
}

void Markers::find()
{
	findContours(mask, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
}

void Markers::draw(Mat background)
{
	drawContours(background, contours, -1, (0, 255, 0), 2);
}

void Markers::min()
{
	rot_rect = minAreaRect(contours[0]);
	center = rot_rect.center;
}

void Markers::point(Mat background)
{
	circle(background, center, 1, (0, 255, 0), 2);
}

void Markers::text(Mat background)
{
	coordinates = to_string(center.x) + "," + to_string(center.y);
	putText(background, coordinates, Point(center.x + 5, center.y - 10), FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0));
}

void Markers::show(Mat background)
{
	return imshow("background", background);
}

int main()
{
	map<int, Markers> dict;
	map<int, Markers>::iterator it = dict.begin();

	Markers R("red", red_left, red_right);
	Markers Y("yellow", yellow_left, yellow_right);
	Markers G("green", green_left, green_right);
	Markers B("blue", blue_left, blue_right);
	dict.insert(make_pair(1, R));
	dict.insert(make_pair(2, Y));
	dict.insert(make_pair(3, G));
	dict.insert(make_pair(4, B));
	Mat picture = imread("C:/Program Files (x86)/picture.jpg");
	Mat background;
	picture.copyTo(background);
	cvtColor(picture, picture, COLOR_BGR2HSV);
	//for (it; it != dict.end(); it++)
	//{
		(*it).second.detect(picture, (*it).second.left, (*it).second.right);
		(*it).second.find();
		(*it).second.draw(background);
		(*it).second.min();
		(*it).second.point(background);
		(*it).second.text(background);
		(*it).second.show(background);

		/*R.detect(picture, red_left, red_right);
		R.find();
		R.draw(background);
		R.min();
		R.point(background);
		R.text(background);
		R.show(background);*/
	//}
	waitKey(0);
}