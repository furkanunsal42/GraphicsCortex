#pragma once
#include "RamCache.h"

Cache::Cache(const std::string& connection_ip, int port) {
    context = redisConnect(connection_ip.c_str(), port);
    if (context == nullptr) {
        #ifdef REDIS_CONNECTION_PRINTS
        std::cout << "redis connection failed " << std::endl;
        #endif
    }
    else if (context->err) {
        #ifdef REDIS_CONNECTION_PRINTS
        std::cout << "Error: " << context->errstr << std::endl;
        #endif
        redisFree(context);
        context = nullptr;
        #ifdef REDIS_CONNECTION_PRINTS
        std::cout << "redis connection failed " << std::endl;
        #endif
    }
    else {
        #ifdef REDIS_CONNECTION_PRINTS
        std::cout << "connetcion successful" << std::endl;
        #endif
    }
}

Cache::~Cache() {
    if(context != nullptr) {
        redisFree(context);
        #ifdef REDIS_CONNECTION_PRINTS
        std::cout << "disconnection successful" << std::endl;
        #endif
    }
}
    
void Cache::set_key(const std::string& key, const unsigned char* data_begin, size_t size) {
    if (context != nullptr) {
        redisReply* reply = (redisReply*)redisCommand(context, "set %b %b", key, key.size(), data_begin, size);
        #ifdef REDIS_CONNECTION_PRINTS
        std::cout << key << " key set with reply : " << reply->str << std::endl;
        #endif
    }
    else {
        #ifdef REDIS_CONNECTION_PRINTS
        std::cout << "set_key() is called but disconnected from server" << std::endl;
        #endif
    }
}

void Cache::set_key(const std::string& key, const std::string& value) {
    if (context != nullptr) {
        set_key(key, (unsigned char*)&(value[0]), value.size());
    }
    else {
        #ifdef REDIS_CONNECTION_PRINTS
        std::cout << "set_key() is called but disconnected from server" << std::endl;
        #endif
    }
}

void Cache::set_key(const std::string& key, Image& image) {
    return set_key(key, std::move(image));
}

void Cache::set_key(const std::string& key, Image&& image) {
    if (context != nullptr) {
        set_key(key,               image.get_image_data(), image.get_size());
        set_key(key + ":width",    image.get_width());
        set_key(key + ":height",   image.get_height());
        set_key(key + ":channels", image.get_channels());
        set_key(key + ":vflip",     image.get_vertical_flip());
    }
    else {
        #ifdef REDIS_CONNECTION_PRINTS
        std::cout << "set_key() is called but disconnected from server" << std::endl;
        #endif
    }
}
