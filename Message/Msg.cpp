#include <Msg.h>
#include <stdio.h>

Msg::Msg(QString context) :
	QObject(0), _context(context), _type(InstantSMS){
}

Msg::~Msg(){
	for (QSet <MsgDestination *>::iterator it = _destinations.begin(); it != _destinations.end(); it ++) delete *it;
}

void Msg::addDestination(MsgDestination *destination){
	_destinations.insert(destination);
}

QString Msg::context() const{
	return _context;
}

QList <MsgDestination *> Msg::destionations() const{
	QList <MsgDestination *> ret;
	for (QSet <MsgDestination *>::const_iterator it = _destinations.begin(); it != _destinations.end(); it ++)
		ret.push_back(*it);
	return ret;
}

int Msg::destinationCount() const{
	return _destinations.size();
}

void Msg::removeDestination(MsgDestination *destination){
	delete destination;
	_destinations.remove(destination);
	emit msgDestinationDestroyed(this, destination);
}

Msg::Type Msg::type() const{
	return _type;
}

Msg::Msg(Type type, QString context) :
	QObject(0), _context(context), _type(type){
}
