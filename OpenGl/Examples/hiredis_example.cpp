#include "API/GraphicsCortex.h"

int main(int argc, char** argv) {

    Cache cache;

    cache.set_key("number", (long long)3);
    cache.set_key("name", "furkan");
    cache.set_key("image", Image("Images/GoldBlock.png"));

    int number_int = cache.get_key<int>("number");
    std::cout << number_int << std::endl;
    
    long long number_long_long = cache.get_key<long long>("number");
    std::cout << number_long_long << std::endl;

    std::string name = cache.get_key<std::string>("name");
    std::cout << name << std::endl;

    Image result_image = cache.get_key<Image>("image");
    result_image.save_to_disc("redis_image.png");

    std::cin.get();

}