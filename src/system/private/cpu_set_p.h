/*
* Copyright (C) 2019 ~ 2020 Uniontech Software Technology Co.,Ltd
*
* Author:      maojj <maojunjie@uniontech.com>
* Maintainer:  maojj <maojunjie@uniontech.com>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef CPU_SET_P_H
#define CPU_SET_P_H

#include "system/cpu.h"

#include <QSharedData>
#include <QMap>

namespace core {
namespace system {

class CPUSet;

enum StatIndex {
    kLastStat = 0,
    kCurrentStat = 1,
    kStatCount = kCurrentStat + 1
};

class CPUSetPrivate : public QSharedData
{
public:
    CPUSetPrivate()
        : QSharedData()
        , ncpus_max {0}
        , ncpus_possible {0}
        , ncpus_present {0}
        , ncpus_online {0}
        , m_cores {0}
        , m_sockets {0}
        , m_processors {0}
        , m_model {}
        , m_vendor {}
        , m_virtualization {}
        , m_stat {}
        , m_usage {}
        , m_infos {}
        , m_statDB {}
        , m_usageDB {}
    {

    }

    CPUSetPrivate(const CPUSetPrivate &other)
        : QSharedData(other)
        , ncpus_max(other.ncpus_max)
        , ncpus_possible(other.ncpus_possible)
        , ncpus_present(other.ncpus_present)
        , ncpus_online(other.ncpus_online)
        , m_cores(other.m_cores)
        , m_sockets(other.m_sockets)
        , m_processors(other.m_processors)
        , m_model(other.m_model)
        , m_vendor(other.m_vendor)
        , m_virtualization(other.m_virtualization)
        , m_stat(std::make_shared<cpu_stat_t>(*(other.m_stat)))
        , m_usage(std::make_shared<cpu_usage_t>(*(other.m_usage)))
    {
        for (auto &info : other.m_infos) {
            CPUInfo cp(info);
            m_infos << cp;
        }
        for (auto &stat : other.m_statDB) {
            if (stat) {
                auto cp = std::make_shared<cpu_stat_t>(*stat);
                m_statDB[stat->cpu] = cp;
            }
        }
        for (auto &usage : other.m_usageDB) {
            if (usage) {
                auto cp = std::make_shared<cpu_usage_t>(*usage);
                m_usageDB[usage->cpu] = cp;
            }
        }
    }
    ~CPUSetPrivate() {}

private:
    int ncpus_max; // kernel_max
    int ncpus_possible; // /sys/devices/system/cpu/possible
    int ncpus_present; // /sys/devices/system/cpu/present
    int ncpus_online; // /sys/devices/system/cpu/online
    quint32 m_cores; // number of cores
    quint32 m_sockets; // number of sockets
    quint32 m_processors; // number of processors
    QString m_model; // processor model
    QString m_vendor; // processor vendor
    QString m_virtualization; // virtualization tech

    CPUStat m_stat; // overall stat
    CPUUsage m_usage; // overall usage

    QList<int> m_cpuNum;

    QMap<QByteArray, CPUStat> m_statDB; // per cpu stat
    QMap<QByteArray, CPUUsage> m_usageDB; // per cpu usage

    qulonglong cpusageTotal[kStatCount] = {0, 0};
    friend class CPUSet;

    QMap<QString, QString> m_info;   //overall info
    QList<CPUInfo> m_infos;         //per cpu info
};

} // namespace system
} // namespace core

#endif // CPU_SET_P_H
