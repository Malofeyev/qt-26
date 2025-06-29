#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <QElapsedTimer>
#include <QTimer>

struct Round {
    int num;
    qint64 interval;
};

class Stopwatch : public QTimer
{
public:
    using QTimer::timeout;
    Stopwatch(/*QObject* parent,*/ int msec);
    qint64 elapsed() const;
    void start();
    void reset();
    void stop();
    void setRound();
    Round getRound() const;
    bool isActive() const;
private:
    int msec;
    QElapsedTimer timer;

    int round;
    qint64 prev_time;
    qint64 round_interval;

};

#endif // STOPWATCH_H
