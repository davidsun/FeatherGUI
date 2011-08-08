#ifndef BUDDYSTRUCTURES_H
#define BUDDYSTRUCTURES_H

#include <QtCore>
#include <QtXml>
#include <Settings.h>

class Buddy;
class Group;
class GroupList;
class Qun;
class QunList;

class Buddy : public QObject{
	Q_OBJECT
	public:
		Buddy(const QDomNode &, int updateCode = -1);
		~Buddy();

		int id() const;
		int updateCode() const;
		QList <int> groupId() const;
		QString nickname() const;
		QString uri() const;
		void deleteFromGroup(int);
		void update(const QDomNode &, int);
	
	signals:
		void deleted(int);
	
	private:
		int _id, _updateCode;
		QList <int> _groupId;
		QString _nickname, _uri;
	
#ifdef FEATHER_GET_BUDDY_ONLINE_STATE
	public:
		enum BuddyState{
			Unknown = 0,
			Online = 1,
			Offline = 2
		};

		BuddyState getState() const;
		void changeState(BuddyState);
		void changeState(int);

	signals:
		void stateChanged(Buddy *, Buddy::BuddyState);

	private:
		BuddyState State;
#endif
};

class Group : public QObject{
	Q_OBJECT
	public:
		Group(const QDomNode &, int updateCode = -1);
		~Group();

		Buddy *addBuddy(const QDomNode &, int);
		Buddy *buddyById(int);
		int buddyCount();
		int id();
		int updateCode();
		QList <Buddy *> buddies();
		QString name();
		void addBuddy(Buddy *, const QDomNode &, int);
		void removeOutdatedData();
		void update(const QDomNode &, int);
	
	private slots:
		void slotBuddyDeleted(int);
	
	signals:
		void buddyAdded(Group *, Buddy *);
		void buddyDeleted(int);
		void deleted(int);

	private:
		int _id, _updateCode;
		QMap <int, Buddy *> _buddies;
		QString _name;

#ifdef FEATHER_GET_BUDDY_ONLINE_STATE
	public:
		int getBuddyCountByState(Buddy::BuddyState);

	private slots:
		void slotBuddyStateChanged(Buddy *, Buddy::BuddyState);

	signals:
		void buddyStateChanged(Buddy *);

	private:
		QMap <Buddy::BuddyState, int> BuddyCountByState;
#endif
};



/* Attention:
 * 		All of the add/delete information will come from group, but not buddy!
 */
class GroupList : public QObject{
	Q_OBJECT
	public:
		GroupList();
		~GroupList();
		
		Buddy *buddyById(int) const;
		Buddy *buddyByUri(const QString &) const;
		Group *groupById(int) const;
		QList <Group *> groups() const;
		void update(const QDomNode &, const QDomNode &);
	
	private slots:
		void slotBuddyAdded(Group *, Buddy *);
		void slotBuddyDeleted(int);
		void slotGroupDeleted(int);

	signals:
		void buddyAdded(Group *, Buddy *);
		void buddyDeleted(int);
		void groupAdded(Group *);
		void groupDeleted(int);
	
	private:
		int _updateCode;
		QMap <int, Buddy *> _buddies;
		QMap <int, Group *> _groups;

		void addBuddy(const QDomNode &, int);
		void addGroup(const QDomNode &, int);
		void removeOutdatedData();

#ifdef FEATHER_GET_BUDDY_ONLINE_STATE
	public:
		void changeBuddyState(int, int);

	private slots:
		void slotBuddyStateChanged(Buddy *);

	signals:
		void buddyStateChanged(Group *, Buddy *);
#endif
};


#ifdef FEATHER_QUN_SUPPORT
class Qun : public QObject{
	Q_OBJECT
	public:
		Qun(const QDomNode &);
		~Qun();
		
		Buddy *getBuddyById(int);
		Buddy *getBuddyByUri(QString);
		int getBuddyCount();
		int getId();
		QList <Buddy *> getBuddies();
		QString getName();
		QString getUri();
		void update(const QDomNode &);

	private slots:
		void slotBuddyDeleted(int);

	signals:
		void deleted(int);
	
	private:
		int Id, UpdateCode;
		QMap <int, Buddy *> Buddies;
		QString Name, Uri;
		
		void addBuddy(const QDomNode &, int);
		void removeOutdatedData();
};



/* Attention:
 * 		All of the add/delete information will come from qun, but not buddy!
 */
class QunList : public QObject{
	Q_OBJECT
	public:
		QunList();
		~QunList();

		QList <Qun *> getQuns();
		QList <QString> getQunsByUri();
		Qun *addQun(const QDomNode &);
		Qun *getQunById(int);
		Qun *getQunByUri(QString);
		void update(const QDomNode &);

	signals:
		void qunAdded(Qun *);
		void qunDeleted(int);
		void qunUpdated(Qun *);
	
	private slots:
		void slotQunDeleted(int);

	private:
		QMap <int, Qun *> Quns;
};
#endif

#endif


