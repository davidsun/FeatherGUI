#include <Msg.h>

MsgDestination::MsgDestination(Msg *parent, FeatherAccount *account, QString uri) :
	_account(account), _parent(parent), _uri(uri){
}

FeatherAccount *MsgDestination::account() const{
	return _account;
}

Msg *MsgDestination::parent() const{
	return _parent;
}

QString MsgDestination::uri() const{
	return _uri;
}

MsgDestination::~MsgDestination(){
}
