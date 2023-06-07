//
// Created by robcholz on 5/19/23.
//

#ifndef RUNCRAFT_INPUT_HPP
#define RUNCRAFT_INPUT_HPP

#include <string>
#include <fstream>
#include <list>
#include <map>
#include <SFML/Window/Event.hpp>
#include "json.hpp"

namespace input {
	enum class MouseKeyType {
		Left,       ///< The left mouse button
		Right,      ///< The right mouse button
		Middle,     ///< The middle (wheel) mouse button
		XButton1,   ///< The first extra mouse button
		XButton2,   ///< The second extra mouse button
		VerticalWheel,  ///< The vertical mouse wheel
		HorizontalWheel ///< The horizontal mouse wheel
	};
	using KeyboardKeyType = sf::Keyboard::Key;

	namespace peripherals {
		class IPeripheralObserver {
		public:
			virtual ~IPeripheralObserver() = default;
		};

		class IPeripheralSubject {
		public:
			virtual ~IPeripheralSubject() = default;

			virtual void attach(IPeripheralObserver* observer) = 0;

			virtual void detach(IPeripheralObserver* observer) = 0;
		};

		template<class Key, typename KeyType>
		class Peripherals {
		protected:
			using String = std::string;
			std::map<KeyType, Key*> loadedPeripheralsList;
		public:
			Key* addKey(KeyType keyType) {
				auto key = new Key(keyType);
				loadedPeripheralsList.insert({keyType, key});
				loadedPeripheralsList[keyType] = key;
				return key;
			}

			Key* getKey(KeyType keyType) {
				return loadedPeripheralsList[keyType];
			}
		};
	}

	namespace keyboard {
		using namespace peripherals;

		class KeyboardKey {
		private:
			KeyboardKeyType toggleKey{};
			bool pressed = false;
		public:
			explicit KeyboardKey(KeyboardKeyType key) {
				toggleKey = key;
			}

			[[nodiscard]] KeyboardKeyType getKey() const { return toggleKey; }

			[[nodiscard]] bool isPressed() const { return pressed; }

			void update() { pressed = sf::Keyboard::isKeyPressed(toggleKey); }

			~KeyboardKey() = default;
		};

		class Keyboard : public Peripherals<KeyboardKey, KeyboardKeyType>, public IPeripheralSubject {
		private:
			std::list<IPeripheralObserver*> peripheralsMonitorList;
		public:
			explicit Keyboard() = default;
			void update() {
				for (auto v: loadedPeripheralsList) { v.second->update(); }
			}

			void attach(IPeripheralObserver* observer) override { peripheralsMonitorList.push_back(observer); }
			void detach(IPeripheralObserver* observer) override { peripheralsMonitorList.remove(observer); }
		};
	}

	namespace mouse {
		class MouseKey {
		private:
			std::string keyName;
			MouseKeyType toggleKey{};
		public:
			explicit MouseKey(std::string name, MouseKeyType key) {
				keyName = std::move(name);
				toggleKey = key;
			}

			[[nodiscard]] std::string getName() const { return keyName; }

			void setKey(MouseKeyType key) { toggleKey = key; }

			[[nodiscard]] MouseKeyType getKey() const { return toggleKey; }

			[[nodiscard]] bool isAnalog() const {
				return toggleKey == MouseKeyType::HorizontalWheel || toggleKey == MouseKeyType::VerticalWheel;
			}

			[[nodiscard]] bool isPressed() const {
				if (!isAnalog())
					return sf::Mouse::isButtonPressed(static_cast<sf::Mouse::Button>(toggleKey));
				return false;
			}

			//TODO forget what to do here...
		};

		class Mouse : protected input::peripherals::Peripherals<MouseKey, MouseKeyType> {
		private:

		public:
			explicit Mouse() {

			}
		};
	}

	using Keyboard = std::shared_ptr<keyboard::Keyboard>;
	using Mouse = std::shared_ptr<mouse::Mouse>;

	class PeripheralsFactory {
	private:
		template<class T>
		static std::shared_ptr<T> getInstance() {
			static std::shared_ptr<T> instance(new T);
			return instance;
		}

	public:
		PeripheralsFactory() = delete;

		~PeripheralsFactory() = delete;

		static Keyboard getKeyboard() {
			return getInstance<keyboard::Keyboard>();
		}

		static Mouse getMouse() {
			return getInstance<mouse::Mouse>();
		}
	};

	class KeyboardObserver : public peripherals::IPeripheralObserver {
	private:
		std::list<keyboard::KeyboardKey*> hotKeyList;
		Keyboard keyboard = PeripheralsFactory::getKeyboard();
	public:
		KeyboardObserver() {
			keyboard->attach(this);
		}

		KeyboardObserver &attachKey(KeyboardKeyType keyType) {
			auto key = keyboard->getKey(keyType);
			if (key == nullptr)
				hotKeyList.push_front(keyboard->addKey(keyType));
			else
				hotKeyList.push_front(key);
			return *this;
		}

		[[nodiscard]] bool isActivated() const {
			return std::all_of(hotKeyList.cbegin(), hotKeyList.cend(), [](keyboard::KeyboardKey* key) { return key->isPressed(); });
		}

		~KeyboardObserver() override {
			keyboard->detach(this);
		}
	};

	class MouseObserver{
		//TODO
	};
}


#endif //RUNCRAFT_INPUT_HPP
