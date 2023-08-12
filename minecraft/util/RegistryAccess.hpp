//
// Created by robcholz on 6/18/23.
//

#ifndef MINECRAFT_REGISTRYACCESS_HPP
#define MINECRAFT_REGISTRYACCESS_HPP

namespace utils {
	template<class ObjectT>
	class RegistryAccess {
	public:
		virtual ObjectT* createObject() = 0;
	};
}

#endif //MINECRAFT_REGISTRYACCESS_HPP
