#pragma once
#include "Config.h"

#include "Image.h"
#include "Debuger.h"

#include <hiredis.h>

#include <string>
#include <iostream>

class Cache {
public:
    redisContext* context;
    
    Cache(const std::string& connection_ip = "127.0.0.1", int port = 6379);

    ~Cache();
    
    void set_key(const std::string& key, const unsigned char* data_begin, size_t size);
    
    template<typename T>
    void set_key(const std::string& key, const T& value) {
        if (context != nullptr) {
            set_key(key, (unsigned char*)&value, sizeof(T));
        }
        else {
            #ifdef REDIS_CONNECTION_PRINTS
            std::cout << "set_key() is called but disconnected from server" << std::endl;
            #endif
        }
    }

    void set_key(const std::string& key, const std::string& value);
    void set_key(const std::string& key, Image& image);
    void set_key(const std::string& key, Image&& image);

    template<typename T>
    T get_key(const std::string& key) {
        if (context != nullptr) {
            redisReply* reply = (redisReply*)redisCommand(context, "get %b", key, key.size());
            #ifdef REDIS_CONNECTION_PRINTS
            std::cout << "key get with reply : " << *(T*)reply->str << std::endl;
            #endif
            return *(T*)reply->str;
        }
        else {
            #ifdef REDIS_CONNECTION_PRINTS
            std::cout << "set_key() is called but disconnected from server" << std::endl;
            #endif
            ASSERT(false);
        }
    }

    template<>
    unsigned char* get_key(const std::string& key) {
        if (context != nullptr) {
            redisReply* reply = (redisReply*)redisCommand(context, "get %b", key, key.size());
            #ifdef REDIS_CONNECTION_PRINTS
            std::cout << "key get with reply : " /* << (unsigned char*)reply->str*/ << std::endl;
            #endif
            return (unsigned char*)reply->str;
        }
        else {
            #ifdef REDIS_CONNECTION_PRINTS
            std::cout << "set_key() is called but disconnected from server" << std::endl;
            #endif
            ASSERT(false);
        }
    }

    template<>
    std::string get_key(const std::string& key) {
        if (context != nullptr) {
            redisReply* reply = (redisReply*)redisCommand(context, "get %b", key, key.size());
            #ifdef REDIS_CONNECTION_PRINTS
            std::cout << "key get with reply : " /* << (unsigned char*)reply->str*/ << std::endl;
            #endif
            return std::string(reply->str);
        }
        else {
            #ifdef REDIS_CONNECTION_PRINTS
            std::cout << "set_key() is called but disconnected from server" << std::endl;
            #endif
            ASSERT(false);
        }
    }

    template<>
    Image get_key<Image>(const std::string& key) {
        if (context != nullptr) {
            int width = get_key<int>(key + ":width");
            int height = get_key<int>(key + ":height");
            int channels = get_key<int>(key + ":channels");
            bool vflip = get_key<bool>(key + ":vflip");
            unsigned char* image_data = true ? get_key<unsigned char*>(key) : nullptr; // such a hacky way of doing it but compiler won't allow otherwise
            return Image(image_data, width, height, channels, vflip);
        }
        else {
            #ifdef REDIS_CONNECTION_PRINTS
            std::cout << "set_key() is called but disconnected from server" << std::endl;
            #endif
            ASSERT(false);
        }
    }
};
