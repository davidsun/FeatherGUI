#ifndef ACCOUNTLIST_H
#define ACCOUNTLIST_H

#include <Lib.h>
#include <QObject>
#include <QTreeWidget>

class AccountManager;
class FeatherAccount;
class Msg;

class AccountList : public QObject{
	Q_OBJECT

	friend class TagItem;

	public:
		AccountList(QTreeWidget *treeWidget, AccountManager *accountManager);
		~AccountList();

		QList <QPair <const FeatherAccount *, FeatherBuddy> > checkedItems() const;
		void updateMsgCheckedDetail(Msg *msg) const;
		void updateMsgDetailByTag(const QString &tag, Msg *msg) const;

	private slots:
		void slotAccountAdded(FeatherAccount *);
		void slotAccountBuddyAdded(FeatherAccount *, FeatherGroup, FeatherBuddy);
		void slotAccountLoginFailed(FeatherAccount *, QString);
		void slotAccountStatusChanged(FeatherAccount *, QString);
		void slotWidgetItemClicked(QTreeWidgetItem *, int);

	private:
		class TagItem;

		QMap <QString, TagItem *> _tagItems;
		QTreeWidget *_treeWidget;

		void updateCheckState(QTreeWidgetItem *item);
		void setCheckState(QTreeWidgetItem *item, Qt::CheckState state);
};

class AccountList::TagItem{
	friend class AccountItem;

	public:
		TagItem(AccountList *parent, const QString &tag);
		~TagItem();

		void addAccount(FeatherAccount *);
		void addBuddy(FeatherAccount *, FeatherGroup, FeatherBuddy);
		QList <QPair <const FeatherAccount *, FeatherBuddy> > checkedItems() const;
		AccountList *parent() const;
		void setAccountLoginFailed(FeatherAccount *);
		void setAccountStatus(FeatherAccount *, const QString &);
		QString tag() const;
		void updateMsgCheckedDetail(Msg *msg) const;
		void updateMsgDetail(Msg *msg) const;

	private:
		class AccountItem;

		QMap <FeatherAccount *, AccountItem *> _accountItems;
		QTreeWidgetItem *_item;
		AccountList *_parent;
		QString _tag;
};

class AccountList::TagItem::AccountItem{
	friend class GroupItem;

	public:
		AccountItem(TagItem *parent, FeatherAccount *account);
		~AccountItem();

		void addBuddy(FeatherGroup group, FeatherBuddy buddy);
		const FeatherAccount *account() const;
		QList <FeatherBuddy> checkedItems() const;
		AccountList::TagItem *parent() const;
		void setLoginFailed();
		void setStatus(const QString &status);
		QString tag() const;
		void updateMsgCheckedDetail(Msg *msg) const;
		void updateMsgDetail(Msg *msg) const;

	private:
		class GroupItem;

		FeatherAccount *_account;
		QMap <int, GroupItem *> _groupItems;
		QTreeWidgetItem *_item;
		AccountList::TagItem *_parent;
};

class AccountList::TagItem::AccountItem::GroupItem{
	public:
		GroupItem(AccountItem *parent, const FeatherGroup &group);
		~GroupItem();

		void addBuddy(const FeatherBuddy &buddy);
		QList <FeatherBuddy> checkedItems() const;
		AccountList::TagItem::AccountItem *parent() const;
		void updateMsgCheckedDetail(Msg *msg) const;
		void updateMsgDetail(Msg *msg) const;

	private:
		QList <QPair <FeatherBuddy, QTreeWidgetItem *> > _buddies;
		int _id;
		QTreeWidgetItem *_item;
		QString _name;
		AccountList::TagItem::AccountItem *_parent;
};

#endif // ACCOUNTLIST_H
