/*
 * Copyright (C) 2020 Mattéo Rossillol‑‑Laruelle <beatussum@protonmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */


#ifndef SYRACUSE_SEQUENCE_HPP
#define SYRACUSE_SEQUENCE_HPP

#include <functional>
#include <memory>

/**
 * @brief Namespace providing some utilities about sequences
 */
namespace sequence
{
    /**
     * @class Sequence core/Sequence.hpp core/Sequence.hpp
     * @brief A class to help create a sequence.
     *
     * This class aims to manage itself the evaluation of the  next  value
     * of a sequence and the calculation of some statistics, while keeping
     * the thread constraints.
     */
    class Sequence
    {
    public:
        /**
         * @brief Type reprensenting a vector of terms.
         *
         * This type is used by the recurrence relation for defining the initial
         * terms.
         *
         * @see Sequence(std::initializer_list<double> uz, seq_t s)
         */
        using vec_t = std::vector<uint64_t>;
        /**
         * @brief Type representing a sequence defined by recursion.
         *
         * For example, the code below corresponds to the Fibonacci sequence
         * such as \f$u_{n + 1} = u_{n - 1} + u_n\f$:
         *
         * ```cpp
         * Sequence::seq_t fibonacci = [](const Sequence::vec_t& un_) {
         *     return un_[1] + un_[0];
         * };
         * ```
         *
         * @see vec_t
         */
        using seq_t = std::function<uint64_t(const vec_t&)>;
    public:
        /**
         * @brief Construct an object from a recurrence relation.
         *
         * For example, the code below set \f$u_0 = 0\f$ and \f$u_1 = 10\f$ for the
         * recurrence relation `rel`:
         *
         * ```cpp
         * Sequence sequence({0, 10}, rel);
         * ```
         *
         * @warning
         * \p uz must count the same number of values as in the recurrence
         * relation \p s.
         *
         * @param uz  list of the initial terms
         * @param s   the recurrence relation
         *
         * @see seq_t
         */
        Sequence(const vec_t& uz, const seq_t& s)
            : m_uz(uz), m_seq(s) {}
        Sequence(const seq_t& seq)
            : Sequence({}, seq) {}
        inline virtual ~Sequence();

        /**
         * @brief Get the term of the corresponding rank.
         *
         * This method is the equivalent of \f$u_n\f$.
         *
         * @param n  corresponding rank
         * @param uz overrides the initial terms
         * @return   nth term
         */
        uint64_t at(vec_t::size_type n, const vec_t& uz = {}) const;
    private:
        vec_t m_uz;
        seq_t m_seq;
    };

    inline Sequence::~Sequence()
    {
        m_uz.clear();
        m_seq = nullptr;
    }
}

#endif // SYRACUSE_SEQUENCE_HPP
