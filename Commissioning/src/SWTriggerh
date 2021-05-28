#pragma once

#include <Bisa.h>

class SWTrigger
{
public:
    SWTrigger(const Bisa::Config& config)
     : config_(config) { }
    
    ~SWTrigger() { }

    // Simple trigger which assumes parameters (<= 2ticks, <= 2strips, 25 ns apart)
    bool simple(const Bisa::HitCollection& hits);

    // Simple trigger which assumes parameters (different layers, same coord, 13 ns apart)
    bool pad(const Bisa::HitCollection& hits);

private:
    const Bisa::Config& config_;
};