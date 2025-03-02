﻿#ifndef CONTACTDETAILWIDGET_H_
#define CONTACTDETAILWIDGET_H_

#include <QWidget>
#include "LineEditwithButton.h"
#include "AngleRoundedWidget.h"
#include <QJsonObject>

namespace Ui { class ContactDetailWidget; }

class ContactDetailWidget :public AngleRoundedWidget
{
	Q_OBJECT
public:
	ContactDetailWidget(QWidget* parent = nullptr);
	~ContactDetailWidget();
	void init();
private:
	QLabel* m_editDetail{};
	QPushButton* m_exitBtn{};
	QLabel* m_headLab{};
	LineEditwithButton* m_nickNameEdit{};
	LineEditwithButton* m_signaltureEdit{};
	LineEditwithButton* m_genderEdit{};
	LineEditwithButton* m_birthdayEdit{};
	LineEditwithButton* m_countryEdit{};
	LineEditwithButton* m_provinceEdit{};
	LineEditwithButton* m_areaEdit{};
private:
	QPixmap m_headPix;
	QString m_avatarPath;
	QJsonObject m_json{};
public:
	void setUser(const QJsonObject& obj);
private:
	QString getAvatarFolderPath();
	bool saveAvatarToLocal(const QString& avatarPath,const QString&user_id);
protected:
	bool eventFilter(QObject* watched, QEvent* event)override;
};



#endif // !CONTACTDETAILWIDGET_H_