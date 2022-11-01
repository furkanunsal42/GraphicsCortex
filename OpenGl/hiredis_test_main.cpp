#include "API/GraphicsCortex.h"

int main(int argc, char** argv) {

    RedisConnection redis;

    redis.set_key("name", "furkan");

    redis.set_key("image", Image("Images/cobble.png"));

    Image result_image = redis.get_key<Image>("image");
    result_image.save_to_disc("redis_image.png");

    std::cin.get();

}