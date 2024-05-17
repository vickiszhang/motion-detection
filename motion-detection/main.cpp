#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <iostream>

using namespace std;
using namespace cv;

/*
* void main() {
	string path = "me.jpg";
	Mat img = imread(path);
	imshow("Frame", img);
	waitKey(0);
}
*/

vector<vector<int>> myColors {
	 {0, 135, 123, 16, 254, 255}, //orange
	{ 91,118,131,112,188,227 }, //blue
	{ 164,125,158,179,255,255 }, //pink
	{ 23,55,111,100,214,251 }, //green
	
};

vector<Scalar> myColorsBGR {
	{0, 165, 255},
	{ 255, 0, 0 },
	{ 203, 192, 255 },
	{ 0, 255, 0 },
};

void findColor(Mat img) {
	Mat imgHSV;
	cvtColor(img, imgHSV, COLOR_BGR2HSV);

	for (int i = 0; i < myColors.size(); i++) {
		Scalar lower(myColors[i][0], myColors[i][1], myColors[i][2]);
		Scalar upper(myColors[i][3], myColors[i][4], myColors[i][5]);
		Mat mask;
		inRange(imgHSV, lower, upper, mask);
		imshow(to_string(i), mask);
	}

}

void main() {
	VideoCapture video(0); // replace 0 with video path
	CascadeClassifier facedetect;
	Mat img, flipped_img;
	facedetect.load("haarcascade_frontalface_default.xml");


	while (true) {
		video.read(img);
		flip(img, flipped_img, 1);


		vector<Rect> faces;

		facedetect.detectMultiScale(flipped_img, faces, 1.3, 5);

		cout << faces.size() << endl;

		for (int i = 0; i < faces.size(); i++) {
			rectangle(flipped_img, faces[i].tl(), faces[i].br(), Scalar(50, 50, 255), 3);
			rectangle(flipped_img, Point(0, 0), Point(250, 70), Scalar(50, 50, 255), FILLED);
			putText(flipped_img, to_string(faces.size()) + " Faces Found", Point(10, 40), FONT_HERSHEY_DUPLEX, 1, Scalar(255, 255, 255));
		}

		findColor(flipped_img);

		imshow("Frame", flipped_img);
		waitKey(1);
	}
}
