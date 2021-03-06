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
 * Various image tools, convolution, ...
 *
 * Authors:
 * Eric Marchand
 *
 *****************************************************************************/

#include <visp3/core/vpImageFilter.h>
#include <visp3/core/vpImageConvert.h>
#if defined(VISP_HAVE_OPENCV) && (VISP_HAVE_OPENCV_VERSION >= 0x020408)
#  include <opencv2/imgproc/imgproc.hpp>
#elif defined(VISP_HAVE_OPENCV) && (VISP_HAVE_OPENCV_VERSION >= 0x020101)
#  include <opencv2/imgproc/imgproc_c.h>
#elif defined(VISP_HAVE_OPENCV)
#  include <cv.h>
#endif

/*!
  Apply a filter to an image.

  \param I : Image to filter
  \param If : Filtered image.
  \param M : Filter coefficients.

*/
void
vpImageFilter::filter(const vpImage<unsigned char> &I,
		      vpImage<double>& If,
		      const vpMatrix& M)
{

  unsigned int size = M.getRows() ;
  unsigned int half_size = size/2 ;

  If.resize(I.getHeight(),I.getWidth()) ;

  If = 0 ;

  for (unsigned int i=half_size ; i < I.getHeight()-half_size ; i++)
  {
    for (unsigned int j=half_size ; j < I.getWidth()-half_size ; j++)
    {
      double   conv_x = 0 ;

      for(unsigned int a = 0 ; a < size ; a++ )
        for(unsigned int b = 0 ; b < size ; b++ )
	{
	  double val =  I[i-half_size+a][j-half_size+b] ;
	  conv_x += M[a][b] * val ;
	}
      If[i][j] = conv_x ;
    }
  }

}

/*!
  Apply a filter to an image.

  \param I : Image to filter
  \param Iu : Filtered image along the horizontal axis (u = columns).
  \param Iv : Filtered image along the vertical axis (v = rows).
  \param M : Separate filter coefficients

*/
void
vpImageFilter::filter(const vpImage<double> &I,
		      vpImage<double>& Iu,
		      vpImage<double>& Iv,
		      const vpMatrix& M)
{

  unsigned int size = M.getRows() ;
  unsigned int half_size = size/2 ;

  Iu.resize(I.getHeight(),I.getWidth()) ;
  Iv.resize(I.getHeight(),I.getWidth()) ;

  Iu = 0 ;
  Iv = 0 ;
  for (unsigned int v=half_size ; v < I.getHeight()-half_size ; v++)
  {
    for (unsigned int u=half_size ; u < I.getWidth()-half_size ; u++)
    {
      double   conv_u = 0 ;
      double   conv_v = 0 ;

      for(unsigned int a = 0 ; a < size ; a++ )
        for(unsigned int b = 0 ; b < size ; b++ )
	{
	  double val =  I[v-half_size+a][u-half_size+b] ;
	  conv_u += M[a][b] * val ;
	  conv_v += M[b][a] * val  ;
	}
      Iu[v][u] = conv_u ;
      Iv[v][u] = conv_v ;
    }
  }

}

#if defined(VISP_HAVE_OPENCV) && (VISP_HAVE_OPENCV_VERSION >= 0x020100)
/*!
  Apply the Canny edge operator on the image \e Isrc and return the resulting
  image \e Ires.

  The following example shows how to use the method:

  \code
#include <visp3/core/vpConfig.h>
#include <visp3/core/vpImage.h>
#include <visp3/core/vpImageFilter.h>

int main()
{
#if VISP_HAVE_OPENCV_VERSION >= 0x020100 // Canny uses OpenCV >=2.1.0
  // Constants for the Canny operator.
  const unsigned int gaussianFilterSize = 5;
  const double thresholdCanny = 15;
  const unsigned int apertureSobel = 3;

  // Image for the Canny edge operator
  vpImage<unsigned char> Isrc;
  vpImage<unsigned char> Icanny;

  //First grab the source image Isrc.

  //Apply the Canny edge operator and set the Icanny image.
  vpImageFilter::canny(Isrc, Icanny, gaussianFilterSize, thresholdCanny, apertureSobel);
#endif
  return (0);
}
  \endcode

  \param Isrc : Image to apply the Canny edge detector to.
  \param Ires : Filtered image (255 means an edge, 0 otherwise).
  \param gaussianFilterSize : The size of the mask of the Gaussian filter to
  apply (an odd number).
  \param thresholdCanny : The threshold for the Canny operator. Only value
  greater than this value are marked as an edge).
  \param apertureSobel : Size of the mask for the Sobel operator (odd number).
*/
void
vpImageFilter:: canny(const vpImage<unsigned char>& Isrc,
                      vpImage<unsigned char>& Ires,
                      const unsigned int gaussianFilterSize,
                      const double thresholdCanny,
                      const unsigned int apertureSobel)
{
#if (VISP_HAVE_OPENCV_VERSION < 0x020408)
  IplImage* img_ipl = NULL;
  vpImageConvert::convert(Isrc, img_ipl);
  IplImage* edges_ipl;
  edges_ipl = cvCreateImage(cvSize(img_ipl->width, img_ipl->height), img_ipl->depth, img_ipl->nChannels);

  cvSmooth(img_ipl, img_ipl, CV_GAUSSIAN, (int)gaussianFilterSize, (int)gaussianFilterSize, 0, 0);
  cvCanny(img_ipl, edges_ipl, thresholdCanny, thresholdCanny, (int)apertureSobel);

  vpImageConvert::convert(edges_ipl, Ires);
  cvReleaseImage(&img_ipl);
  cvReleaseImage(&edges_ipl);
#else
  cv::Mat img_cvmat, edges_cvmat;
  vpImageConvert::convert(Isrc, img_cvmat);
  cv::GaussianBlur(img_cvmat, img_cvmat, cv::Size((int)gaussianFilterSize, (int)gaussianFilterSize), 0, 0);
  cv::Canny(img_cvmat, edges_cvmat, thresholdCanny, thresholdCanny, (int)apertureSobel);
  vpImageConvert::convert(edges_cvmat, Ires);
#endif
}
#endif

/*!
  Apply a separable filter.
 */
void vpImageFilter::filter(const vpImage<unsigned char> &I, vpImage<double>& GI, const double *filter,unsigned  int size)
{
  vpImage<double> GIx ;
  filterX(I, GIx,filter,size);
  filterY(GIx, GI,filter,size);
  GIx.destroy();
}

/*!
  Apply a separable filter.
 */
void vpImageFilter::filter(const vpImage<double> &I, vpImage<double>& GI, const double *filter,unsigned  int size)
{
  vpImage<double> GIx ;
  filterX(I, GIx,filter,size);
  filterY(GIx, GI,filter,size);
  GIx.destroy();
}

void vpImageFilter::filterX(const vpImage<unsigned char> &I, vpImage<double>& dIx, const double *filter,unsigned  int size)
{
  dIx.resize(I.getHeight(),I.getWidth()) ;
  for (unsigned int i=0 ; i < I.getHeight() ; i++)
  {
    for (unsigned int j=0 ; j < (size-1)/2 ; j++)
    {
      dIx[i][j]=vpImageFilter::filterXLeftBorder(I,i,j,filter,size);
      //dIx[i][j]=0;
    }
    for (unsigned int j=(size-1)/2 ; j < I.getWidth()-(size-1)/2 ; j++)
    {
      dIx[i][j]=vpImageFilter::filterX(I,i,j,filter,size);
    }
    for (unsigned int j=I.getWidth()-(size-1)/2 ; j < I.getWidth() ; j++)
    {
      dIx[i][j]=vpImageFilter::filterXRightBorder(I,i,j,filter,size);
      //dIx[i][j]=0;
    }
  }
}
void vpImageFilter::filterX(const vpImage<double> &I, vpImage<double>& dIx, const double *filter,unsigned  int size)
{
  dIx.resize(I.getHeight(),I.getWidth()) ;
  for (unsigned int i=0 ; i < I.getHeight() ; i++)
  {
    for (unsigned int j=0 ; j < (size-1)/2 ; j++)
    {
      dIx[i][j]=vpImageFilter::filterXLeftBorder(I,i,j,filter,size);
      //dIx[i][j]=0;
    }
    for (unsigned int j=(size-1)/2 ; j < I.getWidth()-(size-1)/2 ; j++)
    {
      dIx[i][j]=vpImageFilter::filterX(I,i,j,filter,size);
    }
    for (unsigned int j=I.getWidth()-(size-1)/2 ; j < I.getWidth() ; j++)
    {
      dIx[i][j]=vpImageFilter::filterXRightBorder(I,i,j,filter,size);
      //dIx[i][j]=0;
    }
  }
}
void vpImageFilter::filterY(const vpImage<unsigned char> &I, vpImage<double>& dIy, const double *filter,unsigned  int size)
{
  dIy.resize(I.getHeight(),I.getWidth()) ;
  for (unsigned int i=0 ; i < (size-1)/2 ; i++)
  {
    for (unsigned int j=0 ; j < I.getWidth() ; j++)
    {
      dIy[i][j]=vpImageFilter::filterYTopBorder(I,i,j,filter,size);
    }
  }
  for (unsigned int i=(size-1)/2 ; i < I.getHeight()-(size-1)/2 ; i++)
  {
    for (unsigned int j=0 ; j < I.getWidth() ; j++)
    {
      dIy[i][j]=vpImageFilter::filterY(I,i,j,filter,size);
    }
  }
  for (unsigned int i=I.getHeight()-(size-1)/2 ; i < I.getHeight() ; i++)
  {
    for (unsigned int j=0 ; j < I.getWidth() ; j++)
    {
      dIy[i][j]=vpImageFilter::filterYBottomBorder(I,i,j,filter,size);
    }
  }
}
void vpImageFilter::filterY(const vpImage<double> &I, vpImage<double>& dIy, const double *filter,unsigned  int size)
{
  dIy.resize(I.getHeight(),I.getWidth()) ;
  for (unsigned int i=0 ; i < (size-1)/2 ; i++)
  {
    for (unsigned int j=0 ; j < I.getWidth() ; j++)
    {
      dIy[i][j]=vpImageFilter::filterYTopBorder(I,i,j,filter,size);
    }
  }
  for (unsigned int i=(size-1)/2 ; i < I.getHeight()-(size-1)/2 ; i++)
  {
    for (unsigned int j=0 ; j < I.getWidth() ; j++)
    {
      dIy[i][j]=vpImageFilter::filterY(I,i,j,filter,size);
    }
  }
  for (unsigned int i=I.getHeight()-(size-1)/2 ; i < I.getHeight() ; i++)
  {
    for (unsigned int j=0 ; j < I.getWidth() ; j++)
    {
      dIy[i][j]=vpImageFilter::filterYBottomBorder(I,i,j,filter,size);
    }
  }
}

/*!
  Apply a Gaussian blur to an image.
  \param I : Input image.
  \param GI : Filtered image.
  \param size : Filter size. This value should be odd.
  \param sigma : Gaussian standard deviation. If it is equal to zero or negative, it is computed from filter size as sigma = (size-1)/6.
  \param normalize : Flag indicating whether to normalize the filter coefficients or not.

 */
void vpImageFilter::gaussianBlur(const vpImage<unsigned char> &I, vpImage<double>& GI, unsigned int size, double sigma, bool normalize)
{
  double *fg=new double[(size+1)/2] ;
  vpImageFilter::getGaussianKernel(fg, size, sigma, normalize) ;
  vpImage<double> GIx ;
  vpImageFilter::filterX(I, GIx,fg,size);
  vpImageFilter::filterY(GIx, GI,fg,size);
  GIx.destroy();
  delete[] fg;
}

/*!
  Apply a Gaussian blur to a double image.
  \param I : Input double image.
  \param GI : Filtered image.
  \param size : Filter size. This value should be odd.
  \param sigma : Gaussian standard deviation. If it is equal to zero or negative, it is computed from filter size as sigma = (size-1)/6.
  \param normalize : Flag indicating whether to normalize the filter coefficients or not.

 */
void vpImageFilter::gaussianBlur(const vpImage<double> &I, vpImage<double>& GI, unsigned int size, double sigma, bool normalize)
{
  double *fg=new double[(size+1)/2] ;
  vpImageFilter::getGaussianKernel(fg, size, sigma, normalize) ;
  vpImage<double> GIx ;
  vpImageFilter::filterX(I, GIx,fg,size);
  vpImageFilter::filterY(GIx, GI,fg,size);
  GIx.destroy();
  delete[] fg;
}

/*!
  Return the coefficients of a Gaussian filter.

  \param filter : Pointer to the filter kernel that should refer to a (size+1)/2 array.
  The first value refers to the central coefficient, the next one to the right coefficients. Left coefficients could be deduced by symmetry.
  \param size : Filter size. This value should be odd.
  \param sigma : Gaussian standard deviation. If it is equal to zero or negative, it is computed from filter size as sigma = (size-1)/6.
  \param normalize : Flag indicating whether to normalize the filter coefficients or not.
*/
void vpImageFilter::getGaussianKernel(double *filter, unsigned int size, double sigma, bool normalize)
{
  if (size%2 != 1)
    throw (vpImageException(vpImageException::incorrectInitializationError,
          "Bad Gaussian filter size"));

  if (sigma<= 0)
    sigma = (size-1)/6.0;

  int middle = (int)(size-1)/2;
  double sigma2 = vpMath::sqr(sigma);
  for( int i=0; i<= middle; i++)
  {
    filter[i] = (1./(sigma*sqrt(2.*M_PI)))*exp(-(i*i)/(2.*sigma2));
  }
  if (normalize) {
    //renormalization
    double sum=0;
    for(int i=1; i<=middle; i++)
    {
      sum += 2*filter[i] ;
    }
    sum += filter[0];

    for(int i=0; i<=middle; i++)
    {
      filter[i] = filter[i]/sum;
    }
  }
}

/*!
  Return the coefficients of a Gaussian derivative filter that may be used to compute spatial image derivatives after applying a Gaussian blur.

  \param filter : Pointer to the filter kernel that should refer to a (size+1)/2 array.
  The first value refers to the central coefficient, the next one to the right coefficients. Left coefficients could be deduced by symmetry.
  \param size : Filter size. This value should be odd.
  \param sigma : Gaussian standard deviation. If it is equal to zero or negative, it is computed from filter size as sigma = (size-1)/6.
  \param normalize : Flag indicating whether to normalize the filter coefficients or not.
*/
void vpImageFilter::getGaussianDerivativeKernel(double *filter, unsigned int size, double sigma, bool normalize)
{
  if (size%2 != 1)
    throw (vpImageException(vpImageException::incorrectInitializationError,
          "Bad Gaussian filter size"));

  if (sigma<= 0)
    sigma = (size-1)/6.0;

  int middle = (int)(size-1)/2;
  double sigma2 = vpMath::sqr(sigma);
  filter[0] = 0.;
  for(int i=1; i<= middle; i++)
  {
    filter[i] = -(1./(sigma*sqrt(2.*M_PI)))*(exp(-((i+1)*(i+1))/(2.*sigma2))-exp(-((i-1)*(i-1))/(2.*sigma2)))/2.;
  }

  if (normalize) {
    double sum=0;
    for(int i=1; i<=middle; i++)
    {
      sum += 2.*(1./(sigma*sqrt(2.*M_PI)))*exp(-(i*i)/(2.*sigma2));
    }
    sum += (1./(sigma*sqrt(2.*M_PI))) ;

    for(int i=1; i<=middle; i++)
    {
      filter[i] = filter[i]/sum;
    }
  }
}


void vpImageFilter::getGradX(const vpImage<unsigned char> &I, vpImage<double>& dIx)
{
  dIx.resize(I.getHeight(),I.getWidth()) ;
  //dIx=0;
  for (unsigned int i=0 ; i < I.getHeight() ; i++)
  {
    for (unsigned int j=0 ; j < 3 ; j++)
    {
      dIx[i][j]=0;
    }
    for (unsigned int j=3 ; j < I.getWidth()-3 ; j++)
    {
      dIx[i][j]=vpImageFilter::derivativeFilterX(I,i,j);
    }
    for (unsigned int j=I.getWidth()-3 ; j < I.getWidth() ; j++)
    {
      dIx[i][j]=0;
    }
  }
}

void vpImageFilter::getGradY(const vpImage<unsigned char> &I, vpImage<double>& dIy)
{
  dIy.resize(I.getHeight(),I.getWidth()) ;
  //dIy=0;
  for (unsigned int i=0 ; i < 3 ; i++)
  {
    for (unsigned int j=0 ; j < I.getWidth() ; j++)
    {
      dIy[i][j]=0;
    }
  }
  for (unsigned int i=3 ; i < I.getHeight()-3 ; i++)
  {
    for (unsigned int j=0 ; j < I.getWidth() ; j++)
    {
      dIy[i][j]=vpImageFilter::derivativeFilterY(I,i,j);
    }
  }
  for (unsigned int i=I.getHeight()-3 ; i < I.getHeight() ; i++)
  {
    for (unsigned int j=0 ; j < I.getWidth() ; j++)
    {
      dIy[i][j]=0;
    }
  }
}

void vpImageFilter::getGradX(const vpImage<unsigned char> &I, vpImage<double>& dIx, const double *filter,unsigned  int size)
{
  dIx.resize(I.getHeight(),I.getWidth()) ;
  //#pragma omp parallel for
  for (unsigned int i=0 ; i < I.getHeight() ; i++)
  {
    for (unsigned int j=0 ; j < (size-1)/2 ; j++)
    {
      dIx[i][j]=0;
    }
    for (unsigned int j=(size-1)/2 ; j < I.getWidth()-(size-1)/2 ; j++)
    {
      dIx[i][j]=vpImageFilter::derivativeFilterX(I,i,j,filter,size);
    }
    for (unsigned int j=I.getWidth()-(size-1)/2 ; j < I.getWidth() ; j++)
    {
      dIx[i][j]=0;
    }
  }
}
void vpImageFilter::getGradX(const vpImage<double> &I, vpImage<double>& dIx, const double *filter,unsigned  int size)
{
  dIx.resize(I.getHeight(),I.getWidth()) ;
  //dIx=0;
  for (unsigned int i=0 ; i < I.getHeight() ; i++)
  {
    for (unsigned int j=0 ; j < (size-1)/2 ; j++)
    {
      dIx[i][j]=0;
    }
    for (unsigned int j=(size-1)/2 ; j < I.getWidth()-(size-1)/2 ; j++)
    {
      dIx[i][j]=vpImageFilter::derivativeFilterX(I,i,j,filter,size);
    }
    for (unsigned int j=I.getWidth()-(size-1)/2 ; j < I.getWidth() ; j++)
    {
      dIx[i][j]=0;
    }
  }
}

void vpImageFilter::getGradY(const vpImage<unsigned char> &I, vpImage<double>& dIy, const double *filter,unsigned  int size)
{
  dIy.resize(I.getHeight(),I.getWidth()) ;
  //#pragma omp parallel for
  for (unsigned int i=0 ; i < (size-1)/2 ; i++)
  {
    for (unsigned int j=0 ; j < I.getWidth() ; j++)
    {
      dIy[i][j]=0;
    }
  }
  //#pragma omp parallel for
  for (unsigned int i=(size-1)/2 ; i < I.getHeight()-(size-1)/2 ; i++)
  {
    for (unsigned int j=0 ; j < I.getWidth() ; j++)
    {
      dIy[i][j]=vpImageFilter::derivativeFilterY(I,i,j,filter,size);
    }
  }
  //#pragma omp parallel for
  for (unsigned int i=I.getHeight()-(size-1)/2 ; i < I.getHeight() ; i++)
  {
    for (unsigned int j=0 ; j < I.getWidth() ; j++)
    {
      dIy[i][j]=0;
    }
  }
}

void vpImageFilter::getGradY(const vpImage<double> &I, vpImage<double>& dIy, const double *filter,unsigned  int size)
{
  dIy.resize(I.getHeight(),I.getWidth()) ;
  //dIy=0;
  for (unsigned int i=0 ; i < (size-1)/2 ; i++)
  {
    for (unsigned int j=0 ; j < I.getWidth() ; j++)
    {
      dIy[i][j]=0;
    }
  }
  for (unsigned int i=(size-1)/2 ; i < I.getHeight()-(size-1)/2 ; i++)
  {
    for (unsigned int j=0 ; j < I.getWidth() ; j++)
    {
      dIy[i][j]=vpImageFilter::derivativeFilterY(I,i,j,filter,size);
    }
  }
  for (unsigned int i=I.getHeight()-(size-1)/2 ; i < I.getHeight() ; i++)
  {
    for (unsigned int j=0 ; j < I.getWidth() ; j++)
    {
      dIy[i][j]=0;
    }
  }
}

/*!
   Compute the gradient along X after applying a gaussian filter along Y.
   \param I : Input image
   \param dIx : Gradient along X.
   \param gaussianKernel : Gaussian kernel which values should be computed using vpImageFilter::getGaussianKernel().
   \param gaussianDerivativeKernel : Gaussian derivative kernel which values should be computed using vpImageFilter::getGaussianDerivativeKernel().
   \param size : Size of the Gaussian and Gaussian derivative kernels.
 */
void vpImageFilter::getGradXGauss2D(const vpImage<unsigned char> &I, vpImage<double>& dIx, const double *gaussianKernel, const double *gaussianDerivativeKernel, unsigned  int size)
{
  vpImage<double> GIy;
  vpImageFilter::filterY(I,  GIy, gaussianKernel, size);
  vpImageFilter::getGradX(GIy, dIx, gaussianDerivativeKernel, size);
}

/*!
   Compute the gradient along Y after applying a gaussian filter along X.
   \param I : Input image
   \param dIy : Gradient along Y.
   \param gaussianKernel : Gaussian kernel which values should be computed using vpImageFilter::getGaussianKernel().
   \param gaussianDerivativeKernel : Gaussian derivative kernel which values should be computed using vpImageFilter::getGaussianDerivativeKernel().
   \param size : Size of the Gaussian and Gaussian derivative kernels.
 */
void vpImageFilter::getGradYGauss2D(const vpImage<unsigned char> &I, vpImage<double>& dIy, const double *gaussianKernel, const double *gaussianDerivativeKernel,unsigned  int size)
{
  vpImage<double> GIx;
  vpImageFilter::filterX(I,  GIx, gaussianKernel, size);
  vpImageFilter::getGradY(GIx, dIy, gaussianDerivativeKernel, size);
}

//operation pour pyramide gaussienne
void vpImageFilter::getGaussPyramidal(const vpImage<unsigned char> &I, vpImage<unsigned char>& GI)
{
  vpImage<unsigned char> GIx;
#if defined(VISP_HAVE_OPENCV) && (VISP_HAVE_OPENCV_VERSION >= 0x030000)
  cv::Mat imgsrc, imgdest;
  vpImageConvert::convert(I, imgsrc);
  cv::pyrDown( imgsrc, imgdest, cv::Size((int)I.getWidth()/2,(int)I.getHeight()/2));
  vpImageConvert::convert(imgdest, GI);
#elif defined(VISP_HAVE_OPENCV) && (VISP_HAVE_OPENCV_VERSION >= 0x020408)
  cv::Mat imgsrc, imgdest;
  vpImageConvert::convert(I, imgsrc);
  cv::pyrDown( imgsrc, imgdest, cvSize((int)I.getWidth()/2,(int)I.getHeight()/2));
  vpImageConvert::convert(imgdest, GI);
#elif defined(VISP_HAVE_OPENCV) && (VISP_HAVE_OPENCV_VERSION >= 0x020100)
  IplImage* imgsrc = NULL;//cvCreateImage(cvGetSize(imgign), IPL_DEPTH_8U, 1);
  IplImage* imgdest = NULL;//cvCreateImage(cvGetSize(imgign), IPL_DEPTH_8U, 1);
  imgsrc = cvCreateImage(cvSize((int)I.getWidth(),(int)I.getHeight()), IPL_DEPTH_8U, 1);
  imgdest = cvCreateImage(cvSize((int)I.getWidth()/2,(int)I.getHeight()/2), IPL_DEPTH_8U, 1);
  vpImageConvert::convert(I,imgsrc);
  cvPyrDown( imgsrc, imgdest);
  vpImageConvert::convert(imgdest,GI);

  cvReleaseImage(&imgsrc);
  cvReleaseImage(&imgdest);
  //vpImage<unsigned char> sGI;sGI=GI;

#else
  vpImageFilter::getGaussXPyramidal(I,GIx);
  vpImageFilter::getGaussYPyramidal(GIx,GI);
#endif
}

void vpImageFilter::getGaussXPyramidal(const vpImage<unsigned char> &I, vpImage<unsigned char>& GI)
{
#if 0
  GI.resize(I.getHeight(),(int)((I.getWidth()+1.)/2.)) ;
  for (unsigned int i=0 ; i < I.getHeight() ; i++)
  {
    GI[i][0]=I[i][0];
    for (unsigned int j=1 ; j < ((I.getWidth()+1.)/2.)-1 ; j++)
    {
      GI[i][j]=vpImageFilter::filterGaussXPyramidal(I,i,2*j);
    }
    GI[i][(int)((I.getWidth()+1.)/2.)-1]=I[i][2*((int)((I.getWidth()+1.)/2.)-1)];
  }
#else
  unsigned int w = I.getWidth()/2;

  GI.resize(I.getHeight(), w) ;
  for (unsigned int i=0 ; i < I.getHeight() ; i++)
  {
    GI[i][0]=I[i][0];
    for (unsigned int j=1 ; j < w-1 ; j++)
    {
      GI[i][j]=vpImageFilter::filterGaussXPyramidal(I,i,2*j);
    }
    GI[i][w-1]=I[i][2*w-1];
  }

#endif
}
void vpImageFilter::getGaussYPyramidal(const vpImage<unsigned char> &I, vpImage<unsigned char>& GI)
{

#ifdef ORIG
  GI.resize((int)((I.getHeight()+1.)/2.),I.getWidth()) ;
  for (unsigned int j=0 ; j < I.getWidth() ; j++)
  {
    GI[0][j]=I[0][j];
    for (unsigned int i=1 ; i < ((I.getHeight()+1.)/2.)-1 ; i++)
    {
      GI[i][j]=vpImageFilter::filterGaussYPyramidal(I,2*i,j);
    }
    GI[(int)((I.getHeight()+1.)/2.)-1][j]=I[2*((int)((I.getHeight()+1.)/2.)-1)][j];
  }

#else
  unsigned int h = I.getHeight()/2;

  GI.resize(h, I.getWidth()) ;
  for (unsigned int j=0 ; j < I.getWidth() ; j++)
  {
    GI[0][j]=I[0][j];
    for (unsigned int i=1 ; i < h-1 ; i++)
    {
         GI[i][j]=vpImageFilter::filterGaussYPyramidal(I,2*i,j);
    }
    GI[h-1][j]=I[2*h-1][j];
  }
#endif
}


