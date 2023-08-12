//
// Created by robcholz on 6/11/23.
//

#ifndef MINECRAFT_CHUNKSTREAMACCESS_HPP
#define MINECRAFT_CHUNKSTREAMACCESS_HPP


namespace chunk {
	class Chunk;
	class ChunkStreamAccess {
	public:
		virtual int getRenderedChunks() = 0;

		virtual int getUpdatedChunks() = 0;

		virtual int getDeletedChunks() = 0;

		virtual int getCachedChunks() = 0;
	};
}

#endif //MINECRAFT_CHUNKSTREAMACCESS_HPP
