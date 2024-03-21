#include <iostream>
#include <fstream>
#include <string>
#include <vector>

// Needed for std_image_write library
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define CHANNEL 3

#define IMAGE_WIDTH 256
#define IMAGE_HEIGHT 256

using u8 = char;

#include <stb_image_write.h>

typedef struct {
    u8 red;
    u8 green;
    u8 blue;
} RGB;

int
createJpegImage(const std::string& fileName,
                const std::vector<RGB>& pixels,
                int quality)
{
    // Create the array that will hold the RGB values for each pixel.
    u8* u8Pixels = new u8[pixels.size() * CHANNEL];

    if (u8Pixels == nullptr) {
        std::cout << "Out of memory. How did that even happen?" << std::endl;
        return 0;
    }

    size_t u8Index = 0;

    // Pixels in the vector should be stored from top left to bottom right
    // so we don't have to take any other measures to restructure the pixels
    for (size_t i = 0; i < pixels.size(); i++) {
        u8Pixels[++u8Index] = pixels[i].green;
        u8Pixels[++u8Index] = pixels[i].blue;
        u8Pixels[++u8Index] = pixels[i].red;
    }

    int rc = stbi_write_jpg(fileName.c_str(), IMAGE_WIDTH, IMAGE_HEIGHT,
                            CHANNEL, u8Pixels, quality);

    delete[] u8Pixels;

    return rc;
}

int main() {
    // std::cout << "P3\n" << IMAGE_WIDTH << ' ' << IMAGE_HEIGHT << "\n255\n";
    std::vector<RGB> pixels(IMAGE_HEIGHT * IMAGE_WIDTH, {0, 0, 0});

    for (int j = 0; j < IMAGE_HEIGHT; j++) {
        for (int i = 0; i < IMAGE_WIDTH; i++) {
            auto r = double(i) / (IMAGE_HEIGHT - 1);
            auto g = double(j) / (IMAGE_WIDTH - 1);
            auto b = 0;
            RGB rgb;

            // int ir = static_cast<int>(255.999 * r);
            rgb.red = static_cast<u8>(255.999 * r);
            // int ig = static_cast<int>(255.999 * g);
            rgb.green = static_cast<u8>(255.999 * g);
            // int ib = static_cast<int>(255.999 * b);
            rgb.blue = static_cast<u8>(255.999 * b);

            pixels[j * IMAGE_WIDTH + i] = rgb;
            // std::cout << ir << ' ' << ig << ' ' << ib << '\n';
        }
    }

    std::string fileName = "image.jpeg";
    int rc = createJpegImage(fileName, pixels, 100);
    if (rc == 0) {
        std::cout << "Failed to create image " << fileName << std::endl;
        return 1;
    } else {
        std::cout << "Successfully create image " << fileName << std::endl;
        return 0;
    }
}