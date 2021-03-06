/**
 * @file
 * @copyright This code is licensed under the 3-clause BSD license.\n
 *            Copyright ETH Zurich, Laboratory for Physical Chemistry, Reiher Group.\n
 *            See LICENSE.txt for details.
 */

#include "FockDiisModifier.h"
#include <Utils/MethodEssentials/Methods/SCFMethod.h>
#include <Utils/MethodEssentials/util/LcaoUtil/LcaoUtil.h>

namespace Scine {
namespace Utils {

void FockDiisModifier::onOverlapCalculated() {
  if (!initialized) {
    initialize();
    initialized = true;
  }

  mixer_.setNAOs(m->getNumberAtomicOrbitals());
  mixer_.setOverlapMatrix(m->getOverlapMatrix());

  mixer_.setUnrestricted(m->unrestrictedCalculationRunning());
}

void FockDiisModifier::onFockCalculated() {
  if (!sameNumberOfElectronsInMethodAndInDensityMatrix())
    return;
  mixer_.addMatrices(m->getFockMatrix(), m->getDensityMatrix());
  m->setFockMatrix(mixer_.getMixedFockMatrix());
}

void FockDiisModifier::initialize() {
  if (m->basisSetIsOrthogonal())
    setOrthogonal(true);
}

bool FockDiisModifier::sameNumberOfElectronsInMethodAndInDensityMatrix() {
  int nAlphaInMethod, nBetaInMethod;
  LcaoUtil::getNumberUnrestrictedElectrons(nAlphaInMethod, nBetaInMethod, m->getNumberElectrons(), m->spinMultiplicity());

  int nAlphaInMatrix = m->getDensityMatrix().numberElectronsInAlphaMatrix();
  int nBetaInMatrix = m->getDensityMatrix().numberElectronsInBetaMatrix();

  return nAlphaInMethod == nAlphaInMatrix && nBetaInMethod == nBetaInMatrix;
}

void FockDiisModifier::setSpaceSize(int n) {
  mixer_.setSubspaceSize(n);
}
} // namespace Utils
} // namespace Scine
