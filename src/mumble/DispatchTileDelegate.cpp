// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "DispatchTileDelegate.h"

#include "DispatchRoles.h"

#include <QtCore/QStringList>
#include <QtGui/QPainter>

DispatchTileDelegate::DispatchTileDelegate(QObject *parent) : QStyledItemDelegate(parent) {}

DispatchTileDelegate::~DispatchTileDelegate() = default;

void DispatchTileDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
							 const QModelIndex &index) const {
	painter->save();
	painter->setRenderHint(QPainter::Antialiasing, true);

	QRect tileRect = option.rect.adjusted(4, 4, -4, -4);
	QColor backgroundColor = option.state.testFlag(QStyle::State_Selected)
							 ? option.palette.highlight().color()
							 : option.palette.base().color().darker(108);
	QColor borderColor = option.palette.mid().color();

	painter->setPen(borderColor);
	painter->setBrush(backgroundColor);
	painter->drawRoundedRect(tileRect, 6.0, 6.0);

	QString displayName = index.data(DispatchRoles::DisplayName).toString();
	if (displayName.isEmpty()) {
		displayName = index.data(Qt::DisplayRole).toString();
	}
	QString channelName = index.data(DispatchRoles::ChannelName).toString();
	QString sessionText = tr("Session %1").arg(index.data(DispatchRoles::SessionId).toUInt());

	QRect textRect = tileRect.adjusted(10, 8, -10, -8);
	QFont nameFont = option.font;
	nameFont.setBold(true);
	painter->setFont(nameFont);
	painter->setPen(option.palette.text().color());
	painter->drawText(textRect, Qt::AlignTop | Qt::TextSingleLine, displayName);

	QFont detailFont = option.font;
	detailFont.setPointSizeF(detailFont.pointSizeF() * 0.9);
	painter->setFont(detailFont);
	QRect detailRect = textRect.adjusted(0, 28, 0, 0);
	painter->drawText(detailRect, Qt::AlignTop | Qt::TextSingleLine,
				  channelName.isEmpty() ? tr("No channel") : channelName);
	painter->drawText(detailRect.adjusted(0, 22, 0, 0), Qt::AlignTop | Qt::TextSingleLine, sessionText);

	QStringList states;
	if (index.data(DispatchRoles::Muted).toBool()) {
		states << tr("Muted");
	}
	if (index.data(DispatchRoles::Deafened).toBool()) {
		states << tr("Deafened");
	}
	if (index.data(DispatchRoles::LocalMuted).toBool()) {
		states << tr("Local mute");
	}
	if (states.isEmpty()) {
		states << tr("Audio clear");
	}
	painter->drawText(detailRect.adjusted(0, 44, 0, 0), Qt::AlignTop | Qt::TextSingleLine, states.join(tr(" • ")));

	painter->restore();
}

QSize DispatchTileDelegate::sizeHint(const QStyleOptionViewItem &, const QModelIndex &) const {
	return QSize(190, 120);
}
