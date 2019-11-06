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
#include <boost/filesystem/path.hpp>

namespace Petrosian {

/**
 * @class PetrosianPhotometryTask
 * @brief
 *  This class measures the photometry of a source on a given frame.
 * @details
 *  It inherits from SourceXtractor::SourceTask because it is a per-source property.
 *  For measurements that work on source groups - like the Model Fitting does -,
 *  it would have to inherit from SourceXtractor::GroupTask
 * @see
 *  SourceXtractor::GroupTask
 */
class PetrosianPhotometryTask : public SourceXtractor::SourceTask {

public:

  /**
   * Default destructor
   */
  virtual ~PetrosianPhotometryTask() = default;

  /**
   * Constructor
   * @param m_instance
   *    Since there is one property per source per image, this parameter identifies
   *    the frame on which this task is working
   * @param mag_zeropoint
   *    Magnitude zeropoint
   * @param use_symmetry
   *    Use symmetric pixels to cover for bad/masked out pixels
   * @param checkimage
   *    Optional path for a check image, so we can generate an image with the apertures being used
   */
  PetrosianPhotometryTask(unsigned m_instance, double mag_zeropoint, bool use_symmetry,
                          const boost::filesystem::path& checkimage);

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
  unsigned m_instance;
  double m_mag_zeropoint;
  bool m_use_symmetry;
  boost::filesystem::path m_checkimage;
};  // End of PetrosianPhotometryTask class

}  // namespace Petrosian


#endif
