#pragma once

#ifndef __win64
#include <Windows.h>
#endif

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/viz.hpp>

#include <DataStruct/ListManager/ListManager.h>
using namespace bemong::enum_class;

#include <thread>
#include <mutex>
#include <string>
#include <queue>

class VtkViewer :
	public bemong::vector::ListManager<std::string, std::string>
{
public:
	VtkViewer(
		const std::string& strWindowName = "VtkViewer",
		int nWidth = 960, int nHeight = 540,
		int nWindowPositionX = 960, int nWindowPositionY = 540);
	~VtkViewer();

private:
	std::string m_strWindowName;

	int m_nWidth;
	int m_nHeight;
	int m_nWindowPositionX;
	int m_nWindowPositionY;
	
public:
	int Init();

	int AddSphere(
		const std::string& strWidgetName,
		double dX, double dY, double dZ,
		int nRed, int nGreen, int nBlue,
		double dRadius, int nResolution = 50);
	
	int AddCone(
		const std::string& strWidgetName,
		double dX1, double dY1, double dZ1,
		double dX2, double dY2, double dZ2,
		int nRed, int nGreen, int nBlue,
		double dRadius, int nResolution = 50);

	int AddCube(
		const std::string& strWidgetName,
		double dX_1, double dY_1, double dZ_1,
		double dX_2, double dY_2, double dZ_2,
		int nRed, int nGreen, int nBlue);
		
	int AddLine(
		const std::string& strWidgetName,
		double dX_1, double dY_1, double dZ_1,
		double dX_2, double dY_2, double dZ_2, 
		int nRed, int nGreen, int nBlue,
		double dThickness);

	int AddText3D(
		const std::string& strWidgetName,
		double dX, double dY, double dZ, double dScale,
		const std::string& strText);

	int UpdateSphere(
		const std::string& strWidgetName,
		double dX, double dY, double dZ);
	
	int UpdateCone(
		const std::string& strWidgetName,
		double dX, double dY, double dZ);

	int UpdateText3D(
		const std::string& strWidgetName,
		double dX, double dY, double dZ, double dScale,
		const std::string& strText);

	int RemoveObject(const std::string& strWidgetName);

	int ResetCamera();

	int RunThread();

private:
	int viewerThread();

	int removeObject(const std::string& strWidgetName);
	int removeObjects();

private:
	std::mutex m_Mutex;
	std::thread m_ViewerThread;

	cv::Affine3f m_ptCamPose;
	std::shared_ptr<cv::viz::Viz3d> m_pViewport;

private:
	std::queue<std::string> m_queRemoveObjects;
};

