#ifndef ACCOUNTSTATISTICSLIST_H
#define ACCOUNTSTATISTICSLIST_H

#include <QObject>
#include <QTreeWidget>
#include <Lib.h>

class AccountManager;
class FeatherAccount;

class AccountStatisticsList : public QObject{
	Q_OBJECT

	friend class TagItem;

	public:
		AccountStatisticsList(QTreeWidget *treeWidget, AccountManager *accountManager);
		~AccountStatisticsList();

	private slots:
		void slotAccountAdded(FeatherAccount *);
		void slotAccountStatusChanged(FeatherAccount *, QString);
		void slotAccountSMSSendingResult(FeatherAccount *, Feather::SMSSendingState);

	private:
		class TagItem;

		QMap <QString, TagItem *> _tagItems;
		QTreeWidget *_treeWidget;
};

class AccountStatisticsList::TagItem{
	friend class AccountItem;

	public:
		TagItem(AccountStatisticsList *parent, const QString &tag);
		~TagItem();

		void addAccount(FeatherAccount *account);
		void addSMSSendingResult(FeatherAccount *, Feather::SMSSendingState);
		AccountStatisticsList *parent() const;
		void setAccountStatus(FeatherAccount *account, const QString &status);
		QString tag() const;

	private:
		class AccountItem;

		QMap <QString, QPair <int, QTreeWidgetItem *> > _accountStatusStatistics;
		QTreeWidgetItem *_accountStatusStatisticsItem;
		QMap <FeatherAccount *, AccountItem *> _accountItems;
		QTreeWidgetItem *_item;
		AccountStatisticsList *_parent;
		QMap <Feather::SMSSendingState, QPair <int, QTreeWidgetItem *> > _SMSSendingStatistics;
		QTreeWidgetItem *_SMSSendingStatisticsItem;
		QString _tag;
};

class AccountStatisticsList::TagItem::AccountItem{
	public:
		AccountItem(TagItem *parent, FeatherAccount *account);
		~AccountItem();

		void addSMSSendingResult(Feather::SMSSendingState);
		void setStatus(const QString &status);
		QString status() const;

	private:
		FeatherAccount *_account;
		QTreeWidgetItem *_item;
		AccountStatisticsList::TagItem *_parent;
		QMap <Feather::SMSSendingState, QPair <int, QTreeWidgetItem *> > _SMSSendingStatistics;
		QTreeWidgetItem *_SMSSendingStatisticsItem;
		QTreeWidgetItem *_statusItem;
		QString _status;
};

#endif // ACCOUNTSTATISTICSLIST_H
