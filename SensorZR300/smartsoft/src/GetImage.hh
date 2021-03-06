//--------------------------------------------------------------------------
// Code generated by the SmartSoft MDSD Toolchain
// The SmartSoft Toolchain has been developed by:
//  
// Service Robotics Research Center
// University of Applied Sciences Ulm
// Prittwitzstr. 10
// 89075 Ulm (Germany)
//
// Information about the SmartSoft MDSD Toolchain is available at:
// www.servicerobotik-ulm.de
//
// This file is generated once. Modify this file to your needs. 
// If you want the toolchain to re-generate this file, please 
// delete it before running the code generator.
//--------------------------------------------------------------------------
#ifndef _GETIMAGE_HH
#define _GETIMAGE_HH

#include "GetImageCore.hh"
#include <librealsense/rs.hpp>

class GetImage  : public GetImageCore
{
private:
	rs::device *dev;
	rs::context ctx;

public:


	GetImage(SmartACE::SmartComponent *comp);
	virtual ~GetImage();
	
	virtual int on_entry();
	virtual int on_execute();
	virtual int on_exit();
    void getrgbimage(DomainVision::CommVideoImage& comm_rgb_frame);
    void getdepthimage(DomainVision::CommDepthImage& comm_depth_frame);
    void getpointcloud(DomainVision::Comm3dPointCloud& comm_pointcloud_frame, DomainVision::CommDepthImage comm_depth_frame);
};

#endif
