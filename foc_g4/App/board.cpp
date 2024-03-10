#include "board.h"

RGB rgb{&htim2};
Encoder mt6825;
Controller controller;
Axis axis{rgb,mt6825,controller};
