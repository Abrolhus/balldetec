#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "field_color_detector.h"
#include <iostream>

using namespace std;
using namespace cv;
struct {
    bool operator()(cv::Vec3b a, cv::Vec3b b) const { return a[2] < b[2]; }
} greaterRed;
struct {
    bool operator()(cv::Vec3b a, cv::Vec3b b) const { return a[1] < b[1]; }
} greaterGreen;
struct {
    bool operator()(cv::Vec3b a, cv::Vec3b b) const { return a[0] < b[0]; }
} greaterBlue;

cv::Vec3b bgr_median(vector<cv::Vec3b>& colors){
    cv::Vec3b sample{0,0,0};
    std::sort(colors.begin(), colors.end(), greaterBlue);
    sample[0] = colors.at(colors.size()/2+1)[0];
    std::sort(colors.begin(), colors.end(), greaterGreen);
    sample[1] = colors.at(colors.size()/2+1)[1];
    std::sort(colors.begin(), colors.end(), greaterRed);
    sample[2] = colors.at(colors.size()/2+1)[2];
    return sample;
}

int main(int argc, char **argv)
{

    // Create a VideoCapture object and open the input file
    // If the input is the web camera, pass 0 instead of the video file name
    VideoCapture cap(argv[1]);

    // Check if camera opened successfully
    if (!cap.isOpened())
    {
        cout << "Error opening video stream or file" << endl;
        return -1;
    }

    CascadeClassifier cascade;
    cascade.load("../ballgithub.xml");

    Mat frame_gray;
    // Mat frame_hsv;
    std::vector<Rect> balls;
    // Mat hsv_channels[3];
    Mat resized_frame(Size(320, 240), CV_8UC3);
    Mat frameGrande(Size(640, 480), CV_8UC3);
    Mat frame;

    while (1)
    {
        // Capture frame-by-frame
        cap >> frame;

        Mat frame1 = frame.clone();
        Mat hsv_frame_de_baixo;
        Mat frameCortado = frame1(Rect(0, 320, 240, 200)).clone();
        Mat frameDeCima = frame1(Rect(0, 0, 320, 240)).clone();
        // resize(frameCortado, resized_frame, resized_frame.size());
        resize(frameCortado, frame, resized_frame.size());
        Mat frame_cru = frame.clone();
        cvtColor(frame_cru, hsv_frame_de_baixo, COLOR_BGR2HSV);
        // imshow("frameCortado", frame);
        // cvtColor(frame, frame_hsv, CV_BGR2HSV);

        // imshow("hsv", frame_hsv);

        // split(frame_hsv, hsv_channels);
        // frame_gray = hsv_channels[2];
        // imshow("gray", frame_gray);
        cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
        imshow("gray", frame_gray);
        // inRange(frame, Scalar(38, 50, 50), Scalar(75, 255, 255), frame_gray); //problemas aqui

        // cvtColor(frame_hsv, frame, CV_HSV2BGR);
        cascade.detectMultiScale(frame_gray, balls, 1.3, 5, 8, Size(16, 16)); // Rinobot
        // cascade.detectMultiScale(frame_gray, balls, 1.1, 5, 8, Size(16, 16));

        for (int i = 0; i < frame.cols; i++)
        {
            for (size_t j = 0; j < frame.rows; j++)
            {
                auto pixelColor = frame.at<Vec3b>(j, i);
                if (Field_color_detector::isField(pixelColor))
                {
                    frame.at<Vec3b>(j, i) = Vec3b{255, 0, 255};
                }
            }
        }
        if (balls.size() == 0)
        {
            // cout << "Nao achou nada" << endl;
        }
        else
        {
            for(auto ball : balls){
                double raio = ball.width / 2.0f;
                Point pt(ball.x + raio, ball.y + raio);
                // cout << "Encontrado: " << pt << " [ " << raio << " ] " << endl;
                circle(frame, pt, raio, Scalar(0, 0, 255), CV_FILLED);
                // int r = raio;
                // int score = 0;
                // for (int y_rect = pt.y - r - 4; y_rect <= pt.y + r + 4; y_rect += (r + 4)/2)
                // {
                    // for (int x_rect = pt.x - r - 4; x_rect <= pt.x + r + 4; x_rect+= (r + 4)/2)
                    // {
                        // // cout << x_rect << ", " << y_rect  << endl;
                        // std::vector<Vec3b> cores;
                        // for (int x = x_rect - 2; x <= x_rect + 2; x++)
                        // {
                            // for (int y = y_rect - 2; y <= y_rect + 2; y++)
                            // {
                                // if(x <= 0 || y <= 0 || x >= frame.cols || y >= frame.rows){
                                    // continue;
                                // }
                                // frame.at<Vec3b>(y, x) = {255, 255, 0};
                                // cores.push_back(frame_cru.at<Vec3b>(y,x));
                            // }
                        // }
                        // auto mediana = bgr_median(cores);
                        // if(Field_color_detector::isField(mediana)){
                            // cout << "opa" << endl;
                            // for (int x = x_rect - 2; x <= x_rect + 2; x++)
                            // {
                                // for (int y = y_rect - 2; y <= y_rect + 2; y++)
                                // {
                                    // if (x <= 0 || y <= 0 || x >= frame.cols || y >= frame.rows)
                                    // {
                                        // continue;
                                    // }
                                    // frame.at<Vec3b>(y, x) = {0, 255, 255};
                                    // // cores.push_back(frame.at<Vec3b>(y,x));
                                // }
                            // }
                            // score++;
                        // }
                    // }
                // }
                // if(score >= 5){
                    // cout << "score:" << score << endl;
                    // circle(frame, pt, raio, Scalar(0, 255, 0), CV_FILLED);
                // }
                // for (int x = pt.x - sqrt(2)/2*r; x <= pt.x + sqrt(2)/2*r; x++)
                // {
                //     for (int y = pt.y - sqrt(2)/2*r; y <= pt.y + sqrt(2)/2*r; y++)
                //     {
                //         Vec3b hsv_px = hsv_frame_de_baixo.at<Vec3b>(y,x);
                //         if((hsv_px[0] >= 0 && hsv_px[0] <= 255) && 
                //            (hsv_px[1] >= 0 && hsv_px[1] <=  30)  && 
                //            (hsv_px[2] >= 0 && hsv_px[2] <= 150)){
                //             frame.at<Vec3b>(y, x) = {0, 0, 0};
                //         }
                //         else{
                //             frame.at<Vec3b>(y, x) = {255, 255, 255};
                //         }
                //     }
                // }

                imshow("opa", frame);
                imshow("frame_cru", frame_cru);
                waitKey();
            }
        }

            resize(frame, frame, frameGrande.size());
            resize(frameDeCima, frameDeCima, frameGrande.size());
            imshow("frame", frame);
            imshow("frameCima", frameDeCima);

            // Press  ESC on keyboard to exit
            char c = (char)waitKey(1);
            if (c == 27)
                break;
    }

        // When everything done, release the video capture object
        cap.release();

        // Closes all the frames
        destroyAllWindows();

        return 0;
    }
