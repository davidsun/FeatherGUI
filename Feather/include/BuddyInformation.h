#ifndef BUDDYINFORMATION_H
#define BUDDYINFORMATION_H

#include <Settings.h>
#include <BuddyStructures.h>
#include <QtCore>

class FeatherBuddy;
class FeatherGroup;
class FeatherGroupList;
class FeatherQun;
class FeatherQunList;

class FeatherBuddy : public QObject{
	Q_OBJECT
	public:
		FeatherBuddy(Buddy *);
		FeatherBuddy(const FeatherBuddy &);
		~FeatherBuddy();
	
		bool isNull();
		bool operator < (const FeatherBuddy &buddy) const;
		FeatherBuddy &operator = (const FeatherBuddy &);
		int id() const;
		QString nickname() const;
		QString uri() const;

	private slots:
		void slotBuddyDestroyed(QObject *);
	
	signals:
		void buddyDestroyed();

	private:
		Buddy *_linkedBuddy;
};

class FeatherGroup : public QObject{
	Q_OBJECT
	public:
		FeatherGroup(Group *);
		FeatherGroup(const FeatherGroup &);
		~FeatherGroup();
		
		bool isNull();
		bool operator < (const FeatherGroup &group) const;
		FeatherGroup &operator = (const FeatherGroup &);
		int id() const;
		QList <FeatherBuddy> buddies();
		QString name() const;

	private slots:
		void slotGroupDestroyed(QObject *);

	signals:
		void groupDestroyed();

	private:
		Group *_linkedGroup;
};

class FeatherGroupList : public QObject{
	Q_OBJECT
	public:
		FeatherGroupList(GroupList *);
		FeatherGroupList(const FeatherGroupList &);
		~FeatherGroupList();
		
		bool isNull();
		FeatherBuddy buddyById(int);
		QList <FeatherGroup> groups();

	private slots:	
		void slotGroupListDestroyed(QObject *);
	
	signals:
		void groupListDestroyed();

	private:
		GroupList *_linkedGroupList;
};

#ifdef FEATHER_QUN_SUPPORT
class FeatherQun : public QObject{
	Q_OBJECT
	public:
		FeatherQun(Qun *);
		FeatherQun(const FeatherQun &);
		~FeatherQun();
		
		bool isNull();
		bool operator == (const FeatherQun &);
		FeatherQun &operator = (const FeatherQun &);
		QList <FeatherBuddy> getBuddies();
		QString getName() const;
		QString getUri() const;
	
	private slots:
		void slotQunDestroyed(QObject *);
	
	signals:
		void qunDestroyed();

	private:
		Qun *LinkedQun;
};

class FeatherQunList : public QObject{
	Q_OBJECT
	public:
		FeatherQunList(QunList *);
		FeatherQunList(const FeatherQunList &);
		~FeatherQunList();
		
		bool isNull();
		QList <FeatherQun> getQuns();
		FeatherQun getQunByUri(const QString &);

	private slots:
		void slotQunListDestroyed(QObject *);

	signals:
		void qunListDestroyed();

	private:
		QunList *LinkedQunList;
};
#endif

#endif
