#ifndef PBRT_SAMPLERS_DETERMINISTIC_H
#define PBRT_SAMPLERS_DETERMINISTIC_H

#include "pbrt.h"
#include "sampler.h"

namespace pbrt {

class DeterministicSampler : public Sampler {
  public:
    DeterministicSampler(int64_t samplesPerPixel)
        : Sampler(samplesPerPixel), state(0), counter(0) {}
    
    void StartPixel(const Point2i &p);
    Float Get1D();
    Point2f Get2D();
    std::unique_ptr<Sampler> Clone(int seed);
    
  private:
    uint32_t state;
    int counter;
    
    Float GetFloat() {
        // LCG: Numerical Recipes parameters
        state = state * 1664525u + 1013904223u;
        counter += 1;
        VLOG(2) << "LCG: counter = " << counter << ", u = " << (state >> 8) * 0x1.0p-24f;
        // Convert to [0,1) with fixed precision
        return (state >> 8) * 0x1.0p-24f;
    }
};

DeterministicSampler *CreateDeterministicSampler(const ParamSet &params);

}  // namespace pbrt

#endif  // PBRT_SAMPLERS_DETERMINISTIC_H