/**
 * @file
 * @copyright This code is licensed under the 3-clause BSD license.\n
 *            Copyright ETH Zurich, Laboratory for Physical Chemistry, Reiher Group.\n
 *            See LICENSE.txt for details.
 */

#include "MatrixWithDerivatives.h"

namespace Scine {
namespace Utils {

Eigen::MatrixXd MatrixWithDerivatives::getMatrixXd() const {
  if (order_ == Utils::derivOrder::zero)
    return val;

  if (order_ == Utils::derivOrder::one) {
    Matrix0 m(der.rows(), der.cols());
    for (int i = 0; i < der.rows(); ++i) {
      for (int j = 0; j < der.cols(); ++j) {
        m(i, j) = der(i, j).value();
      }
    }
    return m;
  }

  // if (order_ == Utils::derivOrder::two)...
  Matrix0 m(hes.rows(), hes.cols());
  for (int i = 0; i < hes.rows(); ++i) {
    for (int j = 0; j < hes.cols(); ++j) {
      m(i, j) = hes(i, j).value();
    }
  }
  return m;
}

void MatrixWithDerivatives::setDimension(int rows, int cols) {
  nRows_ = rows;
  nCols_ = cols;
  val = Matrix0::Zero(rows, cols);
  der = Matrix1(rows, cols);
  hes = Matrix2(rows, cols);
}

void MatrixWithDerivatives::setBaseMatrix(const Eigen::MatrixXd& m) {
  setDimension(static_cast<int>(m.cols()), static_cast<int>(m.rows()));
  val = m;

  for (int i = 0; i < nRows_; ++i) {
    for (int j = 0; j < nCols_; ++j) {
      der(i, j).setValue(m(i, j));
      hes(i, j).setValue(m(i, j));
    }
  }
}

} // namespace Utils
} // namespace Scine
