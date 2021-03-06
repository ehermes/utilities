/**
 * @file
 * @copyright This code is licensed under the 3-clause BSD license.\n
 *            Copyright ETH Zurich, Laboratory for Physical Chemistry, Reiher Group.\n
 *            See LICENSE.txt for details.
 */

#ifndef UTILS_MATRIXDER_H
#define UTILS_MATRIXDER_H

#include <Utils/Math/AutomaticDifferentiation/TypeDefinitions.h>
#include <Eigen/Core>

namespace Scine {
namespace Utils {

// clang-format off

/*!
 * Container for a matrix and the derivatives of its elements.
 */
class MatrixWithDerivatives{
public:
  using Der0 = double;
  using Der1 = Utils::AutomaticDifferentiation::First3D;
  using Der2 = Utils::AutomaticDifferentiation::Second3D;
  using Matrix0 = Eigen::MatrixXd;
  using Matrix1 = Eigen::Matrix<Der1, Eigen::Dynamic, Eigen::Dynamic>;
  using Matrix2 = Eigen::Matrix<Der2, Eigen::Dynamic, Eigen::Dynamic>;
  template< Utils::derivOrder O > struct MatrixType { using MType = Matrix0; }; //Default
  template<Utils::derivOrder o> using Matrix = typename MatrixType<o>::MType;

  explicit MatrixWithDerivatives(int rows=0, int cols=0) {
    order_ = Utils::derivOrder::zero;
    setDimension(rows, cols);
  }

  void setOrder(Utils::derivOrder o) {
    order_ = o;
  }

  //TODO: ASSERTS IF WRONG ORDER IS CALLED
  Der0& v0(int i1, int i2) { return val(i1, i2); }
  Der1& v1(int i1, int i2) { return der(i1, i2); }
  Der2& v2(int i1, int i2) { return hes(i1, i2); }
  const Der0& v0(int i1, int i2) const { return val(i1, i2); }
  const Der1& v1(int i1, int i2) const { return der(i1, i2); }
  const Der2& v2(int i1, int i2) const { return hes(i1, i2); }
  template<Utils::derivOrder O> Matrix<O>& get();
  template<Utils::derivOrder O> const Matrix<O>& get() const;
  Matrix0& get0() { return val; }
  Matrix1& get1() { return der; }
  Matrix2& get2() { return hes; }
  const Matrix0& get0() const { return val; }
  const Matrix1& get1() const { return der; }
  const Matrix2& get2() const { return hes; }

  /*! Get a copy of the underlying matrix, without derivatives. */
  Eigen::MatrixXd getMatrixXd() const;

  /*! Initializes the members with dimensions given as parameters. */
  void setDimension(int rows, int cols);

  /*! Sets the base matrix to m. 
      Implicitly calls setDimension with correct dimensions. */
  void setBaseMatrix(const Eigen::MatrixXd& m);

  double getValue(int i, int j) const {
    if (order_ == Utils::derivOrder::zero)
      return v0(i, j);
    if (order_ == Utils::derivOrder::one)
      return v1(i, j).value();
    return v2(i, j).value();
  }
private:
  Utils::derivOrder order_;
  int nCols_ = 0, nRows_ = 0;
  Matrix0 val;
  Matrix1 der;
  Matrix2 hes;
};

template<> struct MatrixWithDerivatives::MatrixType<Utils::derivOrder::one> { using MType = Matrix1; };
template<> struct MatrixWithDerivatives::MatrixType<Utils::derivOrder::two> { using MType = Matrix2; };
template<> inline MatrixWithDerivatives::Matrix0& MatrixWithDerivatives::get<Utils::derivOrder::zero>() { return get0(); }
template<> inline MatrixWithDerivatives::Matrix1& MatrixWithDerivatives::get<Utils::derivOrder::one>() { return get1(); }
template<> inline MatrixWithDerivatives::Matrix2& MatrixWithDerivatives::get<Utils::derivOrder::two>() { return get2(); }
template<> inline const MatrixWithDerivatives::Matrix0& MatrixWithDerivatives::get<Utils::derivOrder::zero>() const { return get0(); }
template<> inline const MatrixWithDerivatives::Matrix1& MatrixWithDerivatives::get<Utils::derivOrder::one>() const { return get1(); }
template<> inline const MatrixWithDerivatives::Matrix2& MatrixWithDerivatives::get<Utils::derivOrder::two>() const { return get2(); }

// clang-format on

} // namespace Utils
} // namespace Scine
#endif // UTILS_MATRIXDER_H
