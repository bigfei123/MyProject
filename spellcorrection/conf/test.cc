///
/// @file    test.cc
/// @author  lemon(cugchenzf@foxmail.com)
/// @date    2021-08-23 14:57:21
///

#include <list>
#include <string>
#include <fstream>
#include <sstream>
#include <json/json.h>
#include <iostream>
#include <unordered_map>
using namespace std;

struct CacheNode{
    string key;
    string value;
    CacheNode(string k, string v) : key(k), value(v){}
};

list<CacheNode> _cache;
unordered_map<string, list<CacheNode>::iterator> _hashMap;

string getElement(string key){
    auto it = _hashMap.find(key);
    if (it == _hashMap.end()){
        return string();
    }else {
        if (it->second != _cache.begin()){//将该cachenode插入到list的头部
            _cache.splice(_cache.begin(), _cache, it->second);
        }
        _hashMap[key] = _cache.begin();
        for(auto & e : _cache){
            cout << e.value << endl;
        }
        cout << endl;

        return _cache.begin()->value;
    }
}

void readFromFile(const string & filename){
    ifstream input(filename);
    if (!input){
        printf("Open cache file \"%s\" failed\n", filename.c_str());
        return;
    }else {
        printf("Open cache file \"%s\" success\n", filename.c_str());
    }

    Json::Value root, content;
    Json::Reader reader;
    Json::Value::Members mem;

    if (!reader.parse(input, root)){
        return;
    }

    content = root["all"];
    mem = content.getMemberNames(); //vector<string>
    for (auto cit = mem.cbegin(); cit != mem.cend(); ++cit){
        cout << content[*cit].toStyledString() << endl;
        _cache.emplace_back(*cit, content[*cit].toStyledString());
        if (_cache.size() == 100){
            break;
        }
    }
    cout << endl;

    for (auto it = _cache.begin(); it != _cache.end(); ++it){
        _hashMap.emplace(it->key, it);
    }

    input.close();
}

int main(void)
{
    readFromFile("../data/cache/cache");
    cout << getElement("你好") << endl;
    cout << getElement("此时") << endl;
    return 0; 
}
