#############################################################################
#
# This file is part of the ViSP software.
# Copyright (C) 2005 - 2017 by Inria. All rights reserved.
#
# This software is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# ("GPL") version 2 as published by the Free Software Foundation.
# See the file LICENSE.txt at the root directory of this source
# distribution for additional information about the GNU GPL.
#
# For using ViSP with software that can not be combined with the GNU
# GPL, please contact Inria about acquiring a ViSP Professional
# Edition License.
#
# See http://visp.inria.fr for more information.
#
# This software was developed at:
# Inria Rennes - Bretagne Atlantique
# Campus Universitaire de Beaulieu
# 35042 Rennes Cedex
# France
#
# If you have questions regarding the use of this file, please contact
# Inria at visp@inria.fr
#
# This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
# WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
#
# Description:
# ViSP packaging configuration file.
#
# Authors:
# Fabien Spindler
#
#############################################################################

set(BUILD_SHARED_LIBS CACHE FORCE "Build ViSP with shared libraries." ON)

# InstallRequiredSystemLibraries: Hack for Visual Studio support Search for
# system runtime libraries based on the platform.
INCLUDE(InstallRequiredSystemLibraries)

set(CPACK_GENERATOR NSIS)
set(CPACK_NSIS_CONTACT "visp@inria.fr")
SET(CPACK_NSIS_HELP_LINK "http://www.irisa.fr/lagadic/visp/visp.html")
set(CPACK_NSIS_URL_INFO_ABOUT "http://www.irisa.fr/lagadic/visp/visp.html")
set(CPACK_NSIS_MODIFY_PATH "ON")
set(CPACK_NSIS_DISPLAY_NAME "${CPACK_PACKAGE_INSTALL_DIRECTORY} SDK")
# set(CPACK_PACKAGE_ICON ${VISP_SOURCE_DIR}/visp.bmp)
# set(CPACK_NSIS_MUI_ICON ${VISP_SOURCE_DIR}/logoViSP.ico)
# set(CPACK_NSIS_MUI_UNIICON ${VISP_SOURCE_DIR}/logoViSP.ico)

SET(CPACK_PACKAGE_FILE_NAME "ViSP-${VISP_VERSION}-win32")
SET(CPACK_SOURCE_PACKAGE_FILE_NAME "ViSP-${VISP_VERSION}-win32-src")
