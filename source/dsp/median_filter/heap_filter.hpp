// Copyright (C) 2024 - zsliu98
// This file is part of ZLSplit
//
// ZLSplit is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// ZLSplit is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with ZLSplit. If not, see <https://www.gnu.org/licenses/>.

// Based on:
// http://stackoverflow.com/a/5970314/383299
// https://gist.github.com/ashelly/5665911
// https://github.com/craffel/median-filter
// https://github.com/suomela/median-filter

#ifndef MEDIAN_HEAP_FILTER_HPP
#define MEDIAN_HEAP_FILTER_HPP

namespace zlMedianFilter {
    /**
     * a median filter based on min/max heap
     * @tparam T type of data
     * @tparam N size of the filter
     */
    template<typename T, size_t N>
    class HeapFilter {
    public:
        HeapFilter() {
            clear();
        }

        /**
         * clear internal state
         */
        void clear() {
            idx = 0;
            minCt = 0;
            maxCt = 0;
            auto nItems = static_cast<int>(N);
            while (nItems--) {
                pos[static_cast<size_t>(nItems)] = ((nItems + 1) / 2) * ((nItems & 1) ? -1 : 1);
                heap[static_cast<size_t>(centerPos + pos[static_cast<size_t>(nItems)])] = nItems;
            }
        }

        /**
         * insert a new data into the circular buffer
         * @param v new data
         */
        void insert(T v) {
            const int p = pos[idx];
            T old = data[idx];
            data[idx] = v;
            idx = (idx + 1) % N;
            // New item is in minheap
            if (p > 0) {
                if (minCt < (N - 1) / 2) {
                    minCt++;
                } else if (v > old) {
                    minSortDown(p);
                    return;
                }
                if (minSortUp(p) && mmCmpExch(0, -1)) {
                    maxSortDown(-1);
                }
            }
            // New item is in maxheap
            else if (p < 0) {
                if (maxCt < N / 2) {
                    maxCt++;
                } else if (v < old) {
                    maxSortDown(p);
                    return;
                }
                if (maxSortUp(p) && minCt && mmCmpExch(1, 0)) {
                    minSortDown(1);
                }
            }
            // New item is at median
            else {
                if (maxCt && maxSortUp(-1)) {
                    maxSortDown(-1);
                }
                if (minCt && minSortUp(1)) {
                    minSortDown(1);
                }
            }
        }

        /**
         *
         * @return current median
         */
        T getMedian() {
            if (minCt < maxCt) {
                return (data[heap[static_cast<size_t>(centerPos)]] + data[heap[static_cast<size_t>(centerPos - 1)]]) / 2;
            } else {
                return data[heap[static_cast<size_t>(centerPos)]];
            }
        }

    private:
        std::array<T, N> data;
        std::array<int, N> pos;
        std::array<int, N> heap;
        static constexpr int centerPos = static_cast<int>(N / 2);
        // Position in circular queue
        int idx{0};
        // Count of items in min heap
        int minCt{0};
        // Count of items in max heap
        int maxCt{0};

        // Swaps items i&j in heap, maintains indexes
        int mmexchange(const int i, const int j) {
            const auto ii = static_cast<size_t>(centerPos + i);
            const auto jj = static_cast<size_t>(centerPos + j);
            const auto t = heap[ii];
            heap[ii] = heap[jj];
            heap[jj] = t;
            pos[heap[ii]] = ii;
            pos[heap[jj]] = jj;
            return 1;
        }

        // Maintains minheap property for all items below i.
        void minSortDown(int i) {
            for (i *= 2; i <= minCt; i *= 2) {
                if (i < minCt && mmless(i + 1, i)) {
                    ++i;
                }
                if (!mmCmpExch(i, i / 2)) {
                    break;
                }
            }
        }

        // Maintains maxheap property for all items below i. (negative indexes)
        void maxSortDown(int i) {
            for (i *= 2; i >= -maxCt; i *= 2) {
                if (i > -maxCt && mmless(i, i - 1)) {
                    --i;
                }
                if (!mmCmpExch(i / 2, i)) {
                    break;
                }
            }
        }

        // Returns 1 if heap[i] < heap[j]
        inline int mmless(const int i, const int j) {
            return (data[heap[static_cast<size_t>(centerPos + i)]] < data[heap[static_cast<size_t>(centerPos + j)]]);
        }

        // Swaps items i&j if i<j; returns true if swapped
        inline int mmCmpExch(const int i, const int j) {
            return (mmless(i, j) && mmexchange(i, j));
        }

        // Maintains minheap property for all items above i, including median
        // Returns true if median changed
        inline int minSortUp(int i) {
            while (i > 0 && mmCmpExch(i, i / 2)) {
                i /= 2;
            }
            return (i == 0);
        }

        // Maintains maxheap property for all items above i, including median
        // Returns true if median changed
        inline int maxSortUp(int i) {
            while (i < 0 && mmCmpExch(i / 2, i)) {
                i /= 2;
            }
            return (i == 0);
        }
    };
} // zlMedianFilter

#endif //MEDIAN_HEAP_FILTER_HPP