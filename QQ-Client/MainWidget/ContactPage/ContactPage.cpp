﻿#include "ContactPage.h"
#include "ui_ContactPage.h"
#include <QFile>
#include <QToolButton>

#include "ImageUtil.h"
#include "SMaskWidget.h"
#include "ContactList.h"
#include "FriendManager.h"


ContactPage::ContactPage(QWidget* parent)
	:AngleRoundedWidget(parent)
	, ui(new Ui::ContactPage)
	, m_detailEditWidget(new ContactDetailWidget)
	, m_oneself(nullptr)
{
	ui->setupUi(this);
	init();
	QFile file(":/stylesheet/Resource/StyleSheet/ContactPage.css");
	if (file.open(QIODevice::ReadOnly))
	{
		this->setStyleSheet(file.readAll());
	}
	else
	{
		qDebug() << file.fileName() << "打开失败";
	}
	this->setWindowFlag(Qt::FramelessWindowHint);
}

ContactPage::~ContactPage()
{
	delete ui;
}

void ContactPage::init()
{
	this->setObjectName("ContactPage");
	//还未初始化禁止信号
	ui->groupcomBox->blockSignals(true);

	ui->headLab->setPixmap(ImageUtils::roundedPixmap(QPixmap(":/picture/Resource/Picture/h2.jpg"), QSize(100, 100)));
	ui->nameLab->setText("哈哈");
	ui->onlineBtn->setIcon(QIcon(":/icon/Resource/Icon/online.png"));
	ui->onlineBtn->setText("在线");
	ui->likeBtn->setIcon(QIcon(":/icon/Resource/Icon/like.png"));
	ui->likeBtn->setText(QString::number(m_likeCount));
	connect(ui->likeBtn, &QToolButton::clicked, [=]()
		{
			ui->likeBtn->setText(QString::number(++m_likeCount));
		});
	ui->genderBtn->setIcon(QIcon(":/icon/Resource/Icon/man.png"));
	ui->genderBtn->setText("男");
	ui->ageLab->setText(QString("25") + "岁");
	ui->friendGroupBtn->setIcon(QIcon(":/icon/Resource/Icon/friendgroup.png"));
	ui->signaltureBtn->setIcon(QIcon(":/icon/Resource/Icon/signalture.png"));
	//获取好友分组
	auto groupingName = ContactList::instance()->getfGrouping();
	for (auto name : groupingName)
	{
		ui->groupcomBox->addItem(name);
	}
	//更新好友分组
	connect(ContactList::instance(), &ContactList::updateFriendgrouping, this, [=]
		{
			ui->groupcomBox->clear();
			auto groupingName = ContactList::instance()->getfGrouping();
			for (auto name : groupingName)
			{
				ui->groupcomBox->addItem(name);
			}
		});
	//信息编辑
	connect(ui->editdetailBtn, &QPushButton::clicked, [=]()
		{
			if (!this->parent())
			{
				this->hide();
			}
			m_detailEditWidget->setUser(m_json);
			SMaskWidget::instance()->popUp(m_detailEditWidget);
			auto mainWidgetSize = SMaskWidget::instance()->getMainWidgetSize();
			int x = (mainWidgetSize.width() - m_detailEditWidget->width()) / 2;
			int y = (mainWidgetSize.height() - m_detailEditWidget->height()) / 2;
			SMaskWidget::instance()->setPopGeometry(QRect(x, y, this->width(), this->height()));
		});
	//更新信息
	connect(FriendManager::instance(), &FriendManager::UpdateFriendMessage, this, [=](const QString& user_id)
		{
			auto user = FriendManager::instance()->findFriend(user_id);
			this->setUser(user->getFriend());
		});
	//发消息
	connect(ui->sendmessageBtn, &QPushButton::clicked, this, [=]
		{
			emit sendMessage(m_json["user_id"].toString());
		});
	//分组改变
	connect(ui->groupcomBox, &QComboBox::currentIndexChanged, [=](int index)
		{
			auto grouping = ui->groupcomBox->itemText(index);
			qDebug() << "分组改变" << grouping;
			if (m_oneself && !grouping.isEmpty())
			{
				qDebug() << "分组" << grouping;
				auto oldGrouping = m_oneself->getGrouping();
				m_oneself->setGrouping(grouping);
				auto friend_id = m_oneself->getFriendId();
				FriendManager::instance()->emit updateFriendGrouping(friend_id, oldGrouping);
				//发送给服务端
				auto user_id = FriendManager::instance()->getOneselfID();
				FriendManager::instance()->updateUserGroupingToServer(user_id, friend_id, grouping);
			}
		});
}

void ContactPage::setUser(const QJsonObject& obj)
{
	m_json = obj;
	m_userId = obj["user_id"].toString();
	ui->groupcomBox->blockSignals(false);
	//m_oneself为空或id不等,存入新的Friend
	if (!m_oneself || m_oneself->getFriendId() != m_userId)
	{
		m_oneself = FriendManager::instance()->findFriend(m_userId);
		qDebug() << "id不同------------";
	}
	//获取friend信息
	m_json = m_oneself->getFriend();
	qDebug() << "个人信息json:" << m_json;
	//未设置信息隐藏
	//可编辑
	if (FriendManager::instance()->getOneselfID() != m_json["user_id"].toString())
	{
		ui->editdetailBtn->setVisible(false);
	}
	else {
		ui->editdetailBtn->setVisible(true);
	}
	//年龄
	if (m_json["age"].toString().isEmpty())
	{
		ui->line_2->setVisible(false);
		ui->ageLab->setVisible(false);
	}
	else {
		ui->line_2->setVisible(true);
		ui->ageLab->setVisible(true);
		ui->ageLab->setText(QString::number(m_json["age"].toInt()) + "岁");
	}
	//生日
	if (m_json["birthday"].toString().isEmpty())
	{
		ui->line_3->setVisible(false);
		ui->birthdayLab->setVisible(false);
	}
	else {
		ui->line_3->setVisible(true);
		ui->birthdayLab->setVisible(true);
		ui->birthdayLab->setText(m_json["birthday"].toString());
	}
	//居住地
	if (m_json["resident"].toString().isEmpty())
	{
		ui->line_4->setVisible(false);
		ui->label_2->setVisible(false);
		ui->residentLab->setVisible(false);
	}
	else
	{
		ui->line_4->setVisible(true);
		ui->label_2->setVisible(true);
		ui->residentLab->setVisible(true);
	}
	//控件信息更新
	ui->nameLab->setText(m_json["username"].toString());
	ui->idLab->setText(m_json["user_id"].toString());
	ui->genderBtn->setText(m_json["gender"].toInt() == 1 ? "男" : (m_json["gender"].toInt() == 2 ? "女" : "未知"));
	if (m_json["gender"].toInt() == 1)
	{
		ui->genderBtn->setIcon(QIcon(":/icon/Resource/Icon/man.png"));
	}
	else if (m_json["gender"].toInt() == 2)
	{
		ui->genderBtn->setIcon(QIcon(":/icon/Resource/Icon/woman.png"));
	}
	else
	{
		ui->genderBtn->setIcon(QIcon(":/icon/Resource/Icon/nogender.png"));
	}
	ui->groupcomBox->setCurrentText(obj["grouping"].toString());
	QSharedPointer<Friend> myfriend = FriendManager::instance()->findFriend(obj["user_id"].toString());
	auto pixmap = ImageUtils::roundedPixmap(myfriend->getAvatar(), QSize(100, 100));
	ui->headLab->setPixmap(pixmap);
	ui->signaltureLab->setText(m_json["signature"].toString());
}

void ContactPage::clearWidget()
{
	m_detailEditWidget = nullptr;
	m_oneself = nullptr;
	//退出后禁止信号
	ui->groupcomBox->blockSignals(true);
}
