/**
 * @file Petrosian/PetrosianPhotometry/PetrosianPhotometryTask.h
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

#ifndef _PLUGIN_PETROSIANPHOTOMETRY_PETROSIANPHOTOMETRYTASK_H
#define _PLUGIN_PETROSIANPHOTOMETRY_PETROSIANPHOTOMETRYTASK_H

#include <SEFramework/Task/SourceTask.h>

namespace Petrosian {

/**
 * @class PetrosianPhotometryTask
 * @brief
 *
 */
class PetrosianPhotometryTask : public SourceXtractor::SourceTask {

public:

  /**
   * @brief Destructor
   */
  virtual ~PetrosianPhotometryTask() = default;

  PetrosianPhotometryTask(unsigned m_instance, double mag_zeropoint, bool use_symmetry, const std::string &checkimage);

  void computeProperties(SourceXtractor::SourceInterface& source) const override;

private:
  unsigned m_instance;
  double m_mag_zeropoint;
  bool m_use_symmetry;
  std::string m_checkimage;
};  // End of PetrosianPhotometryTask class

}  // namespace Petrosian


#endif
