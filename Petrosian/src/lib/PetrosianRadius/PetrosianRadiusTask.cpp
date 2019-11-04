/**
 * @file src/lib/PetrosianRadius/PetrosianRadiusTask.cpp
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

#include "Petrosian/PetrosianRadius/PetrosianRadius.h"
#include "Petrosian/PetrosianRadius/PetrosianRadiusTask.h"

#include <SEFramework/Property/DetectionFrame.h>
#include <SEFramework/Aperture/EllipticalAperture.h>
#include <SEFramework/Aperture/NeighbourInfo.h>

#include <SEImplementation/Measurement/MultithreadedMeasurement.h>
#include <SEImplementation/Plugin/PixelCentroid/PixelCentroid.h>
#include <SEImplementation/Plugin/ShapeParameters/ShapeParameters.h>
#include <SEImplementation/Property/PixelCoordinateList.h>

namespace Petrosian {

PetrosianRadiusTask::PetrosianRadiusTask(double eta, double factor, double minrad)
  : m_eta(eta), m_factor(factor), m_minrad(minrad) {}

static const double PETRO_NSIG = 6.;

void PetrosianRadiusTask::computeProperties(SourceXtractor::SourceInterface& source) const {
  std::lock_guard<std::recursive_mutex> lock(SourceXtractor::MultithreadedMeasurement::g_global_mutex);

  // get the detection frame
  const auto& detection_frame = source.getProperty<SourceXtractor::DetectionFrame>().getFrame();

  // get the images and image information from the frame
  const auto& detection_image = detection_frame->getSubtractedImage();
  const auto& detection_variance = detection_frame->getVarianceMap();
  const auto& variance_threshold = detection_frame->getVarianceThreshold();
  const auto& threshold_image = detection_frame->getThresholdedImage();

  // get the object center
  const auto& centroid_x = source.getProperty<SourceXtractor::PixelCentroid>().getCentroidX();
  const auto& centroid_y = source.getProperty<SourceXtractor::PixelCentroid>().getCentroidY();

  // get the shape parameters
  const auto& shape_parameters = source.getProperty<SourceXtractor::ShapeParameters>();
  const auto& cxx = shape_parameters.getEllipseCxx();
  const auto& cyy = shape_parameters.getEllipseCyy();
  const auto& cxy = shape_parameters.getEllipseCxy();

  // create the elliptical aperture
  auto ell_aper = std::make_shared<SourceXtractor::EllipticalAperture>(cxx, cyy, cxy, PETRO_NSIG);

  // get the aperture borders on the image
  const auto& min_pixel = ell_aper->getMinPixel(centroid_x, centroid_y);
  const auto& max_pixel = ell_aper->getMaxPixel(centroid_x, centroid_y);

  // get the pixel list
  const auto& pix_list = source.getProperty<SourceXtractor::PixelCoordinateList>().getCoordinateList();

  // get the neighbourhood information
  SourceXtractor::NeighbourInfo neighbour_info(min_pixel, max_pixel, pix_list, threshold_image);

  // Look for the Petrosian radius
  double step_size = PETRO_NSIG / 20.;
  double kmin, kmax, kmean = 0.;

  for (kmin = step_size; (kmax = kmin * 1.2) < PETRO_NSIG; kmin += step_size) {
    kmean = (kmin + kmax) / 2.0;

    double kmin2 = kmin * kmin;
    double kmean2 = kmean * kmean;
    double kmax2 = kmax * kmax;

    double flux_outer = 0., flux_inner = 0.;
    double area_outer = 0., area_inner = 0.;

    for (int y = min_pixel.m_y; y < max_pixel.m_y; ++y) {
      for (int x = min_pixel.m_x; x < max_pixel.m_x; ++x) {

        if (x >= 0 && y >= 0 && x < detection_image->getWidth() && y < detection_image->getHeight()) {
          double pixel_variance = detection_variance ? detection_variance->getValue(x, y) : 1;
          double pixel_value = 0.;

          if (pixel_variance < variance_threshold)
            pixel_value = detection_image->getValue(x, y);

          double r2 = ell_aper->getRadiusSquared(centroid_x, centroid_y, x, y);

          if (r2 <= kmax2) {
            if (r2 >= kmin2) {
              flux_outer += pixel_value;
              ++area_outer;
            }
            else if (r2 < kmean2) {
              flux_inner += pixel_value;
              ++area_inner;
            }
          }
        }
      }
    }

    if (area_inner && area_outer) {
      flux_outer /= area_outer;
      flux_inner /= area_inner;
      if (flux_outer < m_eta * flux_inner) {
        break;
      }
    }
  }

  double radius = std::max(kmean * m_factor, m_minrad);
  source.setProperty<PetrosianRadius>(radius);
}

}  // namespace Petrosian
