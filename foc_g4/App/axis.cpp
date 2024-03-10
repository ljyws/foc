#include "axis.h"
#include "board.h"

Axis::Axis(RGB &rgb,
           Encoder& encoder,
           Controller &controller)
           : rgb_(rgb),
             encoder_(encoder),
             controller_(controller)

{
    rgb.axis_ = this;
    encoder.axis_ = this;
    controller.axis_ = this;
}
