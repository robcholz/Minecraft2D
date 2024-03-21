//
// Created by robcholz on 6/18/23.
//

#ifndef MINECRAFT_REGISTRY_HPP
#define MINECRAFT_REGISTRY_HPP

#include <plog/Log.h>
#include <mutex>
#include <string>
#include <unordered_map>

namespace utils {
template <class ObjectT>
class Registry {
 private:
  using String = std::string;
  using ObjectPtr = ObjectT*;

 protected:
  Registry() = default;

  virtual ~Registry() {
    for (auto& v : IDRegistry) {
      delete v.second;
    }
  }

 public:
  Registry(const Registry&) = delete;

  Registry& operator=(const Registry&) = delete;

  template <class T>
  ObjectPtr registerObject() {
    auto object = new T();
    registerObject(object);
    return object;
  }

  // this function will be called in a thread only
  void registerObject(ObjectPtr object) {
    IDRegistry.insert({object->getID().toString(), object});
    serialIDRegistry.insert({object->getSerialID(), object});
  }

  static Registry<ObjectT>& getInstance() {
    static Registry instance;
    return instance;
  }

  ObjectPtr createObject(ObjectPtr block) {
    std::lock_guard<std::mutex> lock(mutex);
    return block->createObject();
  }

  ObjectPtr createObject(const String& id) {
    std::lock_guard<std::mutex> lock(mutex);
    return IDRegistry[id]->createObject();
  }

  ObjectPtr createObject(unsigned int serialID) {
    std::lock_guard<std::mutex> lock(mutex);
    return serialIDRegistry[serialID]->createObject();
  }

  ObjectPtr getObjectInstance(ObjectPtr block) {
    std::lock_guard<std::mutex> lock(mutex);
    auto id = block->getID().toString();
    if (IDRegistry.contains(id))
      return IDRegistry[id];
    PLOG_ERROR << "Cannot find object with id: " << id
               << ". ID might not registered. Returning nullptr.";
    return nullptr;
  }

  ObjectPtr getObjectInstance(const String& id) {
    std::lock_guard<std::mutex> lock(mutex);
    if (IDRegistry.contains(id))
      return IDRegistry[id];
    PLOG_ERROR << "Cannot find object with id: " << id
               << ". ID might not registered. Returning nullptr.";
    return nullptr;
  }

  ObjectPtr getObjectInstance(unsigned int serialID) {
    std::lock_guard<std::mutex> lock(mutex);
    if (IDRegistry.contains(serialID))
      return serialIDRegistry[serialID];
    PLOG_ERROR << "Cannot find object with id: " << serialID
               << ". ID might not registered. Returning nullptr.";
    return nullptr;
  }

 private:
  std::unordered_map<String, ObjectPtr> IDRegistry;
  std::unordered_map<unsigned int, ObjectPtr> serialIDRegistry;
  std::mutex mutex;
};
}  // namespace utils

#endif  // MINECRAFT_REGISTRY_HPP
