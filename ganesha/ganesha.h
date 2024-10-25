//
//  ganesha.h
//  ganesha
//
//  Created by Mihail Terekhov on 23.07.2021.
//

#ifndef GANESHA_H
#define GANESHA_H

//  This is umbrella header of Ganesha engine

//  Basics
#include "gcolor.h"
#include "gmatrix.h"
#include "gpoint.h"
#include "gpoint2d.h"
#include "gvector.h"
#include "gcolor.h"

//  Tools
#include "glog.h"

//  VULKAN
#include "ggraphicsapiprotocol.h"
#include "gvulkanapi.h"
#include "gcommandservice.h"
#include "gdescriptorsetservice.h"
#include "gvulkaninstance.h"
#include "gvulkandevice.h"
#include "gvulkanpipeline.h"
#include "gvulkanswapchain.h"
#include "gvulkanbuffer.h"
#include "gcamera.h"
#include "gcameradata.h"

//  Content
#include "gscene.h"
#include "gsceneservice.h"
#include "gimageservice.h"
#include "gshadersservice.h"
#include "gviewport.h"

//  Application
#include "gapplication.h"
#include "ganeshatypes.h"

//  Events
#include "gevent.h"
#include "geventshell.h"
#include "geventsservice.h"
#include "gwindowresizeevent.h"
#include "gmouseevent.h"
#include "gkeyboardevent.h"

#include "gupdatecameralookevent.h"
#include "gupdatecamerapositionevent.h"
#include "gloadgundleevent.h"
#include "gupdateviewmatrixevent.h"
#include "gupdateviewportevent.h"

//  Layers
#include "glayer.h"
#include "gsystemlayer.h"
#include "glayersservice.h"
#include "gvulkanlayer.h"

#endif  //  GANESHA_H
