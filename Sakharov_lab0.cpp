#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace cv;

int main(int argc, char *argv[])
{
    Mat img = imread("lena.jpg", CV_LOAD_IMAGE_COLOR);
    Mat greyImg, cannyImg, bluredImg, distanceImg;
    if(img.empty())
       return -1;

    cvtColor(img, greyImg, CV_BGR2GRAY);
    threshold(greyImg, greyImg, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
    distanceTransform(greyImg, distanceImg, CV_DIST_L2, 3);
    normalize(distanceImg, distanceImg, 0, 1.0, NORM_MINMAX);

    Canny(greyImg, cannyImg, 10, 100, 3);
    blur(img, bluredImg, Size(7,7), Point(-1,-1));

    namedWindow( "Original", CV_WINDOW_AUTOSIZE );
    imshow("Original", img);
    namedWindow( "Blur", CV_WINDOW_AUTOSIZE );
    imshow("Blur", bluredImg);
    namedWindow( "Canny", CV_WINDOW_AUTOSIZE );
    imshow("Canny", cannyImg);
    namedWindow( "Distance", CV_WINDOW_AUTOSIZE );
    imshow("Distance", distanceImg);
    waitKey(0);
    return 0;
}


