/*vex-vision-config:begin*/
#include "vex.h"
vex::vision::signature ORANGE = vex::vision::signature (1, 7433, 8355, 7894, -1509, -1093, -1301, 6, 0);
vex::vision::signature GREEN = vex::vision::signature (2, -7189, -2639, -4914, -3727, -1655, -2691, 2, 0);
vex::vision::signature PURPLE = vex::vision::signature (3, 2267, 3561, 2914, 4681, 8471, 6576, 2.3, 0);
vex::vision::signature SIG_4 = vex::vision::signature (4, 0, 0, 0, 0, 0, 0, 2.5, 0);
vex::vision::signature SIG_5 = vex::vision::signature (5, 0, 0, 0, 0, 0, 0, 2.5, 0);
vex::vision::signature SIG_6 = vex::vision::signature (6, 0, 0, 0, 0, 0, 0, 2.5, 0);
vex::vision::signature SIG_7 = vex::vision::signature (7, 0, 0, 0, 0, 0, 0, 2.5, 0);
vex::vision vs = vex::vision (vex::PORT20, 75, ORANGE, GREEN, PURPLE, SIG_4, SIG_5, SIG_6, SIG_7);
/*vex-vision-config:end*/