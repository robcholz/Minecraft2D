//
// Created by robcholz on 6/6/23.
//

#ifndef RUNCRAFT_BLOCKIDLOADER_HPP
#define RUNCRAFT_BLOCKIDLOADER_HPP

#include <map>

namespace block {

	class BlockIDLoader {
	private:
		using String = std::string;
		static inline std::map<String, int> blockIDs;
		static inline int serialID = 0;

		static int loadID(const String &id) {
			if (blockIDs[id] == 0)
				blockIDs[id] = ++serialID;
			return blockIDs[id];
		}

	public:
		BlockIDLoader() = delete;

		static int getBlockID(const String &id) { return loadID(id); }

		~BlockIDLoader() = default;
	};
}

#endif //RUNCRAFT_BLOCKIDLOADER_HPP
