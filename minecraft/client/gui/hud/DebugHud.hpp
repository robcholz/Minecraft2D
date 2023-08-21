//
// Created by robcholz on 7/20/23.
//

#ifndef MINECRAFT_DEBUGHUD_HPP
#define MINECRAFT_DEBUGHUD_HPP


#include <functional>
#include "client/MinecraftClientAccess.hpp"
#include "Hud.hpp"
#include "client/gui/GUI.hpp"
#include "resource/System.hpp"
#include "MinecraftVersion.hpp"


namespace hud {
	class DebugHud : public GUI, public Hud {
	private:
	public:
		DebugHud() = delete;

		explicit DebugHud(MinecraftClientAccess* runCraftClientAccess, WorldAccess* worldAccess) {
			this->minecraftClientAccess = runCraftClientAccess;
			this->worldAccess = worldAccess;
			setScheduledUpdaterTicks(40);
			messageStackLeft.setView(&worldAccess->getView());
			messageStackLeft.setLayout(Layout::LEFT | Layout::TOP);
			messageStackLeft.addMessage("name", [&]() -> String {
				return (boost::format("%d (%s/%s)")
				        % MinecraftVersion::getProductionName()
				        % MinecraftVersion::getReleaseTarget()
				        % MinecraftVersion::isModified<String>()
				).str();
			});
			messageStackLeft.addMessage("fps", [&]() -> String {
				auto chunk_updates = this->worldAccess->getChunkManager()->getChunkStream()->getUpdatedChunks();
				return (boost::format("%d/%f fps (%d chunk updates)")
				        % RenderSystem::getMaxFrameRate()
				        % (int) RenderSystem::getFrameRate()
				        % chunk_updates
				).str();
			});
			messageStackLeft.addMessage("pos", [&]() -> String {
				auto pos = this->worldAccess->getPlayer()->getEntityPosition().get();
				auto block_pos=this->worldAccess->getPlayer()->getEntityPosition().get<coordinate::BlockPos>();
				auto chunk_pos= chunk::Chunk::toChunkPosition(block_pos.x, block_pos.z).chunkPos;
				return (boost::format("XZ: %s / %s ChunkPos: %d")
				        % utils::setPrecision(pos.x, 3)
				        % utils::setPrecision(pos.z, 5)
						% chunk_pos
				).str();
			});
			messageStackLeft.addMessage("block", [&]() -> String {
				auto pos = this->worldAccess->getPlayer()->getEntityPosition().get<coordinate::BlockPos>();
				auto stream = this->worldAccess->getChunkManager();
				auto block_id = stream->getBlock(pos)->getID();
				auto id = block_id.toString();
				return (boost::format("Block: %d / %d %s") % pos.x % pos.z % id).str();
			});
			messageStackLeft.addMessage("block_light", [&]() -> String {
				auto pos = this->worldAccess->getPlayer()->getEntityPosition().get<coordinate::BlockPos>();
				auto light = (int)this->worldAccess->getChunkManager()->getBlockLightLevel(pos);
				return (boost::format("BlockLight: %d") % light).str();
			});

			messageStackRight.setView(&worldAccess->getView());
			messageStackRight.setLayout(Layout::RIGHT | Layout::TOP);
			messageStackRight.addMessage("os", [&]() -> String {
				return (boost::format("System: %s %s %s") % OS::getOSName() % OS::getOSVersion() % OS::getOSArch()).str();
			});

			messageStackRight.addMessage("cpu_model", [&]() -> String {
				return OS::getCPUModelName();
			});
			messageStackRight.addMessage("gpu_model", [&]() -> String {
				return OS::getGPUModelName();
			});
			messageStackRight.addMessage("gpu_model", [&]() -> String {
				return OS::getScreenInfo();
			});
			messageStackRight.addMessage("pid", [&]() -> String {
				return "PID: " + OS::getPID<String>();
			});
			messageStackRight.addMessage("cpu_info", [&]() -> String {
				return (boost::format("CPU: %d%%")
				        % (OS::getProcessCPUUsagePercent<int>() / (int) OS::getProcessorCount())
				).str();
			});
			messageStackRight.addMessage("mem_info", [&]() -> String {
				return (boost::format("Mem: %d%% %d/%dMB")
				        % OS::getProcessMemoryUsagePercent()
				        % OS::getProcessMemoryUsage()
				        % OS::getTotalRAM()
				).str();
			});
		}

		~DebugHud() = default;

		void update() override {
			updateData();
			messageStackLeft.updatePosition();
			messageStackRight.updatePosition();
		}

		void render() override {
			messageStackLeft.render();
			messageStackRight.render();
		}

	private:
		class MessageStack {
		private:
			using String = std::string;
		public:
			explicit MessageStack() {
				_layout.setLayout(Layout::LEFT | Layout::TOP);
			}

			void setView(sf::View* view) {
				_view = view;
			}

			void setLayout(Layout::layout layout) {
				_layout.setLayout(layout);
			}

			void addMessage(const String& id, std::function<String()> func) {
				auto message = std::make_unique<RichText>();
				message->setCharacterSize(35)
				       .setColor(sf::Color(255, 255, 255))
				       .setBackgroundColor(sf::Color(170, 170, 170, 168));
				messageStack.push_back(std::move(std::make_tuple(id, 0.0f, std::move(message), std::move(func))));
			}

			void updateMessageHeight() {
				for (auto i = 0; i < messageStack.size(); ++i) {
					float height_prev;
					float* height_current = &std::get<1>(messageStack[i]);
					auto message = std::get<2>(messageStack[i]).get();
					if (i != 0 || Layout::parse(_layout, Layout::Alignment::BOTTOM)) {
						height_prev = std::get<1>(messageStack[i - 1]);
						*height_current = message->getGlobalBounds().height;
						*height_current = height_prev + *height_current;
					} else {
						*height_current = 0.0f;
					}
					message->setTextPosition(getMessageOriginX(), getMessageOriginZ() + *height_current);
				}
			}

			void updatePosition() {
				for (auto& i: messageStack) {
					float* height_current = &std::get<1>(i);
					auto message = std::get<2>(i).get();
					float x, z;
					if (Layout::parse(_layout, Layout::Alignment::LEFT))
						x = getMessageOriginX();
					if (Layout::parse(_layout, Layout::Alignment::RIGHT))
						x = getMessageOriginX() - message->getGlobalBounds().width;
					if (Layout::parse(_layout, Layout::Alignment::TOP))
						z = getMessageOriginZ() + *height_current;
					if (Layout::parse(_layout, Layout::Alignment::BOTTOM))
						z = getMessageOriginZ() - *height_current;
					message->setTextPosition(x, z);
				}
			}

			void updateContents() {
				for (const auto& i: messageStack) {
					auto message = std::get<2>(i).get();
					auto getMessage = std::get<3>(i);
					message->clear();
					message->setMessage(getMessage());
				}
			}

			void render() {
				for (const auto& i: messageStack) {
					auto message = std::get<2>(i).get();
					RenderSystem::render(*message);
				}
			}

		private:
			sf::View* _view = nullptr;
			Layout _layout{};
			std::vector<std::tuple<String, float, std::unique_ptr<RichText>, std::function<String()>>> messageStack;

			float getMessageOriginX() {
				if (Layout::parse(_layout, Layout::Alignment::LEFT))
					return _view->getCenter().x - _view->getSize().x / 2;
				if (Layout::parse(_layout, Layout::Alignment::RIGHT))
					return _view->getCenter().x + _view->getSize().x / 2;
				return _view->getCenter().x - _view->getSize().x / 2;
			}

			float getMessageOriginZ() {
				if (Layout::parse(_layout, Layout::Alignment::BOTTOM))
					return _view->getCenter().y + _view->getSize().y / 2;
				if (Layout::parse(_layout, Layout::Alignment::TOP))
					return _view->getCenter().y - _view->getSize().y / 2;
				return _view->getCenter().y + _view->getSize().y / 2;
			}
		};

		MinecraftClientAccess* minecraftClientAccess = nullptr;
		MessageStack messageStackLeft;
		MessageStack messageStackRight;

		void onScheduledUpdate() override {
			messageStackLeft.updateContents();
			messageStackLeft.updateMessageHeight();
			messageStackRight.updateContents();
			messageStackRight.updateMessageHeight();
		}
	};
}

#endif //MINECRAFT_DEBUGHUD_HPP
