#include <MsgManager.h>
#include <Msg.h>
#include <FeatherAccount.h>
#include <QTimerEvent>

//public
MsgManager::MsgManager() :
	QObject(0){
		_sendingQueueTimerId = startTimer(20);
		_waitingQueueTimerId = startTimer(500);
}

MsgManager::~MsgManager(){
	for (QSet <Msg *>::const_iterator it = _msgs.begin(); it != _msgs.end(); it ++){
		disconnect(*it, SIGNAL(destroyed(QObject *)), this, SLOT(slotMsgDestroyed(QObject *)));
		delete *it;
	}
}

void MsgManager::addMsg(Msg *msg){
	if (msg -> destinationCount() == 0){
		delete msg;
		return;
	}

	_msgs.insert(msg);
	connect(msg, SIGNAL(destroyed(QObject *)), this, SLOT(slotMsgDestroyed(QObject *)));
	connect(msg, SIGNAL(msgDestinationDestroyed(Msg *, MsgDestination *)), this,
			SLOT(slotMsgDestinatinoDestroyed(Msg *, MsgDestination *)));

	const QList <MsgDestination *> &dests = msg -> destionations();
	for (int i = 0; i < dests.size(); i ++){
		FeatherAccount *account = dests[i] -> account();
		if (_accountDestinationCount.find(account) == _accountDestinationCount.end())
			connect(account, SIGNAL(SMSSendingResult(FeatherAccount *, int, Feather::SMSSendingState)),
					this, SLOT(slotMsgSendingResult(FeatherAccount *, int, Feather::SMSSendingState)));
		_accountDestinationCount[account] ++;
	}

	emit msgAdded(msg);
}

void MsgManager::processMsg(Msg *msg){
	if (_msgs.find(msg) == _msgs.end()) return;
	if (msg -> type() == Msg::InstantSMS){
		_sendingQueue.append(msg -> destionations());
	}  else if (msg -> type() == Msg::ScheduledSMS){
		ScheduledSMS *sms = (ScheduledSMS *)msg;
		_waitingQueue[sms -> scheduledTime()].push_back(sms);
	}
}



//private slots
void MsgManager::slotMsgDestinatinoDestroyed(Msg *msg, MsgDestination *dest){
	emit msgDestinationDestroyed(msg, dest);
	if (msg -> destinationCount() == 0) delete msg;
}

void MsgManager::slotMsgDestroyed(QObject *obj){
	Msg *msg = (Msg *)obj;
	_msgs.remove(msg);
	emit msgDestroyed(msg);
}

void MsgManager::slotMsgSendingResult(FeatherAccount *account, int msgId, Feather::SMSSendingState state){
	QMap <QPair <FeatherAccount *, int>, MsgDestination *>::iterator it = _msgLink.find(qMakePair(account, msgId));
	if (it == _msgLink.end()) return;
	MsgDestination *dest = it.value();
	_msgLink.erase(it);
	if (state == Feather::SendingTimeout){
		_sendingQueue.push_back(dest);
	}  else {
		_accountDestinationCount[account] --;
		dest -> parent() -> removeDestination(dest);
	}
}



//private
void MsgManager::timerEvent(QTimerEvent *event){
	if (event -> timerId() == _sendingQueueTimerId){
		if (!_sendingQueue.empty()){
			std::swap(_sendingQueue[0], _sendingQueue[rand() % _sendingQueue.size()]);
			MsgDestination *dest = _sendingQueue.first();
			_sendingQueue.pop_front();
			int msgId = dest -> account() -> sendSMS(dest -> uri(), dest -> parent() -> context());
			_msgLink[qMakePair(dest -> account(), msgId)] = dest;
		}
	}  else if (event -> timerId() == _waitingQueueTimerId){
		while (!_waitingQueue.empty() && _waitingQueue.begin().key().secsTo(QTime::currentTime()) >= 0){
			QList <ScheduledSMS *> msgs = _waitingQueue.begin().value();
			_waitingQueue.erase(_waitingQueue.begin());
			for (int i = 0; i < msgs.size(); i ++)
				_sendingQueue.append(msgs[i] -> destionations());
		}
	}
}
