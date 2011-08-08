#ifndef SCHEDULEDSMSLIST_H
#define SCHEDULEDSMSLIST_H

#include <QObject>
#include <QTreeWidget>
#include <QMap>
#include <Msg.h>

class MsgManager;

class AbstractMsgList : public QObject{
	Q_OBJECT

	/*
		A Link from Msg to QTreeWidgetItem
	*/
	class MsgItem{
		public:
			MsgItem(QTreeWidgetItem *item);
			~MsgItem();

			void addDestination(MsgDestination *destination);
			void removeDestination(MsgDestination *destination);
			QTreeWidgetItem *treeWidgetItem() const;

		private:
			QTreeWidgetItem *_widgetItemDestination;
			QTreeWidgetItem *_widgetItem;
			QMap <MsgDestination *, QTreeWidgetItem *> _destinationItems;
	};


	public:
		enum AcceptedMsgType{
			AcceptInstantSMS = (1 << (int)Msg::InstantSMS),
			AcceptScheduledSMS = (1 << (int)Msg::ScheduledSMS),
		};

		AbstractMsgList(QTreeWidget *treeWidget, MsgManager *msgManager, int acceptedMsgType = 0);

		int msgCount() const;
		QList <Msg *> msgs() const;

	signals:
		void msgCleared();

	private slots:
		void slotMsgAdded(Msg *);
		void slotMsgDestinationDestroyed(Msg *, MsgDestination *);
		void slotMsgDestroyed(Msg *);

	protected:
		virtual void msgAdded(QTreeWidgetItem *, Msg *msg) = 0;
		virtual void msgDestinationDestroyed(QTreeWidgetItem *, Msg *msg, MsgDestination *destination) = 0;
		virtual void msgDestroyed(QTreeWidgetItem *, Msg *msg) = 0;

	private:
		int _acceptedMsgType;
		QMap <Msg *, MsgItem *> _msgItems;
		MsgManager *_msgManager;
		QTreeWidget *_treeWidget;
};

class InstantSMSList : public AbstractMsgList{
	Q_OBJECT

	public:
		InstantSMSList(QTreeWidget *treeWidget, MsgManager *msgManager);

	private:
		void msgAdded(QTreeWidgetItem *, Msg *msg);
		void msgDestinationDestroyed(QTreeWidgetItem *, Msg *msg, MsgDestination *destination);
		void msgDestroyed(QTreeWidgetItem *, Msg *msg);
};

class ScheduledSMSList : public AbstractMsgList{
	Q_OBJECT

	public:
		ScheduledSMSList(QTreeWidget *treeWidget, MsgManager *msgManager);

	private:
		void msgAdded(QTreeWidgetItem *, Msg *msg);
		void msgDestinationDestroyed(QTreeWidgetItem *, Msg *msg, MsgDestination *destination);
		void msgDestroyed(QTreeWidgetItem *, Msg *msg);
};

#endif // SCHEDULEDSMSLIST_H
