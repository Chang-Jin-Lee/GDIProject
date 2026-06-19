#pragma once

#include <string>
#include <vector>

enum class ENetworkSessionRole
{
	None,
	Host,
	Client,
};

class NetworkSession
{
public:
	NetworkSession();
	~NetworkSession();

	bool Host(unsigned short port);
	bool Connect(const std::string& ipAddress, unsigned short port);
	void Poll();
	bool SendLine(const std::string& line);
	bool PopLine(std::string& outLine);
	void Shutdown();

	bool IsConnected() const { return m_connected; }
	bool IsListening() const { return m_listening; }
	ENetworkSessionRole GetRole() const { return m_role; }
	const std::string& GetStatusText() const { return m_statusText; }

private:
	bool EnsureStarted();
	void SetNonBlocking(unsigned long long socketValue);
	void ReceivePending();
	void TryFinishClientConnect();

private:
	ENetworkSessionRole m_role = ENetworkSessionRole::None;
	unsigned long long m_listenSocket = ~0ull;
	unsigned long long m_peerSocket = ~0ull;
	bool m_started = false;
	bool m_connected = false;
	bool m_listening = false;
	bool m_connecting = false;
	std::string m_statusText;
	std::string m_receiveBuffer;
	std::vector<std::string> m_lines;
};
