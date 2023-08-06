#include <iostream>
#include <sys/ioctl.h>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>

#include <unistd.h> // 用于 usleep 函数
#include <cstdlib> // 用于 std::atoi 函数

using namespace cv;

// 函数：清空终端屏幕
void clearScreen()
{
    std::cout << "\033[2J\033[H"; // ANSI 转义序列，用于清空屏幕并设置光标位置为 (0,0)
    std::cout.flush();
}

// 获取终端屏幕尺寸，并将结果存储在传入的变量中
// 我的终端全屏尺寸是 (191x52)
void getSize(unsigned short &width, unsigned short &height)
{
    struct winsize size;
    ioctl(1, TIOCGWINSZ, &size);
    width = size.ws_col;
    height = size.ws_row;
}

/// 主函数
int main(int argc, char **argv)
{
    std::ios_base::sync_with_stdio(false);
    // 获取终端屏幕尺寸
    unsigned short term_width, term_height;
    getSize(term_width, term_height);

    std::string video_path;
    int frame_delay = 33; // 默认帧间延迟时间，以毫秒为单位（每秒30帧）

    // 检查是否提供了时间参数
    if (argc > 2) {
        video_path = argv[1];
        frame_delay = std::atoi(argv[2]);
    } else if (argc > 1) {
        video_path = argv[1];
    } else {
        std::cerr << "用法：" << argv[0] << " 视频路径 [帧间延迟时间]" << std::endl;
        return 1;
    }

    VideoCapture cap(video_path);
    if (!cap.isOpened()) {
        std::cerr << "错误：无法打开视频文件 " << video_path << std::endl;
        return 1;
    }

    Mat frame;
    const unsigned int new_height = term_height * 8U;
    const unsigned int new_width = term_width * 4U;
    const float aspect_ratio = static_cast<float>(new_width) / static_cast<float>(new_height);

    while (cap.read(frame)) {
        if (frame.empty())
            break;

        Mat resizedFrame;
        float frame_aspect = static_cast<float>(frame.cols) / static_cast<float>(frame.rows);
        if (frame_aspect < aspect_ratio)
            resize(frame, resizedFrame, Size(new_height * frame_aspect, new_height), 0, 0, INTER_AREA);
        else
            resize(frame, resizedFrame, Size(new_width, new_width / frame_aspect), 0, 0, INTER_AREA);

        for (unsigned int rows = 0U; rows <= resizedFrame.rows - 8; rows += 8) {
            for (unsigned int cols = 0U; cols <= resizedFrame.cols - 4; cols += 4) {
                Vec3b pixel = resizedFrame.at<Vec3b>(rows, cols);
                uint16_t r = pixel[2];
                uint16_t g = pixel[1];
                uint16_t b = pixel[0];
                printf("\033[38;2;%u;%u;%um", r, g, b);
                pixel = resizedFrame.at<Vec3b>(rows + 4, cols);
                r = pixel[2];
                g = pixel[1];
                b = pixel[0];
                printf("\033[48;2;%u;%u;%um", r, g, b);
                printf("\u2580");
            }
            printf("\033[m\n");
        }

        // 添加延迟以控制帧率
        usleep(frame_delay * 1000); // 将 frame_delay 从毫秒转换为微秒

        // 显示完一帧后，清空终端屏幕
        clearScreen();
    }

    return 0;
}