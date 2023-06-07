//
// Created by robcholz on 5/20/23.
//
#include <gtest/gtest.h>
#include "client/input/Input.hpp"

TEST(InputTest, BasicAssertions) {
	input::Keyboard keyboard = input::PeripheralsFactory::getKeyboard();
	input::Keyboard keyboard_1 = input::PeripheralsFactory::getKeyboard();
	input::Mouse mouse = input::PeripheralsFactory::getMouse();
	input::Mouse mouse_1 = input::PeripheralsFactory::getMouse();

	EXPECT_EQ(keyboard.get(), keyboard_1.get());
	EXPECT_EQ(mouse.get(), mouse_1.get());

	input::KeyboardObserver keyboard_observer;
	keyboard_observer.attachKey(input::KeyboardKeyType::W)
	                 .attachKey(input::KeyboardKeyType::A);
	keyboard->update();
	EXPECT_FALSE(keyboard_observer.isActivated());
}
