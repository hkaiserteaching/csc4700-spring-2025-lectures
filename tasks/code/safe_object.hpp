
#pragma once

#include <vector>

#include "hpx/threading_base/thread_num_tss.hpp"
#include "hpx/runtime_local/get_os_thread_count.hpp"

template <typename T>
struct safe_object
{
private:
    std::vector<T> data;

public:
    // Construct a safe_object, initialize the per-thread data with
    // the given value.
    safe_object(T init = {}) : data(hpx::get_os_thread_count(), init) {}

    safe_object(safe_object const& rhs) = delete;
    safe_object(safe_object&& rhs) noexcept = default;

    safe_object& operator=(safe_object const& rhs) = delete;
    safe_object& operator=(safe_object&& rhs) noexcept = default;

    // Return a reference to the instance of the per-thread data for
    // the calling thread.
    T& operator*()
    {
        return data[hpx::get_worker_thread_num()];
    }
    T const& operator*() const
    {
        return data[hpx::get_worker_thread_num()];
    }

    // Return iterators to the per-thread data instances.
    // Note: this is not thread-safe and should be used only outside
    //       of any parallel region.
    auto begin()
    {
        return data.begin();
    }
    auto begin() const
    {
        return data.begin();
    }
    auto end()
    {
        return data.end();
    }
    auto end() const
    {
        return data.end();
    }
};