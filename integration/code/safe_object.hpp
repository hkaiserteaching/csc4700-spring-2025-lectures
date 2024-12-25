
#pragma once

////////////////////////////////////////////////////////////////////////////
template <typename T>
struct safe_object
{
    safe_object(T init = {})
      : data_(hpx::get_os_thread_count(), init)
    {
    }

    safe_object(safe_object const& rhs) = delete;
    safe_object(safe_object&& rhs) noexcept = default;

    safe_object& operator=(safe_object const& rhs) = delete;
    safe_object& operator=(safe_object&& rhs) noexcept = default;

    T& get()
    {
        return data_[hpx::get_worker_thread_num()];
    }

    T const& get() const
    {
        return data_[hpx::get_worker_thread_num()];
    }

    template <typename F>
    T reduce(F const& f, T init = {})
    {
        for (auto&& d : std::move(data_))
        {
            init = f(init, std::move(d));
        }
        return init;
    }

private:
    std::vector<T> data_;
};

