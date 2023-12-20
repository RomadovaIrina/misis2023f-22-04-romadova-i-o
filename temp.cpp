#include <iostream>
#include <porocheck/porocheck.hpp>

int main() {
    //инициализация данных
    int n = 4;
    std::vector<cv::Mat> pictures;
    {
        pictures.push_back(cv::imread("C:/Users/romad/source/repos/PoroMarker/ex_1.png", cv::IMREAD_GRAYSCALE));
        pictures.push_back(cv::imread("C:/Users/romad/source/repos/PoroMarker/ex_2.png", cv::IMREAD_GRAYSCALE));
        pictures.push_back(cv::imread("C:/Users/romad/source/repos/PoroMarker/ex_3.png", cv::IMREAD_GRAYSCALE));
        pictures.push_back(cv::imread("C:/Users/romad/source/repos/PoroMarker/ex_4.png", cv::IMREAD_GRAYSCALE));

    }
    PoroCheck(pictures);
}