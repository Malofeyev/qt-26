#include "stopwatch.h"

#include <QDebug>

Stopwatch::Stopwatch(/*QObject* parent, */int msec)
    : QTimer(), msec(msec), prev_time(0), round(0), round_interval(0) {}


qint64 Stopwatch::elapsed() const {
    if (isActive()) {
        return timer.elapsed();
    }
    return 0;
}

void Stopwatch::start() {

    reset();
    QTimer::start(msec);
}

void Stopwatch::reset() {
    prev_time = 0;
    round = 0;
    round_interval = 0;
    timer.start();
}

void Stopwatch::stop() {
    QTimer::stop();
    prev_time = 0;
    round = 0;
    round_interval = 0;
}

void Stopwatch::setRound() {
    auto cur_time = timer.elapsed();
    round_interval = cur_time - prev_time;
    prev_time = cur_time;
    round++;
}

Round Stopwatch::getRound() const {
    return {round, round_interval};
}

bool Stopwatch::isActive() const {
    return QTimer::isActive();
}
