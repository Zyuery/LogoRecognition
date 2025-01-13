#ifndef LOGOMODEL_H
#define LOGOMODEL_H

#include <QString>

class logoModel
{
public:
    logoModel(); // 无参数的构造方法
    logoModel(const int &id, const QString &logoName, const float &probability,
              const int &leftPosition, const int &topPosition,
              const int &width, const int &height,
              const QString &imageOrigin, const QString &recognitionTime,
              const int &type,const QString username); // 全参数构造方法

    // 设置和获取成员变量的公有方法
    void setId(const int &id);
    int getId() const;

    void setLogoName(const QString &logoName);
    QString getLogoName() const;

    void setProbability(const float &probability);
    float getProbability() const;

    void setLeftPosition(const int &leftPosition);
    int getLeftPosition() const;

    void setTopPosition(const int &topPosition);
    int getTopPosition() const;

    void setWidth(const int &width);
    int getWidth() const;

    void setHeight(const int &height);
    int getHeight() const;

    void setImageOrigin(const QString &imageOrigin);
    QString getImageOrigin() const;

    void setRecognitionTime(const QString &recognitionTime);
    QString getRecognitionTime() const;

    void setType(const int &type);
    int getType() const;

    void setUsername(const int &username);
    QString getUsername() const;

private:
    // 定义 logo 数据模型的属性
    int id;
    QString logoName;
    float probability;
    int leftPosition;
    int topPosition;
    int width;
    int height;
    QString imageOrigin;
    QString recognitionTime;
    int type;
    QString username;
};

#endif // LOGOMODEL_H
