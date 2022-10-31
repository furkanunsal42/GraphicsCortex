#include "API/GraphicsCortex.h"

#include <hiredis.h>
#include <string>

int main() {
	
	redisContext* redis_context = redisConnect("127.0.0.1", 6379);
	std::string key = "name";

	size_t size = 100;
	unsigned char* value = new unsigned char[size];
	for (int i = 0; i < size; i++) {
		value[i] = 'a';
	}

	Image image("Images/orange.png");
	
	redisReply* reply;
	reply = (redisReply*)redisCommand(redis_context, "set %b %b", key, strlen(key.c_str()), image.get_image_data(), image.get_size());
	reply = (redisReply*)redisCommand(redis_context, "get %b", key, strlen(key.c_str()));

	std::cout << reply->str << std::endl;
	Image image_reply((unsigned char*)reply->str, image.get_width(), image.get_height(), image.get_channels(), true);
	image_reply.save_to_disc("hiredis_reply_image.png");

	std::cin.get();
}