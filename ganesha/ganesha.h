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
#include "gvulkaninstance.h"
#include "gvulkandevice.h"
#include "gvulkanpipeline.h"
#include "gvulkanswapchain.h"
#include "gvulkanbuffer.h"
#include "gcamera.h"
#include "gcameradata.h"

//  Content
#include "grendergraph.h"
#include "gmaterialsservice.h"
#include "gganeshacontent.h"
#include "gdescriptorsetservice.h"
#include "gshadersservice.h"
#include "gviewport.h"

//  Application
#include "gapplication.h"
#include "ganeshatypes.h"

//  Events
#include "gevent.h"
#include "geventshell.h"
#include "gwindowevent.h"
#include "gmouseevent.h"
#include "gkeyboardevent.h"
#include "geventsservice.h"

//  Layers
#include "glayer.h"
#include "gsystemlayer.h"
#include "glayersservice.h"
#include "gvulkanlayer.h"

#endif  //  GANESHA_H
