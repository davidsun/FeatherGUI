#include <MsgList.h>

InstantSMSList::InstantSMSList(QTreeWidget *treeWidget, MsgManager *msgManager) :
	AbstractMsgList(treeWidget, msgManager, AbstractMsgList::AcceptInstantSMS){
}

void InstantSMSList::msgAdded(QTreeWidgetItem *item, Msg *msg){
	Q_UNUSED(msg);

	item -> setText(2, QString::number((long)msg));
}

void InstantSMSList::msgDestinationDestroyed(QTreeWidgetItem *, Msg *msg, MsgDestination *destination){
	Q_UNUSED(msg);
	Q_UNUSED(destination);
}

void InstantSMSList::msgDestroyed(QTreeWidgetItem *, Msg *msg){
	Q_UNUSED(msg);
}
