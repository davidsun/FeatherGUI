#include <DialogCertification.h>

DialogCertification::DialogCertification(){
	Ui.setupUi(this);
}

void DialogCertification::showImage(QImage image){
	Ui.LabelImage -> setPixmap(QPixmap::fromImage(image));
}

QString DialogCertification::getVerifyCode(){
	return Ui.LineEditVerifyCode -> text();
}
