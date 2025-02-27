﻿#ifndef FRIEND_H_
#define FRIEND_H_

#include <QJsonObject>
#include <QPixmap>
class Friend
{
public:
	Friend(const QString& id = "");
	const QString getFriendId()const;
	void setFriend(const QJsonObject& obj);
	const QJsonObject getFriend()const;
private:
	QString m_userID{};
	QString m_userName{};
	int m_gender{};
	int m_age{};
	QString m_phoneNumber{};
	QString m_email{};
	QPixmap m_avatar{};
	QString m_avatarPath{};
	bool m_status{};
	QJsonObject m_json{};

};

#endif // !FRIEND_H_
