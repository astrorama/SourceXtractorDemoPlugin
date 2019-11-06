/**
 * @file Petrosian/PetrosianPhotometry/PetrosianPhotometryArray.h
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

#ifndef _PETROSIAN_PETROSIANPHOTOMETRY_PETROSIANPHOTOMETRYARRAY_H
#define _PETROSIAN_PETROSIANPHOTOMETRY_PETROSIANPHOTOMETRYARRAY_H

#include <SEFramework/Property/Property.h>
#include "Petrosian/PetrosianPhotometry/PetrosianPhotometry.h"

namespace Petrosian {

/**
 * @class PetrosianPhotometryArray
 * @brief
 *  This class holds all the photometries, measured in different frames,
 *  for a given source.
 */
class PetrosianPhotometryArray: public SourceXtractor::Property {

public:

  /**
   * Default destructor
   */
  virtual ~PetrosianPhotometryArray() = default;

  PetrosianPhotometryArray(const std::vector<PetrosianPhotometry>& photometries);

  std::vector<double> getFluxes() const;

  std::vector<double> getFluxErrors() const;

  std::vector<double> getMags() const;

  std::vector<double> getMagErrors() const;

  std::vector<SourceXtractor::Flags> getFlags() const;

private:
  std::vector<double> m_fluxes, m_flux_errors;
  std::vector<double> m_mags, m_mag_errors;
  std::vector<SourceXtractor::Flags> m_flags;

};  // End of PetrosianPhotometryArray class

}  // namespace Petrosian


#endif
