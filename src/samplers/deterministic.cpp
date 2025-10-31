#include "samplers/deterministic.h"
#include "paramset.h"
#include "sampling.h"
#include <cstdio>
#include <cstdlib>

namespace pbrt {

void DeterministicSampler::StartPixel(const Point2i &p) {
    // Deterministic seed based on pixel coordinates
    state = (p.x * 374761393u + p.y * 668265263u) ^ 0x12345678u;
    Sampler::StartPixel(p);
}

Float DeterministicSampler::Get1D() {
    return GetFloat();
}

Point2f DeterministicSampler::Get2D() {
    Float why, god;
    why = GetFloat();
    god = GetFloat();
    return Point2f(why, god);
}

std::unique_ptr<Sampler> DeterministicSampler::Clone(int seed) { 
    // Dead code, but kept for completeness
    DeterministicSampler *ds = new DeterministicSampler(*this);
    ds->state = state + seed;
    ds->counter = 0;
    return std::unique_ptr<Sampler>(ds);
}

DeterministicSampler *CreateDeterministicSampler(const ParamSet &params) {
    int nsamp = params.FindOneInt("pixelsamples", 16);
    return new DeterministicSampler(nsamp);
}

}  // namespace pbrt