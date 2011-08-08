#include <Msg.h>

ScheduledSMS::ScheduledSMS(QString context, QTime scheduledTime) :
	Msg(Msg::ScheduledSMS, context), _scheduledTime(scheduledTime){
}

QTime ScheduledSMS::scheduledTime() const{
	return _scheduledTime;
}
