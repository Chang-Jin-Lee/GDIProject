#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include "NetworkSession.h"

#include <winsock2.h>
#include <ws2tcpip.h>

#include <algorithm>

namespace
{
constexpr SOCKET InvalidSocket = INVALID_SOCKET;

SOCKET ToSocket(unsigned long long value)
{
	return static_cast<SOCKET>(value);
}

unsigned long long FromSocket(SOCKET value)
{
	return static_cast<unsigned long long>(value);
}
}

NetworkSession::NetworkSession()
{
}

NetworkSession::~NetworkSession()
{
	Shutdown();
}

bool NetworkSession::EnsureStarted()
{
	if (m_started)
	{
		return true;
	}

	WSADATA data;
	const int result = WSAStartup(MAKEWORD(2, 2), &data);
	if (result != 0)
	{
		m_statusText = "WSAStartup failed";
		return false;
	}

	m_started = true;
	return true;
}

void NetworkSession::SetNonBlocking(unsigned long long socketValue)
{
	u_long mode = 1;
	ioctlsocket(ToSocket(socketValue), FIONBIO, &mode);
}

bool NetworkSession::Host(unsigned short port)
{
	Shutdown();
	if (!EnsureStarted())
	{
		return false;
	}

	SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listenSocket == InvalidSocket)
	{
		m_statusText = "socket failed";
		return false;
	}

	sockaddr_in address{};
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	address.sin_port = htons(port);

	if (bind(listenSocket, reinterpret_cast<sockaddr*>(&address), sizeof(address)) == SOCKET_ERROR)
	{
		closesocket(listenSocket);
		m_statusText = "bind failed";
		return false;
	}

	if (listen(listenSocket, 1) == SOCKET_ERROR)
	{
		closesocket(listenSocket);
		m_statusText = "listen failed";
		return false;
	}

	m_listenSocket = FromSocket(listenSocket);
	SetNonBlocking(m_listenSocket);
	m_role = ENetworkSessionRole::Host;
	m_listening = true;
	m_connected = false;
	m_statusText = "Waiting for client";
	return true;
}

bool NetworkSession::Connect(const std::string& ipAddress, unsigned short port)
{
	Shutdown();
	if (!EnsureStarted())
	{
		return false;
	}

	SOCKET peerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (peerSocket == InvalidSocket)
	{
		m_statusText = "socket failed";
		return false;
	}

	m_peerSocket = FromSocket(peerSocket);
	SetNonBlocking(m_peerSocket);

	sockaddr_in address{};
	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	if (inet_pton(AF_INET, ipAddress.c_str(), &address.sin_addr) != 1)
	{
		Shutdown();
		m_statusText = "invalid ip";
		return false;
	}

	const int result = connect(peerSocket, reinterpret_cast<sockaddr*>(&address), sizeof(address));
	if (result == SOCKET_ERROR)
	{
		const int error = WSAGetLastError();
		if (error != WSAEWOULDBLOCK && error != WSAEINPROGRESS)
		{
			Shutdown();
			m_statusText = "connect failed";
			return false;
		}
		m_connecting = true;
	}
	else
	{
		m_connected = true;
	}

	m_role = ENetworkSessionRole::Client;
	m_statusText = m_connected ? "Connected" : "Connecting";
	return true;
}

void NetworkSession::Poll()
{
	if (m_role == ENetworkSessionRole::Host && m_listening && !m_connected)
	{
		sockaddr_in clientAddress{};
		int addressSize = sizeof(clientAddress);
		SOCKET accepted = accept(ToSocket(m_listenSocket), reinterpret_cast<sockaddr*>(&clientAddress), &addressSize);
		if (accepted != InvalidSocket)
		{
			m_peerSocket = FromSocket(accepted);
			SetNonBlocking(m_peerSocket);
			m_connected = true;
			m_statusText = "Client connected";
		}
	}

	if (m_role == ENetworkSessionRole::Client && m_connecting)
	{
		TryFinishClientConnect();
	}

	if (m_connected)
	{
		ReceivePending();
	}
}

void NetworkSession::TryFinishClientConnect()
{
	fd_set writeSet;
	FD_ZERO(&writeSet);
	FD_SET(ToSocket(m_peerSocket), &writeSet);
	timeval timeout{};
	const int ready = select(0, nullptr, &writeSet, nullptr, &timeout);
	if (ready <= 0)
	{
		return;
	}

	int socketError = 0;
	int optionLength = sizeof(socketError);
	getsockopt(ToSocket(m_peerSocket), SOL_SOCKET, SO_ERROR, reinterpret_cast<char*>(&socketError), &optionLength);
	if (socketError == 0)
	{
		m_connecting = false;
		m_connected = true;
		m_statusText = "Connected";
	}
	else
	{
		Shutdown();
		m_statusText = "connect failed";
	}
}

void NetworkSession::ReceivePending()
{
	char buffer[4096];
	while (true)
	{
		const int received = recv(ToSocket(m_peerSocket), buffer, sizeof(buffer), 0);
		if (received > 0)
		{
			m_receiveBuffer.append(buffer, buffer + received);
			size_t newline = std::string::npos;
			while ((newline = m_receiveBuffer.find('\n')) != std::string::npos)
			{
				std::string line = m_receiveBuffer.substr(0, newline);
				if (!line.empty() && line.back() == '\r')
				{
					line.pop_back();
				}
				m_lines.push_back(line);
				m_receiveBuffer.erase(0, newline + 1);
			}
			continue;
		}

		if (received == 0)
		{
			Shutdown();
			m_statusText = "peer closed";
			return;
		}

		const int error = WSAGetLastError();
		if (error == WSAEWOULDBLOCK)
		{
			return;
		}
		Shutdown();
		m_statusText = "recv failed";
		return;
	}
}

bool NetworkSession::SendLine(const std::string& line)
{
	if (!m_connected)
	{
		return false;
	}

	std::string payload = line;
	payload.push_back('\n');
	const char* data = payload.c_str();
	int remaining = static_cast<int>(payload.size());
	while (remaining > 0)
	{
		const int sent = send(ToSocket(m_peerSocket), data, remaining, 0);
		if (sent == SOCKET_ERROR)
		{
			const int error = WSAGetLastError();
			if (error == WSAEWOULDBLOCK)
			{
				return false;
			}
			Shutdown();
			m_statusText = "send failed";
			return false;
		}
		data += sent;
		remaining -= sent;
	}
	return true;
}

bool NetworkSession::PopLine(std::string& outLine)
{
	if (m_lines.empty())
	{
		return false;
	}
	outLine = m_lines.front();
	m_lines.erase(m_lines.begin());
	return true;
}

void NetworkSession::Shutdown()
{
	if (m_peerSocket != ~0ull)
	{
		closesocket(ToSocket(m_peerSocket));
		m_peerSocket = ~0ull;
	}
	if (m_listenSocket != ~0ull)
	{
		closesocket(ToSocket(m_listenSocket));
		m_listenSocket = ~0ull;
	}

	m_connected = false;
	m_listening = false;
	m_connecting = false;
	m_receiveBuffer.clear();
	m_lines.clear();
	if (m_started)
	{
		WSACleanup();
	}
	m_started = false;
	m_role = ENetworkSessionRole::None;
}
