#ifndef GANESHA_H
#define GANESHA_H

#include "ganeshatypes.h"

//  Basics
#include "gcolor.h"
#include "gmatrix.h"
#include "gpoint.h"
#include "gpoint2d.h"
#include "gquaternion.h"
#include "gvector.h"

//  VULKAN
#include "gvulkanapi.h"
#include "gvulkaninstance.h"
#include "gvulkandevice.h"
#include "gvulkanpipeline.h"
#include "gvulkanswapchain.h"
#include "gvulkanbuffer.h"

//  Content
#include "gshader.h"
#include "gshadersservice.h"
#include "gvulkanimage.h"
#include "gimageservice.h"
#include "gsceneservice.h"
#include "gscene.h"
#include "gscenenode.h"
#include "gmesh.h"

#include "gviewport.h"
#include "gcamera.h"
#include "gcameradata.h"
#include "gdescriptorsetservice.h"
#include "gcommandservice.h"
#include "gvulkantools.h"
#include "gtga.h"

//  Events and Layers
#include "geventsservice.h"
#include "gevent.h"
#include "geventshell.h"
#include "gwindowresizeevent.h"
#include "gmouseevent.h"
#include "gkeyboardevent.h"
#include "gupdatecameralookevent.h"
#include "gupdatecamerapositionevent.h"
#include "gloadgundleevent.h"
#include "gupdateframesizeevent.h"
#include "glayersservice.h"
#include "glayer.h"
#include "gsystemlayer.h"
#include "gvulkanlayer.h"
#include "gapplication.h"

#endif  //  GANESHA_H
