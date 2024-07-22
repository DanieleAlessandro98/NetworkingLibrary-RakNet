#include "StdAfx.h"
#include <iostream>
#include <slikenet/types.h>
#include <slikenet/MessageIdentifiers.h>
#include <Network/NetDevice.h>
#include <Network/Definition.h>
#include "ServerGame.h"
#include "Packet.h"
#include <Network/PacketIO.hpp>
#include "Peer.h"
#include "PeerManager.h"

using namespace Net;

ServerGame::ServerGame()
{
	InitializePacketHandler();
}

void ServerGame::LoadPacketHeaders()
{
	m_packetHeader->Set(PacketCGHeader::HEADER_CG_ACTION1, std::make_unique<PacketManager::TPacketType>(sizeof(TPacketCGAction1), &ServerGame::TestRecv));
}

void ServerGame::ProcessPacketError(Net::EProcessPacketError errorType, SLNet::Packet* packet, Net::CAbstractPeer* peer)
{
	switch (errorType)
	{
		case EProcessPacketError::HEADER_NOT_FOUND:
			std::cerr << "Header not found: " << (unsigned)packet->data[0] << std::endl;
			peer->SetPhase(PHASE_CLOSE);
			break;

		case Net::EProcessPacketError::SIZE_MISMATCH:
			std::cerr << "Size mismatch for header: " << (unsigned)packet->data[0] << std::endl;
			peer->SetPhase(PHASE_CLOSE);
			break;

		case Net::EProcessPacketError::HANDLE_FAILED:
			std::cerr << "Failed to handle packet with header: " << (unsigned)packet->data[0] << std::endl;
			break;
	}
}

bool ServerGame::TestRecv(SLNet::Packet* packet, Net::CAbstractPeer* peer)
{
	CPeer* d = CPeerManager::ValidPeer(peer);
	if (!d)
		return false;

	TPacketCGAction1 action1;
	if (!CPacketIO::ReadPacketData(packet, action1))
		return false;

	std::cout << "HEADER_CG_ACTION1 receved. num = " << action1.numIntero << std::endl;

	TestSend(packet, peer);
	return true;
}

bool ServerGame::TestSend(SLNet::Packet* packet, Net::CAbstractPeer* peer)
{
	if (!peer)
		return false;

	TPacketGCResponse response;
	peer->Packet(&response, sizeof(response));

	return true;
}
