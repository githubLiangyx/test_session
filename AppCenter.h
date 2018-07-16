#pragma once
#include <boost/asio.hpp>
#include <memory>

class CAppCenter
	: public std::enable_shared_from_this<CAppCenter>
{
public:
	CAppCenter(const unsigned short port);

	bool run();

	void stop();

private:
	void onConnectd(boost::system::error_code ec);

	void do_init();

private:
	boost::asio::io_service _ios;
	boost::asio::ip::tcp::acceptor _acceptor;
	boost::asio::ip::tcp::socket _sockT;
	bool _isRunning;
};

typedef std::shared_ptr<CAppCenter> CAppCenterPtr;