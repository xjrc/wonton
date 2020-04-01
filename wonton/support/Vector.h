/*
 This file is part of the Ristra wonton project.
 Please see the license file at the root of this repository, or at:
 https://github.com/laristra/wonton/blob/master/LICENSE
*/

///////////////////////////////////////////////////////////////////////////////
// Copyright 2016 Los Alamos National Laboratory                             //
//                                                                           //
// Original Author: Paul Henning, from the SCF library.                      //
//                                                                           //
// Modified for GK by: Brian Jean                                            //
//                     TSM, ASCI Problem Setup Team                          //
//                     Applied Physics Division                              //
//                     Los Alamos National Laboratory                        //
//                     505.665.6374                                          //
//                     baj@lanl.gov                                          //
//                                                                           //
// Modified for Wonton by: Rao Garimella, rao@lanl.gov                       //
///////////////////////////////////////////////////////////////////////////////

#ifndef WONTON_VECTOR_H_
#define WONTON_VECTOR_H_

#include <cmath>
#include <cassert>
#include <iostream>
#include <vector>
#include <limits>
#include "wonton/support/wonton.h"

namespace Wonton {

/*!
  @class Vector "Vector.h"
  @brief Represents a vector in N-dimensional space
  @tparam D Indicates the dimensionality of the Vector (this will generally be one
  of [1, 2, 3]).
*/
template <int D> class Vector {
 private:
  double m_comp[D] {};

 public:

  /// Default constructor - zero Vector in D-space.
  WONTON_INLINE
  Vector() {
    for (int i = 0; i < D; i++)
      m_comp[i] = 0.0;
  }

  /*!
    @brief Initialize all components to same value (also constructor for 1d vectors)
    @param[in] xm_comp The x coordinate.
  */
  WONTON_INLINE
  Vector(const double& xm_comp) {
    for (int i = 0; i < D; i++)
      m_comp[i] = xm_comp;
  }

  /*!
    @brief Specialized constructor for 2d Vectors.
    @param[in] xm_comp,ym_comp The (x,y) coordinate pair.
  */
  WONTON_INLINE
  Vector(const double& xm_comp, const double& ym_comp) {
    assert(D == 2);
    m_comp[0] = xm_comp;
    m_comp[1] = ym_comp;
  }

  /*!
    @brief Specialized constructor for 3d Vectors.
    @param[in] xm_comp,ym_comp,zm_comp The (x,y,z) coordinate triple.
  */
  WONTON_INLINE
  Vector(const double& xm_comp, const double& ym_comp, const double& zm_comp) {
    assert(D == 3);
    m_comp[0] = xm_comp;
    m_comp[1] = ym_comp;
    m_comp[2] = zm_comp;
  }

  /*!
    @brief Constructor from a std:vector
  */
  inline
  explicit Vector(std::vector<double> const& invec) {
    assert(D == invec.size());
    for (int i = 0; i < D; i++) m_comp[i] = invec[i];
  }

  /// Return component @c i of the Vector.
  WONTON_INLINE
  const double& operator[](const int& i) const {
    return m_comp[i];
  }

  /// Return component @c i of the Vector.
  WONTON_INLINE
  double& operator[](const int& i) {
    return m_comp[i];
  }

  /// Negative of this vector.
  WONTON_INLINE
  Vector operator-() const {
    Vector v;
    for (int i = 0; i < D; i++) v.m_comp[i] = -m_comp[i];
    return v;
  }

  /// Add the Vector @c rhs to this Vector.
  WONTON_INLINE
  Vector& operator+=(const Vector<D>& rhs) {
    for (int i = 0; i < D; i++) m_comp[i] += rhs.m_comp[i];
    return *this;
  }

  /// Subtract the Vector @c rhs from this vector.
  WONTON_INLINE
  Vector& operator-=(const Vector<D>& rhs) {
    for (int i = 0; i < D; i++) m_comp[i] -= rhs.m_comp[i];
    return *this;
  }

  /// Scalar multiplication of this Vector by @c s.
  WONTON_INLINE
  Vector& operator*=(const double& s) {
    for (int i = 0; i < D; i++) m_comp[i] *= s;
    return *this;
  }

  /// Scalar division of this Vector by @c s.
  WONTON_INLINE
  Vector& operator/=(const double& s) {
    for (int i = 0; i < D; i++) m_comp[i] /= s;
    return *this;
  }

  /*!
    @brief Calculate the norm of a Vector.
    @param[in] doSqrt OPTIONAL: Return the square root of the norm, i.e. the
    magnitude of the Vector.
  */
  WONTON_INLINE
  double norm(bool doSqrt = true) const {
    double result = 0.0;
    for (int i = 0; i < D; i++) result += (m_comp[i] * m_comp[i]);
    if (doSqrt) return sqrt(result);
    return result;
  }

  /*!
    @brief Calculate the 1-norm of a Vector.
  */
  WONTON_INLINE
  double one_norm() const {
    double result = 0.0;
    for (int i = 0; i < D; i++) result += std::fabs(m_comp[i]);
    return result;
  }

  /*!
    @brief Calculate the max norm of a Vector.
  */
  WONTON_INLINE
  double max_norm() const {
    double result = std::fabs(m_comp[0]);
    for (int i = 0; i < D - 1; i++) {
      double abs_val = std::fabs(m_comp[i + 1]);
      if (result < abs_val)
        result = abs_val;
    }
    return result;
  }

  /// Convert this Vector into a unit Vector.
  WONTON_INLINE
  void normalize() {
    double s = norm();
    *this /= s;
  }

  /// Convert this Vector into a zero Vector.
  WONTON_INLINE
  void zero() {
    for (int i = 0; i < D; i++) m_comp[i] = 0;
  }

  /*!
    @brief Check if this Vector is a zero Vector.
    @param[in] dst_tol Distance tolerance:
    Vector is zero if its length is below this tolerance.
  */
  WONTON_INLINE
  bool is_zero(double dst_tol) const {
    return (norm() < dst_tol);
  }

  /*!
    @brief Convenience method for constructing a Vector
    with all components equal to a given value
    @param[in] value Value to assign to all the components
  */
  WONTON_INLINE
  void fill(double val) {
    for (int i = 0; i < D; i++) m_comp[i] = val;
  }

  /*!
    @brief Convenience method for constructing a unit Vector along a particular
    axis
    @param[in] nonZero The coordinate axis along which the Vector should point.
  */
  WONTON_INLINE
  void axis(int nonZero) {
    zero();
    m_comp[nonZero] = 1;
  }

  /// Read in a Vector from an input stream.
  std::istream& readFromStream(std::istream& is) {
    for (int i = 0; i < D; i++)
      is >> m_comp[i];
    return is;
  }

  /// Pretty printing of a Vector to an output stream.
  std::ostream& writeToStream(std::ostream& os) const {
    for (int i = 0; i < D; i++) {
      if (i > 0) os << ' ';
      os << m_comp[i];
    }
    return os;
  }
};

/// Alias for creating a 3D vector
typedef Vector<3> Vector3;

/// Alias for creating a 3D vector
typedef Vector<2> Vector2;

/// Dot product of two vectors, @f$\vec{a} \cdot \vec{b}@f$.
template<int D>
WONTON_INLINE
double dot(const Vector<D>& a, const Vector<D>& b) {
  double r = 0.0;
  for (int i = 0; i < D; i++) r += a[i] * b[i];
  return r;
}

/// Add two vectors.
template<int D>
WONTON_INLINE
Vector<D> operator+(const Vector<D>& a, const Vector<D>& b) {
  return Vector<D>(a) += b;
}

/// Subtract two vectors.
template<int D>
WONTON_INLINE
Vector<D> operator-(const Vector<D>& a, const Vector<D>& b) {
  return Vector<D>(a) -= b;
}

/// Multiply a vector by a scalar, @f$ s \vec{a}@f$.
template<int D>
WONTON_INLINE
Vector<D> operator*(const Vector<D>& a, const double& s) {
  return Vector<D>(a) *= s;
}

/// Multiply a vector by a scalar, @f$ s \vec{a}@f$.
template<int D>
WONTON_INLINE
Vector<D> operator*(const double& s, const Vector<D>& a) {
  return Vector<D>(a) *= s;
}

/// Divide a vector by a scalar, @f$ \frac{1}{s} \vec{a}@f$.
template<int D>
WONTON_INLINE
Vector<D> operator/(const Vector<D>& a, const double& s) {
  return Vector<D>(a) /= s;
}

/// Pretty printing of a Vector to an output stream.
template<int D>
inline
std::ostream& operator<<(std::ostream& os, const Vector<D>& v) {
  return v.writeToStream(os);
}

/// Read in a Vector from an input stream.
template<int D> inline std::istream&
operator>>(std::istream& is, Vector<D>& v) {
  return v.readFromStream(is);
}

/// Cross product operator for two 2d vectors,  @f$\vec{a} \times \vec{b}@f$.
WONTON_INLINE
double cross(const Vector<2>& a, const Vector<2>& b) {
  return (a[0] * b[1] - a[1] * b[0]);
}

/// Cross product operator for two 3d vectors, @f$\vec{a} \times \vec{b}@f$.
WONTON_INLINE
Vector<3> cross(const Vector<3>& a, const Vector<3>& b) {
  Vector<3> r;
  r[0] = a[1] * b[2] - a[2] * b[1];
  r[1] = a[2] * b[0] - a[0] * b[2];
  r[2] = a[0] * b[1] - a[1] * b[0];
  return r;
}

/*!
  @brief Obtain the value and index of the maximum component of a Vector.
  @param[in] v The input Vector.
  @param[in,out] icomp The index of the maximum component of @c v.
  @return The maximum component of @c v.
*/
template<int D>
WONTON_INLINE
double MaxComponent(const Vector<D>& v, int& icomp) {
  double max = v[0];
  icomp = 0;
  for (int i = 1; i < D; i++)
    if (max < v[i]) {
      max = v[i];
      icomp = i;
    }
  return max;
}

}  // namespace Wonton


#endif  // WONTON_VECTOR_H_
