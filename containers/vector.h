#ifndef ODYSSEY_CONTAINERS_VECTOR_H
#define ODYSSEY_CONTAINERS_VECTOR_H

#include <cstdint>

namespace ods
{

#define ODYSSEY_DEFAULT_VECTOR_CAPACITY 5

struct array_header
{
    size_t capacity;
    size_t size;
};

template<typename T>
struct vector
{
    T *p_data;

    // Default constructor.
    vector()
        :p_data(nullptr)
    {
        initialize();
    }

    vector(size_t size)
        :p_data(nullptr)
    {
        initialize(size);
    }

    vector(const vector<T>& v)
        :p_data(nullptr)
    {
        initialize(v);
    }

    vector(size_t size, T value)
        :p_data(nullptr)
    {
        initialize(size, value);
    }

    ~vector()
    {
        ODS_ASSERT(p_data != nullptr);
        free(p_data);
    }

    inline auto begin() -> T *
    {
        return this->data();
    }

    inline auto end() -> T *
    {
        return this->data() + this->size();
    }

    inline  auto begin() const -> T *
    {
        return this->data();
    }

    inline auto end() const -> T *
    {
        return this->data() + this->size();
    }

    vector& operator=(const vector<T>& v)
    {
        copy_vector(v);
    }

    void clear()
    {
        if(this->p_data != nullptr)
        {
            free(p_data);
            p_data = nullptr;
        }
    }

    void push_back(const T& value)
    {
        ODS_ASSERT(p_data != nullptr);
        if(*(((size_t*)p_data) + 1) == *((size_t*)p_data))
        {
            extend_allocation((size_t)(*((size_t*)p_data) + *((size_t*)p_data) * 0.5));
        }

        T* index_pointer = (T*)(((size_t*)p_data) + 2);

        index_pointer[(*(((size_t*)p_data) + 1))++] = value;

        ODS_ASSERT(*(((size_t*)p_data) + 1) <= *((size_t*)p_data));
    }

    /*
    // Resizes the container so that it containts n elements.
    // If n is smaller than the current container size, the content is reduces to its
    // first n elements.
    */
    void resize(size_t length)
    {
        if(p_data == nullptr)
            initialize(length);
        else
            resize_vector(length);
    }

    void set_size(size_t size)
    {
        ODS_ASSERT(p_data != nullptr);
        if(*(size_t*)p_data < size)
            extend_allocation((size_t)(size + size * 0.5));

        *(((size_t*)p_data) + 1) = size;
    }

    void reinit(size_t capacity)
    {
        ODS_ASSERT(this->p_data != nullptr);

        free(this->p_data);

        p_data = (T*)malloc(sizeof(T) * capacity + sizeof(array_header_t));
        ODS_ASSERT(p_data != 0);
        *(size_t*)p_data = capacity;
        *(((size_t*)p_data) + 1) = 0;
        memset((uint8_t*)p_data + sizeof(array_header_t), 0, sizeof(T) * capacity);
    }

    size_t size() const
    {
        ODS_ASSERT(p_data != nullptr);
        return *(((size_t*)p_data) + 1);
    }

    size_t capacity() const
    {
        ODS_ASSERT(p_data != nullptr);
        return *(size_t*)p_data;
    }

    T* data() const
    {
        ODS_ASSERT(p_data != nullptr);
        return (T*)((char*)p_data + sizeof(array_header));
    }

    T& operator[](size_t i)
    {
        ODS_ASSERT(p_data != nullptr);
        return ((T*)((size_t*)p_data + 2))[i];
    }

    const T& operator[](size_t i) const
    {
        // return (((T*)((size_t*)p_data + 2))[i]);
        ODS_ASSERT(p_data != nullptr);
        return ((T*)((size_t*)p_data + 2))[i];
    }

    void initialize()
    {
        // 1. Allocate a new block of memory.
        // 2. When this function is being called, vector pointer must be nullptr.
        // 3. See 2.

        ODS_ASSERT(p_data == nullptr);

        p_data = (T*)malloc(sizeof(T) * ODYSSEY_DEFAULT_VECTOR_CAPACITY + sizeof(array_header));
        ODS_ASSERT(p_data != 0);
        *(size_t*)p_data = ODYSSEY_DEFAULT_VECTOR_CAPACITY;
        *(((size_t*)p_data) + 1) = 0;

        // Set all the contents of the new vector to 0.
        memset(((uint8_t*)p_data + sizeof(array_header)), 0, sizeof(T) * ODYSSEY_DEFAULT_VECTOR_CAPACITY);
    }

    void initialize(size_t size)
    {
        // 1. Allocate a new block of memory.
        // 2. When this function is being called, vector pointer must be nullptr.
        // 3. See 2.

        ODS_ASSERT(p_data == nullptr);

        p_data = (T*)malloc(sizeof(T) * size + sizeof(array_header));
        ODS_ASSERT(p_data != 0);
        *(size_t*)p_data = size;
        *(((size_t*)p_data) + 1) = size;

        // Set all the contents of the new vector to 0.
        memset(((uint8_t*)p_data + sizeof(array_header)), 0, sizeof(T) * size);
    }

    void initialize(const vector<T>& v)
    {
        ODS_ASSERT(v.p_data != nullptr);
        ODS_ASSERT(this->p_data == nullptr);

        size_t vector_capacity = *(size_t*)v.p_data;
        size_t vector_size = *(((size_t*)v.p_data) + 1);

        this->p_data = (T*)malloc(sizeof(T) * vector_capacity + sizeof(array_header));
        ODS_ASSERT(this->p_data != 0);
        memset((char*)this->p_data, 0, sizeof(T) * vector_capacity + sizeof(array_header));

        // NOTE + TODO:
        // We can just use the memcpy, since we copy the whole vector.
        *(size_t*)this->p_data = vector_capacity;
        *(((size_t*)this->p_data) + 1) = vector_size;

        memcpy(((uint8_t*)this->p_data + sizeof(array_header)), (((uint8_t*)v.p_data) + sizeof(array_header)),
            sizeof(T) * (*(((size_t*)this->p_data) + 1)));
    }

    void initialize(size_t size, T value)
    {
        ODS_ASSERT(p_data == nullptr);

        p_data = (T*)malloc(sizeof(T) * size + sizeof(array_header));
        ODS_ASSERT(p_data != 0);

        *(size_t*)p_data = length;
        *(((size_t*)p_data) + 1) = length;

        T *index = (T*)((uint8_t*)p_data + sizeof(array_header_t));

        for(size_t i = 0; i < length; ++i)
        {
            memcpy(&index[i], &value, sizeof(T));
        }
    }

    void copy_vector(const vector<T>& v)
    {
        ODS_ASSERT(v.p_data != nullptr);
        this->clear();

        size_t vector_capacity = *(size_t*)v.p_data;
        size_t vector_size = *(((size_t*)v.p_data) + 1);

        this->p_data = (T*)malloc(sizeof(T) * vector_capacity + sizeof(array_header));
        ODS_ASSERT(new_block != 0);
        memset((char*)this->p_data, 0, sizeof(T) * vector_capacity + sizeof(array_header));

        // NOTE + TODO:
        // We can just use the memcpy, since we copy the whole vector.
        *(size_t*)this->p_data = vector_capacity;
        *(((size_t*)this->p_data) + 1) = vector_size;

        memcpy(((uint8_t*)this->p_data + sizeof(array_header)), (((uint8_t*)v.p_data) + sizeof(array_header)),
            sizeof(T) * (*(((size_t*)this->p_data) + 1)));
    }

    void extend_allocation(size_t capacity)
    {
        // 1. Allocate a new block of memory.
        // 2. Copy or move the old elements into the new block of memory.
        // 3. Delete the old block of memory.

        T *new_block = (T*)malloc(sizeof(T) * capacity + sizeof(array_header));
        ODS_ASSERT(new_block != 0);
        memset((uint8_t*)new_block, 0, sizeof(T) * capacity + sizeof(array_header));

        // if we are shrinking the vector.
        //if( *((size_t*)p_data + 1) > capacity ) *((size_t*)p_data + 1) = capacity;

        *(size_t*)new_block = capacity;
        *(((size_t*)new_block) + 1) = *(((size_t*)p_data) + 1);

        // uint8_t *temp = (uint8_t*)p_data;
        // temp += sizeof(array_header_t);

        memcpy(((uint8_t*)&new_block[0] + sizeof(array_header)),
            &(((((uint8_t*)p_data) + sizeof(array_header)))[0]), sizeof(T)* (*(((size_t*)p_data) + 1)));

        free(p_data);
        p_data = new_block;
    }

    void resize_vector(size_t capacity)
    {
        // 1. Allocate a new block of memory.
        // 2. Copy or move the old elements into the new block of memory.
        // 3. Delete/Free the old block of memory.

        ODS_ASSERT(p_data != nullptr);

        T *new_block = (T*)malloc(sizeof(T) * capacity + sizeof(array_header));
        ODS_ASSERT(new_block != 0);
        memset((char*)new_block, 0, sizeof(T) * capacity + sizeof(array_header));

        *(size_t*)new_block = capacity;
        *(((size_t*)new_block) + 1) = capacity;

        // uint8_t *temp = (uint8_t*)p_data;
        // temp += sizeof(array_header_t);

        memcpy(((uint8_t*)new_block + sizeof(array_header)), (((uint8_t*)p_data) + sizeof(array_header)), sizeof(T) * capacity);

        free(p_data);
        p_data = new_block;
    }

};

}

#endif
