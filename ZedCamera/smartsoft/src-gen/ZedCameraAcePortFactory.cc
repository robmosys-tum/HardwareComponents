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
// Please do not modify this file. It will be re-generated
// running the code generator.
//--------------------------------------------------------------------------

#include "ZedCameraAcePortFactory.hh"

// create a static instance of the default AcePortFactory
static ZedCameraAcePortFactory acePortFactory;

ZedCameraAcePortFactory::ZedCameraAcePortFactory()
{  
	componentImpl = 0;
	ZedCamera::instance()->addPortFactory("ACE_SmartSoft", this);
}

ZedCameraAcePortFactory::~ZedCameraAcePortFactory()
{  }

void ZedCameraAcePortFactory::initialize(ZedCamera *component, int argc, char* argv[])
{
	if(component->connections.component.defaultScheduler != "DEFAULT") {
		ACE_Sched_Params sched_params(ACE_SCHED_OTHER, ACE_THR_PRI_OTHER_DEF);
		if(component->connections.component.defaultScheduler == "FIFO") {
			sched_params.policy(ACE_SCHED_FIFO);
			sched_params.priority(ACE_THR_PRI_FIFO_MIN);
		} else if(component->connections.component.defaultScheduler == "RR") {
			sched_params.policy(ACE_SCHED_RR);
			sched_params.priority(ACE_THR_PRI_RR_MIN);
		}
		// create new instance of the SmartSoft component with customized scheuling parameters 
		componentImpl = new ZedCameraImpl(component->connections.component.name, argc, argv, sched_params);
	} else {
		// create new instance of the SmartSoft component
		componentImpl = new ZedCameraImpl(component->connections.component.name, argc, argv);
	}
}

int ZedCameraAcePortFactory::onStartup()
{
	return componentImpl->startComponentInfrastructure();
}


Smart::IPushServerPattern<DomainVision::Comm3dPointCloud> * ZedCameraAcePortFactory::createPointCloudPushServiceOut(const std::string &serviceName)
{
	return new SmartACE::PushServer<DomainVision::Comm3dPointCloud>(componentImpl, serviceName);
}

Smart::IPushServerPattern<DomainVision::CommRGBDImage> * ZedCameraAcePortFactory::createRGBDImagePushServiceOut(const std::string &serviceName)
{
	return new SmartACE::PushServer<DomainVision::CommRGBDImage>(componentImpl, serviceName);
}

Smart::IPushServerPattern<DomainVision::CommVideoImage> * ZedCameraAcePortFactory::createRGBImagePushServiceOut(const std::string &serviceName)
{
	return new SmartACE::PushServer<DomainVision::CommVideoImage>(componentImpl, serviceName);
}


SmartACE::SmartComponent* ZedCameraAcePortFactory::getComponentImpl()
{
	return componentImpl;
}

int ZedCameraAcePortFactory::onShutdown(const std::chrono::steady_clock::duration &timeoutTime)
{
	componentImpl->stopComponentInfrastructure(timeoutTime);
	return 0;
}

void ZedCameraAcePortFactory::destroy()
{
	// clean-up component's internally used resources (internally used communication middleware) 
	componentImpl->cleanUpComponentResources();
	delete componentImpl;
}
