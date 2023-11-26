#include <iostream>
#include <opencv2/opencv.hpp>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <vector>
#include <string>

void bin(cv::Mat& im) {
    cv::threshold(im, im, 128, 255, cv::THRESH_BINARY);
}
int main() {
    int n = 5;
    std::vector<cv::Mat> pics;
    {
        pics.push_back(cv::imread("C:/Users/romad/source/repos/PoroMarker/test_1.png", cv::IMREAD_GRAYSCALE));
        pics.push_back(cv::imread("C:/Users/romad/source/repos/PoroMarker/test_2.png", cv::IMREAD_GRAYSCALE));
        pics.push_back(cv::imread("C:/Users/romad/source/repos/PoroMarker/test_3.png", cv::IMREAD_GRAYSCALE));
        pics.push_back(cv::imread("C:/Users/romad/source/repos/PoroMarker/test_4.png", cv::IMREAD_GRAYSCALE));
        pics.push_back(cv::imread("C:/Users/romad/source/repos/PoroMarker/test_5.png", cv::IMREAD_GRAYSCALE));
        for (int t = 0; t < n; t++) {
            bin(pics[t]);
        }
    }
    using PairType = std::pair<std::pair<int, int>, std::pair<int, int>>;
    std::set<PairType> test_set;
    for (int p = 0; p < n-1 ; p += 1) {
        cv::Mat connected_1(pics[p].size(), CV_32S);
        cv::Mat connected_2(pics[p+1].size(), CV_32S);
        int labeled_1 = cv::connectedComponents(pics[p], connected_1, 8);
        int labeled_2 = cv::connectedComponents(pics[p+1], connected_2, 8);
        cv::Mat intersect = pics[p] & pics[p+1];
        cv::Mat intersect_label(pics[p].size(), CV_32S);
        cv::Mat stats, center;
        int mask_n = cv::connectedComponentsWithStats(intersect, intersect_label, stats, center, 8);
        for (int comp = 1; comp < mask_n; comp += 1) {
            bool found = 0;
            int border_x = stats.at<int>(comp, cv::CC_STAT_LEFT);
            int border_y = stats.at<int>(comp, cv::CC_STAT_TOP);
            int height = stats.at<int>(comp, cv::CC_STAT_HEIGHT);
            int width = stats.at<int>(comp, cv::CC_STAT_WIDTH);
            for (int c = border_x; c < border_x + width; c += 1) {
                uchar pixel = intersect.at<uchar>(border_y, c);
                if (pixel == 255) {
                    int c_1 = connected_1.at<int>(border_y, c);
                    int c_2 = connected_2.at<int>(border_y, c);
                    test_set.insert(std::make_pair(std::make_pair(p+1, c_1), std::make_pair(p+2, c_2)));
                    found = 1;
                    break;
                }
            }
            if (!found) {
                for (int r = border_y; r < border_y + height; r += 1) {
                    uchar pixel = intersect.at<uchar>(r, border_x);
                    if (pixel == 255) {
                        int c_1 = connected_1.at<int>(r, border_x);
                        int c_2 = connected_2.at<int>(r, border_x);
                        test_set.insert(std::make_pair(std::make_pair(p+1, c_1), std::make_pair(p+2, c_2)));
                        break;
                    }
                }
            }
        }
    }
    for (const auto& pair : test_set) {
        std::cout << pair.first.first << " " << pair.first.second << "||";
        std::cout << pair.second.first << " " << pair.second.second << std::endl;
    }
}
