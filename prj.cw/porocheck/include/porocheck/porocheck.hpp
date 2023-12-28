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
    @brief Структура уникальной вершины, где хранится номер слоя и номер компоненты связности
*/
struct UniqueNode {
    int layer;
    int component;
    bool operator==(const UniqueNode& other) const {
        return (layer == other.layer && component == other.component);
    }
};

/**
    @brief Хэш-функция для структуры уникальных вершин 
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
    @brief Структура, для хранения слоя и компонент связности, которые должны быть отмечены
*/
struct ToColor {
    int layer;
    std::vector<int> components;
};


/**
    @brief Функция для бинаризации изображения. 
    @param in Изображение котрое надо преобразовать 
    @param back Параметр цвета фона, в зависимости от которого цвета будут или не будут инвертированы 
*/
void bin(cv::Mat& im, const uchar& back);



/**
    @brief Функция для нахождения уникальных пар (слой, компонента) на изображении. 
    @param height Высота изображения 
    @param width Ширина изображения 
    @param amount Количество связных компонент на изображении 
    @param graph Граф, к которому требуется добавить вершины 
    @param node_set Список, где хранятся значения вершин 
    @param UniqueNodes	Список вершин графа 
*/
void FillUniqueNodes(const int& height, const int& width, const int& amount, lemon::ListGraph& graph, const int& picture_index,
    lemon::ListGraph::NodeMap<UniqueNode>& node_set, std::unordered_map<UniqueNode, lemon::ListGraph::Node>& UniqueNodes);

/**
    @brief Функция для нахождения висячих камней на изображении. 
    @param limit Пороговый вес, начиная с которого камень считается висячим 
    @param graph Граф, в котором требуется искать такие компоненты 
    @param node_set Список значений вершин графа 
*/
std::vector<ToColor> FindRocks(const int& limit, const lemon::ListGraph& g, lemon::ListGraph::NodeMap<UniqueNode>& node_set);

/**
    @brief Функция для покраски висячего камня на изображении, полученный из функции FindRocks. 
    @param rocks Массив уникальных пар (слой, компоненты), которые требуется покрасить 
    @param filling_color Цвет, которым требуется покрасить висячие камни 
    @param back Цвет фона 
    @param images Сыылка на массив изначальных изображений, часть которых требуется скопировать 
*/
std::vector<cv::Mat> ColorRocks(std::vector<ToColor>& rocks, const cv::Vec3b& filling_color, const uchar& back, const std::vector<cv::Mat>& images);

/**
    @brief Функция для проверки разметки пор на изображениях. 
    @param background Цвет Поры - фона
    @param limit Пороговый вес, с которого компонента связности относится к висячим камням
    @param color цвет, в который будут покрашены компоненты связности относящиеся к висячим камням на кажом слое
*/
std::vector<cv::Mat> PoroCheck(std::vector<cv::Mat>& pics, const uchar& back, const int& limit, cv::Vec3b& color);

/**
    @brief Функция для записи изображений в заданную папку
    @param images изображения, которые надо записать
    @param filename имя папки и паттерн имени файла куда записать изображения
*/
void WriteImages(std::vector<cv::Mat>& images, const std::string& filename);
#endif