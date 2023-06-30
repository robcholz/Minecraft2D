//
// Created by robcholz on 5/19/23.
//

#ifndef RUNCRAFT_INPUT_HPP
#define RUNCRAFT_INPUT_HPP

#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <SFML/Window/Event.hpp>
#include "json.hpp"
#include "events/SystemEvents.hpp"

namespace input {
	enum class MouseKeyType {
		Left = sf::Mouse::Button::Left,      ///< The left mouse button
		Right = sf::Mouse::Button::Right,      ///< The right mouse button
		Middle = sf::Mouse::Button::Middle,     ///< The middle (wheel) mouse button
		XButton1 = sf::Mouse::Button::XButton1,   ///< The first extra mouse button
		XButton2 = sf::Mouse::Button::XButton2,   ///< The second extra mouse button
		VerticalWheel,  ///< The vertical mouse wheel
		HorizontalWheel ///< The horizontal mouse wheel
	};
	using KeyboardKeyType = sf::Keyboard::Key;

	namespace peripherals {
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

			virtual void update() {
				for (auto v: loadedPeripheralsList) { v.second->update(); }
			}
		};

		template<typename KeyType>
		class Key {
		protected:
			bool pressed = false;
			KeyType toggleKey{};
		public:
			explicit Key(KeyType key) { toggleKey = key; }

			[[nodiscard]] KeyType getKey() const { return toggleKey; }

			virtual void update() = 0;

			~Key() = default;

			[[nodiscard]] bool isPressed() const { return pressed; }
		};
	}

	namespace keyboard {

		class KeyboardKey : public peripherals::Key<KeyboardKeyType> {
		public:
			explicit KeyboardKey(KeyboardKeyType key) : Key<KeyboardKeyType>(key) {}

			void update() override { pressed = sf::Keyboard::isKeyPressed(toggleKey); }

			~KeyboardKey() = default;
		};

		class Keyboard : public peripherals::Peripherals<KeyboardKey, KeyboardKeyType> {
		public:
			explicit Keyboard() = default;

			~Keyboard() = default;
		};
	}

	namespace mouse {
		class MouseKey : public peripherals::Key<MouseKeyType> {
		private:
			std::shared_ptr<SystemEvents> event = SystemEvents::getInstance();
			struct Wheel {
				float delta;
				float change;
			} wheel{};
		public:
			explicit MouseKey(MouseKeyType key) : Key<MouseKeyType>(key) {}

			[[nodiscard]] bool isAnalog() const {
				return getKey() == MouseKeyType::HorizontalWheel || getKey() == MouseKeyType::VerticalWheel;
			}

			[[nodiscard]] Wheel getWheel() const {
				return wheel;
			}

			void update() override {
				if (isAnalog()) {
					if (event->getEvent()->type == sf::Event::MouseWheelScrolled)
						wheel.delta = event->getEvent()->mouseWheelScroll.delta;
				} else {
					pressed = sf::Mouse::isButtonPressed(static_cast<sf::Mouse::Button>(toggleKey));
				}
			}
		};

		class Mouse : public input::peripherals::Peripherals<MouseKey, MouseKeyType> {
		private:
			sf::RenderWindow* window = GameInfo.getRender()->getWindowConfig().window;
			sf::Vector2i position{};
		public:
			explicit Mouse() = default;

			void update() override {
				peripherals::Peripherals<MouseKey, MouseKeyType>::update();
				position = sf::Mouse::getPosition(*window);
			}

			sf::Vector2i getPosition() {
				return position;
			}

			~Mouse() = default;
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

	class KeyboardObserver {
	private:
		std::vector<keyboard::KeyboardKey*> hotKeyList;
		Keyboard keyboard = PeripheralsFactory::getKeyboard();
	public:
		KeyboardObserver() = default;

		KeyboardObserver& attachKey(KeyboardKeyType keyType) {
			auto key = keyboard->getKey(keyType);
			if (key == nullptr)
				hotKeyList.push_back(keyboard->addKey(keyType));
			else
				hotKeyList.push_back(key);
			return *this;
		}

		[[nodiscard]] bool isActivated() const {
			return std::all_of(hotKeyList.cbegin(), hotKeyList.cend(), [](keyboard::KeyboardKey* key) { return key->isPressed(); });
		}

		~KeyboardObserver()=default;
	};

	class MouseObserver {
	private:
		std::vector<mouse::MouseKey*> hotKeyList;
		Mouse mouse=PeripheralsFactory::getMouse();
	public:
		MouseObserver()=default;

		MouseObserver& attachKey(MouseKeyType keyType) {
			auto key = mouse->getKey(keyType);
			if (key == nullptr)
				hotKeyList.push_back(mouse->addKey(keyType));
			else
				hotKeyList.push_back(key);
			return *this;
		}

		[[nodiscard]] bool isActivated() const {
			return std::all_of(hotKeyList.cbegin(), hotKeyList.cend(), [](mouse::MouseKey* key) { return key->isPressed(); });
		}

		sf::Vector2i getPosition() {
			return mouse->getPosition();
		}

		~MouseObserver()=default;
	};
}


#endif //RUNCRAFT_INPUT_HPP
