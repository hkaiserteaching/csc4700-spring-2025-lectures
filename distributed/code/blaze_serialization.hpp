//  Copyright (c) 2017 Parsa Amini
//  Copyright (c) 2019 Bita Hasheminezhad
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <hpx/include/serialization.hpp>
#include <hpx/include/util.hpp>

#include <blaze/Math.h>

#include <cassert>
#include <array>
#include <cstddef>

namespace hpx::serialization {

    ///////////////////////////////////////////////////////////////////////////
    template <typename T, bool TF>
    void load(
        input_archive& archive, blaze::DynamicVector<T, TF>& target, unsigned)
    {
        // De-serialize vector
        std::size_t count = 0UL;
        std::size_t spacing = 0UL;
        archive >> count >> spacing;

        target.resize(count, false);
        archive >>
            hpx::serialization::make_array(target.data(), spacing);
    }

    ///////////////////////////////////////////////////////////////////////////
    template <typename T>
    void load(input_archive& archive, blaze::DynamicMatrix<T, true>& target,
        unsigned)
    {
        // De-serialize matrix
        std::size_t rows = 0UL;
        std::size_t columns = 0UL;
        std::size_t spacing = 0UL;
        archive >> rows >> columns >> spacing;

        target.resize(rows, columns, false);
        archive >>
            hpx::serialization::make_array(target.data(), spacing * columns);
    }

    template <typename T>
    void load(input_archive& archive, blaze::DynamicMatrix<T, false>& target,
        unsigned)
    {
        // De-serialize matrix
        std::size_t rows = 0UL;
        std::size_t columns = 0UL;
        std::size_t spacing = 0UL;
        archive >> rows >> columns >> spacing;

        target.resize(rows, columns, false);
        archive >>
            hpx::serialization::make_array(target.data(), rows * spacing);
    }

    ///////////////////////////////////////////////////////////////////////////
    template <typename T, blaze::AlignmentFlag AF, blaze::PaddingFlag PF,
        bool TF, typename RT>
    void load(input_archive& archive,
        blaze::CustomVector<T, AF, PF, TF, RT>& target, unsigned)
    {
        assert(false);      // shouldn't ever be called
    }

    template <typename T, blaze::AlignmentFlag AF, blaze::PaddingFlag PF,
        bool SO, typename RT>
    void load(input_archive& archive,
        blaze::CustomMatrix<T, AF, PF, SO, RT>& target, unsigned)
    {
        assert(false);      // shouldn't ever be called
    }

    ///////////////////////////////////////////////////////////////////////////
    template <typename T, bool TF>
    void save(output_archive& archive,
        blaze::DynamicVector<T, TF> const& target, unsigned)
    {
        // Serialize vector
        std::size_t count = target.size();
        std::size_t spacing = target.spacing();
        archive << count << spacing;

        archive << hpx::serialization::make_array(target.data(), spacing);
    }

    template <typename T>
    void save(output_archive& archive,
        blaze::DynamicMatrix<T, true> const& target, unsigned)
    {
        // Serialize matrix
        std::size_t rows = target.rows();
        std::size_t columns = target.columns();
        std::size_t spacing = target.spacing();
        archive << rows << columns << spacing;

        archive << hpx::serialization::make_array(
            target.data(), spacing * columns);
    }

    template <typename T>
    void save(output_archive& archive,
        blaze::DynamicMatrix<T, false> const& target, unsigned)
    {
        // Serialize matrix
        std::size_t rows = target.rows();
        std::size_t columns = target.columns();
        std::size_t spacing = target.spacing();
        archive << rows << columns << spacing;

        archive << hpx::serialization::make_array(
            target.data(), rows * spacing);
    }

    ///////////////////////////////////////////////////////////////////////////
    template <typename T, blaze::AlignmentFlag AF, blaze::PaddingFlag PF,
        bool TF, typename RT>
    void save(output_archive& archive,
        blaze::CustomVector<T, AF, PF, TF, RT> const& target, unsigned)
    {
        // Serialize vector
        std::size_t count = target.size();
        std::size_t spacing = target.spacing();
        archive << count << spacing;

        archive << hpx::serialization::make_array(target.data(), spacing);
    }

    template <typename T, blaze::AlignmentFlag AF, blaze::PaddingFlag PF,
        typename RT>
    void save(output_archive& archive,
        blaze::CustomMatrix<T, AF, PF, true, RT> const& target, unsigned)
    {
        // Serialize matrix
        std::size_t rows = target.rows();
        std::size_t columns = target.columns();
        std::size_t spacing = target.spacing();
        archive << rows << columns << spacing;

        archive << hpx::serialization::make_array(
            target.data(), spacing * columns);
    }

    template <typename T, blaze::AlignmentFlag AF, blaze::PaddingFlag PF,
        typename RT>
    void save(output_archive& archive,
        blaze::CustomMatrix<T, AF, PF, false, RT> const& target, unsigned)
    {
        // Serialize matrix
        std::size_t rows = target.rows();
        std::size_t columns = target.columns();
        std::size_t spacing = target.spacing();
        archive << rows << columns << spacing;

        archive << hpx::serialization::make_array(
            target.data(), rows * spacing);
    }

    ///////////////////////////////////////////////////////////////////////////
    HPX_SERIALIZATION_SPLIT_FREE_TEMPLATE(
        (template <typename T, bool TF>), (blaze::DynamicVector<T, TF>));

    HPX_SERIALIZATION_SPLIT_FREE_TEMPLATE(
        (template <typename T, bool SO>), (blaze::DynamicMatrix<T, SO>));

    HPX_SERIALIZATION_SPLIT_FREE_TEMPLATE(
        (template <typename T, blaze::AlignmentFlag AF, blaze::PaddingFlag PF,
            bool TF, typename RT>),
        (blaze::CustomVector<T, AF, PF, TF, RT>) );

    HPX_SERIALIZATION_SPLIT_FREE_TEMPLATE(
        (template <typename T, blaze::AlignmentFlag AF, blaze::PaddingFlag PF,
            bool SO, typename RT>),
        (blaze::CustomMatrix<T, AF, PF, SO, RT>) );
}
