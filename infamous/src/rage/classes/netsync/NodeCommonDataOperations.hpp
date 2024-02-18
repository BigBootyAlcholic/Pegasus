#pragma once

namespace Rage
{
	class datBitBuffer;
	class netSyncDataNode;
}

class NodeCommonDataOperations
{
public:
	virtual ~NodeCommonDataOperations() = default;
	virtual void ReadFromBuffer(Rage::netSyncDataNode* node) {};            // 0x08
	virtual void WriteToBuffer(Rage::netSyncDataNode* node) {};             // 0x10
	virtual void Unk() {};                                                  // 0x18
	virtual int CalculateSize(Rage::netSyncDataNode* node) { return 0; };   // 0x20
	virtual int CalculateSize2(Rage::netSyncDataNode* node) { return 0; };  // 0x28
	virtual void LogSyncData(Rage::netSyncDataNode* node) {};               // 0x30
	virtual void LogSyncData2(Rage::netSyncDataNode* node) {};              // 0x38

	Rage::datBitBuffer* m_buffer; // 0x8
};
static_assert(sizeof(NodeCommonDataOperations) == 0x10);