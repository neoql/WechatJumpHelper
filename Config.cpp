//
// Created by neoql on 1/6/18.
//

#include <fstream>

#include "Config.h"

using namespace std;

Config::Config(const char *path) {
    Json::CharReaderBuilder builder;
    ifstream ifs(path);

    this->root = new Json::Value;

    Json::parseFromStream(builder, ifs, this->root, nullptr);
}

const Json::Value &Config::operator[](const char *key) const {

    return this->root->operator[](key);
}

Config::~Config() {
    delete this->root;
}
