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

#include "core/core.hpp"

#include <functional>
#include <map>

using namespace core;

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
         * This type is used by the recurrence relation for defining the
         * initial terms.
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
         */
        using seq_t = std::function<int64_t(const vec_t&)>;

        /**
         * @struct Result core/Sequence.hpp
         * @brief Structure used for containing the result of the `doUntil()`
         * methods.
         *
         * @see doUntil(const int64_t value, const vec_t uz) const
         * @see doUntil(const int64_t value) const
         */
        struct Result
        {
            /**
             * @var vec_t::size_type cycleLen
             * The computed length of the cycle.
             */
            vec_t::size_type cycleLen;
            /**
             * @var int64_t maxTerm
             * The maximum term found during the process.
             */
            int64_t maxTerm;
        };

        /**
         * @brief A map storing where each \p Result pointer is associated
         * with the given initial terms.
         */
        using ResultMap = std::map<vec_t, Ref<Result>>;
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
         */
        Sequence(const vec_t& uz, const seq_t& seq)
            : m_uz(uz), m_seq(seq) {}
        /**
         * @brief Construct an object from a recurrence relation without
         * setting the initial terms.
         *
         * @param seq the recurrence relation
         *
         * @see Sequence(const vec_t& uz, const seq_t& seq)
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
         * @brief Get the term of the corresponding rank with the given
         * initial terms.
         *
         * This method is the equivalent of \f$u_n\f$.
         *
         * @mustinit{uz}
         *
         * @param n  the corresponding rank
         * @param uz the initial terms
         * @return   the nth term
         */
        int64_t at(const vec_t::size_type n, const vec_t& uz) const;
        /**
         * @brief Get the term of the corresponding rank.
         *
         * @mustinit{m_uz}
         *
         * @param n the corresponding rank
         * @return  the nth term
         *
         * @see at(const vec_t::size_type n, const vec_t& uz) const
         */
        inline int64_t at(const vec_t::size_type n) const;
        /**
         * @brief Run the sequence until some value with the given initial
         * terms.
         *
         * @mustinit{uz}
         *
         * @param value run the sequence until
         * @param uz    the initial terms
         * @return      some statistics
         */
        Ref<Result> doUntil(const int64_t value, const vec_t uz) const;
        /**
         * @brief Run the sequence until some value.
         *
         * @mustinit{m_uz}
         *
         * @param value run the sequence until
         * @return      some statistics
         */
        inline Ref<Result> doUntil(const int64_t value) const;
        /**
         * @brief Run `doUntil()` several times asynchronously.
         *
         * @param n     run `doUntil()` \p n times
         * @param value run the sequence until
         * @param step  the step incrementing the initial terms
         * @return      a pointer to a map where each \p Result is associated
         *              with the initial terms
         */
        Ref<ResultMap> loadNUntil(const uint8_t n, const int64_t value, const uint8_t step = 1) const;
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

    inline int64_t Sequence::at(vec_t::size_type n) const
    {
        return at(n, m_uz);
    }

    inline Ref<Sequence::Result> Sequence::doUntil(const int64_t value) const
    {
        return doUntil(value, m_uz);
    }
}

#endif // SYRACUSE_SEQUENCE_HPP
