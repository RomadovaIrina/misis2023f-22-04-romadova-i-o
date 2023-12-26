#include <iostream>
#include <porocheck/porocheck.hpp>
#include "nlohmann/json.hpp"
#include <fstream>




//int main() {
//    //инициализация данных
//    build_check();
//    std::vector<cv::Mat> pictures = ReadImages("C:/Users/romad/source/repos/PoroMarker/pics_copy/test_1/", "t_1_", 5);
//    cv::Vec3b a = { 255, 255, 40 };
//    PoroCheck(pictures, 255, 2, a);
//    //ShowIntersection(pictures[3], pictures[4]);
//
//}

//int main(int argc, char* argv[]) {
//    if (argc > 1 && strcmp(argv[1], "PoroCheck") == 0) {
//        if (argc < 10) {
//            std::cerr << "Not enough arguments for PoroCheck\n";
//            return 1;
//        }
//
//        std::string dir = argv[2];
//        std::string filename = argv[3];
//        int amount = std::stoi(argv[4]);
//        uchar back = static_cast<uchar>(std::stoi(argv[5]));
//        int limit = std::stoi(argv[6]);
//        cv::Vec3b color(static_cast<uchar>(std::stoi(argv[7])),
//            static_cast<uchar>(std::stoi(argv[8])),
//            static_cast<uchar>(std::stoi(argv[9])));
//
//        std::cout << "\n" << "Enter background color: ";
//        std::vector<cv::Mat> pictures = ReadImages(dir, filename, amount);
//        PoroCheck(pictures, back, limit, color);
//    }
//
//    return 0;
//}

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

        pictures.push_back(cv::Mat(cv::imread(filename_1, cv::IMREAD_GRAYSCALE)));
        pictures.push_back(cv::Mat(cv::imread(filename_2, cv::IMREAD_GRAYSCALE)));
        pictures.push_back(cv::Mat(cv::imread(filename_3, cv::IMREAD_GRAYSCALE)));
        pictures.push_back(cv::Mat(cv::imread(filename_4, cv::IMREAD_GRAYSCALE)));
        pictures.push_back(cv::Mat(cv::imread(filename_5, cv::IMREAD_GRAYSCALE)));

        uchar back(j["background"]);
        int limit(j["limit"]);

        cv::Vec3b color(std::stoi(argv[2]), std::stoi(argv[3]), std::stoi(argv[4]));

        PoroCheck(pictures, 0, limit, color);
    
}