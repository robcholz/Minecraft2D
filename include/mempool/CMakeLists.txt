set(MEM_POOL_SRC_FILES
        ObjectPool.hpp
        ObjectPool.cpp
        ObjectPool.inl
        PoolableObject.hpp
        )

add_library(MemPool STATIC ${MEM_POOL_SRC_FILES})