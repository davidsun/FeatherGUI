#ifndef FEATHERMESSAGE_H
#define FEATHERMESSAGE_H

#include <QtCore>
#include <BuddyInformation.h>

class FeatherMessage : public QObject{
	Q_OBJECT
	public:
		enum MessageType{
			BuddyMessage = 0,
			SystemMessage = 1
		};

		FeatherMessage(MessageType, const FeatherBuddy &sender = FeatherBuddy(0), const QString &content = QString());
		FeatherMessage(const FeatherMessage &);

		FeatherBuddy sender() const;
		MessageType type() const;
		QString content() const;

	private:
		FeatherBuddy _sender;
		MessageType _type;
		QString _content;
};

#endif // FEATHERMESSAGE_H
