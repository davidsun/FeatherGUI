#include <AccountStatisticsList.h>
#include <FeatherAccount.h>

AccountStatisticsList::TagItem::TagItem(AccountStatisticsList *parent, const QString &tag) :
	_parent(parent), _tag(tag){
		QFont defaultFont;
		defaultFont.setBold(1);

		_item = new QTreeWidgetItem(_parent -> _treeWidget);
		_item -> setText(0, "Tag: " + tag);
		_item -> setFont(0, defaultFont);

		_accountStatusStatisticsItem = new QTreeWidgetItem(_item);
		_accountStatusStatisticsItem -> setText(0, "Statistics - Account Status");
		_accountStatusStatisticsItem -> setFont(0, defaultFont);
		_accountStatusStatisticsItem -> setForeground(0, Qt::blue);
		_accountStatusStatisticsItem -> setExpanded(1);

		_SMSSendingStatisticsItem = new QTreeWidgetItem(_item);
		_SMSSendingStatisticsItem -> setText(0, "Statistics - SMS Sending");
		_SMSSendingStatisticsItem -> setFont(0, defaultFont);
		_SMSSendingStatisticsItem -> setForeground(0, Qt::blue);
		_SMSSendingStatisticsItem -> setExpanded(1);
}

AccountStatisticsList::TagItem::~TagItem(){
	for (QMap <FeatherAccount *, AccountItem *>::iterator it = _accountItems.begin(); it != _accountItems.end(); it ++)
		delete it.value();
	delete _item;
}

void AccountStatisticsList::TagItem::addAccount(FeatherAccount *account){
	AccountItem *item = new AccountItem(this, account);
	_accountItems[account] = item;

	if (_accountStatusStatistics.find(account -> status()) == _accountStatusStatistics.end()){
		QTreeWidgetItem *item = new QTreeWidgetItem(_accountStatusStatisticsItem);
		_accountStatusStatistics[account -> status()] = qMakePair(0, item);
	}
	_accountStatusStatistics[account -> status()].first ++;
	_accountStatusStatistics[account -> status()].second -> setText(
			0, "(" + account -> status() + "): " + QString::number(_accountStatusStatistics[account -> status()].first));
}

void AccountStatisticsList::TagItem::addSMSSendingResult(FeatherAccount *account, Feather::SMSSendingState state){
	if (_SMSSendingStatistics.find(state) == _SMSSendingStatistics.end()){
		QTreeWidgetItem *item = new QTreeWidgetItem(_SMSSendingStatisticsItem);
		_SMSSendingStatistics[state] = qMakePair(0, item);
	}

	_SMSSendingStatistics[state].first ++;
	switch (state){
		case Feather::SendingFailed:
			_SMSSendingStatistics[state].second -> setText(0, "Sending Failed: " +
														   QString::number(_SMSSendingStatistics[state].first));
			break;
		case Feather::SendingLimitExceeded:
			_SMSSendingStatistics[state].second -> setText(0, "Sending Failed (Exceeded Daily/Monthly Sending Limit): " +
														   QString::number(_SMSSendingStatistics[state].first));
			break;
		case Feather::SendingOk:
			_SMSSendingStatistics[state].second -> setText(0, "Sending Succeeded: " +
														   QString::number(_SMSSendingStatistics[state].first));
			break;
		case Feather::SendingTimeout:
			_SMSSendingStatistics[state].second -> setText(0, "Sending Time Out: " +
														   QString::number(_SMSSendingStatistics[state].first));
			break;
	}

	QMap <FeatherAccount *, AccountItem *>::iterator it = _accountItems.find(account);
	if (it != _accountItems.end()) it.value() -> addSMSSendingResult(state);
}

AccountStatisticsList *AccountStatisticsList::TagItem::parent() const{
	return _parent;
}

void AccountStatisticsList::TagItem::setAccountStatus(FeatherAccount *account, const QString &status){
	QMap <FeatherAccount *, AccountItem *>::iterator it = _accountItems.find(account);
	if (it == _accountItems.end()) return;

	_accountStatusStatistics[it.value() -> status()].first --;
	if (_accountStatusStatistics[it.value() -> status()].first == 0){
		delete _accountStatusStatistics[it.value() -> status()].second;
		_accountStatusStatistics.remove(it.value() -> status());
	}

	it.value() -> setStatus(status);
	if (_accountStatusStatistics.find(status) == _accountStatusStatistics.end()){
		QTreeWidgetItem *item = new QTreeWidgetItem(_accountStatusStatisticsItem);
		_accountStatusStatistics[status] = qMakePair(0, item);
	}
	_accountStatusStatistics[status].first ++;
	_accountStatusStatistics[status].second -> setText(
			0, "(" + status + "): " + QString::number(_accountStatusStatistics[status].first));
}

QString AccountStatisticsList::TagItem::tag() const{
	return _tag;
}
