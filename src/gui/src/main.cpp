#include <Windows.h>
#include <iostream>
#include <uiframework.hpp>

int main(int argc, char**) {
    std::cout << argc << "\n";

    return 0;

    // WINDOW_CONFIG config{};
    // config.flags = { WINDOW_FLAG_SHOW, WINDOW_FLAG_HIDE_TITLE_BAR };
    // config.size = { 700, 600 };
    // config.name = "testwidow";
    // config.iconId = 1;
    // config.iconName = "disc_icon_2.png";

    // BROWSER_CONFIG bwConfig{};
    // bwConfig.flags = { BROWSER_FLAG_DEV_TOOLS };

    // return 0;
}