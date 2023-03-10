/*
 * iCTPMin期货行情交易(开源)
 * 上海卷卷猫信息技术有限公司
 */

#include "stdafx.h"
#include "MD.h"

MD::MD( CThostFtdcMdApi *pUserApi )
:m_pUserApi(pUserApi)
{
	m_mt = 0;
}

MD::~MD()
{
	m_mt = 0;
}

MT* MD::GetMT()
{
	return m_mt;
}

void MD::SetMT(MT* mt)
{
	m_mt = mt;
}

CThostFtdcMdApi* MD::GetUserApi()
{
	return m_pUserApi;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MD::OnFrontConnected()
{
	printf("CallBack:连接行情IP成功，开始登录\r\n");
	CThostFtdcReqUserLoginField reqUserLogin; 
	strcpy(reqUserLogin.BrokerID, m_mt->GetBrokerID().c_str()); 
	strcpy(reqUserLogin.UserID, m_mt->GetInvestorID().c_str()); 
	strcpy(reqUserLogin.Password, m_mt->GetPassword().c_str());
	m_pUserApi->ReqUserLogin(&reqUserLogin, 0); 
}

void MD::OnFrontDisconnected(int nReason)
{
	m_mt->SetMdLogined(false);
}

void MD::OnHeartBeatWarning(int mTimeLapse)
{
}

void MD::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	m_mt->SetMdLogined(true);
	printf("CallBack:行情登录成功\r\n");
	m_mt->ReSubMarketData(0);
}

void MD::OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	m_mt->SetMdLogined(false);
}

void MD::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
}

void MD::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
}

void MD::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
}

void MD::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
	m_mt->AddInsID(pDepthMarketData->InstrumentID);

	CTPListener *listener = m_mt->GetListener();
	if(listener)
	{
		listener->OnDepthMarketDatasCallBack(pDepthMarketData);
	}
}
