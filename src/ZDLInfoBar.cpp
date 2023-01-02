/*
 * This file is part of qZDL
 * Copyright (C) 2007-2010  Cody Harris
 *
 * qZDL is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QtWidgets>
#include <QApplication>
#include <QMainWindow>
#include <string.h>
#include "xpm/exclaim.xpm"
#include "xpm/question.xpm"

#include "ZDLInfoBar.h"

ZDLInfoBar::ZDLInfoBar(ZDLWidget *parent) : ZDLWidget(parent)
{
	QPalette p(palette());
	normal = p.color(QPalette::Window);
	warning = QColor(255, 96, 96);
	info = QColor(255, 234, 191);
	setAutoFillBackground(true);
	QHBoxLayout *box = new QHBoxLayout(this);

	lbl = new QLabel(this);
	txtlbl = new QLabel("", this);
	lbl->setPixmap(QPixmap());
	box->addWidget(lbl);

	box->addWidget(txtlbl);
	box->setSpacing(0);
	setContentsMargins(0, 0, 0, 0);
	layout()->setContentsMargins(0, 0, 0, 0);
	QPushButton *btn = new QPushButton("More", this);
	QPushButton *btnhide = new QPushButton("Hide", this);
	btn->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred));
	btnhide->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred));
	setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum));
	box->addItem(new QSpacerItem(10000, 0, QSizePolicy::Preferred));
	box->addWidget(btn);
	box->addWidget(btnhide);

	connect(btnhide, &QPushButton::clicked, this, &ZDLInfoBar::hidebar);
	connect(btn, &QPushButton::clicked, this, &ZDLInfoBar::more);

	setVisible(false);
}

void ZDLInfoBar::hidebar()
{
	disconnect(this, 0, 0, 0);
	setVisible(false);
}

void ZDLInfoBar::more()
{
	emit moreclicked();
}

void ZDLInfoBar::setMessage(const char *message, int icon)
{
	QPalette p(palette());
	disconnect(this, 0, 0, 0);
	if (icon == 0)
	{
		p.setColor(QPalette::Window, normal);
	}
	else if (icon == 1)
	{
		p.setColor(QPalette::Window, warning);
		txtlbl->setText(message);
		lbl->setPixmap(QPixmap(exclaim));
	}
	else if (icon == 2)
	{
		p.setColor(QPalette::Window, info);
		txtlbl->setText(message);
		lbl->setPixmap(QPixmap(question));
	}
	setVisible(true);
	setPalette(p);
}
