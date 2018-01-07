#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <random>

#include <opencv2/opencv.hpp>

#include "Config.h"

using namespace std;
using namespace cv;

static double press_coefficient;
static int piece_base_height_1_2;
static int piece_body_width;

Mat screenshot() {
    FILE *p, *f;
    char buf[1024];


    p = popen("adb shell screencap -p", "r");
    f = fopen("screenshot.png", "w");

    if (p != nullptr && f != nullptr) {
        while (!feof(p)) {
            size_t size;
            size = fread(buf, 1, 1024, p);
            fwrite(buf, 1, size, f);
        }
        pclose(p);
        fclose(f);
    }

    Mat ret = imread("screenshot.png");
    return ret;
}

void loadConfig() {
    Config cfg("/home/neoql/repo/wechat_jump_game/config/1920x1080/config.json");

    piece_base_height_1_2   = cfg["piece_base_height_1_2"].asInt();
    piece_body_width        = cfg["piece_body_width"].asInt();
    press_coefficient       = cfg["press_coefficient"].asDouble();
}

int randInt(const int &a, const int &b) {
    static random_device rd;

    return (rd() % (b - a) + a);
}

void buttonPostion(const Mat &img, int* const x, int * const y) {
    int width  = img.cols;
    int height = img.rows;

    int left, top;
    left = width / 2;
    top = 1584 * (height / 1920);
    *x = randInt(left - 50, left + 50);
    *y = randInt(top - 10, top + 10);
}

void jump(const double &distance, const int &x, const int &y) {
    double press_time = distance * press_coefficient;
    press_time = MAX(press_time, 200);

    char cmd[128];
    sprintf(cmd, "adb shell input swipe %d %d %d %d %d", x, y, x, y, static_cast<int>(press_time));

    cout << cmd << endl;
    system(cmd);
}

void findPieceAndBoard(const Mat &img,
                       int *const px, int *const py, int *const bx, int *const by) {
    int width  = img.cols;
    int height = img.rows;

    int piece_x_sum = 0;
    int piece_x_c   = 0;
    int piece_y_max = 0;
    int scan_x_border = width / 8;
    int scan_start_y = 0;

    *px = *py = *bx = *by = 0;

    for (int i = height / 3; i < height * 2 / 3; i += 50) {
        const auto &last_pixel = img.at<Vec3b>(i, 0);
        for (int j = 1; j < width; j++) {
            const auto &pixel = img.at<Vec3b>(i, j);
            if (pixel[0] != last_pixel[0]
                || pixel[1] != last_pixel[1]
                || pixel[2] != last_pixel[2]) {

                scan_start_y = i - 50;
                break;
            }
        }

        if (scan_start_y) break;
    }

    cout << "scan_start_y:" << scan_start_y << endl;

    for (int i = scan_start_y; i < height * 2 / 3; i++) {
        for (int j = scan_x_border; j < width - scan_x_border; j++) {
            const auto &pixel = img.at<Vec3b>(i, j);
            if (50 < pixel[2] && pixel[2] < 60
                    && 53 < pixel[1] && pixel[1] < 63
                    && 95 < pixel[0] && pixel[0] < 110) {
                piece_x_sum += j;
                piece_x_c += 1;
                piece_y_max = MAX(i, piece_y_max);
            }
        }
    }

    if (!(piece_x_c && piece_x_sum)) {
        *px = *py = *bx = *by = 0;
        return;
    }
    *px = piece_x_sum / piece_x_c;
    *py = piece_y_max - piece_base_height_1_2;

    int board_x_start, board_x_end;

    if (*px < width / 2) {
        board_x_start = *px;
        board_x_end = width;
    } else {
        board_x_start = 0;
        board_x_end = *px;
    }

    int index = 0;
    for (int i = height / 3; i < height * 2 / 3; i++) {
        index = i;
        const auto &last_pixel = img.at<Vec3b>(i, 0);

        if (*bx || *by) {
            break;
        }

        int board_x_sum = 0;
        int board_x_c   = 0;

        for (int j = board_x_start; j < board_x_end; j++) {
            const auto &pixel = img.at<Vec3b>(i, j);

            if (abs(j - *px) < piece_body_width) {
                continue;
            }

            if ((abs(pixel[0] - last_pixel[0])
                + abs(pixel[1] - last_pixel[1])
                + abs(pixel[2] - last_pixel[2])) > 10) {

                board_x_sum += j;
                board_x_c   += 1;
            }
        }

        if (board_x_sum) {
            *bx = board_x_sum / board_x_c;
        }
    }

    const auto &last_pixel = img.at<Vec3b>(index, *bx);

    int k = 0;
    for (k = index + 274; k > index; k--) {
        const auto &pixel = img.at<Vec3b>(k, *bx);
        if (abs(pixel[0] - last_pixel[0])
                + abs(pixel[1] - last_pixel[1])
                + abs(pixel[2] - last_pixel[2]) < 10) {
            break;
        }
    }
    *by = (index + k) / 2;

    for (int l = index; l < index; l++) {
        const auto &pixel = img.at<Vec3b>(l, *bx);
        if (abs(pixel[0] - 245)
                + abs(pixel[1] - 245)
                + abs(pixel[2] - 245) == 0) {
            *by = l + 10;
        }
    }

    if (!(bx, by)) {
        *px = *py = *bx = *by = 0;
        return;
    }
}

int main(int argc, char *argv[]) {

    loadConfig();
    for (;;) {
        Mat img = screenshot();
        int px, py, bx, by;
        int x, y;
        findPieceAndBoard(img, &px, &py, &bx, &by);
        buttonPostion(img, &x, &y);
        jump(sqrt((bx - px) * (bx - px) + (by - py) * (by - py)), x, y);
        sleep(1);
    }
}
