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
	
	redisCommand(redis_context, "publish %b %b", key, strlen(key.c_str()), image.get_image_data(), image.get_size());


}