/****************************************************************************
 *
 *   Copyright (c) 2015-2016 PX4 Development Team. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name PX4 nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/
/*
 * uvc_ros_driver_node.cpp
 *
 *  Created on: Mar 11, 2016
 *      Author: nicolas, christoph, simone
 *
 */

#include "uvc_ros_driver.h"

// declare helper function
CameraParameters loadCustomCameraCalibration(const std::string calib_path)
{
	CameraParameters cp;

	// load a camera calibration defined in the launch script
	try {
		//wait on inital mount of calib path
		usleep(2000000);
		YAML::Node YamlNode = YAML::LoadFile(calib_path);

		if (YamlNode.IsNull()) {
			ROS_INFO("Continuing without calibration file");
			cp.isValid = false;
		}

		cp = parseYaml(YamlNode);

	} catch (YAML::BadFile &e) {
		ROS_INFO("Continuing without calibration file");
		cp.isValid = false;
	}

	return cp;
}

int main(int argc, char **argv)
{
	RosAPI::init(argc, argv, "uvc_camera");

	auto nh = std::make_shared<RosAPI::Node>("~");
	uvc::uvcROSDriver uvc_ros_driver(nh.get(), "uvc_ros_driver");

	// get params from launch file
	bool flip, set_calibration, depth_map, calibration_mode, ait_msgs;
	int camera_config, number_of_cameras;
	std::string calibration_file_path;
	// TODO: check if parameter exist
	RosAPI::getParamWithDefault(nh.get(), "flip", flip, false);
	RosAPI::getParamWithDefault(nh.get(), "numberOfCameras", number_of_cameras, 2);
	RosAPI::getParamWithDefault(nh.get(), "AITMsgs", ait_msgs, false);
	RosAPI::getParamWithDefault(nh.get(), "setCalibration", set_calibration, false);
	RosAPI::getParamWithDefault(nh.get(), "depthMap", depth_map, false);
	RosAPI::getParamWithDefault(nh.get(), "cameraConfigFile", calibration_file_path, std::string(""));
	RosAPI::getParamWithDefault(nh.get(), "calibrationMode", calibration_mode, true);

	// read yaml calibration file from device
	CameraParameters camParams =
		loadCustomCameraCalibration(calibration_file_path);

	std::vector<std::pair<int, int>> homography_mapping = {std::make_pair(0, 1)};
	// // import homograpy mapping from yaml file
	// XmlRpc::XmlRpcValue homography_import;
	// nh.param("homography_mapping", homography_import, homography_import);

	// for (int i = 0; i < homography_import.size(); i++) {
	// 	homography_mapping.push_back(std::make_pair((int)homography_import[i][0],
	// 				     (int)homography_import[i][1]));
	// }

	// set parameter
	uvc_ros_driver.setNumberOfCameras(number_of_cameras);
	uvc_ros_driver.setUseOFAITMsgs(ait_msgs);
	uvc_ros_driver.setFlip(flip);

	if (camParams.isValid) {
		uvc_ros_driver.setCalibrationParam(set_calibration);
		uvc_ros_driver.setUseOfDepthMap(depth_map);

	} else {
		uvc_ros_driver.setCalibrationParam(0);
		uvc_ros_driver.setUseOfDepthMap(0);
	}

	uvc_ros_driver.setCalibrationMode(calibration_mode);
	uvc_ros_driver.setCameraParams(camParams);
	uvc_ros_driver.setHomographyMapping(homography_mapping);
	// initialize device
	uvc_ros_driver.initDevice();
	// start device
	uvc_ros_driver.startDevice();
	// endless loop
	RosAPI::spin(nh);
	return 0;
}
