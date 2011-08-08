#ifndef DIALOGCERTIFICATION_H
#define DIALOGCERTIFICATION_H

#include <QtGui>
#include "ui_DialogCertification.h"

class DialogCertification : public QDialog{
	Q_OBJECT
	public:
		DialogCertification();
		
		QString getVerifyCode();
		void showImage(QImage);
	
	private:
		Ui::DialogCertification Ui;
};

#endif
