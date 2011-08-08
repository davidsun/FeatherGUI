#include <MsgList.h>

ScheduledSMSList::ScheduledSMSList(QTreeWidget *treeWidget, MsgManager *msgManager) :
	AbstractMsgList(treeWidget, msgManager, AbstractMsgList::AcceptScheduledSMS){
}

void ScheduledSMSList::msgAdded(QTreeWidgetItem *item, Msg *msg){
	QTreeWidgetItem *itemTime = new QTreeWidgetItem(item);
	ScheduledSMS *sms = (ScheduledSMS *)msg;
	itemTime -> setText(0, "Time: " + sms -> scheduledTime().toString("hh:mm:ss"));
}

void ScheduledSMSList::msgDestinationDestroyed(QTreeWidgetItem *, Msg *msg, MsgDestination *destination){
	Q_UNUSED(msg);
	Q_UNUSED(destination);
}

void ScheduledSMSList::msgDestroyed(QTreeWidgetItem *, Msg *msg){
	Q_UNUSED(msg);
}

