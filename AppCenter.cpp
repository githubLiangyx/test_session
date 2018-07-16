#include "AppCenter.h"
#include "session/session.h"
#include "db/dbbaseManager.h"

CAppCenter::CAppCenter(const unsigned short port)
	:_acceptor(_ios, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)), _sockT(_ios)
{
	_isRunning = false;
}

bool CAppCenter::run()
{
	if (_isRunning)
	{
		return false;
	}
	_isRunning = true;
	if (!_acceptor.is_open())
	{
		// acceptor not open, maybe the socket has been used;
		return false;
	}
	//初始化
	try
	{
		do_init();
		_acceptor.async_accept(_sockT, std::bind(&CAppCenter::onConnectd, shared_from_this(), std::placeholders::_1));
		_ios.run();
		return true;
	}
	catch (const std::exception& e)
	{
		std::string msg = e.what();
		// show some message in here 
	}
	return false;
}

void CAppCenter::stop()
{
	_ios.stop();
}

void CAppCenter::onConnectd(boost::system::error_code ec)
{
	if (!ec)
	{
		//开一个端
		CSessionPtr(new session(std::move(_sockT)))->start();
	}
	else
	{
		//out put ec.message();
	}

	//继续监听
	_acceptor.async_accept(_sockT, std::bind(&CAppCenter::onConnectd, shared_from_this(), std::placeholders::_1));
}

void CAppCenter::do_init()
{
	CDispatchEventMgr::start();
	CDBbaseManager::start();
}
