#pragma once

#include <algorithm>
#include <numeric>
#include <vector>

// A type that can be used to collect a histogram for a sequence of values.
template <typename T>
class histogram
{
    std::vector<size_t> bucket_data;
    T min_value;
    T max_value;

public:
    // Store num_buckets counters evenly distributed in between the smallest
    // and largest values. It will also store counters in two additional
    // buckets, one for values smaller and one for values larger than the range
    // given during construction.
    histogram(size_t num_buckets, T smallest, T largest)
      : bucket_data(num_buckets + 2)
      , min_value(smallest)
      , max_value(largest)
    {
    }

    // Increment the counter for the appropriate bucket, which index is computed
    // based on the number of buckets and the smallest and largest values that were
    // used during construction.
    void add_value(T value)
    {
        // for out-of-range values smaller than minimum use first bucket
        size_t index = 0;
        if (value > max_value)
        {
            // for out-of-range values larger than maximum use last bucket
            index = bucket_data.size() - 1;
        }
        else if (value >= min_value)
        {
            auto bucket_size = (max_value - min_value) / (bucket_data.size() - 2);
            index = static_cast<size_t>((value - min_value) / bucket_size) + 1;
        }
        ++bucket_data[index];
    }

    auto begin()
    {
        return bucket_data.begin();
    }
    auto begin() const
    {
        return bucket_data.begin();
    }

    auto end()
    {
        return bucket_data.end();
    }
    auto end() const
    {
        return bucket_data.end();
    }

    auto buckets() const
    {
        std::vector<T> buckets;
        buckets.reserve(bucket_data.size() - 2);
        buckets.push_back(min_value);

        T delta = (max_value - min_value) / (bucket_data.size() - 2);
        for (size_t i = 0; i != bucket_data.size() - 2; ++i)
        {
            buckets.push_back(min_value + i * delta + 0.5 * delta);
        }

        buckets.push_back(max_value);
        return buckets;
    }
    auto normalized_data() const
    {
        std::vector<double> data;
        data.reserve(bucket_data.size());

        size_t max = std::accumulate(bucket_data.begin(), bucket_data.end(), size_t(0));
        std::transform(bucket_data.begin(), bucket_data.end(), std::back_inserter(data), 
            [&](size_t value) { return double(value) / max; });

        return data;
    }

    auto& data()
    {
        return bucket_data;
    }
};

