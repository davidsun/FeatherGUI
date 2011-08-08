#include <FeatherAccount.h>
#include <DialogCertification.h>
#include <Msg.h>

FeatherAccount::FeatherAccount(const QString &username, const QString &password, const QString &tag, const QNetworkProxy &proxy) :
	_password(password), _status("Not Logged In."), _tag(tag), _username(username){
		_feather.setProxy(proxy);
		_feather.setLoginInformation(_username, _password);

		connect(&_feather, SIGNAL(buddyAdded(FeatherGroup, FeatherBuddy)), this,
				SLOT(slotBuddyAdded(FeatherGroup, FeatherBuddy)));
		connect(&_feather, SIGNAL(initiationEvent(Feather::InitiationInformation)), this,
				SLOT(slotInitiationEvent(Feather::InitiationInformation)));
		connect(&_feather, SIGNAL(loginEvent(Feather::LoginInformation)), this, SLOT(slotLoginEvent(Feather::LoginInformation)));
		connect(&_feather, SIGNAL(messageArrival(FeatherMessage)), this, SLOT(slotMsgArrival(FeatherMessage)));
		connect(&_feather, SIGNAL(newCertification(int, QImage)), this, SLOT(slotNewCertification(int, QImage)));
		connect(&_feather, SIGNAL(SMSSendingResult(int, Feather::SMSSendingState)), this,
				SLOT(slotSMSSendingResult(int, Feather::SMSSendingState)));
		connect(this, SIGNAL(certificationInfo(int, QString)), &_feather, SLOT(slotCertificationInfo(int, QString)));
}

FeatherAccount::~FeatherAccount(){
}

void FeatherAccount::initiate(){
	_feather.initiate();
}

Feather::InitiationState FeatherAccount::initiationState() const{
	return _feather.initiationState();
}

void FeatherAccount::login(){
	_feather.login();
}

Feather::LoginState FeatherAccount::loginState() const{
	return _feather.loginState();
}

QString FeatherAccount::password() const{
	return _password;
}

int FeatherAccount::sendSMS(const QString &uri, const QString &context){
	return _feather.sendSMS(_feather.groupList().buddyById(uri.toInt()), context);
}

QString FeatherAccount::status() const{
	return _status;
}

QString FeatherAccount::tag() const{
	return _tag;
}

QString FeatherAccount::username() const{
	return _username;
}

void FeatherAccount::slotBuddyAdded(FeatherGroup group, FeatherBuddy buddy){
	_buddies[group.id()].push_back(buddy);
	emit buddyAdded(this, group, buddy);
}

void FeatherAccount::slotInitiationEvent(Feather::InitiationInformation info){
	switch (info){
		case Feather::StartInitiation:
			changeStatus("Initiating ...");
			break;
		case Feather::InitiationOk:
			emit initiationFinished(this, info);
			changeStatus("Initiation OK.");
			break;
		case Feather::InitiationTimeout:
			emit initiationFinished(this, info);
			changeStatus("Initiation Time Out.");
			break;
		case Feather::InitiationFailed:
			emit initiationFinished(this, info);
			changeStatus("Initiation Failed.");
			break;
	}
}

void FeatherAccount::slotLoginEvent(Feather::LoginInformation info){
	switch (info){
		case Feather::SSILogging:
			changeStatus("SSI Logging ...");
			break;
		case Feather::TCPConnecting:
			changeStatus("TCP Connecting ...");
			break;
		case Feather::SIPCLogging:
			changeStatus("SIPC Logging ...");
			break;
		case Feather::SSILoginSucceeded:
			changeStatus("SSI Login Succeeded.");
			break;
		case Feather::TCPConnected:
			changeStatus("TCP Connected.");
			break;
		case Feather::LoginOk:
			emit loginFinished(this, info);
			changeStatus("Login OK.");
			break;
		case Feather::SSILoginFailed:
			emit loginFinished(this, info);
			changeStatus("SSI Login Failed.");
			break;
		case Feather::LoginPasswordError:
			emit loginFinished(this, info);
			changeStatus("Password Error.");
			break;
		case Feather::LoginRequestFailure:
			emit loginFinished(this, info);
			changeStatus("Request Failure.");
			break;
		case Feather::LoginTimeout:
			emit loginFinished(this, info);
			changeStatus("Login Timeout.");
			break;
		case Feather::LoggedOut:
			emit loggedOut(this);
			changeStatus("Logged Out.");
			break;
	}
}

void FeatherAccount::slotMsgArrival(FeatherMessage msg){
	emit msgArrival(this, msg.sender(), msg.content());
}

void FeatherAccount::slotNewCertification(int id, QImage image){
	DialogCertification dialog;
	dialog.showImage(image);
	dialog.exec();
	emit certificationInfo(id, dialog.getVerifyCode());
}

void FeatherAccount::slotSMSSendingResult(int id, Feather::SMSSendingState state){
	emit SMSSendingResult(this, id, state);
}

void FeatherAccount::changeStatus(const QString &status){
	_status = status;
	emit statusChanged(this, _status);
}
