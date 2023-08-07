#include <iostream>
#include <sys/ioctl.h>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>

using namespace cv;

// 获取一个初始范围（domain）内的值，并将其转换为另一个范围
// 例如：sin(x)：[-1, 1] -> [3, 7]
template<typename T>
T map(T current_value, T minimum_domain_value, T maximum_domain_value, T minimum_range_value, T maximum_range_value);

// 获取终端屏幕尺寸，并将结果存储在传入的变量中
// 我的终端全屏尺寸是 (191x52)
void getSize(unsigned short &width, unsigned short &height);

/// 主函数
int main(int argc, char **argv)
{
    std::ios_base::sync_with_stdio(false);
    // 获取终端屏幕尺寸
    unsigned short term_width, term_height;
    getSize(term_width, term_height);

    std::string image_path;

    for (int i = 1; i < argc; i++) {
        image_path = argv[i];
        break;
    }

    Mat image = imread(image_path, IMREAD_COLOR);
    if (image.empty()) {
        std::cerr << "错误：无法打开文件 " << image_path << std::endl;
        return 1;
    }

    // TODO 如果图像尺寸 < 终端尺寸 * [4, 8]，则仅迭代显示
    const unsigned int new_height = term_height * 8U;
    const unsigned int new_width = term_width * 4U;
    const float image_aspect = static_cast<float>(image.cols) / static_cast<float>(image.rows);

    if (image_aspect < 1.f)
        // 垂直
        resize(image, image, Size(new_height * image_aspect, new_height), 0, 0, INTER_AREA);
    else
        // 水平
        resize(image, image, Size(new_width, new_width / image_aspect), 0, 0, INTER_AREA);

    for (unsigned int rows = 0U; rows <= image.rows - 8; rows += 8) {
        for (unsigned int cols = 0U; cols <= image.cols - 4; cols += 4) {
            Vec3b pixel = image.at<Vec3b>(rows, cols);
            uint16_t r = pixel[2];
            uint16_t g = pixel[1];
            uint16_t b = pixel[0];
            printf("\033[38;2;%u;%u;%um", r, g, b);
            pixel = image.at<Vec3b>(rows + 4, cols);
            r = pixel[2];
            g = pixel[1];
            b = pixel[0];
            printf("\033[48;2;%u;%u;%um", r, g, b);
            printf("\u2580");
        }
        printf("\033[m\n");
    }

    return 0;
}

// 函数
template<typename T>
T map(T val, T min_dom, T max_dom, T min_ran, T max_ran)
{
    return ((val - min_dom) * (max_ran - min_ran) / (max_dom - min_dom) + min_ran);
}

void getSize(unsigned short &width, unsigned short &height)
{
    struct winsize size;
    ioctl(1, TIOCGWINSZ, &size);
    width = size.ws_col;
    height = size.ws_row;
}