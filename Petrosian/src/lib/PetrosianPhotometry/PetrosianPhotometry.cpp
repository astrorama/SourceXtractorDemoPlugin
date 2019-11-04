/**
 * @file src/lib/PetrosianPhotometry/PetrosianPhotometry.cpp
 * @date 11/05/19
 * @author aalvarez
 *
 * @copyright (C) 2012-2020 Euclid Science Ground Segment
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation; either version 3.0 of the License, or (at your option)
 * any later version.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *
 */

#include "Petrosian/PetrosianPhotometry/PetrosianPhotometry.h"

namespace Petrosian {


PetrosianPhotometry::PetrosianPhotometry(double flux, double flux_error, double mag, double mag_error,
                                         SourceXtractor::Flags flags)
  : m_flux(flux), m_flux_error(flux_error), m_mag(mag), m_mag_error(mag_error), m_flags(flags) {

}

double PetrosianPhotometry::getFlux() const {
  return m_flux;
}

double PetrosianPhotometry::getFluxError() const {
  return m_flux_error;
}

double PetrosianPhotometry::getMag() const {
  return m_mag;
}

double PetrosianPhotometry::getMagError() const {
  return m_mag_error;
}

SourceXtractor::Flags PetrosianPhotometry::getFlags() const {
  return m_flags;
}
}  // namespace Petrosian



