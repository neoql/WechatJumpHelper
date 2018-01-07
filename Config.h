//
// Created by neoql on 1/6/18.
//

#ifndef JUMPHELPER_CONFIG_H
#define JUMPHELPER_CONFIG_H

#include <json/json.h>

class Config {
public:
    explicit Config(const char *path);
    Config(const int &height, const int &width);
    ~Config();
    const Json::Value& operator[](const char *key) const;

private:
    Json::Value *root;
};


#endif //JUMPHELPER_CONFIG_H
