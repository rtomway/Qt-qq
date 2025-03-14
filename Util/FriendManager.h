﻿#ifndef FriendMANAGER_H_
#define FriendMANAGER_H_

#include <QJsonObject>
#include <QObject>
#include <QSharedPointer>

#include "Friend.h"

class FriendManager : public QObject
{
	Q_OBJECT
public:
	// 获取单例实例的静态方法
	static FriendManager* instance();
	// 禁止拷贝构造函数和赋值操作符
	FriendManager(const FriendManager&) = delete;
	FriendManager& operator=(const FriendManager&) = delete;
	//当前客户端登录用户
	static QString m_oneselfID;
	static void setOneselfID(const QString& id);
	const QString getOneselfID()const;
	//好友处理
	void addFriend(const QSharedPointer<Friend>& user);
	QSharedPointer<Friend>findFriend(const QString& id)const;
	const QHash<QString, QSharedPointer<Friend>>& getFriends()const;
	void loadAvatar(const QString& user_id);
	//消息发送服务端
	void updateUserMessageToServer(const QJsonObject& obj);
	void updateUserAvatarToServer(const QPixmap& pixmap);
	void updateUserGroupingToServer(const QString& user_id, const QString& friend_id, const QString& grouping);
	//清除
	void clearFriendManager();

private:
	// 私有构造函数，防止外部实例化
	FriendManager();
	//管理所有用户
	QHash<QString, QSharedPointer<Friend>>m_user;
	QHash<QString, QString>m_grouping;
signals:
	void UserAvatarLoaded(const QPixmap& avatar);
	void FriendAvatarLoaded(const QPixmap& avatar);
	void UpdateFriendMessage(const QString& user_id);
	void UpdateFriendAvatar(const QString& user_id);
	void NewFriend(const QString& user_id);
	void updateFriendGrouping(const QString& user_id, const QString& oldGrouping);
};


#endif // !FriendMANAGER_H_