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
#include "GetImage.hh"
#include "RealSenseBasic.hh"

#include <iostream>

GetImage::GetImage(SmartACE::SmartComponent *comp) 
:	GetImageCore(comp)
{
	std::cout << "constructor GetImage\n";

    rs2::config cfg;

    //Add desired streams to configuration
    cfg.enable_stream(RS2_STREAM_COLOR, 640, 480, RS2_FORMAT_BGR8, 30);
    cfg.enable_stream(RS2_STREAM_DEPTH, 640, 480, RS2_FORMAT_Z16, 30);
    //Instruct pipeline to start streaming with the requested configuration
    profile = pipe.start(cfg);

    auto sensor = profile.get_device().first<rs2::depth_sensor>();
    depth_scale =  sensor.get_depth_scale();

}
GetImage::~GetImage() 
{
	std::cout << "destructor GetImage\n";
}

void GetImage::getrgbimage(DomainVision::CommVideoImage& comm_rgb_frame)
{
	rs2::video_frame current_rgb_frame   = current_frameset.get_color_frame();

	const unsigned char *color_frame = (const unsigned char*) current_rgb_frame.get_data();
	auto color_frame_number    = current_rgb_frame.get_frame_number();

	comm_rgb_frame.set_parameters(current_rgb_frame.get_width(), current_rgb_frame.get_height(), DomainVision::FormatType::RGB24);
	comm_rgb_frame.set_data(color_frame);
	comm_rgb_frame.setIs_valid(true);
    comm_rgb_frame.setSeq_count(color_frame_number);

}

void GetImage::getdepthimage(DomainVision::CommDepthImage& comm_depth_frame)
{


	rs2::depth_frame current_depth_frame = current_frameset.get_depth_frame();
    // Query frame size (width and height)
    const int frame_height = current_depth_frame.as<rs2::video_frame>().get_height();
    const int frame_width = current_depth_frame.as<rs2::video_frame>().get_width();

//    for (int x = 0;x < frame_width/2; x++)//To loop through all the pixels
//        for (int y = 0; y < frame_height/2; y++)
//        	if (current_depth_frame.get_distance(x,y) != 0)
//        		std::cout<<current_depth_frame.get_distance(x,y)<<" ";

	/*Read Current Frames*/
	uint16_t* depth_frame = (uint16_t*) current_depth_frame.get_data();
	 std::cout << "The camera is facing an object " << depth_frame[240*640+320] << " meters away "<<std::endl;
//	for (int i = 0;i < frame_width*frame_height; i++)
//		depth_frame[i] = depth_frame[i]*depth_scale;
//
//	 std::cout << "The camera is facing an object1 " << depth_frame[240*640+320] << " meters away "<<std::endl;
//	uint16_t * depth_frame = (uint16_t*)current_depth_frame.get_data();
	//assert(rs_error_ != nullptr  && "error while reading depth stream");




//	// BaseLine
//	rs2::disparity_transform disparity_transform( true );
//	rs2::disparity_frame disparity_frame = disparity_transform.process( current_depth_frame );
//	float baseline = disparity_frame.get_baseline(); // mm
////	std:: cout << "baseline = " << baseline <<std::endl;



	auto depth_frame_timestamp= current_depth_frame.get_timestamp();
	auto depth_frame_number= current_depth_frame.get_frame_number();


	comm_depth_frame.setWidth(frame_width);
	comm_depth_frame.setHeight(frame_height);
	comm_depth_frame.setScale(depth_scale);
	//std::cout << "depth height =" << comm_depth_frame.getHeight()<<"depth width  =" << comm_depth_frame.getWidth()<<std::endl;

	comm_depth_frame.setFormat(DomainVision::DepthFormatType::UINT16);
	comm_depth_frame.setPixel_size(16);
	comm_depth_frame.setSeq_count(depth_frame_number);
//
	comm_depth_frame.set_distances(depth_frame, frame_width, frame_height);
}

void GetImage::getpointcloud(DomainVision::Comm3dPointCloud& comm_pointcloud_frame, DomainVision::CommDepthImage comm_depth_frame)
{
	int img_width = comm_depth_frame.getWidth();
	int img_height = comm_depth_frame.getHeight();
	std::vector<CommBasicObjects::CommPosition3d> points;

	float cx=308.2192687988281,  cy = 245.59515380859375; //rgb intrinsic
	//float fx = 473.1015319824219, fy = 473.1015625;
	float fx =  618.5377197265625, fy = 618.5377807617188;

	//float* data = (float*) comm_depth_frame.get_distances_float();


	rs2::depth_frame current_depth_frame = current_frameset.get_depth_frame();
    // Query frame size (width and height)
    const int frame_height = current_depth_frame.as<rs2::video_frame>().get_height();
    const int frame_width = current_depth_frame.as<rs2::video_frame>().get_width();

    for (int x = 0;x < frame_width; x++)//To loop through all the pixels
           for (int y = 0; y < frame_height; y++)
        	if (current_depth_frame.get_distance(x,y) != 0){
				CommBasicObjects::CommPosition3d pt;
				double ptx, pty, ptz;

				ptz = current_depth_frame.get_distance(x,y);
				ptx =  (x - cx)*ptz/fx;
				pty =  (y - cy)*ptz/fy;

				pt.setX(ptx);	pt.setY(pty);	pt.setZ(ptz);
				points.push_back(pt);
		}

	comm_pointcloud_frame.setPoints(points);
	std:: cout << "New PC "  <<std::endl;
}

int GetImage::on_entry()
{
	// do initialization procedures here, which are called once, each time the task is started
	// it is possible to return != 0 (e.g. when initialization fails) then the task is not executed further
	return 0;
}
int GetImage::on_execute()
{
	Smart::StatusCode status;

	DomainVision::CommRGBDImage rgbd_frame;
	DomainVision::CommDepthImage comm_depth_frame;
	DomainVision::CommVideoImage comm_rgb_frame;
	DomainVision::Comm3dPointCloud comm_pointcloud_frame;
	
	current_frameset = pipe.wait_for_frames();


	getrgbimage(comm_rgb_frame);
	getdepthimage(comm_depth_frame);
	getpointcloud(comm_pointcloud_frame, comm_depth_frame);

	COMP->rGBImagePushServiceOut->put(comm_rgb_frame);
	COMP->pointCloudPushServiceOut->put(comm_pointcloud_frame);

	rgbd_frame.setColor_image(comm_rgb_frame);
	rgbd_frame.setDepth_image(comm_depth_frame);
	rgbd_frame.setSeq_count(comm_rgb_frame.getSeq_count());
	rgbd_frame.setIs_valid(true);
	COMP->rGBDImagePushServiceOut->put(rgbd_frame);
	// it is possible to return != 0 (e.g. when the task detects errors), then the outer loop breaks and the task stops
	return 0;
}
int GetImage::on_exit()
{
	// use this method to clean-up resources which are initialized in on_entry() and needs to be freed before the on_execute() can be called again
	return 0;
}
