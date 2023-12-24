#include <iostream>
#include <porocheck/porocheck.hpp>




//int main() {
//    //инициализация данных
//    build_check();
//    std::vector<cv::Mat> pictures = ReadImages("C:/Users/romad/source/repos/PoroMarker/pics_copy/test_1/", "t_1_", 5);
//    cv::Vec3b a = { 255, 255, 40 };
//    PoroCheck(pictures, 255, 2, a);
//    //ShowIntersection(pictures[3], pictures[4]);
//
//}

int main(int argc, char* argv[]) {
    ////инициализация данных
    // porocheck, dirname, filename, pics_amount, back, limit, color(3), fn ount
    if (argv[1]=="PoroCheck") {
        std::string dir = argv[2], filename = argv[3];
        int amount = *argv[4]-'0';
        std::cout << "\n" << "Enter background color: ";
        std::vector<cv::Mat> pictures = ReadImages(dir, filename, amount);
        PoroCheck(pictures, uchar(argv[5]), *argv[6] - '0', cv::Vec3b(*argv[7], *argv[8], *argv[9]));

}