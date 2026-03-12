// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "DispatchTileDelegate.h"

#include "DispatchRoles.h"

#include <QtCore/QStringList>
#include <QtGui/QFontMetrics>
#include <QtGui/QPainter>

DispatchTileDelegate::DispatchTileDelegate(QObject *parent) : QStyledItemDelegate(parent) {}

DispatchTileDelegate::~DispatchTileDelegate() = default;

void DispatchTileDelegate::setTileSizePreset(TileSizePreset preset) {
	m_tileSizePreset = preset;
}

DispatchTileDelegate::TileSizePreset DispatchTileDelegate::tileSizePreset() const {
	return m_tileSizePreset;
}

void DispatchTileDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
							 const QModelIndex &index) const {
	painter->save();
	painter->setRenderHint(QPainter::Antialiasing, true);

	const bool isLarge = (m_tileSizePreset == TileSizePreset::Large);
	const int margin   = isLarge ? 8 : 6;
	QRect tileRect     = option.rect.adjusted(margin, margin, -margin, -margin);

	QColor backgroundColor = option.state.testFlag(QStyle::State_Selected)
							 ? option.palette.highlight().color()
							 : option.palette.base().color().darker(120);
	QColor borderColor     = option.state.testFlag(QStyle::State_Selected)
							 ? option.palette.highlightedText().color()
							 : option.palette.mid().color();

	painter->setPen(QPen(borderColor, 1.2));
	painter->setBrush(backgroundColor);
	painter->drawRoundedRect(tileRect, 6.0, 6.0);

	QString displayName = index.data(DispatchRoles::DisplayName).toString();
	if (displayName.isEmpty()) {
		displayName = index.data(Qt::DisplayRole).toString();
	}
	QString channelName = index.data(DispatchRoles::ChannelName).toString();
	QString sessionText = tr("Session %1").arg(index.data(DispatchRoles::SessionId).toUInt());

	QRect contentRect = tileRect.adjusted(10, 8, -10, -8);
	QFont nameFont    = option.font;
	nameFont.setBold(true);
	if (isLarge) {
		nameFont.setPointSizeF(nameFont.pointSizeF() + 1.0);
	}
	painter->setFont(nameFont);
	painter->setPen(option.palette.text().color());

	QFontMetrics nameMetrics(nameFont);
	const QString elidedName = nameMetrics.elidedText(displayName, Qt::ElideRight, contentRect.width());
	painter->drawText(QRect(contentRect.left(), contentRect.top(), contentRect.width(), nameMetrics.height()),
				 Qt::AlignLeft | Qt::AlignVCenter, elidedName);

	QFont detailFont = option.font;
	detailFont.setPointSizeF(detailFont.pointSizeF() * (isLarge ? 0.95 : 0.9));
	painter->setFont(detailFont);
	QFontMetrics detailMetrics(detailFont);

	const QString channelLabel = channelName.isEmpty() ? tr("No channel") : channelName;
	const QString elidedChannel = detailMetrics.elidedText(channelLabel, Qt::ElideRight, contentRect.width());
	QRect channelRect(contentRect.left(), contentRect.top() + nameMetrics.height() + 4, contentRect.width(),
				 detailMetrics.height());
	painter->drawText(channelRect, Qt::AlignLeft | Qt::AlignVCenter, elidedChannel);

	const QString elidedSession = detailMetrics.elidedText(sessionText, Qt::ElideRight, contentRect.width());
	QRect sessionRect(channelRect.left(), channelRect.bottom() + 2, channelRect.width(), detailMetrics.height());
	painter->drawText(sessionRect, Qt::AlignLeft | Qt::AlignVCenter, elidedSession);

	QStringList states;
	if (index.data(DispatchRoles::Muted).toBool()) {
		states << tr("Muted");
	}
	if (index.data(DispatchRoles::Deafened).toBool()) {
		states << tr("Deafened");
	}
	if (index.data(DispatchRoles::LocalMuted).toBool()) {
		states << tr("Local");
	}
	if (states.isEmpty()) {
		states << tr("Clear");
	}

	QRect badgeRect(contentRect.left(), tileRect.bottom() - detailMetrics.height() - 10,
				   contentRect.width(), detailMetrics.height());
	painter->setPen(option.state.testFlag(QStyle::State_Selected) ? option.palette.highlightedText().color()
									   : option.palette.brightText().color());
	painter->drawText(badgeRect, Qt::AlignLeft | Qt::AlignVCenter,
				 detailMetrics.elidedText(states.join(tr(" · ")), Qt::ElideRight, badgeRect.width()));

	painter->restore();
}

QSize DispatchTileDelegate::sizeHint(const QStyleOptionViewItem &, const QModelIndex &) const {
	return m_tileSizePreset == TileSizePreset::Large ? QSize(260, 150) : QSize(210, 124);
}
