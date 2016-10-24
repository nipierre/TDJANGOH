// @(#)root/djangoh:$Id$

/** \class TMCParticle
   \ingroup djangoh

This class serves as a data storage for description of one particle.

It is especially convenient to store information taken from LUJETS common,
which is done by interface class TDjangoh.

*/

#include "TMCParticle.h"
#include "TPrimary.h"

# define pyname PYNAME
extern "C" void pyname(const Int_t &kf, const char *name, const Int_t len);

////////////////////////////////////////////////////////////////////////////////

void TMCParticle::ls(Option_t *) const
{
  printf("(%2i,%4i) <-%3i, =>[%3i,%3i]",fKS,fKF,fParent,
         fFirstChild,fLastChild);
  printf(":  p=(%7.3f,%7.3f,%9.3f) ;",fPx,fPy,fPz);

  printf(" E=%8.3f ; m=%7.3f ; V=(%g,%g,%g); t=%g, tau=%g\n",
         fEnergy,fMass,fVx,fVy,fVz,fTime,fLifetime);
}
