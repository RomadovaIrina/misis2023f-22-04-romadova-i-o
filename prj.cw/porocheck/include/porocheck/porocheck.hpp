#pragma once
#ifndef MARKUP_CHECK_2023
#define MARKUP_CHECK_2023


#include <iostream>
#include <opencv2/opencv.hpp>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <vector>
#include <lemon/connectivity.h>
#include <lemon/list_graph.h>
#include <string>
#include <unordered_map>

void build_check() {
    std::cout << "Ok";
}


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



//костыль, которого быть не должно но пока он есть
void bin(cv::Mat& im) {
    cv::threshold(im, im, 128, 255, cv::THRESH_BINARY);
}
void CheckColor(int& color_to_check) {
    if (color_to_check < 0 || color_to_check>255) {
        throw  std::out_of_range("Color out of range");
    }
}

void FillUniqueNodes(const int& height, const int& width, const int& amount, lemon::ListGraph& graph, const int& picture_index,
    lemon::ListGraph::NodeMap<UniqueNode>& node_set, std::unordered_map<UniqueNode, lemon::ListGraph::Node>& UniqueNodes) {

    cv::Mat labels(height, width, CV_32S);
    for (int c = 1; c < amount; c += 1) {
        UniqueNode pair{ picture_index, c };
        // кажетс€ все вершины стоит добавл€ть еще до начала их стыковки
        if (UniqueNodes.find(pair) == UniqueNodes.end()) {
            lemon::ListGraph::Node vertex = graph.addNode();
            UniqueNodes[pair] = vertex;
            node_set[vertex] = pair;
        }
    }

}

std::vector<UniqueNode> FindRocks(const int& limit, const lemon::ListGraph& g, lemon::ListGraph::NodeMap<UniqueNode>& node_set) {


    lemon::ListGraph::NodeMap<int> abc(g);
    lemon::connectedComponents(g, abc);

    std::vector<int> weight(lemon::countConnectedComponents(g));
    std::map<int, std::vector<UniqueNode>> comp_list;
    int counter = 0;
    std::cout << std::endl;
    counter = 0;
    for (lemon::ListGraph::NodeIt nodeit(g); nodeit != lemon::INVALID; ++nodeit, ++counter) {
        auto curr = abc[nodeit];
        comp_list[curr].push_back(node_set[nodeit]);
        weight[curr] += 1;
    }
    /*for (const auto& el : comp_list) {
    std::cout << el.first << " -> " << "{ ";
    for (const auto& el2 : el.second) {
        std::cout << "(" << el2.layer << " " << el2.component << ") ";
    }
    std::cout << "} " << weight[el.first] << "\n";
    }*/

    std::vector<UniqueNode> to_color;
    for (int i = 0; i < weight.size(); i += 1) {
        if (weight[i] <= limit) {
            to_color.insert(to_color.end(), comp_list[i].begin(), comp_list[i].end());
        }
    }
    return to_color;
}

void ColorRocks(std::vector<UniqueNode>& rocks, const std::vector<int>& filling_color, const std::vector<cv::Mat>& images) {
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
        colors[el.component] = cv::Vec3b(filling_color[0], filling_color[1], filling_color[2]);

        cv::Mat colored_img(orig_img.size(), CV_8UC3);
        for (int r = 0; r < colored_img.rows; ++r) {
            for (int c = 0; c < colored_img.cols; ++c) {
                int label = labeled_img.at<int>(r, c);
                cv::Vec3b& pixel = colored_img.at<cv::Vec3b>(r, c);
                pixel = colors[label];
            }
        }
        std::string filename = ".png";
        filename.insert(0, std::to_string(el.component));
        filename.insert(0, " ");
        filename.insert(0, std::to_string(el.layer));
        cv::imwrite(filename, colored_img);
    }

}

void PoroCheck(std::vector<cv::Mat>& pics) {
    for (int t = 0; t < pics.size(); t++) {
        bin(pics[t]);
    }
    lemon::ListGraph g;
    //создаем список вершин, где хранитс€ информаци€ о значении вершины графа
    lemon::ListGraph::NodeMap<UniqueNode> node_set(g);
    //вершина в графе
    std::unordered_map<UniqueNode, lemon::ListGraph::Node> uniqueNodes;
    //построение графа
    for (int p = 0; p < pics.size() - 1; p += 1) {
        //создаем размеченные изображени€ 
        cv::Mat connected_1(pics[p].size(), CV_32S);
        cv::Mat connected_2(pics[p + 1].size(), CV_32S);
        //находим кол-во компоненет
        int labeled_1 = cv::connectedComponents(pics[p], connected_1, 8);
        int labeled_2 = cv::connectedComponents(pics[p + 1], connected_2, 8);
        //запоминаем размеры изображени€
        int height = pics[p].rows;
        int width = pics[p].cols;
        //«аполн€ем граф уникальными вершинами
        FillUniqueNodes(height, width, labeled_1, g, p + 1, node_set, uniqueNodes);
        FillUniqueNodes(height, width, labeled_2, g, p + 2, node_set, uniqueNodes);
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
                    int c_1 = connected_1.at<int>(border_y, c);
                    int c_2 = connected_2.at<int>(border_y, c);
                    UniqueNode pair_1{ p + 1, c_1 };
                    UniqueNode pair_2{ p + 2, c_2 };
                    g.addEdge(uniqueNodes[pair_1], uniqueNodes[pair_2]);
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
                        UniqueNode pair_1{ p + 1, c_1 };
                        UniqueNode pair_2{ p + 2, c_2 };
                        g.addEdge(uniqueNodes[pair_1], uniqueNodes[pair_2]);
                        break;
                    }
                }
            }
        }

    }
    //анализ графа 
    int limit;
    std::cout << " Enter limit weight ";
    std::cin >> limit;
    std::vector<UniqueNode> to_color = FindRocks(limit, g, node_set);
    if (to_color.empty()) {
        std::cout << "Checked, no dandling rocks detected" << std::endl;
    }
    else {
        std::cout << "Dandling rocks detected, check markup" << std::endl;
        std::cout << "\n" << "Enter color to fill components ";
        std::vector<int> color(3);
        for (int t = 0; t < color.size(); t += 1) {
            int temp;
            std::cin >> color[t];
            CheckColor(color[t]);
        }
        ColorRocks(to_color, color, pics);
        std::cout << "\nColored";
    }

}

#endif