#include "logomodel.h"

// 无参数的构造方法
logoModel::logoModel()
    : id(0), logoName(""), probability(0.0f), leftPosition(0), topPosition(0),
      width(0), height(0), imageOrigin(""), recognitionTime(""), type(0)
{
}

// 全参数构造方法
logoModel::logoModel(const int &id, const QString &logoName, const float &probability,
                     const int &leftPosition, const int &topPosition,
                     const int &width, const int &height,
                     const QString &imageOrigin, const QString &recognitionTime,
                     const int &type)
    : id(id), logoName(logoName), probability(probability), leftPosition(leftPosition),
      topPosition(topPosition), width(width), height(height), imageOrigin(imageOrigin),
      recognitionTime(recognitionTime), type(type)
{
}

// 各成员方法的实现

void logoModel::setId(const int &id) {
    this->id = id;
}

int logoModel::getId() const {
    return id;
}

void logoModel::setLogoName(const QString &logoName) {
    this->logoName = logoName;
}

QString logoModel::getLogoName() const {
    return logoName;
}

void logoModel::setProbability(const float &probability) {
    this->probability = probability;
}

float logoModel::getProbability() const {
    return probability;
}

void logoModel::setLeftPosition(const int &leftPosition) {
    this->leftPosition = leftPosition;
}

int logoModel::getLeftPosition() const {
    return leftPosition;
}

void logoModel::setTopPosition(const int &topPosition) {
    this->topPosition = topPosition;
}

int logoModel::getTopPosition() const {
    return topPosition;
}

void logoModel::setWidth(const int &width) {
    this->width = width;
}

int logoModel::getWidth() const {
    return width;
}

void logoModel::setHeight(const int &height) {
    this->height = height;
}

int logoModel::getHeight() const {
    return height;
}

void logoModel::setImageOrigin(const QString &imageOrigin) {
    this->imageOrigin = imageOrigin;
}

QString logoModel::getImageOrigin() const {
    return imageOrigin;
}

void logoModel::setRecognitionTime(const QString &recognitionTime) {
    this->recognitionTime = recognitionTime;
}

QString logoModel::getRecognitionTime() const {
    return recognitionTime;
}

void logoModel::setType(const int &type) {
    this->type = type;
}

int logoModel::getType() const {
    return type;
}
