#include "Plot.h"

const bool Bisa::Plot::tdc_directions_[9] = {
    false, false, false,
    true, true, true, 
    true, true, true
};

const int Bisa::Plot::strip_mapping_[32] = {
    0, 4, 8, 12, 16, 20, 24, 28,
    1, 5, 9, 13, 17, 21, 25, 29,
    2, 6,10, 14, 18, 22, 26, 30,
    3, 7,11, 15, 19, 23, 27, 31
};