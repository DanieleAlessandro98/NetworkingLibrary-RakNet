#pragma once

#include "AbstractPacketHandler.h"
#include "PacketManager.hpp"

namespace Net
{
    template<typename TDerived>
    class CAbstractPacketServerHandler : public CAbstractPacketServerHandlerBase
    {
        public:
            using PacketManager = CPacketManagerServer<TDerived>;

            virtual void ProcessPacketError(EProcessPacketError errorType, NetPacket* packet, CAbstractPeer* peer) = 0;

            CAbstractPacketServerHandler()
            {
                m_packetHeader = std::make_unique<PacketManager>();
            }

            void InitializePacketHandler()
            {
                static_cast<TDerived*>(this)->LoadPacketHeaders();
            }

            void Process(CAbstractPeer* peer, NetPacket* packet) override
            {
                ProcessPacket(static_cast<TDerived*>(this), *m_packetHeader, packet, peer);
            }

        protected:
            std::unique_ptr<PacketManager> m_packetHeader;
    };
}
