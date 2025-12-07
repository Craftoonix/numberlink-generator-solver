#ifndef EXPERIMENT_H
#define EXPERIMENT_H

#include <vector>
#include <chrono>

#include <functional>
#include <algorithm>
#include <numeric>
#include <cmath>

#include <iostream>
#include <iomanip>

#include "options.h"


class ExperimentTimer
{   
    public:
        using time_point = std::chrono::time_point<std::chrono::steady_clock>;

        static time_point now()
        {
            return std::chrono::steady_clock::now();
        }

        void start()
        {
            startTime = now();
        }

        void end()
        {
            endTime = now();

            std::chrono::duration<double> diff = endTime - startTime;
            measurements.push_back(diff.count());
        }

        void clear()
        {
            measurements.clear();
        }

        double getMean() const
        {
            double sum = std::accumulate(std::begin(measurements), std::end(measurements), 0.0);
            return sum / measurements.size();
        }

        void printStatistics(std::ostream &out,
                            const double baseline=0.0f) const
        {
            const auto minmax = std::minmax_element(std::begin(measurements),
                                                    std::end(measurements));
            double min = *minmax.first;
            double max = *minmax.second;

            /* stdev computation */
            double mean = getMean();
            double sum_differences = 0.0;

            for (double value : measurements)
                sum_differences += (value - mean) * (value - mean);

            double stdev = std::sqrt(sum_differences / measurements.size());

            /* output */
            out << "Summary: "
                << min << "s - " << max
                << "s, mean " << mean << "s +/- " << stdev
                << " (N=" << measurements.size() << ")";

            if (baseline != 0.0f)
                out << ", speedup " << baseline/mean << "x";

            out << "\n\n";
        }

        void printStatisticsCSV(std::ostream &out) const
        {
            out << "time\n";

            for (auto &m : measurements)
                out << m << "\n";
        }


    private:
        std::vector<double> measurements{};

        time_point startTime{};
        time_point endTime{};
};

#endif
