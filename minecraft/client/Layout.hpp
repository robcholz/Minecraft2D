//
// Created by robcholz on 7/22/23.
//

#ifndef MINECRAFT_LAYOUT_HPP
#define MINECRAFT_LAYOUT_HPP

#include <list>
#include "util/math/MathHelper.hpp"

class Widget;

class Layout {
public:
	using layout = uint32_t;

	enum class Alignment : layout {
		DEFAULT,
		VCENTER,
		HCENTER,
		TOP,
		BOTTOM,
		LEFT,
		RIGHT,
	};

	Layout() = default;

	explicit Layout(layout layout) {
		setLayout(layout);
	}

	void setLayout(layout layout) {
		this->_layout = layout;
	}

	void addLayout(layout layout) {
		this->_layout = _layout | layout;
	}

	/**
	 * @brief elements layout
	 * @note value assigned in binary so it's easy to use bitwise or (|) operator
	 */
	static constexpr layout DEFAULT = 1 << static_cast<layout>(Alignment::DEFAULT);
	static constexpr layout VCENTER = 1 << static_cast<layout>(Alignment::VCENTER);
	static constexpr layout HCENTER = 1 << static_cast<layout>(Alignment::HCENTER);
	static constexpr layout TOP = 1 << static_cast<layout>(Alignment::TOP);
	static constexpr layout BOTTOM = 1 << static_cast<layout>(Alignment::BOTTOM);
	static constexpr layout LEFT = 1 << static_cast<layout>(Alignment::LEFT);
	static constexpr layout RIGHT = 1 << static_cast<layout>(Alignment::RIGHT);


	// an element that can be used to separate contents
	struct Spacer {
		int space = 0;
	};

	// a view that arranges its elements in a line
	struct Stack {
		Vec2f position;
		Spacer spacer;
		std::list<Widget*> elements;
	};

	static bool parse(const Layout& layout, Alignment alignment) {
		return ((layout.getLayout() >> static_cast<Layout::layout>(alignment)) & 1) > 0;
	}

	[[nodiscard]]
	layout getLayout() const {
		return _layout;
	}

private:
	layout _layout = DEFAULT;
};

#endif //MINECRAFT_LAYOUT_HPP
