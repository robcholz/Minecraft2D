//
// Created by robcholz on 6/18/23.
//

#ifndef MINECRAFT_REGISTRY_HPP
#define MINECRAFT_REGISTRY_HPP

#include <string>
#include <map>

#define RegisterObject(object) registerObject(std::make_shared<object>())

namespace util {
	template<class T>
	class Registry {
	private:
		using String = std::string;
		using SmartPtr = std::shared_ptr<T>;
	public:
		explicit Registry() = default;

		void registerObject(const SmartPtr& block) {
			IDRegistry.insert({block->getID().identifier, block});
			serialIDRegistry.insert({block->getID().serialID, block});
		}

		static std::shared_ptr<Registry> getInstance() {
			static std::shared_ptr<Registry> instance(new Registry);
			return instance;
		}

		T* newBlock(const String& id) {
			return IDRegistry[id]->newBlock();
		}

		T* newBlock(int serialID) {
			return serialIDRegistry[serialID]->newBlock();
		}

		T* getBlockInstance(const String& id) {
			return IDRegistry[id].get();
		}

		T* getBlockInstance(int serialID) {
			return serialIDRegistry[serialID].get();
		}

		~Registry() = default;

	private:
		std::map<String, SmartPtr> IDRegistry;
		std::map<int, SmartPtr> serialIDRegistry;
	};
}

#endif //MINECRAFT_REGISTRY_HPP
