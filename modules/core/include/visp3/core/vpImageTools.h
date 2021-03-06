/****************************************************************************
 *
 * This file is part of the ViSP software.
 * Copyright (C) 2005 - 2017 by Inria. All rights reserved.
 *
 * This software is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * ("GPL") version 2 as published by the Free Software Foundation.
 * See the file LICENSE.txt at the root directory of this source
 * distribution for additional information about the GNU GPL.
 *
 * For using ViSP with software that can not be combined with the GNU
 * GPL, please contact Inria about acquiring a ViSP Professional
 * Edition License.
 *
 * See http://visp.inria.fr for more information.
 *
 * This software was developed at:
 * Inria Rennes - Bretagne Atlantique
 * Campus Universitaire de Beaulieu
 * 35042 Rennes Cedex
 * France
 *
 * If you have questions regarding the use of this file, please contact
 * Inria at visp@inria.fr
 *
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 * Description:
 * Image tools.
 *
 * Authors:
 * Fabien Spindler
 *
 *****************************************************************************/


#ifndef vpImageTools_H
#define vpImageTools_H

/*!
  \file vpImageTools.h

  \brief Various image tools; sub-image extraction, modification of
  the look up table, binarisation...

*/

#include <visp3/core/vpImage.h>

#ifdef VISP_HAVE_PTHREAD
#  include <pthread.h>
#endif

#include <visp3/core/vpImageException.h>
#include <visp3/core/vpMath.h>
#include <visp3/core/vpRect.h>
#include <visp3/core/vpCameraParameters.h>

#include <fstream>
#include <iostream>
#include <math.h>
#include <string.h>

/*!
  \class vpImageTools

  \ingroup group_core_image

  \brief Various image tools; sub-image extraction, modification of
  the look up table, binarisation...

*/
class VISP_EXPORT vpImageTools
{

public:
  template<class Type>
  static inline void binarise(vpImage<Type> &I,
                              Type threshold1, Type threshold2,
                              Type value1, Type value2, Type value3, const bool useLUT=true);
  static void changeLUT(vpImage<unsigned char>& I,
                        unsigned char A,
                        unsigned char newA,
                        unsigned char B,
                        unsigned char newB);

  template<class Type>
  static void crop(const vpImage<Type> &I,
                   double roi_top,  double roi_left,
                   int roi_height,  int roi_width,
                   vpImage<Type> &crop, unsigned int v_scale=1, unsigned int h_scale=1);

  template<class Type>
  static void crop(const vpImage<Type> &I,
                   const vpImagePoint &topLeft,
                   unsigned int roi_height, unsigned int roi_width,
                   vpImage<Type> &crop, unsigned int v_scale=1, unsigned int h_scale=1);
  template<class Type>
  static void crop(const vpImage<Type> &I, const vpRect &roi, vpImage<Type> &crop, unsigned int v_scale=1, unsigned int h_scale=1);
  template<class Type>
  static void crop(const unsigned char *bitmap, unsigned int width, unsigned int height, const vpRect &roi, vpImage<Type> &crop,
                   unsigned int v_scale=1, unsigned int h_scale=1);

  template<class Type>
  static void flip(const vpImage<Type> &I, vpImage<Type> &newI);

  template<class Type>
  static void flip(vpImage<Type> &I);


  static void imageDifference(const vpImage<unsigned char> &I1,
                              const vpImage<unsigned char> &I2,
                              vpImage<unsigned char> &Idiff);
  static void imageDifference(const vpImage<vpRGBa> &I1,
                              const vpImage<vpRGBa> &I2,
                              vpImage<vpRGBa> &Idiff);

  static void imageDifferenceAbsolute(const vpImage<unsigned char> &I1,
                                      const vpImage<unsigned char> &I2,
                                      vpImage<unsigned char> &Idiff);
  static void imageDifferenceAbsolute(const vpImage<vpRGBa> &I1,
                                      const vpImage<vpRGBa> &I2,
                                      vpImage<vpRGBa> &Idiff);

  static void imageAdd(const vpImage<unsigned char> &I1,
                       const vpImage<unsigned char> &I2,
                       vpImage<unsigned char> &Ires,
                       const bool saturate=false);

  static void imageSubtract(const vpImage<unsigned char> &I1,
                            const vpImage<unsigned char> &I2,
                            vpImage<unsigned char> &Ires,
                            const bool saturate=false);

  template<class Type>
  static void undistort(const vpImage<Type> &I,
                        const vpCameraParameters &cam,
                        vpImage<Type> &newI);

#if defined(VISP_BUILD_DEPRECATED_FUNCTIONS)
  /*!
    @name Deprecated functions
  */
  //@{
  template<class Type>
  vp_deprecated static void createSubImage(const vpImage<Type> &I,
                             unsigned int i_sub, unsigned int j_sub,
                             unsigned int nrow_sub, unsigned int ncol_sub,
                             vpImage<Type> &S);

  template<class Type>
  vp_deprecated static void createSubImage(const vpImage<Type> &I, const vpRect &rect, vpImage<Type> &S);
  //@}
#endif
} ;

#if defined(VISP_BUILD_DEPRECATED_FUNCTIONS)
/*!
  Crop a region of interest (ROI) in an image.

  \deprecated This fonction is deprecated. You should rather use
  crop(const vpImage<Type> &, unsigned int, unsigned int, unsigned int, unsigned int, vpImage<Type> &).

  \param I : Input image from which a sub image will be extracted.
  \param roi_top : ROI vertical position of the upper/left corner in the input image.
  \param roi_left : ROI  horizontal position of the upper/left corner in the input image.
  \param roi_height : Cropped image height corresponding to the ROI height.
  \param roi_width : Cropped image width corresponding to the ROI height.
  \param crop : Cropped image.

  \sa crop(const vpImage<Type> &, unsigned int, unsigned int, unsigned int, unsigned int, vpImage<Type> &)
*/

template<class Type>
void vpImageTools::createSubImage(const vpImage<Type> &I,
                                  unsigned int roi_top, unsigned int roi_left,
                                  unsigned int roi_height, unsigned int roi_width,
                                  vpImage<Type> &crop)
{
  vpImageTools::crop(I, roi_top, roi_left, roi_height, roi_width, crop);
}

/*!
  Crop an image region of interest.

  \deprecated This fonction is deprecated. You should rather use
  crop(const vpImage<Type> &, const vpRect &, vpImage<Type> &).

  \param I : Input image from which a sub image will be extracted.

  \param roi : Region of interest in image \e I corresponding to the
  cropped part of the image.

  \param crop : Cropped image.

  \sa crop(const vpImage<Type> &, const vpRect &, vpImage<Type> &)
*/
template<class Type>
void vpImageTools::createSubImage(const vpImage<Type> &I,
                                  const vpRect &roi,
                                  vpImage<Type> &crop)
{
  vpImageTools::crop(I, roi, crop);
}

#endif // #if defined(VISP_BUILD_DEPRECATED_FUNCTIONS)

/*!
  Crop a region of interest (ROI) in an image. The ROI coordinates and dimension are defined in the original image.

  Setting \e v_scale and \e h_scale to values different from 1 allows also to subsample the cropped image.

  \param I : Input image from which a sub image will be extracted.
  \param roi_top : ROI vertical position of the upper/left corner in the input image.
  \param roi_left : ROI  horizontal position of the upper/left corner in the input image.
  \param roi_height : Cropped image height corresponding to the ROI height.
  \param roi_width : Cropped image width corresponding to the ROI height.
  \param crop : Cropped image.
  \param v_scale [in] : Vertical subsampling factor applied to the ROI.
  \param h_scale [in] : Horizontal subsampling factor applied to the ROI.

  \sa crop(const vpImage<Type> &, const vpRect &, vpImage<Type> &)

*/
template<class Type>
void vpImageTools::crop(const vpImage<Type> &I,
                        double roi_top,  double roi_left,
                        int roi_height,  int roi_width,
                        vpImage<Type> &crop, unsigned int v_scale, unsigned int h_scale)
{
  int i_min = std::max((int)(ceil(roi_top/v_scale)), 0);
  int j_min = std::max((int)(ceil(roi_left/h_scale)), 0);
  int i_max = std::min((int)(ceil((roi_top + roi_height))/v_scale), (int)(I.getHeight()/v_scale));
  int j_max = std::min((int)(ceil((roi_left + roi_width)/h_scale)), (int)(I.getWidth()/h_scale));

  unsigned int r_width  = (unsigned int)(j_max-j_min);
  unsigned int r_height = (unsigned int)(i_max-i_min);

  crop.resize(r_height, r_width) ;

  if (v_scale == 1 && h_scale == 1) {
    for (unsigned int i=0 ; i < r_height ; i++) {
      void *src = (void *)(I[i+i_min] + j_min);
      void *dst = (void *)crop[i];
      memcpy(dst, src, r_width*sizeof(Type));
    }
  }
  else if (h_scale == 1) {
    for (unsigned int i=0 ; i < r_height ; i++) {
      void *src = (void *)(I[(i + i_min)*v_scale]+j_min);
      void *dst = (void *)crop[i];
      memcpy(dst, src, r_width*sizeof(Type));
    }
  }
  else {
    for (unsigned int i=0 ; i < r_height ; i++) {
      for (unsigned int j=0 ; j < r_width ; j++) {
        crop[i][j] = I[(i + i_min)*v_scale][(j + j_min)*h_scale];
      }
    }
  }
}

/*!
  Crop a region of interest (ROI) in an image. The ROI coordinates and dimension are defined in the original image.

  Setting \e v_scale and \e h_scale to values different from 1 allows also to subsample the cropped image.

  \param I : Input image from which a sub image will be extracted.
  \param topLeft : ROI position of the upper/left corner in the input image.
  \param roi_height : Cropped image height corresponding to the ROI height.
  \param roi_width : Cropped image width corresponding to the ROI height.
  \param crop : Cropped image.
  \param v_scale [in] : Vertical subsampling factor applied to the ROI.
  \param h_scale [in] : Horizontal subsampling factor applied to the ROI.

  \sa crop(const vpImage<Type> &, const vpRect &, vpImage<Type> &)

*/
template<class Type>
void vpImageTools::crop(const vpImage<Type> &I,
                        const vpImagePoint &topLeft,
                        unsigned int roi_height, unsigned int roi_width,
                        vpImage<Type> &crop, unsigned int v_scale, unsigned int h_scale)
{
  vpImageTools::crop(I, topLeft.get_i(), topLeft.get_j(), roi_height, roi_width, crop, v_scale, h_scale);
}

/*!
  Crop a region of interest (ROI) in an image. The ROI coordinates and dimension are defined in the original image.

  Setting \e v_scale and \e h_scale to values different from 1 allows also to subsample the cropped image.

  \param I : Input image from which a sub image will be extracted.

  \param roi : Region of interest in image \e I corresponding to the
  cropped part of the image.

  \param crop : Cropped image.
  \param v_scale [in] : Vertical subsampling factor applied to the ROI.
  \param h_scale [in] : Horizontal subsampling factor applied to the ROI.

*/
template<class Type>
void vpImageTools::crop(const vpImage<Type> &I, const vpRect &roi, vpImage<Type> &crop, unsigned int v_scale, unsigned int h_scale)
{
  vpImageTools::crop(I, roi.getTop(), roi.getLeft(), (unsigned int)roi.getHeight(), (unsigned int)roi.getWidth(), crop, v_scale, h_scale);
}

/*!
  Crop a region of interest (ROI) in an image. The ROI coordinates and dimension are defined in the original image.

  Setting \e v_scale and \e h_scale to values different from 1 allows also to subsample the cropped image.

  \param bitmap : Pointer to the input image from which a sub image will be extracted.
  \param width, height : Size of the input image.

  \param roi : Region of interest corresponding to the cropped part of the image.

  \param crop : Cropped image.
  \param v_scale [in] : Vertical subsampling factor applied to the ROI.
  \param h_scale [in] : Horizontal subsampling factor applied to the ROI.
*/
template<class Type>
void vpImageTools::crop(const unsigned char *bitmap, unsigned int width, unsigned int height, const vpRect &roi, vpImage<Type> &crop,
                        unsigned int v_scale, unsigned int h_scale)
{
  int i_min = std::max((int)(ceil(roi.getTop()/v_scale)), 0);
  int j_min = std::max((int)(ceil(roi.getLeft()/h_scale)), 0);
  int i_max = std::min((int)(ceil((roi.getTop() + roi.getHeight()))/v_scale), (int)(height/v_scale));
  int j_max = std::min((int)(ceil((roi.getLeft() + roi.getWidth())/h_scale)), (int)(width/h_scale));

  unsigned int r_width  = (unsigned int)(j_max-j_min);
  unsigned int r_height = (unsigned int)(i_max-i_min);

  crop.resize(r_height, r_width) ;

  if (v_scale == 1 && h_scale == 1) {
    for (unsigned int i=0 ; i < r_height ; i++) {
      void *src = (void *)(bitmap + ( (i+i_min)*width + j_min ) * sizeof(Type));
      void *dst = (void *)crop[i];
      memcpy(dst, src, r_width*sizeof(Type));
    }
  }
  else if (h_scale == 1) {
    for (unsigned int i=0 ; i < r_height ; i++) {
      void *src = (void *)(bitmap + ( (i+i_min)*width*v_scale + j_min ) * sizeof(Type) );
      void *dst = (void *)crop[i];
      memcpy(dst, src, r_width*sizeof(Type));
    }
  }
  else {
    for (unsigned int i=0 ; i < r_height ; i++) {
      unsigned int i_src = (i+i_min)*width*v_scale + j_min*h_scale;
      for (unsigned int j=0 ; j < r_width ; j++) {
        void *src = (void *)(bitmap + (i_src + j*h_scale)*sizeof(Type));
        void *dst = (void *)&crop[i][j];
        memcpy(dst, src, sizeof(Type));
      }
    }
  }
}

/*!

  Binarise an image.

  - Pixels whose values are less than \e threshold1 are set to \e value1

  - Pixels whose values are greater then or equal to \e threshold1 and
    less then or equal to \e threshold2 are set to \e value2

  - Pixels whose values are greater than \e threshold2 are set to \e value3

*/
template<class Type>
inline void vpImageTools::binarise(vpImage<Type> &I,
                                   Type threshold1, Type threshold2,
                                   Type value1, Type value2, Type value3, const bool useLUT)
{
  if(useLUT) {
    std::cerr << "LUT not available for this type ! Will use the iteration method." << std::endl;
  }

  Type v;
  Type *p = I.bitmap;
  Type *pend = I.bitmap + I.getWidth()*I.getHeight();
  for (; p < pend; p ++) {
    v = *p;
    if (v < threshold1) *p = value1;
    else if (v > threshold2) *p = value3;
    else *p = value2;
  }
}

/*!

  Binarise an image.

  - Pixels whose values are less than \e threshold1 are set to \e value1

  - Pixels whose values are greater then or equal to \e threshold1 and
    less then or equal to \e threshold2 are set to \e value2

  - Pixels whose values are greater than \e threshold2 are set to \e value3

*/
template<>
inline void vpImageTools::binarise(vpImage<unsigned char> &I,
                                   unsigned char threshold1, unsigned char threshold2,
                                   unsigned char value1, unsigned char value2, unsigned char value3, const bool useLUT)
{
  if(useLUT) {
    //Construct the LUT
    unsigned char lut[256];
    for(unsigned int i = 0; i < 256; i++) {
      lut[i] = i < threshold1 ? value1 : (i > threshold2 ? value3 : value2);
    }

    I.performLut(lut);
  } else {
    unsigned char v;
    unsigned char *p = I.bitmap;
    unsigned char *pend = I.bitmap + I.getWidth()*I.getHeight();
    for (; p < pend; p ++) {
      v = *p;
      if (v < threshold1) *p = value1;
      else if (v > threshold2) *p = value3;
      else *p = value2;
    }
  }
}

#ifdef VISP_HAVE_PTHREAD

#ifndef DOXYGEN_SHOULD_SKIP_THIS
template<class Type>
class vpUndistortInternalType
{
public:
  Type *src;
  Type *dst;
  unsigned int width;
  unsigned int height;
  vpCameraParameters cam;
  unsigned int nthreads;
  unsigned int threadid;
public:
  vpUndistortInternalType()
    : src(NULL), dst(NULL), width(0), height(0), cam(), nthreads(0), threadid(0)
  {};

  vpUndistortInternalType(const vpUndistortInternalType<Type> &u) {
    *this = u;
  };
  vpUndistortInternalType &operator=(const vpUndistortInternalType<Type> &u) {
    src = u.src;
    dst = u.dst;
    width = u.width;
    height = u.height;
    cam = u.cam;
    nthreads = u.nthreads;
    threadid = u.threadid;

    return *this;
  }

  static void *vpUndistort_threaded(void *arg);
};


template<class Type>
void *vpUndistortInternalType<Type>::vpUndistort_threaded(void *arg)
{
  vpUndistortInternalType<Type> *undistortSharedData = (vpUndistortInternalType<Type>*)arg;
  int offset   = (int)undistortSharedData->threadid;
  int width    = (int)undistortSharedData->width;
  int height   = (int)undistortSharedData->height;
  int nthreads = (int)undistortSharedData->nthreads;

  double u0 = undistortSharedData->cam.get_u0();
  double v0 = undistortSharedData->cam.get_v0();
  double px = undistortSharedData->cam.get_px();
  double py = undistortSharedData->cam.get_py();
  double kud = undistortSharedData->cam.get_kud();

  double invpx = 1.0/px;
  double invpy = 1.0/py;

  double kud_px2 = kud * invpx * invpx;
  double kud_py2 = kud * invpy * invpy;

  Type *dst = undistortSharedData->dst+(height/nthreads*offset)*width;
  Type *src = undistortSharedData->src;

  for (double v = height/nthreads*offset;v < height/nthreads*(offset+1) ; v++) {
    double  deltav  = v - v0;
    //double fr1 = 1.0 + kd * (vpMath::sqr(deltav * invpy));
    double fr1 = 1.0 + kud_py2 * deltav * deltav;

    for (double u = 0 ; u < width ; u++) {
      //computation of u,v : corresponding pixel coordinates in I.
      double  deltau  = u - u0;
      //double fr2 = fr1 + kd * (vpMath::sqr(deltau * invpx));
      double fr2 = fr1 + kud_px2 * deltau * deltau;

      double u_double = deltau * fr2 + u0;
      double v_double = deltav * fr2 + v0;

      //computation of the bilinear interpolation

      //declarations
      int u_round  = (int) (u_double);
      int v_round  = (int) (v_double);
      if (u_round < 0.f) u_round = -1;
      if (v_round < 0.f) v_round = -1;
      double  du_double  = (u_double) - (double) u_round;
      double  dv_double  = (v_double) - (double) v_round;
      Type v01;
      Type v23;
      if ( (0 <= u_round) && (0 <= v_round) &&
           (u_round < ((width) - 1)) && (v_round < ((height) - 1)) ) {
        //process interpolation
        const Type* _mp = &src[v_round*width+u_round];
        v01 = (Type)(_mp[0] + ((_mp[1] - _mp[0]) * du_double));
        _mp += width;
        v23 = (Type)(_mp[0] + ((_mp[1] - _mp[0]) * du_double));
        *dst = (Type)(v01 + ((v23 - v01) * dv_double));
      }
      else {
        *dst = 0;
      }
      dst++;
    }
  }

  pthread_exit((void*) 0);
  return NULL;
}
#endif // DOXYGEN_SHOULD_SKIP_THIS
#endif // VISP_HAVE_PTHREAD

/*!
  Undistort an image

  \param I : Input image to undistort.

  \param cam : Parameters of the camera causing distortion.

  \param undistI : Undistorted output image. The size of this image
  will be the same than the input image \e I. If the distortion
  parameter \f$K_d\f$ is null (see cam.get_kd_mp()), \e undistI is
  just a copy of \e I.

  \warning This function works only with Types authorizing "+,-,
  multiplication by a scalar" operators.

  \warning This function is time consuming :
    - On "Rhea"(Intel Core 2 Extreme X6800 2.93GHz, 2Go RAM)
      or "Charon"(Intel Xeon 3 GHz, 2Go RAM) : ~8 ms for a 640x480 image.
*/
template<class Type>
void vpImageTools::undistort(const vpImage<Type> &I,
                             const vpCameraParameters &cam,
                             vpImage<Type> &undistI)
{
#ifdef VISP_HAVE_PTHREAD
  //
  // Optimized version using pthreads
  //
  unsigned int width = I.getWidth();
  unsigned int height = I.getHeight();

  undistI.resize(height, width);

  double kud = cam.get_kud();

  //if (kud == 0) {
  if (std::fabs(kud) <= std::numeric_limits<double>::epsilon()) {
    // There is no need to undistort the image
    undistI = I;
    return;
  }

  unsigned int nthreads = 2;
  pthread_attr_t attr;
  pthread_t *callThd = new pthread_t [nthreads];
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

  vpUndistortInternalType<Type> *undistortSharedData;
  undistortSharedData = new vpUndistortInternalType<Type> [nthreads];

  for(unsigned int i=0;i<nthreads;i++) {
    // Each thread works on a different set of data.
    //    vpTRACE("create thread %d", i);
    undistortSharedData[i].src      = I.bitmap;
    undistortSharedData[i].dst      = undistI.bitmap;
    undistortSharedData[i].width    = I.getWidth();
    undistortSharedData[i].height   = I.getHeight();
    undistortSharedData[i].cam      = cam;
    undistortSharedData[i].nthreads = nthreads;
    undistortSharedData[i].threadid = i;
    pthread_create( &callThd[i], &attr,
                    &vpUndistortInternalType<Type>::vpUndistort_threaded,
                    &undistortSharedData[i]);
  }
  pthread_attr_destroy(&attr);
  /* Wait on the other threads */

  for(unsigned int i=0;i<nthreads;i++) {
    //  vpTRACE("join thread %d", i);
    pthread_join( callThd[i], NULL);
  }

  delete [] callThd;
  delete [] undistortSharedData;
#else // VISP_HAVE_PTHREAD
  //
  // optimized version without pthreads
  //
  unsigned int width = I.getWidth();
  unsigned int height = I.getHeight();

  undistI.resize(height, width);

  double u0 = cam.get_u0();
  double v0 = cam.get_v0();
  double px = cam.get_px();
  double py = cam.get_py();
  double kud = cam.get_kud();

  //if (kud == 0) {
  if (std::fabs(kud) <= std::numeric_limits<double>::epsilon()) {
    // There is no need to undistort the image
    undistI = I;
    return;
  }

  double invpx = 1.0/px;
  double invpy = 1.0/py;

  double kud_px2 = kud * invpx * invpx;
  double kud_py2 = kud * invpy * invpy;

  Type *dst = undistI.bitmap;
  for (double v = 0;v < height ; v++) {
    double  deltav  = v - v0;
    //double fr1 = 1.0 + kd * (vpMath::sqr(deltav * invpy));
    double fr1 = 1.0 + kud_py2 * deltav * deltav;

    for (double u = 0 ; u < width ; u++) {
      //computation of u,v : corresponding pixel coordinates in I.
      double  deltau  = u - u0;
      //double fr2 = fr1 + kd * (vpMath::sqr(deltau * invpx));
      double fr2 = fr1 + kud_px2 * deltau * deltau;

      double u_double = deltau * fr2 + u0;
      double v_double = deltav * fr2 + v0;

      //printf("[%g][%g] %g %g : ", u, v, u_double, v_double );

      //computation of the bilinear interpolation

      //declarations
      int u_round  = (int) (u_double);
      int v_round  = (int) (v_double);
      if (u_round < 0.f) u_round = -1;
      if (v_round < 0.f) v_round = -1;
      double  du_double  = (u_double) - (double) u_round;
      double  dv_double  = (v_double) - (double) v_round;
      Type  v01;
      Type  v23;
      if ( (0 <= u_round) && (0 <= v_round) &&
           (u_round < (((int)width) - 1)) && (v_round < (((int)height) - 1)) ) {
        //process interpolation
        const Type* _mp = &I[(unsigned int)v_round][(unsigned int)u_round];
        v01 = (Type)(_mp[0] + ((_mp[1] - _mp[0]) * du_double));
        _mp += width;
        v23 = (Type)(_mp[0] + ((_mp[1] - _mp[0]) * du_double));
        *dst = (Type)(v01 + ((v23 - v01) * dv_double));
        //printf("R %d G %d B %d\n", dst->R, dst->G, dst->B);
      }
      else {
        *dst = 0;
      }
      dst++;
    }
  }
#endif // VISP_HAVE_PTHREAD




#if 0
  // non optimized version
  int width = I.getWidth();
  int height = I.getHeight();

  undistI.resize(height,width);

  double u0 = cam.get_u0();
  double v0 = cam.get_v0();
  double px = cam.get_px();
  double py = cam.get_py();
  double kd = cam.get_kud();

  if (kd == 0) {
    // There is no need to undistort the image
    undistI = I;
    return;
  }

  for(int v = 0 ; v < height; v++){
    for(int u = 0; u < height; u++){
      double r2 = vpMath::sqr(((double)u - u0)/px) +
                  vpMath::sqr(((double)v-v0)/py);
      double u_double = ((double)u - u0)*(1.0+kd*r2) + u0;
      double v_double = ((double)v - v0)*(1.0+kd*r2) + v0;
      undistI[v][u] = I.getPixelBI((float)u_double,(float)v_double);
    }
  }
#endif
}

/*!
  Flip vertically the input image and give the result in the output image.

  \param I : Input image to flip.
  \param newI : Output image which is the flipped input image.
*/

template<class Type>
void vpImageTools::flip(const vpImage<Type> &I,
                        vpImage<Type> &newI)
{
    unsigned int height = 0, width = 0;

    height = I.getHeight();
    width = I.getWidth();
    newI.resize(height, width);

    for (unsigned int i = 0; i < height; i++)
    {
      memcpy(newI.bitmap+i*width, I.bitmap+(height-1-i)*width,
             width*sizeof(Type));
    }
}


/*!
  Flip vertically the input image.

  \param I : Input image which is flipped and modified in output.

  The following example shows how to use this function:
  \code
#include <visp3/core/vpImageTools.h>
#include <visp3/core/vpImage.h>
#include <visp3/io/vpImageIo.h>

int main()
{
  vpImage<vpRGBa> I;
#ifdef _WIN32
  std::string filename("C:/temp/ViSP-images/Klimt/Klimt.ppm");
#else
  std::string filename("/local/soft/ViSP/ViSP-images/Klimt/Klimt.ppm");
#endif

  // Read an image from the disk
  vpImageIo::read(I, filename);

  // Flip the image
  vpImageTools::flip(I);

  vpImageIo::write(I, "Klimt-flip.ppm"); // Write the image in a PGM P5 image file format
}
  \endcode
*/
template<class Type>
void vpImageTools::flip(vpImage<Type> &I)
{
    unsigned int height = 0, width = 0;
    unsigned int i = 0;
    vpImage<Type> Ibuf;

    height = I.getHeight();
    width = I.getWidth();
    Ibuf.resize(1, width);

    for ( i = 0; i < height/2; i++)
    {
      memcpy(Ibuf.bitmap, I.bitmap+i*width,
             width*sizeof(Type));

      memcpy(I.bitmap+i*width, I.bitmap+(height-1-i)*width,
             width*sizeof(Type));
      memcpy(I.bitmap+(height-1-i)*width, Ibuf.bitmap,
             width*sizeof(Type));
    }
}

#endif


/*
 * Local variables:
 * c-basic-offset: 2
 * End:
 */
