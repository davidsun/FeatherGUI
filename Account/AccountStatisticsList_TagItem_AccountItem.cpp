#include <AccountStatisticsList.h>
#include <FeatherAccount.h>

AccountStatisticsList::TagItem::AccountItem::AccountItem(TagItem *parent, FeatherAccount *account) :
	_account(account), _parent(parent), _status(_account -> status()){
		_item = new QTreeWidgetItem(_parent -> _item);
		_item -> setText(0, "Account: " + account -> username());
		_item -> setForeground(0, Qt::darkBlue);
		
		_statusItem = new QTreeWidgetItem(_item);
		_statusItem -> setText(0, "Status - " + _status);

		_SMSSendingStatisticsItem = new QTreeWidgetItem(_item);
		_SMSSendingStatisticsItem -> setText(0, "Statistics - SMS Sending");
}

AccountStatisticsList::TagItem::AccountItem::~AccountItem(){
	delete _item;
}

void AccountStatisticsList::TagItem::AccountItem::addSMSSendingResult(Feather::SMSSendingState state){
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
}

void AccountStatisticsList::TagItem::AccountItem::setStatus(const QString &status){
	_status = status;
	_statusItem -> setText(0, "Status: " + _status);
}

QString AccountStatisticsList::TagItem::AccountItem::status() const{
	return _status;
}

