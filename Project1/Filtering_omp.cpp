/**
 From https://docs.opencv.org/3.4.0/d3/dc1/tutorial_basic_linear_transform.html
 Check the webpage for description
 */
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
#include <omp.h>
using namespace std;
using namespace cv;
int main( int argc, char** argv )
{
    // double alpha = 1.0; /*< Simple contrast control */
    // int beta = 0;       /*< Simple brightness control */
    String imageName("data/lena.jpg"); // by default
    if (argc > 1)
    {
        imageName = argv[1];
    }
    Mat image = imread( imageName );
    Mat new_image = Mat::zeros( image.size(), image.type() );

    new_image = image.clone();

    int filter[3][3] = { {1, 1, 1},
                       {1, 1, 1},
                       {1, 1, 1} };

    int divisor = 9;


    // cout << " Basic Linear Transforms " << endl;
    // cout << "-------------------------" << endl;
    cout << "Image Filtering (Convolution)" << endl;

    // filtering dont need it
    //
    // cout << "* Enter the alpha value [1.0-3.0]: "; cin >> alpha;
    // cout << "* Enter the beta value [0-100]: ";    cin >> beta;
    // for( int y = 0; y < image.rows; y++ ) {
    //     for( int x = 0; x < image.cols; x++ ) {
    //         for( int c = 0; c < 3; c++ ) {
    //             new_image.at<Vec3b>(y,x)[c] =
    //               saturate_cast<uchar>( alpha*( image.at<Vec3b>(y,x)[c] ) + beta );
    //         }
    //     }
    // }

    #pragma omp parallel for
    for(int y = 1; y < image.rows - 1; y++) {
        for(int x = 1; x < image.cols - 1; x++) {

            Vec3i sum = Vec3i(0,0,0);

            for(int a = -1; a <= 1; a++) {
                for(int b = -1; b <= 1; b++) {

                    Vec3b pixel = image.at<Vec3b>(y + a, x + b);

                    for(int c = 0; c < 3; c++) {
                        sum[c] += pixel[c] * filter[a+1][b+1];
                    }
                }
            }

            for(int c = 0; c < 3; c++) {
                new_image.at<Vec3b>(y,x)[c] =
                saturate_cast<uchar>(sum[c] / divisor);
            }
        }
    }




    namedWindow("Original Image", WINDOW_AUTOSIZE);
    namedWindow("New Image", WINDOW_AUTOSIZE);
    imshow("Original Image", image);
    imshow("New Image", new_image);
    waitKey();
    return 0;
}
