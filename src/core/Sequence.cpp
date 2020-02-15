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

namespace sequence
{
    int64_t Sequence::at(vec_t::size_type n) const
    {
        int64_t ret;
        auto uzSize = m_uz.size();

        if (uzSize == 0) {
            throw std::invalid_argument("Sequence::at(): When you create a "
                                        "`Sequence` object without initial "
                                        "terms, you must specify them to "
                                        "call this method.");
        }

        if (n < uzSize) {
            ret = m_uz.at(n);
        } else {
            vec_t vec;
            for (vec_t::size_type i = 1; i <= uzSize; ++i)
                vec.push_back(at(n - i));

            ret = m_seq(vec);
        }

        return ret;
    }

    Sequence::vec_t::size_type Sequence::doUntil(int64_t value) const
    {
        vec_t::size_type stepCounter = 0;

        while (at(stepCounter) != value)
            ++stepCounter;

        return stepCounter;
    }
}
