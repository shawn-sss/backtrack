#ifndef WINDOW_CONFIG_H
#define WINDOW_CONFIG_H

// Qt includes
#include <QSize>

// Window size constraints
namespace App::Window {
constexpr QSize k_MINIMUM_WINDOW_SIZE(500, 250);
constexpr QSize k_DEFAULT_WINDOW_SIZE(1000, 590);
constexpr QSize k_MAXIMUM_WINDOW_SIZE(2000, 800);
}

#endif // WINDOW_CONFIG_H
