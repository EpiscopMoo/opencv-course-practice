#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace cv;
using namespace std;
#define MAX_GRID 25

int getGridSize (float pixelBrightness)
{
    // По-хорошему нужна некая функциональная зависимость размаха окошка от близости к границе
    if (pixelBrightness < 0.05) return 1;
    else if (pixelBrightness < 0.1) return (MAX_GRID/2 % 2 ? MAX_GRID/2 : MAX_GRID/2+1);
    else return MAX_GRID;
}

int main(int argc, char *argv[])
{
    Mat img = imread("canvas.png", CV_LOAD_IMAGE_COLOR);
    Mat* channels = new Mat[3];
    Mat* distances = new Mat[3];
    Mat* blures = new Mat[3];
    Mat* safecopies = new Mat[3];
    Mat* integrals = new Mat[3];
    if(img.empty())
       return -1;


    split(img, channels);
    for (int i=0; i<3; i++)
    {
        safecopies[i] = channels[i].clone();
        blures[i] = channels[i].clone();
        integral(channels[i], integrals[i], CV_32S);

        Canny(channels[i], channels[i], 180, 230, 3);
        threshold(channels[i], channels[i], 0, 255, CV_THRESH_BINARY_INV);
        distanceTransform(channels[i], distances[i], CV_DIST_L2, 3);
        normalize(distances[i], distances[i], 0, 1.0, NORM_MINMAX);

        int w = channels[i].size().width;
        int h = channels[i].size().height;
        for (int m=0; m < w; m++)
        {
            for (int n=0; n < h; n++)
            {
                int currentGridSize = getGridSize(distances[i].at<float>(Point(m, n)));
                if (currentGridSize > 1)
                {
                    int regionSum;
                    int step = currentGridSize/2;
                    int cornerX = n - step;
                    int cornerY = m - step;
                    if (cornerX < 0) cornerX = 0;
                    if (cornerX > h - currentGridSize + 1) cornerX = h - currentGridSize;
                    if (cornerY < 0) cornerY = 0;
                    if (cornerY > w - currentGridSize + 1) cornerY = w - currentGridSize;
                    currentGridSize--;

                    int tl= integrals[i].at<int>((cornerX),(cornerY));
                    int tr= integrals[i].at<int>((cornerX+currentGridSize),(cornerY));
                    int bl= integrals[i].at<int>((cornerX),(cornerY+currentGridSize));
                    int br= integrals[i].at<int>((cornerX+currentGridSize),(cornerY+currentGridSize));

                    regionSum = (tl+br-bl-tr)/(currentGridSize*currentGridSize);
                    blures[i].at<uchar>(Point(m,n)) = (uchar)regionSum;
                }
            }
        }
    }
    merge(blures, 3, img);

    namedWindow( "New", CV_WINDOW_AUTOSIZE );
    imshow("New", img);
    delete [] channels;
    delete [] distances;
    delete [] blures;
    delete [] safecopies;
    delete [] integrals;
    waitKey(0);
    return 0;
}


