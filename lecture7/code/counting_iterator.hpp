#pragma once

#include <cstddef>
#include <iterator>

class counting_iterator
{
    std::size_t pos;

public:
    using value_type = std::size_t;
    using reference = std::size_t&;
    using difference = std::ptrdiff_t;
    using pointer = std::size_t*;
    using category = std::random_access_iterator_tag;

    explicit counting_iterator(std::size_t start_at = 0) noexcept
      : pos(start_at)
    {
    }

    // dereference
    std::size_t& operator*() noexcept
    {
        return pos;
    }
    std::size_t* operator->() noexcept
    {
        return &pos;
    }

private:
    template <typename Iterator>
    class operator_brackets_proxy
    {
    public:
        explicit operator_brackets_proxy(Iterator const& iter) noexcept
          : iter_(iter)
        {
        }

        operator reference() const noexcept
        {
            return *iter_;
        }

        operator_brackets_proxy& operator=(value_type const& val) noexcept
        {
            *iter_ = val;
            return *this;
        }

    private:
        Iterator iter_;
    };

public:
    operator_brackets_proxy<counting_iterator> operator[](
        std::ptrdiff_t delta) const noexcept
    {
        return operator_brackets_proxy(counting_iterator(pos + delta));
    }

    // prefix++
    counting_iterator& operator++() noexcept
    {
        ++pos;
        return *this;
    }
    counting_iterator& operator--() noexcept
    {
        --pos;
        return *this;
    }

    // postfix++
    counting_iterator operator++(int) noexcept
    {
        return counting_iterator(pos++);
    }
    counting_iterator operator--(int) noexcept
    {
        return counting_iterator(pos--);
    }

    // random-access operations
    friend counting_iterator operator+(
        counting_iterator const& lhs, std::ptrdiff_t delta) noexcept
    {
        return counting_iterator(lhs.pos + delta);
    }
    friend counting_iterator operator+(
        std::ptrdiff_t delta, counting_iterator const& rhs) noexcept
    {
        return counting_iterator(delta + rhs.pos);
    }

    friend counting_iterator operator-(
        counting_iterator const& lhs, std::ptrdiff_t delta) noexcept
    {
        return counting_iterator(lhs.pos - delta);
    }
    friend std::ptrdiff_t operator-(
        counting_iterator const& lhs, counting_iterator const& rhs) noexcept
    {
        return std::ptrdiff_t(lhs.pos) - std::ptrdiff_t(rhs.pos);
    }

    counting_iterator& operator+=(std::size_t delta) noexcept
    {
        pos += delta;
        return *this;
    }
    counting_iterator& operator-=(std::size_t delta) noexcept
    {
        pos -= delta;
        return *this;
    }

    // comparison
    friend bool operator==(
        counting_iterator const& lhs, counting_iterator const& rhs) noexcept
    {
        return lhs.pos == rhs.pos;
    }
    friend bool operator!=(
        counting_iterator const& lhs, counting_iterator const& rhs) noexcept
    {
        return !(lhs.pos == rhs.pos);
    }
    friend bool operator<(
        counting_iterator const& lhs, counting_iterator const& rhs) noexcept
    {
        return lhs.pos < rhs.pos;
    }
    friend bool operator>=(
        counting_iterator const& lhs, counting_iterator const& rhs) noexcept
    {
        return !(lhs.pos < rhs.pos);
    }
    friend bool operator>(
        counting_iterator const& lhs, counting_iterator const& rhs) noexcept
    {
        return rhs.pos < lhs.pos;
    }
    friend bool operator<=(
        counting_iterator const& lhs, counting_iterator const& rhs) noexcept
    {
        return !(rhs.pos < lhs.pos);
    }
};
