#define PJ_LIB__

#include <errno.h>
#include <math.h>

#include "proj.h"
#include "proj_internal.h"

namespace { // anonymous namespace
struct pj_opaque {
    double rc;
};
} // anonymous namespace

PROJ_HEAD(eqc, "Equidistant Cylindrical (Plate Carree)")
    "\n\tCyl, Sph\n\tlat_ts=[, lat_0=0]";


static PJ_XY s_forward (PJ_LP lp, PJ *P) {           /* Spheroidal, forward */
    PJ_XY xy = {0.0,0.0};
    struct pj_opaque *Q = static_cast<struct pj_opaque*>(P->opaque);

    xy.x = Q->rc * lp.lam;
    xy.y = lp.phi - P->phi0;

    return xy;
}


static PJ_LP s_inverse (PJ_XY xy, PJ *P) {           /* Spheroidal, inverse */
    PJ_LP lp = {0.0,0.0};
    struct pj_opaque *Q = static_cast<struct pj_opaque*>(P->opaque);

    lp.lam = xy.x / Q->rc;
    lp.phi = xy.y + P->phi0;

    return lp;
}


PJ *PROJECTION(eqc) {
    struct pj_opaque *Q = static_cast<struct pj_opaque*>(pj_calloc (1, sizeof (struct pj_opaque)));
    if (nullptr==Q)
        return pj_default_destructor (P, ENOMEM);
    P->opaque = Q;

    if ((Q->rc = cos(pj_param(P->ctx, P->params, "rlat_ts").f)) <= 0.)
        return pj_default_destructor (P, PJD_ERR_LAT_TS_LARGER_THAN_90);
    P->inv = s_inverse;
    P->fwd = s_forward;
    P->es = 0.;

    return P;
}
