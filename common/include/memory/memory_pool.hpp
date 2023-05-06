//
// Created by TSU on 2023-05-03.
//

template<int BufferSize, int InitPoolSize, typename Alloc = std::allocator<std::byte>>
class memory_pool
{
public:
    typedef struct
    {
        Alloc::value_type buffer[BufferSize];
    } memory_block;

private:
    typedef std::allocator_traits<Alloc>::template rebind_alloc<memory_block> MemoryBlockAlloc;

    const std::string _name;
    MemoryBlockAlloc _allocator;
    memory_block* _buffer;
    std::mutex _mutex;
    std::queue<memory_block*, std::deque<memory_block*>> _queue;

public:
    explicit memory_pool(std::string name = "Unnamed memory pool", Alloc const& allocator = Alloc())
        : _name(std::move(name))
        , _allocator(MemoryBlockAlloc(allocator))
        , _buffer()
        , _mutex()
        , _queue()
    {
        auto buffers = _allocator.allocate(InitPoolSize);
        for (auto i = 0; i < InitPoolSize; ++i)
            _queue.push(&buffers[i]);
        LOGV << "Initialized " << BufferSize << " * " << InitPoolSize << " memory pool.";
    }

    virtual ~memory_pool()
    {
        _allocator.deallocate(_buffer, InitPoolSize);
        LOGV << "Destroyed " << BufferSize << " * " << InitPoolSize << " memory pool.";
    }

    memory_block* acquire()
    {
        memory_block* ret;
        synchronized (_mutex)
        {
            if (_queue.empty())
            {
                ret = nullptr;
            }
            else
            {
                ret = _queue.front();
                _queue.pop();
            }
        }
        if (ret == nullptr)
        {
            LOGF << "No memory block for acquire. (" << BufferSize << " * " << InitPoolSize << ")";
        }
        return ret;
    }

    void release(memory_block* block)
    {
        synchronized (_mutex)
            _queue.push(block);
    }
};