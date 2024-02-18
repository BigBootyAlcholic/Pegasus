#pragma once
#include "submenu.hpp"
#include "rage/classes/rage/joaat.hpp"
#include "rage/invoker/natives.hpp"
#include "cheat/util/player_manager.hpp"
namespace Framework::Options {
	class Core : public BaseSubmenu<Core> {
	public:
		explicit Core(const char* name, const char* target, std::function<void(Core*)> action) {
			SetName(name); SetID(Rage::LowerCaseJoaat(target)); SetAction(std::move(action));
		}
	};

	class PCore : public BaseSubmenu<PCore> {
	public:
		explicit PCore(uint32_t* player, const char* target, std::function<void(PCore*)> action) :
			m_selected_player(player)
		{
			SetID(Rage::LowerCaseJoaat(target));
			SetAction(std::move(action));
		}
		const char* GetName() override {
			return Native::NetworkPlayerGetName(*m_selected_player);
		}
	private:
		uint32_t* m_selected_player;
	};



	class VCore : public BaseSubmenu<VCore> {
	public:
		explicit VCore(int* vehicle, uint32_t id, std::function<void(VCore*)> action) :
			m_selected_vehicle(vehicle)
		{
			SetID(id);
			SetAction(std::move(action));
		}
		const char* GetName() override {
			return get_vehicle_class_name(*m_selected_vehicle);
		}
	private:
		int* m_selected_vehicle;
	};

}