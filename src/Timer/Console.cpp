//
// Created by Dylan Meadows on 2020-03-10.
//

#include "Console.h"

namespace EonTimer::Timer {
    const double GBA_FRAMERATE = 1000 / 59.7275;
    const double NDS_GBA_FRAMERATE = 1000 / 59.6555;
    const double NDS_FRAMERATE = 1000 / 59.8261;

    const std::vector<Console> &getConsoles() {
        static const std::vector<Console> values{GBA, NDS_GBA, NDS, DSI, _3DS};
        return values;
    }

    const QString &getName(const Console console) {
        static const std::vector<QString> names{"GBA", "NDS-GBA", "NDS", "DSI", "3DS"};
        return names[console];
    }

    double getFrameRate(const Console console) {
        switch (console) {
            case GBA:
                return GBA_FRAMERATE;
            case NDS_GBA:
                return NDS_GBA_FRAMERATE;
            default:
                return NDS_FRAMERATE;
        }
    }
}  // namespace EonTimer