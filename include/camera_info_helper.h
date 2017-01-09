#ifndef __CAMERA_INFO_HELPER_H__
#define __CAMERA_INFO_HELPER_H__

#include "fpga_calibration.h"
#include <ros_api/ros_api.h>
#include <ros_api_extend/ros_api_extend.h>

#include <string>

inline void setCameraInfoIntrinsics(sensor_msgs::CameraInfo &ci, double fx,
				    double fy, double cx, double cy)
{
	// TODO: Doesn't work with ros1
	ci.k[0] = fx;
	ci.k[1] = 0;
	ci.k[2] = cx;
	ci.k[3] = 0;
	ci.k[4] = fy;
	ci.k[5] = cy;
	ci.k[6] = 0;
	ci.k[7] = 0;
	ci.k[8] = 1;
}

inline void setCameraInfoDistortionMdl(
	sensor_msgs::CameraInfo &ci, uvc_ros_driver::ProjectionModelTypes pmt)
{
	std::string model;

	switch (pmt) {
	case uvc_ros_driver::RADTAN:
		model = "rational_polynomial";
		break;

	case uvc_ros_driver::PINHOLE:
	default:
		model = "plumb_bob";
	}

	ci.distortion_model = model;
}

inline void setCameraInfoDistortionParams(sensor_msgs::CameraInfo &ci,
		double k1, double k2, double t1,
		double t2, double k3)
{
	// define size of the distortion vector
	ci.d.resize(5);
	ci.d[0] = k1;
	ci.d[1] = k2;
	ci.d[2] = t1;
	ci.d[3] = t2;
	ci.d[4] = k3;
}

inline void setCameraInfoHeader(sensor_msgs::CameraInfo &ci, int width,
				int height, const RosAPI::Time &t,
				const std::string &frame_id)
{
	ci.width = width;
	ci.height = height;
	ci.header.stamp = t;
	ci.header.frame_id = frame_id;
}

#endif /* end of include guard: __CAMERA_INFO_HELPER_H__ */
