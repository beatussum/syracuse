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


#include "core/Sequence.hpp"

#include <future>
#include <iostream>

namespace sequence
{
    int64_t Sequence::at(const vec_t::size_type n, const vec_t& uz) const
    {
        int64_t ret;
        const auto uzSize = uz.size();

        if (uzSize == 0) {
            throw std::invalid_argument("Sequence::at(): When you create a "
                                        "`Sequence` object without initial "
                                        "terms, you must specify them to "
                                        "call this method.");
        }

        if (n < uzSize) {
            ret = uz.at(n);
        } else {
            vec_t vec;
            for (vec_t::size_type i = 1; i <= uzSize; ++i)
                vec.push_back(at(n - i, uz));

            ret = m_seq(vec);
        }

        return ret;
    }

    Ref<Sequence::Result> Sequence::doUntil(const int64_t value,
                                            const vec_t uz) const
    {
        vec_t::size_type cycle = 0;
        int64_t maxTerm = at(0);

        for (auto term = maxTerm; term != value; term = at(++cycle, uz)) {
            if (term > maxTerm)
                maxTerm = term;
        }

        return std::make_shared<Aggregater<Result>>(cycle, maxTerm);
    }

    Ref<Sequence::ResultMap> Sequence::loadNUntil(const uint8_t n,
                                                  const int64_t value,
                                                  const uint8_t step) const
    {
        vec_t uz = m_uz;

        using doUntil_t = Ref<Result> (Sequence::*)(int64_t,
                                                    const vec_t) const;
        std::map<vec_t, std::future<Ref<Result>>> map;
        for (uint8_t i = 0; i < n; ++i) {
            map[uz] = std::async(static_cast<doUntil_t>(&Sequence::doUntil),
                                 this, value, uz);

            std::for_each(uz.begin(), uz.end(),
                          [=](int64_t& i) { i += step; });
        }

        auto ret = std::make_shared<ResultMap>();
        for (auto& [key, i] : map)
            (*ret)[key] = i.get();

        return ret;
    }
}
