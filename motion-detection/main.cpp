#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <iostream>

using namespace std;
using namespace cv;


Mat img, flipped_img;

vector<vector<int>> newPoints;

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

Point getContours(Mat image) {


	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;

	Point myPoint(0, 0);

	findContours(image, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	vector<vector<Point>> conPoly(contours.size());
	vector<Rect> boundRect(contours.size());

	for (int i = 0; i < contours.size(); i++)
	{
		int area = contourArea(contours[i]);
		cout << area << endl;


		string objectType;

		if (area > 1000)
		{
			float peri = arcLength(contours[i], true);
			approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);

			cout << conPoly[i].size() << endl;
			boundRect[i] = boundingRect(conPoly[i]);

			myPoint.x = boundRect[i].x + boundRect[i].width / 2;
			myPoint.y = boundRect[i].y;

			/*drawContours(flipped_img, conPoly, i, Scalar(255, 0, 255), 2);
			rectangle(flipped_img, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 255, 0), 5);*/
		}
	}
	return myPoint;
}


vector<vector<int>> findColor(Mat img) {
	Mat imgHSV;
	cvtColor(img, imgHSV, COLOR_BGR2HSV);

	for (int i = 0; i < myColors.size(); i++) {
		Scalar lower(myColors[i][0], myColors[i][1], myColors[i][2]);
		Scalar upper(myColors[i][3], myColors[i][4], myColors[i][5]);
		Mat mask;
		inRange(imgHSV, lower, upper, mask);
		//imshow(to_string(i), mask);
		Point currPoint = getContours(mask);

		if (currPoint.x != 0 && currPoint.y != 0) {
			newPoints.push_back({ currPoint.x, currPoint.y, i });

		}
	}
	return newPoints;

}

void drawPoints(vector<vector<int>> newPoints) {
	for (int i = 0; i < newPoints.size(); i++) {
		circle(flipped_img, Point(newPoints[i][0],newPoints[i][1]), 10, myColorsBGR[newPoints[i][2]], FILLED);
	}
}

void main() {
	VideoCapture video(0);
	CascadeClassifier facedetect;
	facedetect.load("haarcascade_frontalface_default.xml");


	while (true) {
		video.read(img);
		flip(img, flipped_img, 1);

		// face detection

		//vector<Rect> faces;

		//facedetect.detectMultiScale(flipped_img, faces, 1.3, 5);

		//cout << faces.size() << endl;

		//for (int i = 0; i < faces.size(); i++) {
		//	rectangle(flipped_img, faces[i].tl(), faces[i].br(), Scalar(50, 50, 255), 3);
		//	rectangle(flipped_img, Point(0, 0), Point(250, 70), Scalar(50, 50, 255), FILLED);
		//	putText(flipped_img, to_string(faces.size()) + " Faces Found", Point(10, 40), FONT_HERSHEY_DUPLEX, 1, Scalar(255, 255, 255));
		//}

		newPoints = findColor(flipped_img);
		drawPoints(newPoints);

		imshow("Frame", flipped_img);
		waitKey(1);
	}
}
