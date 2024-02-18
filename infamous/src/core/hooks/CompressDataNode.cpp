#include "stdafx.hpp"
#include "../hooks.hpp"
#include "core/core.hpp"

#include "rage/classes/network/netObject.hpp"
namespace Hooks {
	
	std::string GetVtableName(uint64_t table) {
		uint64_t object_locator = *(uint64_t*)(table - 8);
		if (object_locator) {
			uint32_t offset = *(uint32_t*)(object_locator + 0xC);
			if (offset) {
				const char* string = (const char*)(Core::g_GameAddress + offset + 0x10);
				if (string) {
					std::string str(string);
					if (str.find(".?A") == 0) {
						str = str.substr(4, str.length());
					}

					if (str.find("NodeCommonDataOperations") != std::string::npos) {
						str = str.substr(2, str.length());

						std::size_t at = str.find_first_of('@');
						if (at != std::string::npos) {
							std::string base_class = str.substr(at + 1);
							if (!base_class.empty()) {
								at = base_class.find_first_of("@@");
								if (at != std::string::npos) {
									str = base_class.substr(0, at);
									if (!str.empty()) {
										if (str.length() > 2) {
											if (str[0] == 'V' && str[1] == 'C') {
												str = "NodeCommonDataOperations<" + str.substr(1) + ">";
											}
										}
									}
								}
							}
						}

					}
					else if (str.find("AutoIdDesc__") != std::string::npos) {
						std::size_t at = str.find("AutoIdDesc__");
						if (at == 0) {
							at = str.find_first_of('@');
							if (at != std::string::npos) {
								str = str.substr(13);
								str = str.substr(0, str.find('@'));
							}
						}
						else {
							at = str.find("::AutoIdDesc__");
							if (at != std::string::npos) {
								str = str.substr(0, at);
							}
						}
					}
					else {
						std::size_t at = str.find_first_of('@');
						if (at > 0 && at < 0xffff) {
							str = str.substr(0, at);
						}
					}

					return str;
				}

				return string;
			}
		}

		return "";
	}

	void HandleNode(DataNode* Node, NetObjectBase* Object, uint32_t nodeName) {
		switch (nodeName) {
		case joaat("CPedMovementDataNode"): {
			NetObjectBase* node_accessor_object = Caller::Call<NetObjectBase*>(*(uint64_t*)(Object->m_vtable + 0x8), Object, 5);
			Caller::Call<void>(*(uint64_t*)(node_accessor_object->m_vtable + 0x58), node_accessor_object, Node->m_DataNodeCommonOperations->m_Data);

			*(bool*)(Node->m_DataNodeCommonOperations->m_Data + 0xC0) = *(float*)(Node->m_DataNodeCommonOperations->m_Data + 0xC4) != 0.0f;
			*(bool*)(Node->m_DataNodeCommonOperations->m_Data + 0xC1) = *(float*)(Node->m_DataNodeCommonOperations->m_Data + 0xC8) != 0.0f;

			//if (network::spoofing::vars::m_vars.m_movement_animation) {
				if (*Patterns::Vars::g_PedFactory) {
					if ((*Patterns::Vars::g_PedFactory)->m_local_ped) {
						if ((*Patterns::Vars::g_PedFactory)->m_local_ped->m_net_object) {
							if (Object->m_network_id == (*Patterns::Vars::g_PedFactory)->m_local_ped->m_net_object->m_object_id) {
								*(bool*)(Node->m_DataNodeCommonOperations->m_Data + 0xC0) = false;
								*(bool*)(Node->m_DataNodeCommonOperations->m_Data + 0xC1) = false;
							}
						}
					}
				}
			//}

			break;
		}
		}
	}

	void NodeForeach(DataNode* node, std::function<void(std::string, DataNode*)> callback) {
		if (IsValidPtr(node)) {
			DataNode* current = node;
			while (IsValidPtr(current) && IsValidPtr(*(uint64_t*)((uint64_t)current))) {
				if (current->IsDataNode()) {
					if (current->m_DataNodeCommonOperations) {
						if (current->m_DataNodeCommonOperations->m_Data) {
							std::string name = GetVtableName(*(uint64_t*)((uint64_t)current));
							if (!name.empty()) {
								callback(name, current);
							}
						}
					}
				}

				if (current->IsParentNode()) {
					if (current->m_ChildCount > 0) {
						DataNode* children = current->m_FirstChild;
						if (IsValidPtr(children)) {
							NodeForeach(children, callback);
						}
					}
				}

				current = current->m_Next;
			}
		}
	}

	void HandleNodeCaller(DataNode* Node, NetObjectBase* Object, bool Recursive) {
		if (Recursive) {
			NodeForeach(Node, [=](std::string name, DataNode* child) {
				HandleNode(child, Object, Native::GetHashKey(name.c_str()));
				});
		}
		else {
			uint64_t vtable = *(uint64_t*)((uint64_t)Node);
			HandleNode(Node, Object, Native::GetHashKey(GetVtableName(vtable).c_str()));
		}
	}

	
	//can be used for crashes
	bool CompressDataNodeHook(DataNode* node, uint32_t size, CNetGamePlayer* sender, CNetGamePlayer* receiver, Rage::datBitBuffer* buffer, int bufferSize, uint64_t a7, bool a8, uint32_t* a9, uint32_t* a10) {
		
		//HandleNodeCaller(node, Object, true);
		return CompressDataNodeHook(node, size, sender, receiver, buffer, bufferSize, a7, a8, a9, a10);
	}

	
	bool CompressDataNodeHook2(DataNode* Node, NetObjectBase* Object) {
		uint64_t rcx = (uint64_t)Node;
		uint64_t rcx_vtable = *(uint64_t*)rcx;

		if (!Caller::Call<bool>(*(uint64_t*)(rcx_vtable + 0xD0), rcx, 0))
			return false;

		uint64_t v4 = Caller::Call<uint64_t>(*(uint64_t*)(Object->m_vtable + 0x20), Object);
		uint64_t v5 = Caller::Call<uint64_t>(*(uint64_t*)(*(uint64_t*)(v4)+0x8), v4);

		if (!v5)
			return false;

		HandleNodeCaller(Node, Object, false);
		return CompressDataNodeHook2(Node, Object);
	}
}