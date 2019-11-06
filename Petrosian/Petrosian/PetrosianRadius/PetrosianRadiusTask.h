/**
 * @file Petrosian/PetrosianRadius/PetrosianRadiusTask.h
 * @date 11/04/19
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

#ifndef _PETROSIAN_PETROSIANRADIUS_PETROSIANRADIUSTASK_H
#define _PETROSIAN_PETROSIANRADIUS_PETROSIANRADIUSTASK_H

#include <SEFramework/Task/SourceTask.h>

namespace Petrosian {

/**
 * @class PetrosianRadiusTask
 *  This is the class responsible for computing the Petrosian radius.
 * @details
 *  It inherits from SourceXtractor::SourceTask because it is a per-source property.
 *  For measurements that work on source groups - like the Model Fitting does -,
 *  it would have to inherit from SourceXtractor::GroupTask
 * @see
 *  SourceXtractor::GroupTask
 *
 * @see
 *  https://sextractor.readthedocs.io/en/latest/Photom.html#petrosian-aperture-flux-flux-petro
 */
class PetrosianRadiusTask : public SourceXtractor::SourceTask {

public:

  /**
   * Default destructor
   */
  virtual ~PetrosianRadiusTask() = default;

  /**
   * Constructor. This is called by the task factory, which bridge the configuration system and the task
   * creation
   * @param eta
   *    η
   * @param factor
   *    \f$N_{\rm P}\f$
   * @param minrad
   *    Minimum radius
   */
  PetrosianRadiusTask(double eta, double factor, double minrad);

  /**
   * @brief
   *    Compute the corresponding property. Multiple properties could be computed at once.
   * @param source
   *    The source for which to compute the property. If this were an implementation of
   *    SourceXtractor::GroupTask, it would receive an instance of
   *    SourceXtractor::SourceGroupInterface instead
   */
  void computeProperties(SourceXtractor::SourceInterface& source) const override;

private:
  double m_eta, m_factor, m_minrad;
};  // End of PetrosianRadiusTask class

}  // namespace Petrosian


#endif
