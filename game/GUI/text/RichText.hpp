//
// Cloned by robcholz on 3/25/23.
// Credits: https://github.com/skyrpex/RichText
//

#ifndef RUNCRAFT_RICHTEXT_HPP
#define RUNCRAFT_RICHTEXT_HPP

#pragma once

#include <cassert>
#include <cmath>
#include <vector>

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/String.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>

namespace sf {
	class Font;

	class String;

	template<class T>
	class Rect;

	typedef Rect<float> FloatRect;
}

struct TextStroke {
	sf::Color fill = sf::Color::White;
	sf::Color outline = sf::Color::Transparent;
	float thickness = 0.f;
};

struct Outline {
	sf::Color outline = sf::Color::Transparent;
	float thickness = 0.f;
};

class RichText : public sf::Drawable, public sf::Transformable {

public:
	/**
	 * @brief Nested class that represents a single line
	 */
	class Line : public sf::Transformable, public sf::Drawable {
	public:
		/**
		 * @brief Set a character's color.
		 * @note Attempting to access a character outside of the line bounds causes a crash.
		 * @param pos
		 * @param color
		 */
		void setCharacterColor(std::size_t pos, sf::Color color) {
			assert(pos < getLength());
			isolateCharacter(pos);
			std::size_t string_to_format = convertLinePosToLocal(pos);
			mTexts[string_to_format].setFillColor(color);
			updateGeometry();
		}

		/**
		 * @brief Set a character's style
		 * @note Attempting to access a character outside of the line bounds causes a crash.
		 * @param pos
		 * @param style
		 */
		void setCharacterStyle(std::size_t pos, sf::Text::Style style) {
			assert(pos < getLength());
			isolateCharacter(pos);
			std::size_t string_to_format = convertLinePosToLocal(pos);
			mTexts[string_to_format].setStyle(style);
			updateGeometry();
		}

		/**
		 * @brief Set a character
		 * @note Attempting to access a character outside of the line bounds causes a crash.
		 * @param pos
		 * @param character
		 */
		void setCharacter(std::size_t pos, sf::Uint32 character) {
			assert(pos < getLength());
			sf::Text &text = mTexts[convertLinePosToLocal(pos)];
			sf::String string = text.getString();
			string[pos] = character;
			text.setString(string);
			updateGeometry();
		}

		/**
		 * @brief Set character size
		 * @param size
		 */
		void setCharacterSize(unsigned int size) {
			for (sf::Text &text: mTexts)
				text.setCharacterSize(size);

			updateGeometry();
		}

		/**
		 * @brief Set font
		 * @param font
		 */
		void setFont(const sf::Font &newFont) {
			for (sf::Text &text: mTexts)
				text.setFont(newFont);
			updateGeometry();
		}

		/**
		 * @brief Get the length of the line
		 * @return
		 */
		std::size_t getLength() const {
			std::size_t count = 0;
			for (sf::Text &text: mTexts) {
				count += text.getString().getSize();
			}
			return count;
		}

		/**
		 * @brief Get a character's color
		 * @note Attempting to access a character outside of the line bounds causes a crash.
		 * @param pos
		 * @return
		 */
		sf::Color getCharacterColor(std::size_t pos) const {
			assert(pos < getLength());
			return mTexts[convertLinePosToLocal(pos)].getFillColor();
		}

		/**
		 * @brief Get a character's style
		 * @note Attempting to access a character outside of the line bounds causes a crash.
		 * @param pos
		 * @return
		 */
		sf::Uint32 getCharacterStyle(std::size_t pos) const {
			assert(pos < getLength());
			return mTexts[convertLinePosToLocal(pos)].getStyle();
		}

		/**
		 * @brief Get a character
		 * @note NOTE: Attempting to access a character outside of the line bounds causes a crash
		 * @param pos
		 * @return
		 */
		sf::Uint32 getCharacter(std::size_t pos) const {
			assert(pos < getLength());
			sf::Text &text = mTexts[convertLinePosToLocal(pos)];
			return text.getString()[pos];
		}

		/**
		 * @brief Get texts
		 * @return
		 */
		const std::vector<sf::Text> &getTexts() const { return mTexts; }

		/**
		 * @brief Append text
		 * @param text
		 */
		void appendText(sf::Text text) {
			updateTextAndGeometry(text);
			mTexts.push_back(std::move(text));
		}

		/**
		 * @brief Get local bounds
		 * @return
		 */
		sf::FloatRect getLocalBounds() const { return mBounds; }

		/**
		 * @brief Get local bounds
		 * @return
		 */
		sf::FloatRect getGlobalBounds() const { return getTransform().transformRect(getLocalBounds()); }

	protected:
		/**
		 * @brief Draw
		 * @param target
		 * @param states
		 */
		void draw(sf::RenderTarget &target, sf::RenderStates states) const override {
			states.transform *= getTransform();

			for (const sf::Text &text: mTexts)
				target.draw(text, states);
		}

	private:
		/**
		 * @brief Get the index of the sf::Text containing the pos'th character.
		 * \n Also changes pos to the widgetOutlinePosition of the character in the sf::Text.
		 * @param pos
		 * @return
		 */
		std::size_t convertLinePosToLocal(std::size_t &pos) const {
			assert(pos < getLength());
			std::size_t array_index = 0;
			for (; pos >= mTexts[array_index].getString().getSize(); ++array_index) {
				pos -= mTexts[array_index].getString().getSize();
			}
			return array_index;
		}

		/**
		 * @brief Split a string to isolate the given character
		 * into a string of its own for individual formatting
		 * @param pos
		 */
		void isolateCharacter(std::size_t pos) {
			std::size_t local_pos = pos;
			std::size_t index = convertLinePosToLocal(local_pos);
			sf::Text temp = mTexts[index];

			sf::String string = temp.getString();
			if (string.getSize() == 1)
				return;

			mTexts.erase(mTexts.begin() + index);
			if (local_pos != string.getSize() - 1) {
				temp.setString(string.substring(local_pos + 1));
				mTexts.insert(mTexts.begin() + index, temp);
			}

			temp.setString(string.substring(local_pos, 1));
			mTexts.insert(mTexts.begin() + index, temp);

			if (local_pos != 0) {
				temp.setString(string.substring(0, local_pos));
				mTexts.insert(mTexts.begin() + index, temp);
			}
		}

		/**
		 * @brief Update geometry
		 */
		void updateGeometry() const {
			mBounds = sf::FloatRect();

			for (sf::Text &text: mTexts)
				updateTextAndGeometry(text);
		}

		/**
		 * @brief Update geometry for a given text
		 * @param text
		 */
		void updateTextAndGeometry(sf::Text &text) const {
			// Set text offset
			text.setPosition(mBounds.width, 0.f);

			// Update bounds
			float line_spacing = std::floor(text.getFont()->getLineSpacing(text.getCharacterSize()));
			mBounds.height = std::max(mBounds.height, line_spacing);
			mBounds.width += text.getGlobalBounds().width;
		}

		// Member data
		mutable std::vector<sf::Text> mTexts; ///< List of texts
		mutable sf::FloatRect mBounds;        ///< Local bounds
	};

protected:
	/**
	 * @brief Render
	 * @param target
	 * @param states
	 */
	void draw(sf::RenderTarget &target, sf::RenderStates states) const override {
		states.transform *= getTransform();

		for (const Line &line: lines)
			target.draw(line, states);
	}

private:
	// Member data
	mutable std::vector<Line> lines; ///< List of lines
	const sf::Font *font;            ///< Font
	unsigned int characterSize;      ///< Character size
	mutable sf::FloatRect bounds;    ///< Local bounds
	TextStroke currentStroke;        ///< Last used stroke
	sf::Text::Style currentStyle;    ///< Last style used

	/**
	 * @brief Delegate constructor
	 * @param font
	 */
	explicit RichText(const sf::Font *font)
			: font(font),
			  characterSize(30),
			  currentStroke{sf::Color::White, sf::Color::Transparent},
			  currentStyle(sf::Text::Regular) {
	}

	/**
	 * @brief Creates a sf::Text instance using the current styles
	 * @param string
	 * @return
	 */
	sf::Text createText(const sf::String &string) const {
		sf::Text text;
		text.setString(string);
		text.setFillColor(currentStroke.fill);
		text.setOutlineColor(currentStroke.outline);
		text.setOutlineThickness(currentStroke.thickness);
		text.setStyle(currentStyle);
		text.setCharacterSize(characterSize);
		if (font)
			text.setFont(*font);

		return text;
	}

	/**
	 * @brief Update geometry
	 */
	void updateGeometry() const {
		bounds = sf::FloatRect();

		for (Line &line: lines) {
			line.setPosition(0.f, bounds.height);

			bounds.height += line.getGlobalBounds().height;
			bounds.width = std::max(bounds.width, line.getGlobalBounds().width);
		}
	}

public:
	RichText() : RichText(nullptr) {}

	explicit RichText(const sf::Font &font) : RichText(&font) {}

	RichText &setStroke(const TextStroke &stroke){
		currentStroke = stroke;
		return *this;
	}

	RichText &setOutline(const Outline &outline) {
		currentStroke.outline = outline.outline;
		currentStroke.thickness = outline.thickness;
		return *this;
	}

	RichText &setColor(const sf::Color &color) {
		currentStroke.fill = color;
		return *this;
	}

	RichText &setStyle(sf::Text::Style style) {
		currentStyle = style;
		return *this;
	}

	RichText &setMessage(const sf::String &string) {
		// Maybe skip
		if (string.isEmpty())
			return *this;

		// Explode into substrings
		std::vector<sf::String> substrings = explode(string, '\n');

		// Append first substring using the last line
		auto it = substrings.begin();
		if (it != substrings.end()) {
			// If there isn't any line, just create it
			if (lines.empty())
				lines.resize(1);

			// Remove last line's height
			Line &line = lines.back();
			bounds.height -= line.getGlobalBounds().height;

			// Append text
			line.appendText(createText(*it));

			// Update bounds
			bounds.height += line.getGlobalBounds().height;
			bounds.width = std::max(bounds.width, line.getGlobalBounds().width);
		}

		// Append the rest of substrings as new lines
		while (++it != substrings.end()) {
			Line line;
			line.setPosition(0.f, bounds.height);
			line.appendText(createText(*it));
			lines.push_back(std::move(line));

			// Update bounds
			bounds.height += line.getGlobalBounds().height;
			bounds.width = std::max(bounds.width, line.getGlobalBounds().width);
		}

		return *this;
	}

	static std::vector<sf::String> explode(const sf::String &string, sf::Uint32 delimiter) {
		if (string.isEmpty())
			return {};

		// For each character in the string
		std::vector<sf::String> result;
		sf::String buffer;
		for (sf::Uint32 character: string) {
			// If we've hit the delimiter character
			if (character == delimiter) {
				// Add them to the result vector
				result.push_back(buffer);
				buffer.clear();
			} else {
				// Accumulate the next character into the sequence
				buffer += character;
			}
		}

		// Add to the result if buffer still has contents or if the last character is a delimiter
		if (!buffer.isEmpty() || string[string.getSize() - 1] == delimiter)
			result.push_back(buffer);

		return result;
	}

	/**
	 * @brief Set the color of a character.
	 * @note Attempting to access a character outside of the bounds causes a crash.
	 * @param line
	 * @param pos
	 * @param color
	 */
	void setCharacterColor(std::size_t line, std::size_t pos, sf::Color color) {
		assert(line < lines.size());
		lines[line].setCharacterColor(pos, color);
		updateGeometry();
	}

	/**
	 * @brief Set the style of a character.
	 * @note Attempting to access a character outside of the bounds causes a crash.
	 * @param line
	 * @param pos
	 * @param style
	 */
	void setCharacterStyle(std::size_t line, std::size_t pos, sf::Text::Style style) {
		assert(line < lines.size());
		lines[line].setCharacterStyle(pos, style);
		updateGeometry();
	}

	/**
	 * @brief Set a character
	 * @note Attempting to access a character outside of the bounds causes a crash.
	 * @param line
	 * @param pos
	 * @param character
	 */
	void setCharacter(std::size_t line, std::size_t pos, sf::Uint32 character) {
		assert(line < lines.size());
		lines[line].setCharacter(pos, character);
		updateGeometry();
	}

	/**
	 * @brief Set character size
	 * @param size
	 */
	void setCharacterSize(unsigned int size) {
		// Maybe skip
		if (characterSize == size)
			return;

		// Update character size
		characterSize = size;

		// Set texts character size
		for (Line &line: lines)
			line.setCharacterSize(size);

		updateGeometry();
	}

	/**
	 * @brief Set font
	 * @param newFont
	 * @return *this
	 */
	RichText &setFont(const sf::Font &newFont) {
		// Maybe skip
		if (font == &newFont)
			return *this;

		// Update font
		font = &newFont;

		// Set texts font
		for (Line &line: lines)
			line.setFont(newFont);

		updateGeometry();
		return *this;
	}

	/**
	 * @brief Clear
	 */
	void clear() {
		// Clear texts
		lines.clear();
		// Reset bounds
		bounds = sf::FloatRect();
	}

	/**
	 * @brief Get the color of a character.
	 * @note Attempting to access a character outside of the bounds causes a crash.
	 * @param line
	 * @param pos
	 * @return
	 */
	sf::Color getCharacterColor(std::size_t line, std::size_t pos) const {
		assert(line < lines.size());
		return lines[line].getCharacterColor(pos);
	}

	/**
	 * @brief Get the style of a character.
	 * @note Attempting to access a character outside of the bounds causes a crash.
	 * @param line
	 * @param pos
	 * @return
	 */
	sf::Uint32 getCharacterStyle(std::size_t line, std::size_t pos) const {
		assert(line < lines.size());
		return lines[line].getCharacterStyle(pos);
	}

	/**
	 * @brief Get a character.
	 * @note Attempting to access a character outside of the bounds causes a crash.
	 * @param line
	 * @param pos
	 * @return
	 */
	sf::Uint32 getCharacter(std::size_t line, std::size_t pos) const {
		assert(line < lines.size());
		return lines[line].getCharacter(pos);
	}

	/**
	 * @brief Get text list
	 * @return
	 */
	const std::vector<Line> &getLines() const { return lines; }

	/**
	 * Get character size
	 * @return
	 */
	unsigned int getCharacterSize() const { return characterSize; }

	/**
	 * @brief Get font
	 * @return
	 */
	const sf::Font *getFont() const { return font; }

	/**
	 * @brief Get local bounds
	 * @return
	 */
	sf::FloatRect getLocalBounds() const { return bounds; }

	/**
	 * @brief Get global bounds
	 * @return
	 */
	sf::FloatRect getGlobalBounds() const { return getTransform().transformRect(getLocalBounds()); }
};

#endif //RUNCRAFT_RICHTEXT_HPP
