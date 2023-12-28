#include <iostream>
#include <porocheck/porocheck.hpp>
#include "nlohmann/json.hpp"
#include <fstream>


int main(int argc, char* argv[]) {
        std::ifstream file(argv[1]);
        nlohmann::json j;
        file >> j;
        std::vector<cv::Mat> pictures;
        std::string filename_1(j["filename_1"]);
        std::string filename_2(j["filename_2"]);
        std::string filename_3(j["filename_3"]);
        std::string filename_4(j["filename_4"]);
        std::string filename_5(j["filename_5"]);

        std::string filename(j["filename"]);

        pictures.push_back(cv::Mat(cv::imread(filename_1, cv::IMREAD_GRAYSCALE)));
        pictures.push_back(cv::Mat(cv::imread(filename_2, cv::IMREAD_GRAYSCALE)));
        pictures.push_back(cv::Mat(cv::imread(filename_3, cv::IMREAD_GRAYSCALE)));
        pictures.push_back(cv::Mat(cv::imread(filename_4, cv::IMREAD_GRAYSCALE)));
        pictures.push_back(cv::Mat(cv::imread(filename_5, cv::IMREAD_GRAYSCALE)));

        uchar back= static_cast<uchar>(j["background"]);
        int limit(j["limit"]);

        cv::Vec3b color(std::stoi(argv[2]), std::stoi(argv[3]), std::stoi(argv[4]));

        std::vector<cv::Mat> error(PoroCheck(pictures, back, limit, color));
        WriteImages(error, filename);
    
}