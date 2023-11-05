#include <iostream>
#include <opencv2/opencv.hpp>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <vector>

int main() {

    //понять как сюда пачку изображений-масок грузить
    cv::Mat img;
    img = cv::imread("C:/Users/romad/source/repos/PoroMarker/on.tif.thumb.jpg", cv::IMREAD_GRAYSCALE);
    cv::Mat connected(img.size(), CV_32S);
    //Маска из 0 и 1 по изобрадению
    int limit = 100;
    cv::Mat mask(img.size(), CV_8UC1);
    for (int r = 0; r < img.rows; r += 1) {
        for (int c = 0; c < img.cols; c += 1) {
            uchar pixel = img.at<uchar>(r, c);
            if (limit < 128) {
                if (pixel < limit) {
                    pixel = 1;
                }
                else {
                    pixel = 0;
                }

            }
            mask.at<uchar>(r, c) = pixel;
        }
    }
    //Ищем количество связных компонент
    int amount_of_components = cv::connectedComponents(mask, connected, 8);
    std::cout << "\n" << amount_of_components << "\n";
    cv::Mat res_img(img.size(), CV_8UC1);
    std::vector<uchar> colors(amount_of_components);
    //определяем цвет связных компонент
    for (int i = 0; i < amount_of_components; i += 1) {
        colors[i] = uchar(std::rand() & 255);
    }
    colors[0] = uchar(0);//фон
    //меняем цвет у связных компонент
    for (int r = 0; r < res_img.rows; r += 1) {
        for (int c = 0; c < res_img.cols; c += 1) {
            int label = connected.at<int>(r, c);
            uchar& pixel = res_img.at<uchar>(r, c);
            pixel = colors[label];
        }
    }
    //а как по полученному строить график
    cv::imshow("Connected Components", res_img);
    cv::waitKey(0);
}