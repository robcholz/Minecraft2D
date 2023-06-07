//
// Created by robcholz on 5/4/23.
//

#ifndef RUNCRAFT_CHUNKSTREAM_HPP
#define RUNCRAFT_CHUNKSTREAM_HPP

#include <list>
#include <vector>
#include "Chunk.hpp"

namespace chunk {

	class ChunkStream {
	protected:
		using IndexT = unsigned short;

	private:
		std::vector<Chunk*> chunkQueue; // the queue that stores the current loaded chunks
		std::list<Chunk*> renderChunkList; // the list that stores the chunks that need to render
		std::list<Chunk*> activeChunkList; // the list that stores the chunks that need to update but not render
		IndexT chunkCapacity = 0, renderChunkCapacity = 0, activeChunkCapacity = 0;
		Intervali renderChunkIndexInterval{};

		/**
 		 * @param chunk Chunk pointer
		 */
		void push_front(Chunk* chunk) {
			chunkQueue.insert(chunkQueue.begin(), chunk);
			if (chunkQueue.size() == chunkCapacity)
				chunkQueue.erase(chunkQueue.end());
		}

		/**
		 * @param chunk Chunk pointer
		 */
		void push_back(Chunk* chunk) {
			chunkQueue.push_back(chunk);
			if (chunkQueue.size() == chunkCapacity)
				chunkQueue.erase(chunkQueue.begin());
		}

		/**
		 * @usage Set the capacity of the ChunkStream. \n
		 * 		\n When the queue is less than the new capacity, the nonnull element will be moved to the middle of the queue. \n
		 * 		\n When the queue is larger than the new capacity, the left and right side of the queue will be erased.
		 * @param newCapacity the new capacity of the queue
		 */
		void resize(IndexT newCapacity) {
			while (newCapacity > chunkCapacity) {
				chunkQueue.resize(newCapacity, nullptr);
				chunkCapacity = newCapacity;
				// the size of elements that are null in the front of the container
				unsigned short front_margin = 0;
				// the size of elements that are null in the end of the container
				unsigned short back_margin = chunkQueue.capacity() - front_margin - chunkQueue.size();

				while (approxEqual(front_margin, back_margin)) {
					front_margin++;
					back_margin--;
					for (int i = 0; i < chunkQueue.size(); i--)
						chunkQueue[chunkQueue.size() - i] = chunkQueue[chunkQueue.size() - i - 1]; // move element 1 index to the right
				} // quit the field if the front and end margin is around 1
			}
			bool erase_side = true; // right-true left-false
			while (newCapacity < chunkCapacity) {
				if (erase_side) // erase the right side of the queue
					chunkQueue.erase(chunkQueue.end());
				else // erase the left side of the queue
					chunkQueue.erase(chunkQueue.begin());
				chunkCapacity--;
				erase_side = !erase_side;
			}
		}

		/**
		 * @Usage Update chunk queue(vector)
		 */
		void updateQueue() {
			// TODO renderChunkIndexInterval
		}

		/**
		 * @Usage Update active queue(list)
		 */
		void updateActiveQueue() {
			activeChunkList.clear();
			for (int i = 0; i < activeChunkCapacity; i++) {
				activeChunkList.push_front(chunkQueue[i]);
			}
		}

		/**
		 * @Usage Update render queue(list)
		 */
		void updateRenderQueue() {
			//TODO Should also depend on the zoom size
			renderChunkList.clear();
			for (int index = renderChunkIndexInterval.lower; index < renderChunkIndexInterval.upper + 1; index++) {
				renderChunkList.push_front(chunkQueue[index]);
			}
		}

		/**
		 * @Usage Just listen
		 */
		void onUpdate() {
			updateQueue();
			renderChunkIndexInterval.lower = chunkCapacity / 2 - renderChunkCapacity / 2;
			renderChunkIndexInterval.upper = chunkCapacity / 2 + renderChunkCapacity / 2;
			updateActiveQueue();
			updateRenderQueue();
		}

	public:
		explicit ChunkStream(IndexT capacity) {
			this->chunkCapacity = capacity;
		}

		void setRenderDistance(IndexT distance) {
			renderChunkCapacity = distance * 2;
			activeChunkCapacity = chunkCapacity;
		}

		void setSimulationDistance(IndexT distance) { resize(distance * 2); }


		[[nodiscard]] IndexT capacity() const { return chunkCapacity; }

		void render() {

		}
	};
}

#endif //RUNCRAFT_CHUNKSTREAM_HPP
