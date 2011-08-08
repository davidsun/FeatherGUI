#include <QtNetwork>
#include <MainWindow.h>
#include <QMessageBox>
#include <MsgList.h>
#include <MsgManager.h>
#include <FeatherAccount.h>
#include <AccountManager.h>
#include <AccountList.h>
#include <AccountStatisticsList.h>

static char intToHex(int x){
	if (x < 10) return x + '0';
	else return x - 10 + 'A';
}

static QByteArray binaryToHex(const QByteArray &arr){
	QByteArray ret;
	for (int i = 0; i < arr.size(); i ++){
		ret.append(intToHex((arr[i] + 128) / 16));
		ret.append(intToHex((arr[i] + 128) % 16));
	}
	return ret;
}

MainWindow::MainWindow(){
	/* UI setting up */
	_ui.setupUi(this);
	slotAccountManagerStateChanged(AccountManager::Idle);

	_accountManager = new AccountManager;
	connect(_accountManager, SIGNAL(stateChanged(AccountManager::State)), this,
			SLOT(slotAccountManagerStateChanged(AccountManager::State)));
	connect(_accountManager, SIGNAL(accountMsgArrival(FeatherAccount *, FeatherBuddy, QString)), this,
			SLOT(slotAccountMsgArrival(FeatherAccount *, FeatherBuddy, QString)));
	connect(_accountManager, SIGNAL(accountLoginFailed(FeatherAccount *, QString)), this,
			SLOT(slotAccountLoginFailed(FeatherAccount *, QString)));
	connect(_accountManager, SIGNAL(accountStatusChanged(FeatherAccount *, QString)), this,
			SLOT(slotAccountStatusChanged(FeatherAccount *, QString)));

	_accountList = new AccountList(_ui.ListAccount, _accountManager);

	_accountStatisticsList = new AccountStatisticsList(_ui.ListAccountStatistics, _accountManager);

	_accountManager -> updateAccounts();

	_msgManager = new MsgManager;

	_instantSMSList = new InstantSMSList(_ui.ListInstantSMS, _msgManager);
	connect(_instantSMSList, SIGNAL(msgCleared()), this, SLOT(slotInstantSMSListMessageCleared()));

	_scheduledSMSList = new ScheduledSMSList(_ui.ListScheduledSMS, _msgManager);

	connect(_ui.TextMessage, SIGNAL(textChanged()), this, SLOT(slotTextMessageChanged()));
	
	/* Makedir */
	QDir dir(QApplication::applicationDirPath());
	dir.mkdir("Log");

	/* Read settings */
	QFile file(QApplication::applicationDirPath() + "/Settings.xml");
	if (file.open(QFile::ReadOnly | QFile::Text)){
		QDomDocument doc;
		doc.setContent(&file, 0, 0, 0, 0);
		QDomElement docFib = doc.firstChildElement("settings").firstChildElement();
		for (; !docFib.isNull(); docFib = docFib.nextSiblingElement()){
			if (docFib.tagName() == "forbiddance"){
				QDomElement docAccount = docFib.firstChildElement("account");
				for (; !docAccount.isNull(); docAccount = docAccount.nextSiblingElement()){
					QTreeWidgetItem *item = new QTreeWidgetItem(_ui.ListForbiddance);
					item -> setText(0, docAccount.attribute("username"));
					QDomElement docBuddy = docAccount.firstChildElement("buddy");
					for (; !docBuddy.isNull(); docBuddy = docBuddy.nextSiblingElement()){
						QTreeWidgetItem *itemBuddy = new QTreeWidgetItem(item);
						itemBuddy -> setText(0, docBuddy.attribute("text"));
						itemBuddy -> setText(1, docBuddy.attribute("id"));
					}
				}
			}
		}
	}
}

MainWindow::~MainWindow(){
	QFile file(QApplication::applicationDirPath() + "/Settings.xml");
	if (file.open(QFile::WriteOnly | QFile::Text)){
		QXmlStreamWriter xml;
		xml.setAutoFormatting(1);
		xml.setDevice(&file);
		xml.writeStartDocument("1.0");
		xml.writeStartElement("settings");
		xml.writeStartElement("forbiddance");
		for (int i = 0; i < _ui.ListForbiddance -> topLevelItemCount(); i ++){
			QTreeWidgetItem *item = _ui.ListForbiddance -> topLevelItem(i);
			xml.writeStartElement("account");
			xml.writeAttribute("username", item -> text(0));
			for (int j = 0; j < item -> childCount(); j ++){
				xml.writeStartElement("buddy");
				xml.writeAttribute("text", item -> child(j) -> text(0));
				xml.writeAttribute("id", item -> child(j) -> text(1));
				xml.writeEndElement();	//Element: buddy
			}
			xml.writeEndElement();		//Element: account
		}
		xml.writeEndElement();			//Element: forbiddance
		xml.writeEndElement();			//Element: settings
		xml.writeEndDocument();
	}
}

void MainWindow::slotAccountLoginFailed(FeatherAccount *account, QString description){
	QTreeWidgetItem *item = new QTreeWidgetItem(_ui.ListErrorAccount);
	item -> setText(0, account -> tag() + ": " + account -> username());
	item -> setText(1, description);
}

void MainWindow::slotAccountManagerStateChanged(AccountManager::State state){
	if (state != AccountManager::Ready){
		_ui.ButtonAddMessage -> setEnabled(0);
		_ui.ButtonDeleteMessage -> setEnabled(0);
		_ui.ButtonSendMessage -> setEnabled(0);
		_ui.ButtonImportMessages -> setEnabled(0);

		_ui.ActionAddMessage -> setEnabled(0);
		_ui.ActionDeleteMessage -> setEnabled(0);
		_ui.ActionForbid -> setEnabled(0);
		_ui.ActionImportMessage -> setEnabled(0);
	}  else {
		if (!_instantSMSList -> msgCount()){
			_ui.ButtonAddMessage -> setEnabled(1);
			_ui.ButtonDeleteMessage -> setEnabled(1);
			_ui.ButtonSendMessage -> setEnabled(1);
		}  else {
			_ui.ButtonAddMessage -> setEnabled(0);
			_ui.ButtonDeleteMessage -> setEnabled(0);
			_ui.ButtonSendMessage -> setEnabled(0);
		}
		_ui.ButtonImportMessages -> setEnabled(1);

		_ui.ActionAddMessage -> setEnabled(1);
		_ui.ActionDeleteMessage -> setEnabled(1);
		_ui.ActionForbid -> setEnabled(1);
		_ui.ActionImportMessage -> setEnabled(1);
	}
}

void MainWindow::slotAccountMsgArrival(FeatherAccount *account, FeatherBuddy buddy, QString context){
	QTreeWidgetItem *item = new QTreeWidgetItem(_ui.ListMessageArrived);
	item -> setText(0, account -> username());
	item -> setText(1, buddy.uri());
	item -> setText(2, context);
	item -> setToolTip(2, context);
}

void MainWindow::slotAccountStatusChanged(FeatherAccount *, QString){
	if (_accountManager -> state() == AccountManager::Ready){
		_ui.StatusBar -> showMessage("All logged in. (With " + QString::number(_accountManager -> loginFailedAccountsCount())
									 + " error account(s))");
	}  else {
		_ui.StatusBar -> showMessage("Logging ... (" + QString::number(_accountManager -> loginSucceededAccountsCount()) + "/" +
									 QString::number(_accountManager -> accountCount() - _accountManager -> loginFailedAccountsCount()) +
									 ") (With " + QString::number(_accountManager -> loginFailedAccountsCount()) + " error account(s))");
	}
}

void MainWindow::slotActionTriggered(QAction *action){
	if (action -> objectName() == "ActionAddMessage"){
		slotButtonAddMessageClicked();
	}  else if (action -> objectName() == "ActionDeleteMessage"){
		slotButtonDeleteMessageClicked();
	}  else if (action -> objectName() == "ActionForbid"){
		slotButtonAddForbiddanceClicked();
	}  else if (action -> objectName() == "ActionImportMessage"){
		slotButtonImportMessagesClicked();
	}
}

void MainWindow::slotButtonAddForbiddanceClicked(){
	QList <QPair <const FeatherAccount *, FeatherBuddy> > checkedItems = _accountList -> checkedItems();
	for (int i = 0; i < checkedItems.size(); i ++){
		QList <QTreeWidgetItem *> itemList = _ui.ListForbiddance -> findItems(checkedItems[i].first -> username(), Qt::MatchExactly);
		QTreeWidgetItem *item;
		if (itemList.empty()){
			item = new QTreeWidgetItem(_ui.ListForbiddance);
			item -> setText(0, checkedItems[i].first -> username());
		}  else {
			item = itemList[0];
		}

		bool found = 0;
		for (int j = 0; !found && j < item -> childCount(); j ++)
			if (item -> child(j) -> text(1).toInt() == checkedItems[i].second.id()) found = 1;
		if (!found){
			QTreeWidgetItem *itemx = new QTreeWidgetItem(item);
			if (checkedItems[i].second.nickname().size()){
				itemx -> setText(0, checkedItems[i].second.nickname() + "(" + checkedItems[i].second.uri() + ")");
			}  else {
				itemx -> setText(0, checkedItems[i].second.uri());
			}
			itemx -> setText(1, QString::number(checkedItems[i].second.id()));
		}
	}
}

void MainWindow::slotButtonAddMessageClicked(){
	if (!_ui.TextMessage -> toPlainText().size()) return;

	InstantSMS *sms = new InstantSMS(_ui.TextMessage -> toPlainText());
	_accountList -> updateMsgCheckedDetail(sms);
	QList <MsgDestination *> dest = sms -> destionations();
	for (int i = 0; i < dest.size(); i ++){
		bool found = 0;
		QList <QTreeWidgetItem *> itemList = _ui.ListForbiddance -> findItems(dest[i] -> account() -> username(), Qt::MatchExactly);
		if (itemList.size()){
			QTreeWidgetItem *item = itemList[0];
			for (int j = 0; !found && j < item -> childCount(); j ++)
				if (item -> child(j) -> text(1) == dest[i] -> uri()) found = 1;
		}
		if (found) sms -> removeDestination(dest[i]);
	}
	_msgManager -> addMsg(sms);
}

void MainWindow::slotButtonDeleteMessageClicked(){
	QTreeWidgetItem *item = _ui.ListInstantSMS -> currentItem();
	delete (QObject *)item -> text(2).toInt();
}

void MainWindow::slotButtonImportMessagesClicked(){
	QString fileName = QFileDialog::getOpenFileName(this, tr("Save Message"), QDir::currentPath(), tr("XML Messages (*.xmlm)"));
	if (fileName.isEmpty()) return;
	QFile file(fileName);
	if (!file.open(QFile::ReadOnly | QFile::Text)){
		QMessageBox::warning(this, tr("Error"),
							 tr("Cannot open file %1:\n%2.").arg(fileName).arg(file.errorString()));
		return;
	}
	QDomDocument doc;
	doc.setContent(&file, 0, 0, 0, 0);
	QDomElement docMessage = doc.firstChildElement("message-list").firstChildElement();
	while (!docMessage.isNull()){
		if (docMessage.tagName() == "message"){
			QByteArray context = docMessage.attribute("context").toUtf8();
			if (binaryToHex(QCryptographicHash::hash(context, QCryptographicHash::Md5)) == docMessage.attribute("checksum")){
				QDomElement docReceiver = docMessage.firstChildElement("receiver");
				while (!docReceiver.isNull()){
					QTime smsTime = QTime::currentTime();
					if (docReceiver.attribute("time") != "<Empty>") smsTime = QTime::fromString(docReceiver.attribute("time"), "h:m:s");

					ScheduledSMS *sms = new ScheduledSMS(QString::fromUtf8(context), smsTime);
					_accountList -> updateMsgDetailByTag(docReceiver.attribute("name"), sms);
					QList <MsgDestination *> dest = sms -> destionations();
					for (int i = 0; i < dest.size(); i ++){
						bool found = 0;
						QList <QTreeWidgetItem *> itemList = _ui.ListForbiddance -> findItems(dest[i] -> account() -> username(), Qt::MatchExactly);
						if (itemList.size()){
							QTreeWidgetItem *item = itemList[0];
							for (int j = 0; !found && j < item -> childCount(); j ++)
								if (item -> child(j) -> text(1) == dest[i] -> uri()) found = 1;
						}
						if (found) sms -> removeDestination(dest[i]);
					}
					_msgManager -> addMsg(sms);
					_msgManager -> processMsg(sms);
					docReceiver = docReceiver.nextSiblingElement();
				}
			}
        }
		docMessage = docMessage.nextSiblingElement();
    }
}

void MainWindow::slotButtonLoginClicked(){
	_ui.ButtonLogin -> setEnabled(0);
	_accountManager -> startLogin();
}

void MainWindow::slotButtonRemoveForbiddanceClicked(){
	if (_ui.ListForbiddance -> currentItem()){
		QTreeWidgetItem *item = _ui.ListForbiddance -> currentItem() -> parent();
		delete _ui.ListForbiddance -> currentItem();
		if (item && item -> childCount() == 0) delete item;
	}
}

void MainWindow::slotButtonSendMessageClicked(){
	if (_instantSMSList -> msgCount()){
		_ui.ButtonAddMessage -> setEnabled(0);
		_ui.ButtonDeleteMessage -> setEnabled(0);
		_ui.ButtonSendMessage -> setEnabled(0);
		QList <Msg *> msg = _instantSMSList -> msgs();
		for (int i = 0; i < msg.size(); i ++)
			_msgManager -> processMsg(msg[i]);
	}
}

void MainWindow::slotInstantSMSListMessageCleared(){
	_ui.ButtonAddMessage -> setEnabled(1);
	_ui.ButtonDeleteMessage -> setEnabled(1);
	_ui.ButtonSendMessage -> setEnabled(1);
}

void MainWindow::slotTextMessageChanged(){
	int length = _ui.TextMessage -> toPlainText().size();
	if (length <= 180) _ui.LabelMessageLength -> setText("<font color=\"black\">" + QString::number(length) + "/180</font>");
	else _ui.LabelMessageLength -> setText("<font color=\"red\">" + QString::number(length) + "/180</font>");
}
