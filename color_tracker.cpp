#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main() {
    VideoCapture cap(0);
    if (!cap.isOpened()) {
        cerr << "카메라를 열 수 없습니다." << endl;
        return -1;
    }

    namedWindow("Color Based Object Tracking", WINDOW_AUTOSIZE);

    while (true) {
        Mat frame, hsv, mask;
        cap >> frame;
        if (frame.empty())
            break;

        cvtColor(frame, hsv, COLOR_BGR2HSV);

        // 관심 색상 범위 정의 
        Scalar lower_yellow(20, 100, 100);
        Scalar upper_yellow(30, 255, 255);

        // 색상 범위에 따른 마스크 생성
        inRange(hsv, lower_yellow, upper_yellow, mask);

        // 결과를 처리
        vector<vector<Point>> contours;
        findContours(mask, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

        for (const auto& contour : contours) {
            Rect rect = boundingRect(contour);
            if (rect.width > 30 && rect.height > 30) {
                rectangle(frame, rect, Scalar(0, 255, 0), 2);
                // 물체의 중심 좌표 출력
                Point center(rect.x + rect.width / 2, rect.y + rect.height / 2);
                cout << "Detected object at: " << center << endl;
            }
        }

        imshow("Color Based Object Tracking", frame);

        if (waitKey(25) >= 0)
            break;
    }

    cap.release();
    destroyAllWindows();

    return 0;
}
