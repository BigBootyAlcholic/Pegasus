#pragma once
#include "abstract_option.hpp"
#include "abstract_submenu.hpp"

namespace Framework::Options {
	template <typename T>
	class BaseSubmenu : public AbstractSubmenu {
	public:
		const char* GetName() override {
			return &m_name[0];
		}

		uint32_t GetID() override {
			return m_id;
		}

		void Execute() override {
			if (m_action)
				std::invoke(m_action, static_cast<T*>(this));
			if (m_selected_option >= m_options.size())
				m_selected_option = 0;
		}

		void Reset() override {
			m_options.clear();
		}

		AbstractOption* GetOption(std::size_t i) override {
			if (i > m_options.size())
				return nullptr;
			return m_options[i].get();
		}

		std::size_t GetOptionsSize() override {
			return m_options.size();
		}

		std::size_t GetSelectedOption() override {
			return m_selected_option;
		}

		void SetSelectedOption(std::size_t n) override {
			m_selected_option = n;
		}

		template<typename T>
		inline BaseSubmenu& addOption(T&& option) {
			auto ptr = std::make_unique<std::remove_reference_t<T>>(option);
			m_options.push_back(std::move(ptr));
			return *this;
		}

		T& SetName(const char* name) {
			std::strncpy(&m_name[0], name, sizeof(m_name) - 1);
			return static_cast<T&>(*this);
		}

		T& SetID(std::uint32_t id) {
			m_id = id;
			return static_cast<T&>(*this);
		}

		T& SetAction(std::function<void(T*)> action) {
			m_action = std::move(action);
			return static_cast<T&>(*this);
		}

	protected:
		char m_name[64] = {};
		uint32_t m_id{};
		std::function<void(T*)> m_action;

		std::vector<std::unique_ptr<AbstractOption>> m_options;
		std::size_t m_selected_option{};
	};

}