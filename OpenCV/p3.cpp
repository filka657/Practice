#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <vector>
#include <map>
using namespace cv;
using namespace std;

Scalar red_right = Scalar(179, 158, 208);
Scalar red_left = Scalar(153, 111, 134);

Scalar yellow_right = Scalar(32, 173, 255);
Scalar yellow_left = Scalar(0, 101, 213);

Scalar blue_right = Scalar(179, 255, 255);
Scalar blue_left = Scalar(95, 161, 165);

Scalar green_right = Scalar(82, 146, 255);
Scalar green_left = Scalar(61, 54, 132);

Scalar purple_right = Scalar(175, 83, 186);
Scalar purple_left = Scalar(125, 59, 151);

class Markers
{
public:
	string color_str;
	Scalar left, right;
	vector<vector<Point>>contours;
	Point center;
	Mat mask;
	string coordinates;
	double m01;
	double m10;
	double area;
	float min_d;

	Markers(string color_str, Scalar left, Scalar right);
	void detect(Mat picture, Scalar left, Scalar right);
	void find();
	void draw(Mat background);
	void momentss();
	void point(Mat background);
	void draw_line(Mat background, Point last);
	void text(Mat background);
	void show(Mat background);
	void text_distance(Mat background, float d);
};

Markers::Markers(string color_str, Scalar left, Scalar right) {
	this->color_str = color_str;
	this->left = left;
	this->right = right;
	this->min_d = 1000;
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
	vector<vector<Point> >::iterator it = contours.begin();
	while (it != contours.end())
	{
		if (it->size() < 30)
		{
			it = contours.erase(it);
		}
		else it++;
	}
	drawContours(background, contours, -1, (0, 255, 0), 2);
}
void Markers::momentss()
{
	Moments oMoments = moments(mask);
	m01 = oMoments.m01;
	m10 = oMoments.m10;
	area = oMoments.m00;
	center.x = m10 / area;
	center.y = m01 / area;
}

void Markers::point(Mat background)
{
	circle(background, center, 1, (0, 255, 0), 2);
}

void Markers::draw_line(Mat background, Point last)
{
	line(background, center, last, Scalar(0, 0, 255), 2);
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

void Markers::text_distance(Mat background, float d)
{
	putText(background, "d: " + to_string(d), Point(center.x + 5, center.y + 10), FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0));
}

int main()
{
	VideoCapture cap(0);

	Markers R("red", red_left, red_right);
	Markers Y("yellow", yellow_left, yellow_right);
	Markers G("green", green_left, green_right);
	Markers B("blue", blue_left, blue_right);
	Markers P("purple", purple_left, purple_right);
	Markers mark[] = { R, Y, G, B, P };
	
	if (!cap.isOpened()) cout << "Can not open the camera" << endl;

	Point last;
	last.x = -1;
	last.y = -1;

	//Mat robo_line;
	//cap.read(robo_line);
	//Mat imgLines = Mat::zeros(robo_line.size(), CV_8UC3);;
	float min_distance(1500.0);
	while (1)
	{
		Mat video;
		cap >> video;
		imshow("Video", video);

			Mat background;
			video.copyTo(background);
			cvtColor(video, video, COLOR_BGR2HSV);
			float distance;
			for (int i = 0; i < 5; i++)
			{
				mark[i].detect(video, mark[i].left, mark[i].right);
				mark[i].find();
				mark[i].draw(background);
				mark[i].momentss();
				mark[i].point(background);
				/*if (i == 0)
				{
					if (last.x >= 0 && last.y >= 0 && mark[i].center.x >= 0 && mark[i].center.y >= 0)
					{
						mark[i].draw_line(background, last);
					}
					last.x = mark[i].center.x;
					last.y = mark[i].center.y;
				}*/
				mark[i].text(background);
				mark[i].show(background);
				if (i != 0)
				{
					distance = sqrt(pow(R.center.x - mark[i].center.x, 2) + (pow(R.center.y - mark[i].center.y, 2)));
					if (distance < mark[i].min_d)
						mark[i].min_d = distance;
					if (distance < 1000.0) {
						cout << "check the " << mark[i].color_str << " marker" << endl;
					}
					mark[i].text_distance(background, distance);
					if (distance < min_distance)
						min_distance = distance;
				}
			}
			char c = waitKey(33);
			if (c == 27)
			{
				destroyWindow("Video");
				break;
			}		
	}
	for (int i = 1; i < 5; i++)
	{
		cout << "minimal distance by " << mark[i].color_str << ": " << mark[i].min_d << endl;
	}
	waitKey(0);
}