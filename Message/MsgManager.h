#ifndef MSGMANAGER_H
#define MSGMANAGER_H

#include <QObject>
#include <QMap>
#include <QPair>
#include <QSet>
#include <QTime>
#include <Lib.h>

class FeatherAccount;
class Msg;
class MsgDestination;
class ScheduledSMS;

class MsgManager : public QObject{
	Q_OBJECT
	public:
		MsgManager();
		~MsgManager();

		void addMsg(Msg *);
		void processMsg(Msg *);

	private slots:
		void slotMsgDestinatinoDestroyed(Msg *, MsgDestination *);
		void slotMsgDestroyed(QObject *);
		void slotMsgSendingResult(FeatherAccount *, int, Feather::SMSSendingState);

	signals:
		void msgAdded(Msg *);
		void msgDestinationDestroyed(Msg *, MsgDestination *);
		void msgDestroyed(Msg *);

	private:
		QMap <FeatherAccount *, int> _accountDestinationCount;
		QMap <QPair <FeatherAccount *, int>, MsgDestination *> _msgLink;
		QSet <Msg *> _msgs;
		QList <MsgDestination *> _sendingQueue;
		int _sendingQueueTimerId;
		QMap <QTime, QList <ScheduledSMS *> > _waitingQueue;
		int _waitingQueueTimerId;

		void timerEvent(QTimerEvent *event);
};

#endif // MSGMANAGER_H
