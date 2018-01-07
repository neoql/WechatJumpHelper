//
// Created by neoql on 1/6/18.
//

#include <fstream>

#include "Config.h"

#define CONFIG_PATH "/etc/WechatJumpHelper/"

using namespace std;

Config::Config(const char *path) {
    Json::CharReaderBuilder builder;
    ifstream ifs(path);

    this->root = new Json::Value;

    Json::parseFromStream(builder, ifs, this->root, nullptr);
}

Config::Config(const int &height, const int &width) {
    char path[128];

    sprintf(path, "%s%dx%d/config.json", CONFIG_PATH, height, width);
    new (this)Config(path);
}

Config::~Config() {
    delete this->root;
}

const Json::Value &Config::operator[](const char *key) const {

    return this->root->operator[](key);
}
