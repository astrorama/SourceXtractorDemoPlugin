/**
 * @file Petrosian/PetrosianPhotometry/PetrosianPhotometry.h
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

#ifndef _PLUGIN_PETROSIANPHOTOMETRY_PETROSIANPHOTOMETRY_H
#define _PLUGIN_PETROSIANPHOTOMETRY_PETROSIANPHOTOMETRY_H

#include <SEFramework/Property/Property.h>
#include <SEFramework/Source/SourceFlags.h>

namespace Petrosian {

/**
 * @class PetrosianPhotometry
 * @brief
 *
 */
class PetrosianPhotometry: public SourceXtractor::Property {

public:

  /**
   * @brief Destructor
   */
  virtual ~PetrosianPhotometry() = default;

  PetrosianPhotometry(double flux, double flux_error, double mag, double mag_error, SourceXtractor::Flags flags);

  double getFlux() const;

  double getFluxError() const;

  double getMag() const;

  double getMagError() const;

  SourceXtractor::Flags getFlags() const;

private:
  double m_flux, m_flux_error;
  double m_mag, m_mag_error;
  SourceXtractor::Flags m_flags;
};  // End of PetrosianPhotometry class

}  // namespace Petrosian


#endif
