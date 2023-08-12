//
// Created by robcholz on 8/9/23.
//

#ifndef MINECRAFT_2D_TILECOLOR_HPP
#define MINECRAFT_2D_TILECOLOR_HPP


#include <cstdint>
#include <SFML/Graphics/Color.hpp>


class TileColor {
private:
public:
	using TileColorT = uint32_t;

	TileColor() = default;

	explicit TileColor(uint32_t rgba) {
		setColor(rgba);
	}

	explicit TileColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
		setColor(r, g, b, a);
	}

	static uint8_t convertToR(uint32_t rgba) {
		return (rgba & 0xff000000) >> 24;
	}

	static uint8_t convertToG(uint32_t rgba) {
		return (rgba & 0x00ff0000) >> 16;
	}

	static uint8_t convertToB(uint32_t rgba) {
		return (rgba & 0x0000ff00) >> 8;
	}

	static uint8_t convertToAlpha(uint32_t rgba) {
		return (rgba & 0x000000ff) >> 0;
	}

	static uint32_t convertToRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
		return (r << 24) | (g << 16) | (b << 8) | a;
	}

	void setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
		rgba = convertToRGBA(r, g, b, a);
	}

	void setColor(uint32_t rgba) {
		this->rgba = rgba;
	}

	[[nodiscard]]
	sf::Color getColor() const {
		unsigned char extractedRed = convertToR(rgba);                  // Extract red value from the least significant 8 bits
		unsigned char extractedGreen = convertToG(rgba);         // Extract green value from the next 8 bits
		unsigned char extractedBlue = convertToB(rgba);         // Extract blue value from the next 8 bits
		unsigned char extractedAlpha = convertToAlpha(rgba);        // Extract alpha value from the most significant 8 bits
		return {extractedRed, extractedGreen, extractedBlue, extractedAlpha};
	}

	[[nodiscard]]
	uint8_t getR() const {
		return convertToR(rgba);
	}

	[[nodiscard]]
	uint8_t getG() const {
		return convertToG(rgba);
	}

	[[nodiscard]]
	uint8_t getB() const {
		return convertToB(rgba);
	}

	[[nodiscard]]
	uint8_t getAlpha() const {
		return convertToAlpha(rgba);
	}

	[[nodiscard]]
	uint32_t getRGBA() const {
		return rgba;
	}

private:
	uint32_t rgba = 4294967295; // white
};

#endif //MINECRAFT_2D_TILECOLOR_HPP
