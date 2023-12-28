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


/**
    @brief ��������� ���������� �������, ��� �������� ����� ���� � ����� ���������� ���������
*/
struct UniqueNode {
    int layer;
    int component;
    bool operator==(const UniqueNode& other) const {
        return (layer == other.layer && component == other.component);
    }
};

/**
    @brief ���-������� ��� ��������� ���������� ������ 
*/
template <>
struct std::hash<UniqueNode>
{
    std::size_t operator()(const UniqueNode& k) const
    {

        return ((std::hash<int>()(k.layer)
            ^ (std::hash<int>()(k.component) << 1)) >> 1);
    }
};

/**
    @brief ���������, ��� �������� ���� � ��������� ���������, ������� ������ ���� ��������
*/
struct ToColor {
    int layer;
    std::vector<int> components;
};


/**
    @brief ������� ��� ����������� �����������. 
    @param in ����������� ������ ���� ������������� 
    @param back �������� ����� ����, � ����������� �� �������� ����� ����� ��� �� ����� ������������� 
*/
void bin(cv::Mat& im, const uchar& back);



/**
    @brief ������� ��� ���������� ���������� ��� (����, ����������) �� �����������. 
    @param height ������ ����������� 
    @param width ������ ����������� 
    @param amount ���������� ������� ��������� �� ����������� 
    @param graph ����, � �������� ��������� �������� ������� 
    @param node_set ������, ��� �������� �������� ������ 
    @param UniqueNodes	������ ������ ����� 
*/
void FillUniqueNodes(const int& height, const int& width, const int& amount, lemon::ListGraph& graph, const int& picture_index,
    lemon::ListGraph::NodeMap<UniqueNode>& node_set, std::unordered_map<UniqueNode, lemon::ListGraph::Node>& UniqueNodes);

/**
    @brief ������� ��� ���������� ������� ������ �� �����������. 
    @param limit ��������� ���, ������� � �������� ������ ��������� ������� 
    @param graph ����, � ������� ��������� ������ ����� ���������� 
    @param node_set ������ �������� ������ ����� 
*/
std::vector<ToColor> FindRocks(const int& limit, const lemon::ListGraph& g, lemon::ListGraph::NodeMap<UniqueNode>& node_set);

/**
    @brief ������� ��� �������� �������� ����� �� �����������, ���������� �� ������� FindRocks. 
    @param rocks ������ ���������� ��� (����, ����������), ������� ��������� ��������� 
    @param filling_color ����, ������� ��������� ��������� ������� ����� 
    @param back ���� ���� 
    @param images ������ �� ������ ����������� �����������, ����� ������� ��������� ����������� 
*/
std::vector<cv::Mat> ColorRocks(std::vector<ToColor>& rocks, const cv::Vec3b& filling_color, const uchar& back, const std::vector<cv::Mat>& images);

/**
    @brief ������� ��� �������� �������� ��� �� ������������. 
    @param background ���� ���� - ����
    @param limit ��������� ���, � �������� ���������� ��������� ��������� � ������� ������
    @param color ����, � ������� ����� ��������� ���������� ��������� ����������� � ������� ������ �� ����� ����
*/
std::vector<cv::Mat> PoroCheck(std::vector<cv::Mat>& pics, const uchar& back, const int& limit, cv::Vec3b& color);

/**
    @brief ������� ��� ������ ����������� � �������� �����
    @param images �����������, ������� ���� ��������
    @param filename ��� ����� � ������� ����� ����� ���� �������� �����������
*/
void WriteImages(std::vector<cv::Mat>& images, const std::string& filename);
#endif