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

/**
 * @brief Namespace providing some utilities about sequences.
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
         */
        using vec_t = std::vector<int64_t>;
        /**
         * @brief Type representing a sequence defined by recursion.
         *
         * @par Example
         * The code below corresponds to the Fibonacci sequence such as
         * \f$u_{n + 1} = u_{n - 1} + u_n\f$:
         *
         * ```cpp
         * using MySeq = sequence::Sequence;
         *
         * MySeq::seq_t fibonacci = [](const MySeq::vec_t& un_) {
         *     return un_[1] + un_[0];
         * };
         * ```
         *
         * @see vec_t
         */
        using seq_t = std::function<int64_t(const vec_t&)>;
    public:
        /**
         * @brief Construct an object from a recurrence relation and set the
         * initial terms.
         *
         * @par Example
         * The code below set \f$u_0 = 0\f$ and \f$u_1 = 10\f$ for the
         * recurrence relation `rel`:
         *
         * ```cpp
         * sequence::Sequence mySeq({0, 10}, rel);
         * ```
         *
         * @warning
         * \p uz must count the same number of values as in the recurrence
         * relation \p seq.
         *
         * @param uz  list of the initial terms
         * @param seq the recurrence relation
         *
         * @see seq_t
         */
        Sequence(const vec_t& uz, const seq_t& seq)
            : m_uz(uz), m_seq(seq) {}
        /**
         * @brief Construct an object from a recurrence relation without
         * setting the initial terms.
         *
         * @param seq the recurrence relation
         *
         * @see Sequence(const vec_t& uz, const seq_t& s)
         */
        Sequence(const seq_t& seq)
            : Sequence({}, seq) {}
        inline virtual ~Sequence();
        /**
         * @brief Set the initial terms to the current object.
         *
         * @par Example
         *
         * ```cpp
         * sequence::Sequence mySeq(fibonacci);
         * mySeq.withUz({0, 1}).at(6); // returns 8
         * ```
         *
         * @note
         * This method does not create another object: it returns a reference
         * to the current modified object.
         *
         * @param uz the initial terms
         * @return   a reference to the modified object
         */
        inline Sequence& withUz(const vec_t& uz);

        /**
         * @brief Get the term of the corresponding rank.
         *
         * This method is the equivalent of \f$u_n\f$.
         *
         * @warning
         * To use this method, \p m_uz must be initialized, or otherwise an
         * `std::invalid_argument` will be thrown.
         *
         * @param n corresponding rank
         * @return  nth term
         */
        int64_t at(vec_t::size_type n) const;
        /**
         * @brief Get the cycle length.
         *
         * @param value run the sequence until
         * @return      the number of steps before arriving at \p value
         */
        vec_t::size_type doUntil(int64_t value) const;
    private:
        vec_t m_uz;
        seq_t m_seq;
    };

    inline Sequence::~Sequence()
    {
        m_uz.clear();
        m_seq = nullptr;
    }

    inline Sequence& Sequence::withUz(const vec_t& uz)
    {
        m_uz = uz;
        return *this;
    }
}

#endif // SYRACUSE_SEQUENCE_HPP
