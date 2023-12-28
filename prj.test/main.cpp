#include <iostream>
#include <opencv2/opencv.hpp>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <vector>
#include <lemon/connectivity.h>
#include <lemon/list_graph.h>
#include <lemon/adaptors.h>
#include <string>
#include <unordered_map>
#include<map>
#include <lemon/core.h>
#include <lemon/maps.h>
#include <lemon/bits/variant.h>
#include <lemon/bits/graph_adaptor_extender.h>
#include <lemon/bits/map_extender.h>
#include <lemon/tolerance.h>
#include <algorithm>


struct UniqueNode {
    int layer;
    int component;
    bool operator==(const UniqueNode& other) const {
        return (layer == other.layer && component == other.component);
    }
};

template <>
struct std::hash<UniqueNode>
{
    std::size_t operator()(const UniqueNode& k) const
    {

        return ((std::hash<int>()(k.layer)
            ^ (std::hash<int>()(k.component) << 1)) >> 1);
    }
};

struct ToColor {
    int layer;
    std::vector<int> components;
};

//костыль, которого быть не должно но пока он есть
void bin(cv::Mat& im) {
    cv::threshold(im, im, 128, 255, cv::THRESH_BINARY);
}
void CheckColor(int& color_to_check ){
    if (color_to_check < 0 || color_to_check>255) {
        throw  std::out_of_range("Color out of range");
    }
}

void FillUniqueNodes(const int& height, const int& width, int amount, lemon::ListGraph& graph, int picture_index,
    lemon::ListGraph::NodeMap<UniqueNode>& node_set, std::unordered_map<UniqueNode, lemon::ListGraph::Node>& UniqueNodes) {

    cv::Mat labels(height, width, CV_32S);
    for (int c = 1; c < amount; c += 1) {
        UniqueNode pair{ picture_index, c };
        if (UniqueNodes.find(pair) == UniqueNodes.end()) {
            lemon::ListGraph::Node vertex = graph.addNode();
            UniqueNodes[pair] = vertex;
            node_set[vertex] = pair;
        }
    }

}
std::vector<ToColor> FindRocks(const int& limit, const lemon::ListGraph& g,  lemon::ListGraph::NodeMap<UniqueNode>& node_set) {


    lemon::ListGraph::NodeMap<int> abc(g);
    lemon::connectedComponents(g, abc);

    std::vector<int> weight(lemon::countConnectedComponents(g));
    //std::cout<<"\n" << lemon::countConnectedComponents(g) << std::endl;
    std::map<int, std::vector<UniqueNode>> comp_list;
    int counter = 0;
    std::cout << std::endl;
    counter = 0;
    for (lemon::ListGraph::NodeIt nodeit(g); nodeit != lemon::INVALID; ++nodeit, ++counter) {
        auto curr = abc[nodeit];
        comp_list[curr].push_back(node_set[nodeit]);
        weight[curr] += 1;
    }
 

    std::vector<UniqueNode> to_color;
    for (int i = 0; i < weight.size(); i += 1) {
        std::cout << weight[i] << " ";
        if (weight[i] <= limit) {
            to_color.insert(to_color.end(), comp_list[i].begin(), comp_list[i].end());
        }
    }
    std::vector<ToColor> error_layers;

    for (const auto& node : to_color) {
        bool layer_found = false;
        //если слой уже есть, то просто добавляем к его компонентам компоненты
        for (auto& entry : error_layers) {
            if (entry.layer == node.layer) {
                entry.components.push_back(node.component);
                layer_found = 1;
                break;
            }
        }
        // Если слоя нет, то создаем пару слой-компоненты
        if (!layer_found) {
            ToColor new_entry;
            new_entry.layer = node.layer;
            new_entry.components.push_back(node.component);
            error_layers.push_back(new_entry);
        }
    }
    return error_layers;
}
std::vector<cv::Mat> ColorRocks(std::vector<ToColor>& rocks, const std::vector<int>& filling_color, const std::vector<cv::Mat>& images) {
    std::vector<cv::Mat> res;
    for (const auto& el : rocks) {


        cv::Mat orig_img = images[el.layer - 1];
        cv::Mat labeled_img(orig_img.size(), CV_32S);
        int labels_amount = connectedComponents(orig_img, labeled_img, 8);

        std::vector<cv::Vec3b> colors(labels_amount);
        for (int label = 1; label < labels_amount; ++label) {
            colors[label] = cv::Vec3b(255, 255, 255);
        }
        // задаем цвета фону и той компоненте, которую надо покрасить
        colors[0] = cv::Vec3b(0, 0, 0);

        for (int component : el.components) {
            colors[component] = cv::Vec3b(filling_color[0], filling_color[1], filling_color[2]);
        }
        
        cv::Mat colored_img(orig_img.size(), CV_8UC3);
        for (int r = 0; r < colored_img.rows; ++r) {
            for (int c = 0; c < colored_img.cols; ++c) {
                int label = labeled_img.at<int>(r, c);
                cv::Vec3b& pixel = colored_img.at<cv::Vec3b>(r, c);
                pixel = colors[label];
            }
        }
        res.push_back(colored_img);
    }
    return res;
}


void ColorRocks_2(std::vector<ToColor>& rocks, const std::vector<int>& filling_color, const std::vector<cv::Mat>& images, std::string filename) {
    for (const auto& el : rocks) {
        cv::Mat orig_img = images[el.layer - 1];
        cv::Mat labeled_img(orig_img.size(), CV_32S);
        int labels_amount = connectedComponents(orig_img, labeled_img, 8);

        std::vector<cv::Vec3b> colors(labels_amount);
        for (int label = 1; label < labels_amount; ++label) {
            colors[label] = cv::Vec3b(255, 255, 255);
        }
        // задаем цвета фону и той компоненте, которую надо покрасить
        colors[0] = cv::Vec3b(0, 0, 0);

        for (int component : el.components) {
            colors[component] = cv::Vec3b(filling_color[0], filling_color[1], filling_color[2]);
        }

        cv::Mat colored_img(orig_img.size(), CV_8UC3);
        for (int r = 0; r < colored_img.rows; ++r) {
            for (int c = 0; c < colored_img.cols; ++c) {
                int label = labeled_img.at<int>(r, c);
                cv::Vec3b& pixel = colored_img.at<cv::Vec3b>(r, c);
                pixel = colors[label];
            }
        }
        std::string fn = filename;
        std::string filename_2 = ".png";
        filename_2.insert(0, std::to_string(el.layer));
        fn.insert(fn.length(), filename_2);
        cv::imwrite(fn, colored_img);
    }

}

std::vector<cv::Mat> PoroCheck(std::vector<cv::Mat>& pics, std::string filename) {
    for (int t = 0; t < pics.size(); t++) {
        bin(pics[t]);
    }
    lemon::ListGraph g;
    //создаем список вершин, где хранится информация о значении вершины графа
    lemon::ListGraph::NodeMap<UniqueNode> node_set(g);
    //вершина в графе
    std::unordered_map<UniqueNode,lemon:: ListGraph::Node> uniqueNodes;
    //построение графа
    for (int p = 0; p < pics.size() - 1; p += 1) {
        //создаем размеченные изображения 
        cv::Mat connected_1(pics[p].size(), CV_32S);
        cv::Mat connected_2(pics[p + 1].size(), CV_32S);
        //находим кол-во компоненет
        int labeled_1 = cv::connectedComponents(pics[p], connected_1, 8);
        int labeled_2 = cv::connectedComponents(pics[p + 1], connected_2, 8);
        //запоминаем размеры изображения
        int height = pics[p].rows;
        int width = pics[p].cols;
        int pic_1 = p + 1;
        int pc_2 = p + 2;
        //Заполняем граф уникальными вершинами
        FillUniqueNodes(height, width, labeled_1, g, pic_1, node_set, uniqueNodes);
        FillUniqueNodes(height, width, labeled_2, g, pc_2, node_set, uniqueNodes);
        // пересекаем маски
        cv::Mat intersect = pics[p] & pics[p + 1];

       

        cv::Mat intersect_label(pics[p].size(), CV_32S);
        cv::Mat stats, center;
        // проходим маску пересечений
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
                    int c_temp = intersect_label.at<int>(border_y, c);
                    if (c_temp == comp) {

                        int c_1 = connected_1.at<int>(border_y, c);
                        int c_2 = connected_2.at<int>(border_y, c);
                        UniqueNode pair_1{ p + 1, c_1 };
                        UniqueNode pair_2{ p + 2, c_2 };
                        g.addEdge(uniqueNodes[pair_1], uniqueNodes[pair_2]);

                        found = 1;
                        break;
                    }
                }
            }
            if (!found) {
                for (int r = border_y; r < border_y + height; r += 1) {
                    uchar pixel = intersect.at<uchar>(r, border_x);
                    if (pixel == 255) {
                        int c_temp = intersect_label.at<int>(r, border_x);
                        if (c_temp == comp) {
                            int c_1 = connected_1.at<int>(r, border_x);
                            int c_2 = connected_2.at<int>(r, border_x);
                            UniqueNode pair_1{ p + 1, c_1 };
                            UniqueNode pair_2{ p + 2, c_2 };                      
                            g.addEdge(uniqueNodes[pair_1], uniqueNodes[pair_2]);
                            break;
                        }
                    }
                }
            }
        }

    }
    //анализ графа 
    int limit;
    std::cout << " Enter limit weight ";
    std::cin >> limit;
    std::vector<ToColor> to_color = FindRocks(limit, g, node_set);

    std::vector<cv::Mat> mod;

    if (to_color.empty()) {
        std::cout << "Checked, no dandling rocks detected" << std::endl;
    }
    else {
        std::cout<< "Dandling rocks detected, check markup" << std::endl;
        std::cout << "\n" << "Enter color to fill components ";
        std::vector<int> color(3);
        for (int t = 0; t < color.size(); t += 1) {
            int temp;
            std::cin >> color[t];
            CheckColor(color[t]);
        }
        mod=ColorRocks(to_color, color, pics);

        ColorRocks_2(to_color, color, pics, filename);
        std::cout << "\nColored";
    }
    return mod;

}







int main() {

    //инициализация данных
    int n = 2;
    std::vector<cv::Mat> pictures;
    {
        pictures.push_back(cv::imread("C:/Users/romad/source/repos/temporary/misis2023f-22-04-romadova-i-o/input_pictures/t_2_1.png", cv::IMREAD_GRAYSCALE));
        pictures.push_back(cv::imread("C:/Users/romad/source/repos/temporary/misis2023f-22-04-romadova-i-o/input_pictures/t_2_2.png", cv::IMREAD_GRAYSCALE));
        pictures.push_back(cv::imread("C:/Users/romad/source/repos/temporary/misis2023f-22-04-romadova-i-o/input_pictures/t_2_3.png", cv::IMREAD_GRAYSCALE));
        pictures.push_back(cv::imread("C:/Users/romad/source/repos/temporary/misis2023f-22-04-romadova-i-o/input_pictures/t_2_4.png", cv::IMREAD_GRAYSCALE));
        pictures.push_back(cv::imread("C:/Users/romad/source/repos/temporary/misis2023f-22-04-romadova-i-o/input_pictures/t_2_5.png", cv::IMREAD_GRAYSCALE));
        

    }
    std::string filename = "C:/Users/romad/source/repos/temporary/misis2023f-22-04-romadova-i-o/input_pictures/";

    std::vector<cv::Mat> a(PoroCheck(pictures, filename));
    /*cv::imshow("qwertyu", a[2]);
    cv::waitKey(0);*/

}