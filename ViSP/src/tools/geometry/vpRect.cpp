/****************************************************************************
 *
 * $Id$
 *
 * Copyright (C) 1998-2010 Inria. All rights reserved.
 *
 * This software was developed at:
 * IRISA/INRIA Rennes
 * Projet Lagadic
 * Campus Universitaire de Beaulieu
 * 35042 Rennes Cedex
 * http://www.irisa.fr/lagadic
 *
 * This file is part of the ViSP toolkit.
 *
 * This file may be distributed under the terms of the Q Public License
 * as defined by Trolltech AS of Norway and appearing in the file
 * LICENSE included in the packaging of this file.
 *
 * Licensees holding valid ViSP Professional Edition licenses may
 * use this file in accordance with the ViSP Commercial License
 * Agreement provided with the Software.
 *
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 * Contact visp@irisa.fr if any conditions of this licensing are
 * not clear to you.
 *
 * Description:
 * Defines a rectangle in the plane.
 *
 * Author:
 * Fabien Spindler
 *
 *****************************************************************************/

/*!
  \file vpRect.cpp
  \brief Defines a rectangle in the plane.
  \ingroup libtools
*/


#include <visp/vpRect.h>
#include <visp/vpDebug.h>

/*!
 
  Constructs a default rectangle with the \e top, \e left corner set to (0,0)
  and \e width and \e height set to 1.

*/

vpRect::vpRect()
{
  this->left = 0;
  this->top = 0;
  this->width = 0;
  this->height = 0;
};

/*!
 
  Constructs a rectangle with the \e top, \e left corner and \e width
  and \e height.

*/
vpRect::vpRect(double left, double top, double width, double height)
{
  this->left = left;
  this->top = top;
  this->width = width;
  this->height = height;
};

/*!
 
  Constructs a rectangle with \e topLeft the top-left corner location
  and \e width and \e height the rectangle size.

*/
vpRect::vpRect(const vpImagePoint &topLeft, double width, double height)
{
  this->left = topLeft.get_u();
  this->top = topLeft.get_v();
  this->width = width;
  this->height = height;
};

/*!
 
  Constructs a rectangle with \e topLeft the top-left corner location
  and \e bottomRight the bottom-right corner.

*/
vpRect::vpRect(const vpImagePoint &topLeft, const vpImagePoint &bottomRight)
{
  this->left = topLeft.get_u();
  this->top = topLeft.get_v();
  
  setBottom( bottomRight.get_v() );
  setRight( bottomRight.get_u() );
};

/*!
 
  Constructs a rectangle that is a copy of \e r.

*/
vpRect::vpRect(const vpRect& r)
{
  this->left   = r.left;
  this->top    = r.top;
  this->width  = r.width;
  this->height = r.height;
};

/*!

  Assign \e r to this rectangle and return a reference to this rectangle.
*/  
vpRect &vpRect::operator=(const vpRect& r) 
{
  this->left   = r.left;
  this->top    = r.top;
  this->width  = r.width;
  this->height = r.height;
  return *this;
};