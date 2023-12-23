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
void bin(cv::Mat& im);
void CheckColor(int& color_to_check);
void FillUniqueNodes(const int& height, const int& width, const int& amount, lemon::ListGraph& graph, const int& picture_index,
    lemon::ListGraph::NodeMap<UniqueNode>& node_set, std::unordered_map<UniqueNode, lemon::ListGraph::Node>& UniqueNodes);

std::vector<UniqueNode> FindRocks(const int& limit, const lemon::ListGraph& g, lemon::ListGraph::NodeMap<UniqueNode>& node_set);

void ColorRocks(std::vector<UniqueNode>& rocks, const std::vector<int>& filling_color, const std::vector<cv::Mat>& images);
void PoroCheck(std::vector<cv::Mat>& pics);

#endif