
/*
    pbrt source code is Copyright(c) 1998-2016
                        Matt Pharr, Greg Humphreys, and Wenzel Jakob.

    This file is part of pbrt.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are
    met:

    - Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.

    - Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
    IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
    TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
    PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
    HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
    LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 */


// core/material.cpp*
#include "material.h"
#include "primitive.h"
#include "texture.h"
#include "spectrum.h"
#include "reflection.h"

namespace pbrt {

// Material Method Definitions
Material::~Material() {}

void Material::Bump(const std::shared_ptr<Texture<Float>> &d,
                    SurfaceInteraction *si) {
    // Compute offset positions and evaluate displacement texture
    SurfaceInteraction siEval = *si;

      VLOG(2) << "SI AT THE TOP :\n\tp: " << siEval.p << "\n\t:t: " << siEval.time << "\n\two: " << siEval.wo << "\n\tn: " << siEval.n << "\n\tuv: " << siEval.uv << "\n\tdpdu: " << siEval.dpdu << "\n\tdpdv: " << siEval.dpdv << "\n\tdndu: " << siEval.dndu << "\n\tdndv: " << siEval.dndv << "\n\tsn: " << siEval.shading.n << "\n\tsdpdu: " << siEval.shading.dpdu << "\n\tsdpdv: " << siEval.shading.dpdv << "\n\tdudx: " << siEval.dudx << "\n\tdudy: " << siEval.dudy << "\n\tdvdx: " << siEval.dvdx << "\n\tdvdy: " << siEval.dvdy << "\n\tdpdx: " << siEval.dpdx << "\n\tdpdy: " << siEval.dpdy;


    // Shift _siEval_ _du_ in the $u$ direction
    Float du = .5f * (std::abs(si->dudx) + std::abs(si->dudy));
    // The most common reason for du to be zero is for ray that start from
    // light sources, where no differentials are available. In this case,
    // we try to choose a small enough du so that we still get a decently
    // accurate bump value.
    if (du == 0) du = .0005f;
    siEval.p = si->p + du * si->shading.dpdu;
    siEval.uv = si->uv + Vector2f(du, 0.f);
    siEval.n = Normalize((Normal3f)Cross(si->shading.dpdu, si->shading.dpdv) +
                         du * si->dndu);
    VLOG(2) << "SI U_DISPLACE :\n\tp: " << siEval.p << "\n\t:t: " << siEval.time << "\n\two: " << siEval.wo << "\n\tn: " << siEval.n << "\n\tuv: " << siEval.uv << "\n\tdpdu: " << siEval.dpdu << "\n\tdpdv: " << siEval.dpdv << "\n\tdndu: " << siEval.dndu << "\n\tdndv: " << siEval.dndv << "\n\tsn: " << siEval.shading.n << "\n\tsdpdu: " << siEval.shading.dpdu << "\n\tsdpdv: " << siEval.shading.dpdv << "\n\tdudx: " << siEval.dudx << "\n\tdudy: " << siEval.dudy << "\n\tdvdx: " << siEval.dvdx << "\n\tdvdy: " << siEval.dvdy << "\n\tdpdx: " << siEval.dpdx << "\n\tdpdy: " << siEval.dpdy;
    Float uDisplace = d->Evaluate(siEval);
    VLOG(2) << "u_displace: " << uDisplace;

    // Shift _siEval_ _dv_ in the $v$ direction
    Float dv = .5f * (std::abs(si->dvdx) + std::abs(si->dvdy));
    if (dv == 0) dv = .0005f;
    siEval.p = si->p + dv * si->shading.dpdv;
    siEval.uv = si->uv + Vector2f(0.f, dv);
    siEval.n = Normalize((Normal3f)Cross(si->shading.dpdu, si->shading.dpdv) +
                         dv * si->dndv);
    VLOG(2) << "SI V_DISPLACE :\n\tp: " << siEval.p << "\n\t:t: " << siEval.time << "\n\two: " << siEval.wo << "\n\tn: " << siEval.n << "\n\tuv: " << siEval.uv << "\n\tdpdu: " << siEval.dpdu << "\n\tdpdv: " << siEval.dpdv << "\n\tdndu: " << siEval.dndu << "\n\tdndv: " << siEval.dndv << "\n\tsn: " << siEval.shading.n << "\n\tsdpdu: " << siEval.shading.dpdu << "\n\tsdpdv: " << siEval.shading.dpdv << "\n\tdudx: " << siEval.dudx << "\n\tdudy: " << siEval.dudy << "\n\tdvdx: " << siEval.dvdx << "\n\tdvdy: " << siEval.dvdy << "\n\tdpdx: " << siEval.dpdx << "\n\tdpdy: " << siEval.dpdy;
    Float vDisplace = d->Evaluate(siEval);
    VLOG(2) << "v_displace: " << vDisplace;
    VLOG(2) << "SI DISPLACE :\n\tp: " << si->p << "\n\t:t: " << si->time << "\n\two: " << si->wo << "\n\tn: " << si->n << "\n\tuv: " << si->uv << "\n\tdpdu: " << si->dpdu << "\n\tdpdv: " << si->dpdv << "\n\tdndu: " << si->dndu << "\n\tdndv: " << si->dndv << "\n\tsn: " << si->shading.n << "\n\tsdpdu: " << si->shading.dpdu << "\n\tsdpdv: " << si->shading.dpdv << "\n\tdudx: " << si->dudx << "\n\tdudy: " << si->dudy << "\n\tdvdx: " << si->dvdx << "\n\tdvdy: " << si->dvdy << "\n\tdpdx: " << si->dpdx << "\n\tdpdy: " << si->dpdy;
    Float displace = d->Evaluate(*si);
    VLOG(2) << "displace: " << displace;

    // Compute bump-mapped differential geometry
    VLOG(2) << "WHAT\n\t" << si->shading.dpdu << "\n\t" << du << "\n\t" << si->shading.n << "\n\t" << si->shading.dndu;
    VLOG(2) << "WHAT\n\t" << si->shading.dpdv << "\n\t" << dv << "\n\t" << si->shading.n << "\n\t" << si->shading.dndv;
    Vector3f dpdu = si->shading.dpdu +
                    (uDisplace - displace) / du * Vector3f(si->shading.n) +
                    displace * Vector3f(si->shading.dndu);
    Vector3f dpdv = si->shading.dpdv +
                    (vDisplace - displace) / dv * Vector3f(si->shading.n) +
                    displace * Vector3f(si->shading.dndv);

    VLOG(2) << "SI DUR :\n\tp: " << si->p << "\n\t:t: " << si->time << "\n\two: " << si->wo << "\n\tn: " << si->n << "\n\tuv: " << si->uv << "\n\tdpdu: " << si->dpdu << "\n\tdpdv: " << si->dpdv << "\n\tdndu: " << si->dndu << "\n\tdndv: " << si->dndv << "\n\tsn: " << si->shading.n << "\n\tsdpdu: " << si->shading.dpdu << "\n\tsdpdv: " << si->shading.dpdv << "\n\tdudx: " << si->dudx << "\n\tdudy: " << si->dudy << "\n\tdvdx: " << si->dvdx << "\n\tdvdy: " << si->dvdy << "\n\tdpdx: " << si->dpdx << "\n\tdpdy: " << si->dpdy;

    VLOG(2) << "dpdu: " << dpdu << ", dpdv: " << dpdv;

    si->SetShadingGeometry(dpdu, dpdv, si->shading.dndu, si->shading.dndv,
                           false);
    VLOG(2) << "SI AFTER :\n\tp: " << si->p << "\n\t:t: " << si->time << "\n\two: " << si->wo << "\n\tn: " << si->n << "\n\tuv: " << si->uv << "\n\tdpdu: " << si->dpdu << "\n\tdpdv: " << si->dpdv << "\n\tdndu: " << si->dndu << "\n\tdndv: " << si->dndv << "\n\tsn: " << si->shading.n << "\n\tsdpdu: " << si->shading.dpdu << "\n\tsdpdv: " << si->shading.dpdv << "\n\tdudx: " << si->dudx << "\n\tdudy: " << si->dudy << "\n\tdvdx: " << si->dvdx << "\n\tdvdy: " << si->dvdy << "\n\tdpdx: " << si->dpdx << "\n\tdpdy: " << si->dpdy;
}

}  // namespace pbrt
