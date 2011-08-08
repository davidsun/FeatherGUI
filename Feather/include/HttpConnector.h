#ifndef HTTPCONNECTOR_H
#define HTTPCONNECTOR_H

#include <QHttp>
#include <QNetworkProxy>

class HttpConnector : public QHttp{
	Q_OBJECT
	public:
		HttpConnector(QObject *parent = 0);

		void performGet(const QString &address);
		void performPost(const QString &address, const QString &postData);
		void setProxy(const QNetworkProxy &proxy);

	private:
		QNetworkProxy _proxy;

};

#endif // HTTPCONNECTOR_H
