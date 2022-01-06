#include "VtkViewer.h"

VtkViewer::VtkViewer(
	const std::string& strWindowName,
	int nWidth, int nHeight, int nWindowPositionX, int nWindowPositionY)
{
	m_strWindowName = strWindowName;

	m_nWidth = nWidth;
	m_nHeight = nHeight;
	m_nWindowPositionX = nWindowPositionX;
	m_nWindowPositionY = nWindowPositionY;

	Init();
};

VtkViewer::~VtkViewer()
{
	if (m_ViewerThread.joinable())
	{
		m_ViewerThread.join();
	}
}

int VtkViewer::Init()
{
	m_pViewport = std::make_shared<cv::viz::Viz3d>(m_strWindowName);

	m_pViewport->setWindowPosition(cv::Point(m_nWidth, m_nHeight));
	m_pViewport->setWindowSize(cv::Size(m_nWindowPositionX, m_nWindowPositionY));
	m_pViewport->setBackgroundColor(); // black by default 

	m_pViewport->showWidget(m_strWindowName, cv::viz::WCoordinateSystem(1.f));

	return 0;
}

int VtkViewer::AddSphere(
	const std::string& strWidgetName,
	double dX, double dY, double dZ,
	int nRed, int nGreen, int nBlue,
	double dRadius, int nResolution)
{
	m_Mutex.lock();
	LM_RESULT lmRet = AddData(strWidgetName, "Sphere");
	if (lmRet == LM_RESULT::ERROR_DATA_ALREADY_EXIST)
	{
		m_Mutex.unlock();
		return static_cast<int>(LM_RESULT::ERROR_DATA_ALREADY_EXIST);
	}

	cv::viz::WSphere wSphere(
		cv::Point3d(dX, dY, dZ), dRadius, nResolution,
		cv::viz::Color::Color(nBlue, nGreen, nRed));

	m_pViewport->showWidget(strWidgetName, wSphere);
	m_Mutex.unlock();

	return 0;
}

int VtkViewer::AddCone(
	const std::string& strWidgetName,
	double dX1, double dY1, double dZ1,
	double dX2, double dY2, double dZ2,
	int nRed, int nGreen, int nBlue,
	double dRadius, int nResolution)
{
	m_Mutex.lock();
	LM_RESULT lmRet = AddData(strWidgetName, "Cone");
	if (lmRet == LM_RESULT::ERROR_DATA_ALREADY_EXIST)
	{
		m_Mutex.unlock();
		return static_cast<int>(LM_RESULT::ERROR_DATA_ALREADY_EXIST);
	}

	cv::viz::WCone wCone(
		dRadius,
		cv::Point3d(dX1, dY1, dZ1), 
		cv::Point3d(dX2, dY2, dZ2), 
		500,
		cv::viz::Color::Color(nBlue, nGreen, nRed));

	m_pViewport->showWidget(strWidgetName, wCone);
	m_Mutex.unlock();

	return 0;
}

int VtkViewer::AddCube(
	const std::string& strWidgetName,
	double dX_1, double dY_1, double dZ_1,
	double dX_2, double dY_2, double dZ_2,
	int nRed, int nGreen, int nBlue)
{
	m_Mutex.lock();
	LM_RESULT lmRet = AddData(strWidgetName, "Cube");
	if (lmRet == LM_RESULT::ERROR_DATA_ALREADY_EXIST)
	{
		m_Mutex.unlock();
		return static_cast<int>(LM_RESULT::ERROR_DATA_ALREADY_EXIST);
	}

	cv::viz::WCube wCubeWidget(
		cv::Point3d(dX_1, dY_1, dZ_1),
		cv::Point3d(dX_2, dY_2, dZ_2),
		true, cv::viz::Color::Color(nBlue, nGreen, nRed));

	m_pViewport->showWidget(strWidgetName, wCubeWidget);
	m_Mutex.unlock();

	return 0;
}

int VtkViewer::AddLine(
	const std::string& strWidgetName,
	double dX_1, double dY_1, double dZ_1,
	double dX_2, double dY_2, double dZ_2,
	int nRed, int nGreen, int nBlue,
	double dThickness)
{
	m_Mutex.lock();
	LM_RESULT lmRet = AddData(strWidgetName, "Line");
	if (lmRet == LM_RESULT::ERROR_DATA_ALREADY_EXIST)
	{
		m_Mutex.unlock();
		return static_cast<int>(LM_RESULT::ERROR_DATA_ALREADY_EXIST);
	}

	cv::viz::WLine wLineWidget(
		cv::Point3d(dX_1, dY_1, dZ_1),
		cv::Point3d(dX_2, dY_2, dZ_2),
		cv::viz::Color::Color(nBlue, nGreen, nRed));
		
	wLineWidget.setRenderingProperty(cv::viz::LINE_WIDTH, dThickness);

	m_pViewport->showWidget(strWidgetName, wLineWidget);
	m_Mutex.unlock();

	return 0;
}

int VtkViewer::AddText3D(
	const std::string& strWidgetName,
	double dX, double dY, double dZ, double dScale,
	const std::string& strText)
{
	m_Mutex.lock();
	LM_RESULT lmRet = AddData(strWidgetName, "Text3D");
	if (lmRet == LM_RESULT::ERROR_DATA_ALREADY_EXIST)
	{
		m_Mutex.unlock();
		return static_cast<int>(LM_RESULT::ERROR_DATA_ALREADY_EXIST);
	}

	cv::viz::WText3D wText3D = cv::viz::WText3D(
		strText,
		cv::Point3d(dX, dY, dZ - dScale * 2),
		dScale,
		true,
		cv::viz::Color::white());

	m_pViewport->showWidget(strWidgetName, wText3D);
	m_Mutex.unlock();

	return 0;
}

int VtkViewer::UpdateCone(
	const std::string& strWidgetName,
	double dX, double dY, double dZ)
{
	m_Mutex.lock();
	cv::Affine3d wCone= m_pViewport->getWidgetPose(strWidgetName);
	cv::Vec3d rvec = wCone.rvec();
	cv::Vec3d tvec = cv::Vec3d(dX, dY, dZ);
	wCone = cv::Affine3d(rvec, tvec);

	m_pViewport->setWidgetPose(strWidgetName, wCone);
	m_Mutex.unlock();

	return 0;
}

int VtkViewer::UpdateSphere(
	const std::string& strWidgetName,
	double dX, double dY, double dZ)
{
	m_Mutex.lock();
	cv::Affine3d wSpherePose = m_pViewport->getWidgetPose(strWidgetName);
	cv::Vec3d rvec = wSpherePose.rvec();
	cv::Vec3d tvec = cv::Vec3d(dX, dY, dZ);
	wSpherePose = cv::Affine3d(rvec, tvec);

	m_pViewport->setWidgetPose(strWidgetName, wSpherePose);
	m_Mutex.unlock();

	return 0;
}

int VtkViewer::UpdateText3D(
	const std::string& strWidgetName,
	double dX, double dY, double dZ, double dScale,
	const std::string& strText)
{
	m_Mutex.lock();
	cv::Affine3d wSpherePose = m_pViewport->getWidgetPose(strWidgetName);
	cv::Vec3d rvec = wSpherePose.rvec();
	cv::Vec3d tvec = cv::Vec3d(dX, dY, dZ - dScale*2);
	wSpherePose = cv::Affine3d(rvec, tvec);

	m_pViewport->setWidgetPose(strWidgetName, wSpherePose);
	m_pViewport->getWidget(strWidgetName).cast<cv::viz::WText3D>().setText(strText);
	m_Mutex.unlock();

	return 0;
}

int VtkViewer::RemoveObject(const std::string& strWidgetName)
{
	m_Mutex.lock();
	m_queRemoveObjects.push(strWidgetName);
	m_Mutex.unlock();

	return 0;
}

int VtkViewer::ResetCamera()
{
	m_Mutex.lock();
	m_pViewport->resetCamera();
	m_Mutex.unlock();

	return 0;
}

int VtkViewer::removeObject(const std::string& strWidgetName)
{
	m_Mutex.lock();
	LM_RESULT lmRet = IsExistData(strWidgetName);
	if (lmRet != LM_RESULT::OK)
	{
		m_Mutex.unlock();
		return static_cast<int>(LM_RESULT::ERROR_DATA_IS_NOT_EXIST);
	}

	m_pViewport->removeWidget(strWidgetName);
	RemoveDataByKey(strWidgetName);
	m_Mutex.unlock();

	return 0;
}

int VtkViewer::removeObjects()
{
	while (!m_queRemoveObjects.empty())
	{
		removeObject(m_queRemoveObjects.front());
		
		m_Mutex.lock();
		m_queRemoveObjects.pop();
		m_Mutex.unlock();
	}
	return 0;
}

int VtkViewer::RunThread()
{
	m_ViewerThread = std::thread(&VtkViewer::viewerThread, this);

	m_ViewerThread.detach();

	return 0;
}

int VtkViewer::viewerThread()
{
	while (!m_pViewport->wasStopped())
	{
		m_Mutex.lock();
		m_pViewport->spinOnce(1, true);
		m_Mutex.unlock();

		removeObjects();
		Sleep(1);
	}

	return 0;
}