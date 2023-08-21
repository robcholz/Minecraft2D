set(MEM_POOL_SRC_FILES
        ObjectPool.hpp
        ObjectPool.cpp
        ObjectPool.inl

        MemoryPool.cpp
        MemoryPool.h
        )

add_library(MemPool STATIC ${MEM_POOL_SRC_FILES})